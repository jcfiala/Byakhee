// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "SkillManager.h"
#include "SectionFile.h"
#include "ParseBuffer.h"
#include "Investigator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//the one and only skill manager object
CSkillManager SkillManager;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COccupation::COccupation( CString strName, ERA_MASK EraMask /*ERA_ISALL*/, CString strScript /*""*/ )
{
    m_strName = strName;
    m_Era = EraMask;
    m_strScript = strScript;
    m_nOptional = 0;
    m_SkillArray.SetSize( 0, 10 );
    m_OptionalSkillArray.SetSize( 0, 10 );
}

COccupation::~COccupation()
{
}


CString COccupation::GetName()
{
    int nLength = m_strName.GetLength();
    if( nLength > 0 && m_strName[0] == '*' ) 
        return m_strName.Right( nLength-1 );
    else
        return m_strName;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuleFile::CRuleFile( CString strRuleFile )
{
    m_strFileName = strRuleFile;

    m_AllSkillArray.SetSize(0, 50 );
    m_SkillArray.SetSize(0, 25 );
    m_FirearmArray.SetSize(0, 25 );
    m_MeleeArray.SetSize(0, 25 );
    m_OccupationArray.SetSize(0, 25 );
}

CRuleFile::~CRuleFile()
{
    int i;
    for( i = 0; i < m_AllSkillArray.GetSize(); i++ ) delete m_AllSkillArray[i];
    m_AllSkillArray.RemoveAll();

    for( i = 0; i < m_OccupationArray.GetSize(); i++ ) delete m_OccupationArray[i];
    m_OccupationArray.RemoveAll();

    m_FirearmArray.RemoveAll();
    m_MeleeArray.RemoveAll();
    m_SkillArray.RemoveAll();
}


BOOL CRuleFile::AddSkillsToInvestigator( class CInvestigator* pInvestigator )
{
    ASSERT(pInvestigator);

    //scan through the main skill list
    int n;
    for( n = 0; n < m_SkillArray.GetSize(); n++ )
    {
        CSkill* pSkill = m_SkillArray[n];
        if( pSkill && pSkill->IsEra(pInvestigator->m_Era) ) pInvestigator->AddSkill( pSkill->GetCopy() );
    }

    //scan through melee list and add skills with a * in front
    for( n = 0; n < m_MeleeArray.GetSize(); n++ )
    {
        CSkill* pSkill = m_MeleeArray[n];
        if( pSkill && pSkill->IsEra(pInvestigator->m_Era) && pSkill->m_strName.IsEmpty() == FALSE && pSkill->m_strName[0] == '*' ) pInvestigator->AddSkill( pSkill->GetCopy() );
    }

    return TRUE;
}


COccupation* CRuleFile::FindOccupation( CString strOccupation )
{
    //FIXME: keep the occupation sorted and use a binary search?
    for( int n = 0; n < m_OccupationArray.GetSize(); n++ )
    {
        COccupation* pOccupation = m_OccupationArray[n];
        if( strOccupation.Compare( pOccupation->GetName() ) == 0 ) return pOccupation;
    }

    return NULL;
}


BOOL CRuleFile::IsOccupationScript( CString strOccupation )
{
    COccupation* pOccupation = FindOccupation(strOccupation);
    if( pOccupation == NULL ) return FALSE;

    return pOccupation->m_strScript.GetLength() > 0;
}


int CRuleFile::GetOccupationCount()
{
    return m_OccupationArray.GetSize();
}


CString CRuleFile::GetOccupationName( int n )
{
    if( n >= 0 && n < m_OccupationArray.GetSize() )
        return m_OccupationArray[n]->GetName();
    
    ASSERT(FALSE);
    return "";
}


CSkill* CRuleFile::FindSkill( CString strName )
{
    //skip over trailing *s
    LPCSTR pszName = LPCSTR(strName);
    while( *pszName == '*' ) pszName++;

    //scan down the main skill list
    for( int n = 0; n < m_AllSkillArray.GetSize(); n++ )
    {
        CSkill* pSkill = m_AllSkillArray[n];
        if( pSkill )
        {
            //skip over trailing *s
            LPCSTR pszSkillName = LPCSTR(pSkill->m_strName);
            while( *pszSkillName == '*' ) pszSkillName++;

            //see if the name matches
            if( strcmpi( pszSkillName, pszName ) == 0 ) return pSkill;
        }
    }

    return NULL;
}



BOOL CRuleFile::ParseRuleFile()
{
    CWaitCursor WaitCursor;

    CString strFilename = CByakheeApp::GetDefaultDirectory() + m_strFileName;

	/* open and load the rule file */
    CSectionFile RuleFile;
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Opening ") + CString(m_strFileName) + CString("...") );
	if( RuleFile.Open( strFilename ) == FALSE )
    {
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        return FALSE;
    }
       

	/* extract all sections */

    //load skills
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading skills from ") + CString(m_strFileName) + CString("...") );
	if( BuildSkillsList( RuleFile.GetSection( "SKILLS" ) ) == FALSE ) 
    {
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        return FALSE;
    }

    //load firearms
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading firearms from ") + CString(m_strFileName) + CString("...") );
	if( BuildFirearmsList( RuleFile.GetSection( "FIREARMS" ) ) == FALSE )
    {
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        return FALSE;
    }

    //load melee weapons
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading melee weapons from ") + CString(m_strFileName) + CString("...") );
	if( BuildMeleeList( RuleFile.GetSection( "MELEE" ) ) == FALSE )
    {
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        return FALSE;
    }
        
    //load occupations
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading occupations from ") + CString(m_strFileName) + CString("...") );
    if( BuildOccupationsList( RuleFile.GetSection( "OCCUPATIONS" ) ) == FALSE )
    {
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        return FALSE;
    }
    
    //all OK - clear the status bar and return
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
	return TRUE;

}



ERA_MASK CRuleFile::CreateEraMask( CString strEraField )
{
    //blank mask
    ERA_MASK Mask = ERA_ISNONE;

    //only do this if there is enough data in the field
    int nMaskLength = strEraField.GetLength();
    if( nMaskLength >= 1 ) if( strEraField[0] != '-' ) Mask |= ERA_IS1890;
    if( nMaskLength >= 2 ) if( strEraField[1] != '-' ) Mask |= ERA_IS1920;
    if( nMaskLength >= 3 ) if( strEraField[2] != '-' ) Mask |= ERA_IS1990;

    //return the result
    return Mask;
}


BOOL CRuleFile::BuildMeleeList( CString strSection )
{
//skill Dmg      Atks HP  Name             era  category
// "50" "1D3+db" "1"  "-" "****Fist/Punch" xxx  n


    //create a parse buffer for this section
	CParseBuffer Buffer(strSection);

    //loop until the buffer is empty
	while( Buffer.IsEmpty() == FALSE )
	{
		//parse values out of buffer
		CString strBase =       Buffer.ExtractQuotedString();
        CString strDmg =        Buffer.ExtractQuotedString();
        CString strAttacks =    Buffer.ExtractQuotedString();
        CString strHP =         Buffer.ExtractQuotedString();
		CString strName =       Buffer.ExtractQuotedString();
        ERA_MASK EraMask =      CreateEraMask( Buffer.ExtractUnquotedString() );

        //check for parse error
        if( Buffer.IsParseError() == FALSE )
        {
            //was there enough data to create this?
            if( Buffer.IsUnderflow() == FALSE )
            {
                //add this firearm
                CMeleeSkill* pSkill = new CMeleeSkill( strName, strBase, EraMask, strDmg, strAttacks, strHP );
                if( !FindSkill( strName ) )
                {
                    m_MeleeArray.Add( pSkill );
                    m_AllSkillArray.Add( pSkill );
                }
                else
                {
                    CString str;
                    str.Format( "Warning: Duplicate skill \"%s\" in %s will not be added", strName, m_strFileName );
                    AfxMessageBox( str );
                    delete pSkill;
                }
            }
            else
            {
                //buffer underflow
                AfxMessageBox( "Unexpected end of section 'MELEE'" );
                return FALSE;
            }
        }
        else
        {
            //parse error
            return FALSE;
        }

        //skip to the next line
        Buffer.SkipToNextLine();
	}

    //all OK!
    return TRUE;
}


BOOL CRuleFile::BuildFirearmsList( CString strSection )
{
//skill Dmg     Range    Sht/Rnd Bul In Gun  HP  Mal  Name                era
//"20"  "1D6+1" "10 yds" "1/4"	 "1"         "8" "95" "Flintlock Pistol*" xxx

    //create a parse buffer for this section
	CParseBuffer Buffer(strSection);

    //loop until the buffer is empty
	while( Buffer.IsEmpty() == FALSE )
	{
		//parse values out of buffer
		CString strBase =       Buffer.ExtractQuotedString();
        CString strDmg =        Buffer.ExtractQuotedString();
        CString strRange =      Buffer.ExtractQuotedString();
        CString strShots =      Buffer.ExtractQuotedString();
        CString strBullets =    Buffer.ExtractQuotedString();
        CString strHP =         Buffer.ExtractQuotedString();
        CString strMAL =        Buffer.ExtractQuotedString();
		CString strName =       Buffer.ExtractQuotedString();
        ERA_MASK EraMask =      CreateEraMask( Buffer.ExtractUnquotedString() );

        //check for parse error
        if( Buffer.IsParseError() == FALSE )
        {
            //was there enough data to create this?
            if( Buffer.IsUnderflow() == FALSE )
            {
                //add this firearm
                CFirearmSkill* pSkill = new CFirearmSkill( strName, strBase, EraMask, strDmg, strRange, strShots, strBullets, strHP, strMAL );
                if( !FindSkill( strName ) )
                {
                    m_FirearmArray.Add( pSkill );
                    m_AllSkillArray.Add( pSkill );
                }
                else
                {
                    CString str;
                    str.Format( "Warning: Duplicate skill \"%s\" in %s will not be added", strName, m_strFileName );
                    AfxMessageBox( str );
                    delete pSkill;
                }
            }
            else
            {
                //buffer underflow
                AfxMessageBox( "Unexpected end of section 'FIREARMS'" );
                return FALSE;
            }
        }
        else
        {
            //parse error
            return FALSE;
        }

        //skip to the next line
        Buffer.SkipToNextLine();
	}

    //all OK!
    return TRUE;
}


BOOL CRuleFile::BuildSkillsList( CString strSection )
{
// "10" 0 "*Accounting" xxx

    //create a parse buffer for this section
	CParseBuffer Buffer(strSection);

    //loop until the buffer is empty
	while( Buffer.IsEmpty() == FALSE )
	{
		//parse values out of buffer
		CString strBase =      Buffer.ExtractQuotedString();
		int nSubskills =       Buffer.ExtractInteger();
		CString strSkillName = Buffer.ExtractQuotedString();
        ERA_MASK EraMask =     CreateEraMask( Buffer.ExtractUnquotedString() );

        //check for parse error
        if( Buffer.IsParseError() == FALSE )
        {
            //was there enough data to create this?
            if( Buffer.IsUnderflow() == FALSE )
            {
		        //add the skill
                if( nSubskills != 0 )
                {
                    //this skill has subsets
                    CSpecialSkill* pSpecial = new CSpecialSkill( strSkillName, strBase, EraMask, nSubskills );
                    if( !FindSkill( strSkillName ) )
                    {
                        m_SkillArray.Add( pSpecial );
                        m_AllSkillArray.Add( pSpecial );
                    }
                    else
                    {
                        CString str;
                        str.Format( "Warning: Duplicate skill \"%s\" in %s will not be added", strSkillName, m_strFileName );
                        AfxMessageBox( str );
                        delete pSpecial;
                    }
                }
                else
                {
                    //this is a normal skill
                    CSkill* pSkill = new CSkill( strSkillName, strBase, EraMask );
                    if( !FindSkill( strSkillName ) )
                    {
                        m_SkillArray.Add( pSkill );
                        m_AllSkillArray.Add( pSkill );
                    }
                    else
                    {
                        CString str;
                        str.Format( "Warning: Duplicate skill \"%s\" in %s will not be added", strSkillName, m_strFileName );
                        AfxMessageBox( str );
                        delete pSkill;
                    }
                }
            }
            else
            {
                //buffer underflow
                AfxMessageBox( "Unexpected end of section 'SKILLS'" );
                return FALSE;
            }
        }
        else
        {
            //parse error
            return FALSE;
        }

        //skip to the next line
        Buffer.SkipToNextLine();
	}

    //all OK!
    return TRUE;
}


BOOL CRuleFile::BuildOccupationsList( CString strSection )
{
	CParseBuffer Buffer(strSection);

    //loop until the buffer is empty
	while( Buffer.IsEmpty() == FALSE )
	{
		//parse values out of buffer
		CString strOccupation =  Buffer.ExtractQuotedString();
        CString strSkillBuffer = Buffer.ExtractSurroundedString( '(', ')' );
        ERA_MASK EraMask = ERA_ISALL;
        CString strScript;

        //load optional items (backward compatibility)
        if( Buffer.PeekNextChar() == '-' || Buffer.PeekNextChar() == 'x' )
        {
            EraMask = CreateEraMask( Buffer.ExtractUnquotedString() );
            if( Buffer.PeekNextChar() == '{' )
                strScript = Buffer.ExtractSurroundedString( '{', '}' );
        }

        //create the occupation
        COccupation* pOccupation = new COccupation( strOccupation, EraMask, strScript ); 

        if( Buffer.IsParseError() == FALSE )
        {
            //was there enough data to create this?
            if( Buffer.IsUnderflow() == FALSE )
            {
                //now check the individual skills
                CParseBuffer SkillBuffer(strSkillBuffer);

                //loop until the buffer is empty
                BOOL fGotSubsets = FALSE;
                while( SkillBuffer.IsEmpty() == FALSE )
                {
                    char chNext = SkillBuffer.PeekNextChar();
                    switch( chNext )
                    {
                        case '\0':
                            //end of buffer...
                            break;

                        case '*':
                        {
                            //wild card
                            CString strWildcardSkill = SkillBuffer.ExtractUnquotedString();
                            pOccupation->m_SkillArray.Add( "*" );
                            break;
                        }

                        case '\"':
                        {
                            //normal skill in quotes
                            CString strSkill = SkillBuffer.ExtractQuotedString();
                            CString strExtra = strSkill;

                            //terminate at the $ and find it in the skill list...
                            int nDollarPos = strSkill.Find( '$' );
                            if( nDollarPos != -1 ) strSkill = strSkill.Left( nDollarPos );

                            if( FindSkill(strSkill) == FALSE )
                            {
                                //buffer underflow
                                delete pOccupation;
                                CString str;
                                str.Format( "Occupation %s: Skill %s not recognised", strOccupation, strSkill );
                                AfxMessageBox( str );
                                return FALSE;
                            }

                            //add it, dollar and all
                            pOccupation->m_SkillArray.Add( strExtra );
                            break;
                        }

                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        {
                            //pickable-subset
                            CString strNumSubsets = SkillBuffer.ExtractUnquotedString();
                            CString strSubsets = SkillBuffer.ExtractSurroundedString( '{', '}' );
                            if( SkillBuffer.IsParseError() == FALSE )
                            {
                                //was there enough data to create this?
                                if( SkillBuffer.IsUnderflow() == FALSE )
                                {
                                    if( fGotSubsets )
                                    {
                                        delete pOccupation;
                                        CString str;
                                        str.Format( "Subsets already defined in occupation %s", strOccupation );
                                        AfxMessageBox( str );
                                        return FALSE;
                                    }
                                    else
                                    {
                                        fGotSubsets = TRUE;
                                        pOccupation->m_nOptional = atoi(strNumSubsets);

                                        //now check each subset skill too
                                        CParseBuffer SubsetSkillBuffer(strSubsets);
                                        while( SubsetSkillBuffer.IsEmpty() == FALSE )
                                        {
                                            //normal skill in quotes
                                            CString strSkill = SubsetSkillBuffer.ExtractQuotedString();

                                            if( SubsetSkillBuffer.IsParseError() == FALSE )
                                            {
                                                //was there enough data to create this?
                                                if( SubsetSkillBuffer.IsUnderflow() == FALSE )
                                                {
                                                    //terminate at the $ and find it in the skill list...
                                                    CString strExtra = strSkill;
                                                    int nDollarPos = strSkill.Find( '$' );
                                                    if( nDollarPos != -1 ) strSkill = strSkill.Left( nDollarPos );

                                                    if( FindSkill(strSkill) == FALSE )
                                                    {
                                                        //buffer underflow
                                                        delete pOccupation;
                                                        CString str;
                                                        str.Format( "Occupation %s subset: Skill %s not recognised", strOccupation, strSkill );
                                                        AfxMessageBox( str );
                                                        return FALSE;
                                                    }

                                                    //add it, dollar and all
                                                    pOccupation->m_OptionalSkillArray.Add( strExtra );
                                                }
                                                else
                                                {
                                                    //buffer underflow
                                                    //...do nothing - we're just at the end of the subset
                                                }
                                            }
                                            else
                                            {
                                                //parse error
                                                delete pOccupation;
                                                return FALSE;
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    //buffer underflow
                                    delete pOccupation;
                                    CString str;
                                    str.Format( "Unexpected end of subset in occupation %s", strOccupation );
                                    AfxMessageBox( str );
                                    return FALSE;
                                }
                            }
                            else
                            {
                                //parse error
                                delete pOccupation;
                                return FALSE;
                            }
                            break;
                        }

                        default:
                        {
                            //invalid token
                            delete pOccupation;
                            CString str;
                            str.Format( "Unexpected character \"%c\" in subset in occupation %s", chNext, strOccupation );
                            AfxMessageBox( str );
                            return FALSE;
                        }
                    }
                }

                //add this occupation
                m_OccupationArray.Add( pOccupation );
            }
            else
            {
                //buffer underflow
                delete pOccupation;
                AfxMessageBox( "Unexpected end of section 'OCCUPATIONS'" );
                return FALSE;
            }
        }
        else
        {
            //parse error
            delete pOccupation;
            return FALSE;
        }

        //skip to the next line
        Buffer.SkipToNextLine();
    }

    return TRUE;
}



CString CRuleFile::GetKeyPairValue(CString strName, CString strDefault)
{
    CSectionFile SectionFile;
    SectionFile.Open( CByakheeApp::GetDefaultDirectory() + m_strFileName );
    return SectionFile.GetKeyPairValueFromSection( "KEYVALUEPAIRS", strName, strDefault);
}










//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillManager::CSkillManager()
{
    for( int i = 0; i < MAX_RULEFILE_CACHE; i++ ) 
    {
        m_RuleFileCache[i].dwTickCount = 0;
        m_RuleFileCache[i].pRuleFile = NULL;
        m_RuleFileCache[i].strRuleFile = "";
    }
}

CSkillManager::~CSkillManager()
{
    for( int i = 0; i < MAX_RULEFILE_CACHE; i++ ) 
        if( m_RuleFileCache[i].pRuleFile ) delete m_RuleFileCache[i].pRuleFile;
}

BOOL CSkillManager::AddRuleFile(CString strRuleFile)
{
    //see if this rule file is already loaded
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
    {
        //set it's last used time
        m_RuleFileCache[iRuleFile].dwTickCount = GetTickCount();
        return TRUE;
    }
    else
    {
        //find a blank spot (or the oldest spot)
        int iItem = -1;
        for( int i = 0; i < MAX_RULEFILE_CACHE; i++ )
        {
            //keep a record of the oldest item in case all slots are used
            if( iItem == -1 || m_RuleFileCache[i].dwTickCount < m_RuleFileCache[iItem].dwTickCount ) iItem = i;

            //see if this slot is empty
            if( m_RuleFileCache[i].pRuleFile == NULL )
            {
                iItem = i;
                break;
            }
        }

        //make sure *something* was found
        if( iItem == -1 ) return FALSE;

        //no empty spots were found - kill the oldest
        delete m_RuleFileCache[iItem].pRuleFile;
        m_RuleFileCache[iItem].pRuleFile = NULL;
        m_RuleFileCache[iItem].strRuleFile = "";
        m_RuleFileCache[iItem].dwTickCount = 0;

        //load this one
        m_RuleFileCache[iItem].pRuleFile = new CRuleFile(strRuleFile);
        if( m_RuleFileCache[iItem].pRuleFile->ParseRuleFile() == FALSE )
        {
            delete m_RuleFileCache[iItem].pRuleFile;
            m_RuleFileCache[iItem].pRuleFile = NULL;
            m_RuleFileCache[iItem].dwTickCount = 0;
            m_RuleFileCache[iItem].strRuleFile = "";
            return FALSE;
        }

        //this is empty - we'll take it
        m_RuleFileCache[iItem].dwTickCount = GetTickCount();
        m_RuleFileCache[iItem].strRuleFile = strRuleFile;
        return TRUE;
    }
}

BOOL CSkillManager::RemoveRuleFile(CString strRuleFile)
{
    /*
    This function could be used for reference counting.
    
      However, it should be noted that if the user does the following:
      1) create a new investigator document using verylarge.rul
      2) closes the investigator document
      3) creates another investigator doc usign verylarge.rul

      ...then this function will be called on 2) and might
      cause unwanted delays at 3) when reloading the rule file
    */

    //see if this rule file is already loaded
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return TRUE;
    else
        return FALSE;
}

CSkill* CSkillManager::FindSkill( CString strRuleFile, CString strName )
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->FindSkill(strName);
    else
        return NULL;
}

CString CSkillManager::GetOccupationName( CString strRuleFile, int n)
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->GetOccupationName(n);
    else
        return "";
}

int CSkillManager::GetOccupationCount( CString strRuleFile )
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->GetOccupationCount();
    else
        return 0;
}

BOOL CSkillManager::IsOccupationScript( CString strRuleFile, CString strOccupation )
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->IsOccupationScript(strOccupation);
    else
        return FALSE;
}

COccupation* CSkillManager::FindOccupation( CString strRuleFile, CString strOccupation)
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->FindOccupation(strOccupation);
    else
        return NULL;
}


BOOL CSkillManager::AddSkillsToInvestigator( CString strRuleFile, CInvestigator * pInvestigator)
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->AddSkillsToInvestigator(pInvestigator);
    else
        return FALSE;
}

int CSkillManager::FindRuleFile( CString strRuleFile )
{
    if( strRuleFile.IsEmpty() ) strRuleFile = "default.rul";

    for( int i = 0; i < MAX_RULEFILE_CACHE; i++ )
        if( strRuleFile.CompareNoCase(m_RuleFileCache[i].strRuleFile) == 0 && m_RuleFileCache[i].pRuleFile != NULL ) return i;

    return -1;
}

const CRuleFile* CSkillManager::GetTempRuleFile(CString strRuleFile)
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile;
    else
        return NULL;
        
}


/*
CString CSkillManager::GetFirstAvailableRuleFile()
{
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.rul");

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        //close the search
    	FindClose( hFind );

        //trim off path, if there is one
        char* pszFilename = wfd.cFileName;
        char* pszPathSep = strrchr(pszFilename, '\\');
        if( pszPathSep ) pszFilename = ++pszPathSep;

        //return it
        return CString(pszFilename);
    }
    else
        return "";
}
*/

CString CSkillManager::GetKeyPairValue( CString strRuleFile, CString strName, CString strDefault)
{
    int iRuleFile = FindRuleFile(strRuleFile);
    if( iRuleFile != -1 )
        return m_RuleFileCache[iRuleFile].pRuleFile->GetKeyPairValue(strName, strDefault);
    else
        return strDefault;
}
