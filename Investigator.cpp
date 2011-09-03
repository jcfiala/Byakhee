// Investigator.cpp: implementation of the CInvestigator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "DiceRoller.h"
#include "Output.h"
#include "OptionalSkillsDlg.h"
#include "SectionFile.h"
#include "ScriptEngine/ScriptingEngine.h"
#include "Investigator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvestigatorSkill::CInvestigatorSkill( CSkill* pSkill, int nCurrent, int nMinimum /*-1*/ )
{
    m_pSkill = pSkill;
    m_nCurrent = nCurrent;
    if( nMinimum == -1 )
        m_nMinimum = nCurrent;
    else
        m_nMinimum = nMinimum;

    m_fOccupational = FALSE;
    m_fChecked = FALSE;
}

CInvestigatorSkill::~CInvestigatorSkill()
{
    delete m_pSkill;
}

CInvestigatorSkill* CInvestigatorSkill::GetCopy()
{
    //create a duplicate of ourselves
    CInvestigatorSkill* pInvestigatorSkillCopy = new CInvestigatorSkill( m_pSkill->GetCopy(), m_nCurrent, m_nMinimum );
    pInvestigatorSkillCopy->m_fChecked = m_fChecked;
    pInvestigatorSkillCopy->m_fOccupational = m_fOccupational;

    //return it
    return pInvestigatorSkillCopy;
}


CInvestigatorSkill* CInvestigatorSkill::GetEmptyCopy()
{
    //create a duplicate of ourselves
    CInvestigatorSkill* pInvestigatorSkillCopy = new CInvestigatorSkill( NULL, m_nCurrent, m_nMinimum );
    pInvestigatorSkillCopy->m_fChecked = m_fChecked;
    pInvestigatorSkillCopy->m_fOccupational = m_fOccupational;

    //return it
    return pInvestigatorSkillCopy;

}

signed char CInvestigatorSkill::Compare(const CInvestigatorSkill * pInvestigatorSkill)
{
    CString strSortName1, strSortName2;
    SkillType Type1 = m_pSkill->m_SkillType, Type2 = pInvestigatorSkill->m_pSkill->m_SkillType;

    //if it's a special, sort on parent's name initially...
    if( Type1 == SUBSKILL ) strSortName1 = ((CSubSkill*)m_pSkill)->m_pSkill->m_strName; else strSortName1 = m_pSkill->m_strName;
    if( Type2 == SUBSKILL ) strSortName2 = ((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName; else strSortName2 = pInvestigatorSkill->m_pSkill->m_strName;

    //sort on name first of all
    int nResult = strSortName1.CompareNoCase(strSortName2);
    if( nResult == 0 )
    {
        //sort sub items
        if( Type1 == SUBSKILL && Type2 == SUBSKILL )
        {
            //alphabetic sort on sub items, gaps at the bottom
            if( m_pSkill->m_strName.GetLength() == 0 ) return ( pInvestigatorSkill->m_pSkill->m_strName.GetLength() == 0 ) ? 0 : 1;
            if( pInvestigatorSkill->m_pSkill->m_strName.GetLength() == 0 ) return -1;
            return m_pSkill->m_strName.CompareNoCase( pInvestigatorSkill->m_pSkill->m_strName );
        }

        //put parent items before subitems
        if( Type1 == SUBSKILL && Type2 == SPECIALSKILL ) return 1;
        if( Type1 == SPECIALSKILL && Type2 == SUBSKILL ) return -1;
        return 0;
    }
    else return nResult;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvestigator::CInvestigator()
{
    //intialise all attributes (note: string constructor defaults to "" anyway)
	m_nSkillPoints = 0;
	m_nInterestPoints = 0;

	m_nAge = 0;
	m_Era = Era_1920;

	m_nAPP = 0;
	m_nCON = 0;
	m_nDEX = 0;
	m_nEDU = 0;
	m_nINT = 0;
	m_nPOW = 0;
	m_nSIZ = 0;
	m_nSTR = 0;

    m_nSAN = 0;
    m_nHitPoints = 0;
    m_nMagic = 0;

	m_ExtraSchooling = EXTRASCHOOLING_NOTAPPLICABLE;

    m_strRuleFile = "";
}

CInvestigator::CInvestigator( const CInvestigator& source )
{
    source.DuplicateInvestigator( this );
}

CInvestigator::~CInvestigator()
{
    //delete all skills
    for( int i = 0; i < m_AllSkillList.GetSize(); i++ ) delete m_AllSkillList[i];

    //reset all lists
    m_AllSkillList.RemoveAll();
    m_FirearmList.RemoveAll();
    m_MeleeList.RemoveAll();
    m_SkillList.RemoveAll();
}

BOOL CInvestigator::SetRuleFile( CString strRuleFile )
{
    //cache the new rule file and make sure it's OK
    if( SkillManager.AddRuleFile(strRuleFile) )
    {
        //remove the reference on our own rule file and change it to a new one
        SkillManager.RemoveRuleFile(m_strRuleFile);
        m_strRuleFile = strRuleFile;
        return TRUE;
    }
    else
    {
        strRuleFile = "default.rul";
        if( SkillManager.AddRuleFile(strRuleFile) )
        {
            SkillManager.RemoveRuleFile(m_strRuleFile);
            m_strRuleFile = strRuleFile;
            return TRUE;
        }
        else
            return FALSE;
    }
}


int CInvestigator::ReadSkill( CArchive& ar, BOOL fOccupational, int nCurrent, int nMinimumValue )
{
    int nType;
    ar >> nType;

    switch( SkillType(nType) )
    {
        case ANYSKILL:
        {
            CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSkill( "", "", ERA_IS1890|ERA_IS1920|ERA_IS1990 ), nCurrent, nMinimumValue );
            pNew->m_fOccupational = fOccupational;
            pNew->m_pSkill->m_SkillType = ANYSKILL;

            m_AllSkillList.Add( pNew );
            m_SkillList.Add( pNew );
            return 1;
            break;
        }

        case SKILL:
        {
            CString strName, strBase;
            BOOL f1890, f1920, f1990;
            ar.ReadString( strName );
            ar.ReadString( strBase );
            ar >> f1890;
            ar >> f1920;
            ar >> f1990;
            ERA_MASK EraMask = ( f1890 ? ERA_IS1890 : ERA_ISNONE ) | ( f1920 ? ERA_IS1920 : ERA_ISNONE ) | ( f1990 ? ERA_IS1990 : ERA_ISNONE );

            CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSkill( strName, strBase, EraMask ), nCurrent, nMinimumValue );
            pNew->m_fOccupational = fOccupational;

            m_AllSkillList.Add( pNew );
            m_SkillList.Add( pNew );
            return 1;
            break;
        }

        case FIREARM:
        {
            CString strName, strBase, strDmg, strRange, strShots, strBullets, strHP, strMAL;
            BOOL f1890, f1920, f1990;
            ar.ReadString( strName );
            ar.ReadString( strBase );
            ar >> f1890;
            ar >> f1920;
            ar >> f1990;
            ERA_MASK EraMask = ( f1890 ? ERA_IS1890 : ERA_ISNONE ) | ( f1920 ? ERA_IS1920 : ERA_ISNONE ) | ( f1990 ? ERA_IS1990 : ERA_ISNONE );

            ar.ReadString( strDmg );
            ar.ReadString( strRange );
            ar.ReadString( strShots );
            ar.ReadString( strBullets );
            ar.ReadString( strHP );
            ar.ReadString( strMAL );

            CInvestigatorSkill* pNew = new CInvestigatorSkill( new CFirearmSkill( strName, strBase, EraMask, strDmg, strRange, strShots, strBullets, strHP, strMAL ), nCurrent, nMinimumValue );
            pNew->m_fOccupational = fOccupational;

            m_AllSkillList.Add( pNew );
            m_FirearmList.Add( pNew );
            return 1;
            break;
        }

        case MELEE:
        {
            CString strName, strBase, strDmg, strAttacks, strHP;
            BOOL f1890, f1920, f1990;
            ar.ReadString( strName );
            ar.ReadString( strBase );
            ar >> f1890;
            ar >> f1920;
            ar >> f1990;
            ERA_MASK EraMask = ( f1890 ? ERA_IS1890 : ERA_ISNONE ) | ( f1920 ? ERA_IS1920 : ERA_ISNONE ) | ( f1990 ? ERA_IS1990 : ERA_ISNONE );

            ar.ReadString( strDmg );
            ar.ReadString( strAttacks );
            ar.ReadString( strHP );

            CInvestigatorSkill* pNew = new CInvestigatorSkill( new CMeleeSkill( strName, strBase, EraMask, strDmg, strAttacks, strHP ), nCurrent, nMinimumValue );
            pNew->m_fOccupational = fOccupational;

            m_AllSkillList.Add( pNew );
            m_MeleeList.Add( pNew );
            return 1;
            break;
        }

        case SPECIALSKILL:
        {
            CString strName, strBase;
            BOOL f1890, f1920, f1990;
            int nSkills;
            ar.ReadString( strName );
            ar.ReadString( strBase );
            ar >> f1890;
            ar >> f1920;
            ar >> f1990;
            ERA_MASK EraMask = ( f1890 ? ERA_IS1890 : ERA_ISNONE ) | ( f1920 ? ERA_IS1920 : ERA_ISNONE ) | ( f1990 ? ERA_IS1990 : ERA_ISNONE );

            ar >> nSkills;

            CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSpecialSkill( strName, strBase, EraMask, nSkills, FALSE/*don't create subskills*/), nCurrent, nMinimumValue );
            pNew->m_fOccupational = fOccupational;

            m_AllSkillList.Add( pNew );
            m_SkillList.Add( pNew );
            return 1;
            break;
        }

        case SUBSKILL:
        {
            CString strName, strBase, strParent;
            BOOL f1890, f1920, f1990;
            ar.ReadString( strName );
            ar.ReadString( strBase );
            ar >> f1890;
            ar >> f1920;
            ar >> f1990;
            ERA_MASK EraMask = ( f1890 ? ERA_IS1890 : ERA_ISNONE ) | ( f1920 ? ERA_IS1920 : ERA_ISNONE ) | ( f1990 ? ERA_IS1990 : ERA_ISNONE );

            ar.ReadString( strParent );

            CInvestigatorSkill* pParent = FindSkill( strParent );
            if( pParent && pParent->m_pSkill->m_SkillType == SPECIALSKILL )
            {
                //create the subskill
                CSpecialSkill* pSpecial = (CSpecialSkill*)pParent->m_pSkill;
                CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSubSkill( pSpecial, strBase, EraMask ), nCurrent, nMinimumValue );
                pNew->m_pSkill->m_strName = strName;
                pNew->m_fOccupational = fOccupational;

                //now link the parent to it

                //find free slot on the parent
                int i = 0;
                while( i < pSpecial->m_nSkills && pSpecial->m_pSubSkills[i] != NULL ) i++;
                if( i < pSpecial->m_nSkills )
                {
                    pSpecial->m_pSubSkills[i] = (CSubSkill*)pNew->m_pSkill;
                    m_AllSkillList.Add( pNew );
                    m_SkillList.Add( pNew );
                    return 1;
                }
                else
                {
                    delete pNew;
                    return 0;
                }
            }
            else
            {
                AfxThrowArchiveException( CArchiveException::badIndex, "" );
                return 0;
            }
        }
    }

    //unknown type
    AfxThrowArchiveException( CArchiveException::badIndex, "" );
    return 0;
}

void CInvestigator::WriteSkill( CArchive& ar, CSkill* pSkill )
{
    SkillType Type = pSkill->m_SkillType;

    switch( Type )
    {
        case ANYSKILL:
            ar << Type;
            break;

        case SKILL:
            ar << Type;
            ar.WriteString( pSkill->m_strName ); ar << '\n';
            ar.WriteString( pSkill->m_strBase ); ar << '\n';
            ar << (pSkill->m_EraMask & ERA_IS1890);
            ar << (pSkill->m_EraMask & ERA_IS1920);
            ar << (pSkill->m_EraMask & ERA_IS1990);
            break;

        case FIREARM:
        {
            CFirearmSkill* pFirearm = (CFirearmSkill*)pSkill;
            ar << Type;
            ar.WriteString( pFirearm->m_strName ); ar << '\n';
            ar.WriteString( pFirearm->m_strBase ); ar << '\n';
            ar << (pSkill->m_EraMask & ERA_IS1890);
            ar << (pSkill->m_EraMask & ERA_IS1920);
            ar << (pSkill->m_EraMask & ERA_IS1990);

            ar.WriteString( pFirearm->m_strDmg ); ar << '\n';
            ar.WriteString( pFirearm->m_strRange ); ar << '\n';
            ar.WriteString( pFirearm->m_strShots ); ar << '\n';
            ar.WriteString( pFirearm->m_strBullets ); ar << '\n';
            ar.WriteString( pFirearm->m_strHP ); ar << '\n';
            ar.WriteString( pFirearm->m_strMAL ); ar << '\n';
            break;
        }

        case MELEE:
        {
            CMeleeSkill* pMelee = (CMeleeSkill*)pSkill;
            ar << Type;
            ar.WriteString( pMelee->m_strName ); ar << '\n';
            ar.WriteString( pMelee->m_strBase ); ar << '\n';
            ar << (pSkill->m_EraMask & ERA_IS1890);
            ar << (pSkill->m_EraMask & ERA_IS1920);
            ar << (pSkill->m_EraMask & ERA_IS1990);

            ar.WriteString( pMelee->m_strDmg ); ar << '\n';
            ar.WriteString( pMelee->m_strAttacks ); ar << '\n';
            ar.WriteString( pMelee->m_strHP ); ar << '\n';
            break;
        }

        case SPECIALSKILL:
        {
            CSpecialSkill* pSpecial = (CSpecialSkill*)pSkill;
            ar << Type;
            ar.WriteString( pSpecial->m_strName ); ar << '\n';
            ar.WriteString( pSpecial->m_strBase ); ar << '\n';
            ar << (pSkill->m_EraMask & ERA_IS1890);
            ar << (pSkill->m_EraMask & ERA_IS1920);
            ar << (pSkill->m_EraMask & ERA_IS1990);

            ar << pSpecial->m_nSkills;
            break;
        }

        case SUBSKILL:
        {
            CSubSkill* pItem = (CSubSkill*)pSkill;
            ar << pItem->m_SkillType;
            ar.WriteString( pItem->m_strName ); ar << '\n';
            ar.WriteString( pItem->m_strBase ); ar << '\n';
            ar << (pSkill->m_EraMask & ERA_IS1890);
            ar << (pSkill->m_EraMask & ERA_IS1920);
            ar << (pSkill->m_EraMask & ERA_IS1990);

            //write out the *name* of the parent skill
            ar.WriteString( pItem->m_pSkill->m_strName ); ar << '\n';
            break;
        }
    }
}

CInvestigatorSkill* CInvestigator::FindSkill(CString strName)
{
    //skip over trailing *s, unless the name is just * (ANYSKILL)
    LPCSTR pszName = LPCSTR(strName);
    if( !(strName.GetLength() == 1 && strName.Compare("*") == 0 ) ) while( *pszName == '*' ) pszName++;

    //scan down the main skill list
    for( int n = 0; n < m_AllSkillList.GetSize(); n++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_AllSkillList.GetAt(n);
        if( pInvestigatorSkill )
        {
            //skip over trailing *s
            LPCSTR pszSkillName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
            if( !(pInvestigatorSkill->m_pSkill->m_strName.GetLength() == 1 && pInvestigatorSkill->m_pSkill->m_strName.Compare("*") == 0 ) )
                while( *pszSkillName == '*' ) pszSkillName++;

            //see if the name matches
            if( strcmpi( pszSkillName, pszName ) == 0 ) return pInvestigatorSkill;
        }
    }

    return NULL;
}

CString CInvestigator::GetDBString(int nSTR, int nSIZ) //static
{
    if( nSTR < 0 || nSIZ < 0 ) return ""; //< 0 == don't display it

	CString strDB;
	int nDB = nSTR + nSIZ;

    if( nDB <= 12 )
        strDB = "-1D6";
    else
        if( nDB <= 16 )
            strDB = "-1D4";
        else
            if( nDB <= 24 )
                strDB = "none";
            else
                if( nDB <= 32 )
                    strDB = "+1D4";
                else
					strDB.Format( "+%dD6", 1 + ( (nDB-25) / 16 )  );

	return strDB;
}

CString CInvestigator::GetDBString()
{
    return CInvestigator::GetDBString( m_nSTR, m_nSIZ );
}

BOOL CInvestigator::SetOccupation(CString strOccupation)
{
    if( strOccupation.IsEmpty() == FALSE )
    {
        //make sure this occupation exists
        COccupation* pOccupation = SkillManager.FindOccupation( m_strRuleFile, strOccupation );
        if( pOccupation == NULL ) return FALSE;

        //delete all skills
        int i;
        for( i = 0; i < m_AllSkillList.GetSize(); i++ ) delete m_AllSkillList[i];

        //reset all lists
        m_AllSkillList.RemoveAll();
        m_FirearmList.RemoveAll();
        m_MeleeList.RemoveAll();
        m_SkillList.RemoveAll();

        //add all skills from the current set
        SkillManager.AddSkillsToInvestigator( m_strRuleFile, this );

        //add optional skills
        if( pOccupation->m_nOptional > 0 )
        {
            COptionalSkillsDlg OptionalSkillsDlg( this, pOccupation );
            if( OptionalSkillsDlg.DoModal() != IDOK ) return FALSE;
        }

        //add all occupational skills or mark them as occupational if they already exist (including weapons!)
        for( i = 0; i < pOccupation->m_SkillArray.GetSize(); i++ ) AddOccupationalSkill( pOccupation->m_SkillArray[i] );
    }
    else
    {
        //delete all skills
        int i;
        for( i = 0; i < m_AllSkillList.GetSize(); i++ ) delete m_AllSkillList[i];

        //reset all lists
        m_AllSkillList.RemoveAll();
        m_FirearmList.RemoveAll();
        m_MeleeList.RemoveAll();
        m_SkillList.RemoveAll();

        //add all skills from the current set
        SkillManager.AddSkillsToInvestigator( m_strRuleFile, this );
    }

    //sort the skills in order
    SortSkillList();

    return TRUE;
}

int CInvestigator::GetAttributeByName(CString strName)
{
	//check basic stats
	if( strName.CompareNoCase( "APP" ) == 0 ) return m_nAPP;
	if( strName.CompareNoCase( "CON" ) == 0 ) return m_nCON;
	if( strName.CompareNoCase( "DEX" ) == 0 ) return m_nDEX;
	if( strName.CompareNoCase( "EDU" ) == 0 ) return m_nEDU;
	if( strName.CompareNoCase( "INT" ) == 0 ) return m_nINT;
	if( strName.CompareNoCase( "POW" ) == 0 ) return m_nPOW;
	if( strName.CompareNoCase( "SIZ" ) == 0 ) return m_nSIZ;
	if( strName.CompareNoCase( "STR" ) == 0 ) return m_nSTR;

	//check calculated stats
	if( strName.CompareNoCase( "LUCK" ) == 0 ) return m_nPOW * 5;
	if( strName.CompareNoCase( "KNOW" ) == 0 ) return m_nEDU * 5;
	if( strName.CompareNoCase( "IDEA" ) == 0 ) return m_nINT * 5;

    //check extended stats
	if( strName.CompareNoCase( "SAN" ) == 0 ) return m_nSAN;
	if( strName.CompareNoCase( "HP" ) == 0 ) return m_nHitPoints;
	if( strName.CompareNoCase( "HIT" ) == 0 ) return m_nHitPoints;
	if( strName.CompareNoCase( "MAGIC" ) == 0 ) return m_nMagic;

	//failed...
	CString str;
	str.Format( "Unknown character statistic: %s", strName );
	AfxMessageBox( str, MB_ICONEXCLAMATION );
	return -666;
}

BOOL CInvestigator::RemoveSkill(CString strSkillName)
{
    //locate the skill and remove it from the list
    return RemoveSkill( FindSkill(strSkillName) );
}

BOOL CInvestigator::RemoveSkill(CInvestigatorSkill * pInvestigatorSkill)
{
    if( pInvestigatorSkill == NULL ) return FALSE;

    //remove it from all lists
    int n;
    for( n = 0; n < m_AllSkillList.GetSize(); n++ ) if( m_AllSkillList.GetAt(n) == pInvestigatorSkill ) m_AllSkillList.RemoveAt(n);
    for( n = 0; n < m_SkillList.GetSize(); n++ ) if( m_SkillList.GetAt(n) == pInvestigatorSkill ) m_SkillList.RemoveAt(n);
    for( n = 0; n < m_MeleeList.GetSize(); n++ ) if( m_MeleeList.GetAt(n) == pInvestigatorSkill ) m_MeleeList.RemoveAt(n);
    for( n = 0; n < m_FirearmList.GetSize(); n++ ) if( m_FirearmList.GetAt(n) == pInvestigatorSkill ) m_FirearmList.RemoveAt(n);

    //remove all subskills
    if( pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
    {
        for( n = 0; n < m_AllSkillList.GetSize(); n++ )
        {
            CInvestigatorSkill* pInvestigatorSubSkill = m_AllSkillList[n];
            if( pInvestigatorSubSkill->m_pSkill->m_SkillType == SUBSKILL )
            {
                CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSubSkill->m_pSkill;
                if( pSubSkill->m_pSkill == pInvestigatorSkill->m_pSkill )
                {
                    //remove it from both lists
                    RemoveSkill( pInvestigatorSubSkill );
                }
            }
        }
    }

    //delete the structure
    delete pInvestigatorSkill;

    //all OK...
    return TRUE;
}

void CInvestigator::AddOccupationalSkill(CString strSkill)
{
    CString strSkillName = strSkill;

    //see if it's a wildcard skill
    if( strSkill.Compare( "*" ) == 0 )
    {
        //add a wildcard skill
        CInvestigatorSkill* pInvestigatorSkill = new CInvestigatorSkill( new CSkill(ERA_ISALL), 0 );

        //make it an occupational skill
        pInvestigatorSkill->m_fOccupational = TRUE;

        //add it to the investigator's skill list
        m_SkillList.Add(pInvestigatorSkill);
        m_AllSkillList.Add(pInvestigatorSkill);
    }
    else
    {
        //terminate at the $ and find it in the skill list...
        int nDollarPos = strSkill.Find( '$' );
        if( nDollarPos != -1 ) strSkillName = strSkill.Left( nDollarPos );

        CInvestigatorSkill* pInvestigatorSkill = FindSkill(strSkillName);
        if( pInvestigatorSkill )
        {
            //make it an occupational skill
            pInvestigatorSkill->m_fOccupational = TRUE;

            //do all subskills, if there are any
            if( pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
            {
                //find all subskills
                CSpecialSkill* pSpecialSkill = (CSpecialSkill*)pInvestigatorSkill->m_pSkill;
                for( int n = 0; n < pSpecialSkill->m_nSkills; n++ )
                {
                    for( int nn = 0; nn < m_SkillList.GetSize(); nn++ )
                    {
                        CInvestigatorSkill* pInvestigatorSubSkill = m_SkillList.GetAt(nn);

                        //check if this is a subskill
                        if( pInvestigatorSubSkill && pInvestigatorSubSkill->m_pSkill->m_SkillType == SUBSKILL )
                        {
                            //see if this is a subskill of the one we've found
                            CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSubSkill->m_pSkill;
                            if( pSubSkill == pSpecialSkill->m_pSubSkills[n] )
                            {
                                //make sure it points both ways & set it's occupational flag
                                ASSERT(pSubSkill->m_pSkill == pSpecialSkill );
                                pInvestigatorSubSkill->m_fOccupational = TRUE;
                                break; //break out of while( p ) and go onto next iteration in "for"
                            }
                        }
                    }
                }

                //apply the $, if it's relevant
                if( nDollarPos != -1 )
                {
                    //get the bit after the $
                    CString strExtra = strSkill.Right( strSkill.GetLength() - (nDollarPos + 1) );

                    //assign it to the first subskill name
                    pSpecialSkill->m_pSubSkills[0]->m_strName = strExtra;
                }
            }
        }
        else
        {
            //we don't have it already - look in the skill manager instead
            CSkill* pSkill = SkillManager.FindSkill( m_strRuleFile, strSkillName );
            if( pSkill )
            {
                //add a copy of this skill
                CInvestigatorSkill* pInvestigatorSkill = AddSkill( pSkill->GetCopy(), TRUE );
            }
            else
            {
                //FIXME: add it?
                TRACE1("Warning: AddOccupationalSkill() - skill %s not found\n", strSkill );
            }
        }
    }
}

CInvestigatorSkill* CInvestigator::AddSkill(CSkill * pSkill, BOOL fOccupational /*FALSE*/ )
{
    //create an investigator skill object based on the skill
    CInvestigatorSkill* pInvestigatorSkill = new CInvestigatorSkill( pSkill, pSkill->EvaluateBase(this) );
    pInvestigatorSkill->m_fOccupational = fOccupational;

    //add it to the investigator's main skill list
    m_AllSkillList.Add(pInvestigatorSkill);

    //determine which other list to add it to by the skill's type
    switch( pInvestigatorSkill->m_pSkill->m_SkillType )
    {
        case SPECIALSKILL:
        case ANYSKILL:  
        case SUBSKILL:
        case SKILL:     
            m_SkillList.Add(pInvestigatorSkill); 
            break;

        case FIREARM:   
            m_FirearmList.Add(pInvestigatorSkill); 
            break;

        case MELEE:     
            m_MeleeList.Add(pInvestigatorSkill); 
            break;

        default: 
            ASSERT(FALSE);
            return NULL;
    }

    //if it's a special skill item, add it's subskills too
    if( pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
    {
        CSpecialSkill* pSpecialSkill = (CSpecialSkill*)pSkill;
        for( int n = 0; n < pSpecialSkill->m_nSkills; n++ ) AddSkill( pSpecialSkill->m_pSubSkills[n], fOccupational );
        pSpecialSkill->m_fManageSubskills = FALSE; //as these have been added, they will be deleted by the CInvestigatorSkill class
    }

    return pInvestigatorSkill;
}


const CInvestigator& CInvestigator::operator=(const CInvestigator& source )
{
    source.DuplicateInvestigator( this );
	return *this;
}

void CInvestigator::DuplicateInvestigator( CInvestigator* destination ) const
{
    /* remove all skills to start with */
    for( int i = 0; i < destination->m_AllSkillList.GetSize(); i++ ) delete destination->m_AllSkillList[i];
    destination->m_AllSkillList.RemoveAll();
    destination->m_FirearmList.RemoveAll();
    destination->m_MeleeList.RemoveAll();
    destination->m_SkillList.RemoveAll();

    /* set the investigator's new rule file
        note: we do "add" before "remove"
        to timestamp our rulefile - this prevents
        the rule file from being unloaded then
        reloaded
    */
    SkillManager.AddRuleFile( m_strRuleFile );
    SkillManager.RemoveRuleFile( destination->m_strRuleFile );
    destination->m_strRuleFile = m_strRuleFile;

    /* copy over basic attributes */
    destination->m_nSkillPoints = m_nSkillPoints;
    destination->m_nInterestPoints = m_nInterestPoints;

	destination->m_strName = m_strName;
	destination->m_strOccupation = m_strOccupation;
	destination->m_strBirthplace = m_strBirthplace;
    destination->m_strColleges = m_strColleges;
    destination->m_strMentalDisorders = m_strMentalDisorders;
    destination->m_strSex = m_strSex;
	destination->m_nAge = m_nAge;
	destination->m_Era = m_Era;

    destination->m_strPortrait = m_strPortrait;
    destination->m_ExtraSchooling = m_ExtraSchooling;
	
	destination->m_nAPP = m_nAPP;
	destination->m_nCON = m_nCON;
	destination->m_nDEX = m_nDEX;
	destination->m_nEDU = m_nEDU;
	destination->m_nINT = m_nINT;
	destination->m_nPOW = m_nPOW;
	destination->m_nSIZ = m_nSIZ;
	destination->m_nSTR = m_nSTR;

	destination->m_strPlayerName = m_strPlayerName;

    destination->m_nSAN = m_nSAN;
    destination->m_nHitPoints = m_nHitPoints;
    destination->m_nMagic = m_nMagic;
    destination->m_strAffiliation = m_strAffiliation;
    destination->m_strCodeName = m_strCodeName;
    destination->m_strCurrentDate = m_strCurrentDate;
    destination->m_strInsanities = m_strInsanities;

    destination->m_ExtraSchooling = m_ExtraSchooling;
    destination->m_strIncome = m_strIncome;
    destination->m_strSpells = m_strSpells;
    destination->m_strRuleFile = m_strRuleFile;

    destination->m_strResidence = m_strResidence;
    destination->m_strPersonalDescription = m_strPersonalDescription;
    destination->m_strContacts = m_strContacts;
    destination->m_strWounds = m_strWounds;
    destination->m_strScars = m_strScars;
    destination->m_strHistory = m_strHistory;
    destination->m_strCashOnHand = m_strCashOnHand;
    destination->m_strSavings = m_strSavings;
    destination->m_strPersonalProperty = m_strPersonalProperty;
    destination->m_strRealEstate = m_strRealEstate;
    destination->m_strPossessions = m_strPossessions;
    destination->m_strTomesRead = m_strTomesRead;
    destination->m_strArtifacts = m_strArtifacts;
    destination->m_strEntitiesEncountered = m_strEntitiesEncountered;


    /* set the default size of all array elements */
    destination->m_AllSkillList.SetSize( 0, m_AllSkillList.GetSize() );
    destination->m_FirearmList.SetSize( 0, m_FirearmList.GetSize() );
    destination->m_MeleeList.SetSize( 0, m_MeleeList.GetSize() );
    destination->m_SkillList.SetSize( 0, m_SkillList.GetSize() );

    /* copy over the firearms */
    int n;
    for( n = 0; n < m_FirearmList.GetSize(); n++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_FirearmList.GetAt(n);
        if( pInvestigatorSkill )
        {
            CInvestigatorSkill* pInvestigatorSkillCopy = pInvestigatorSkill->GetCopy();
            destination->m_AllSkillList.Add( pInvestigatorSkillCopy );
            destination->m_FirearmList.Add( pInvestigatorSkillCopy );
        }
    }

    /* copy over melee weapons */
    for( n = 0; n < m_MeleeList.GetSize(); n++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_MeleeList.GetAt(n);
        if( pInvestigatorSkill )
        {
            CInvestigatorSkill* pInvestigatorSkillCopy = pInvestigatorSkill->GetCopy();
            destination->m_AllSkillList.Add( pInvestigatorSkillCopy );
            destination->m_MeleeList.Add( pInvestigatorSkillCopy );
        }
    }

    /* add all main skills */
    for( i = 0; i < m_SkillList.GetSize(); i++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_SkillList.GetAt(i);
        if( pInvestigatorSkill )
        {
            switch( pInvestigatorSkill->m_pSkill->m_SkillType )
            {
                case SUBSKILL: //don't add it directly
                    break;

                case SPECIALSKILL: //add it and search for all subsets
                {
                    CSpecialSkill* pSpecialSkill = (CSpecialSkill*)pInvestigatorSkill->m_pSkill;

                    /* add a raw copy of it */

                    //duplicate it
                    CInvestigatorSkill* pInvestigatorSkillCopy = pInvestigatorSkill->GetEmptyCopy();
                    CSpecialSkill* pSpecialSkillCopy = pSpecialSkill->GetEmptyCopy();
                    pInvestigatorSkillCopy->m_pSkill = pSpecialSkillCopy;

                    pSpecialSkillCopy->m_fManageSubskills = FALSE;

                    //add it to the list
                    destination->m_AllSkillList.Add( pInvestigatorSkillCopy );
                    destination->m_SkillList.Add( pInvestigatorSkillCopy );

                    /* add all subskills */
                    for( int nn = 0; nn < m_SkillList.GetSize(); nn++ )
                    {
                        CInvestigatorSkill* pInvestigatorSkill2 = m_SkillList.GetAt(nn);
                        if( pInvestigatorSkill2 )
                        {
                            //see if this is a subskill
                            if( pInvestigatorSkill2->m_pSkill->m_SkillType == SUBSKILL )
                            {
                                //see if this subskill is the one we're looking for
                                CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSkill2->m_pSkill;
                                if( pSubSkill->m_pSkill == pSpecialSkill )
                                {
                                    //find the index of this one on the parent
                                    int n = pSpecialSkill->GetSubSkillIndex( pSubSkill );
                                    if( n != -1 )
                                    {
                                        //create a copy of the subskill
                                        CInvestigatorSkill* pInvestigatorSkillCopy2 = pInvestigatorSkill2->GetCopy();

                                        //link it to it's parent
                                        CSubSkill* pSubSkill2 = (CSubSkill*)pInvestigatorSkillCopy2->m_pSkill;
                                        pSubSkill2->m_pSkill = pSpecialSkillCopy;

                                        //link the parent to it
                                        pSpecialSkillCopy->m_pSubSkills[n] = pSubSkill2;

                                        //add it to the list
                                        destination->m_AllSkillList.Add( pInvestigatorSkillCopy2 );
                                        destination->m_SkillList.Add( pInvestigatorSkillCopy2 );
                                    }
                                    else
                                        TRACE0( "Warning: Subskill without an index!\n" );
                                }
                            }
                        }
                    }
                    break;
                }

                default:
                {
                    //just add it
                    CInvestigatorSkill* pInvestigatorSkillCopy = pInvestigatorSkill->GetCopy();
                    destination->m_AllSkillList.Add( pInvestigatorSkillCopy );
                    destination->m_SkillList.Add( pInvestigatorSkillCopy );
                    break;
                }
            }
        }
    }

    //sort all skills into order
    destination->SortSkillList();
}

void CInvestigator::SortSkillList_Partition( CArray<CInvestigatorSkill*, CInvestigatorSkill*>& SkillArraySort,  const int nLowerBound, const int nUpperBound )
{
    //get the skill at the middle of this section of the array
    CInvestigatorSkill* pMiddleInvestigatorSkill = SkillArraySort.GetAt( (nLowerBound+nUpperBound) / 2 );

    //move the boundaries, sorting as we go
    int nLower = nLowerBound, nUpper = nUpperBound;
    while( nLower <= nUpper )
    {
        while( SkillArraySort.GetAt(nLower)->Compare( pMiddleInvestigatorSkill ) < 0 ) nLower++;
        while( SkillArraySort.GetAt(nUpper)->Compare( pMiddleInvestigatorSkill ) > 0 ) nUpper--;
        
        //see if we need to swap
        if( nLower < nUpper )
        {
            //swap them over
            CInvestigatorSkill* tmp = SkillArraySort.GetAt(nLower);
            SkillArraySort.SetAt(nLower, SkillArraySort.GetAt(nUpper) );
            SkillArraySort.SetAt(nUpper, tmp );

            //move boundaries
            nLower++; nUpper--;
        }
        else
            nLower++;
    }

    //split the array and sort each half
    if( nLowerBound < nUpper ) SortSkillList_Partition( SkillArraySort, nLowerBound, nUpper );
    if( nUpper+1 < nUpperBound ) SortSkillList_Partition( SkillArraySort, nUpper+1, nUpperBound );
}

void CInvestigator::SortSkillList()
{
    //set the quick sort going on each skill list
    if( m_AllSkillList.GetSize() > 0 ) SortSkillList_Partition( m_AllSkillList, 0, m_AllSkillList.GetUpperBound() );
    if( m_SkillList.GetSize() > 0 ) SortSkillList_Partition( m_SkillList, 0, m_SkillList.GetUpperBound() );
    if( m_FirearmList.GetSize() > 0 ) SortSkillList_Partition( m_FirearmList, 0, m_FirearmList.GetUpperBound() );
    if( m_MeleeList.GetSize() > 0 ) SortSkillList_Partition( m_MeleeList, 0, m_MeleeList.GetUpperBound() );
}


/***********************************************************************************************/

/* Script Stuff */






CScriptInvestigator::CScriptInvestigator( CInvestigator* pInvestigator )
{
    //store the investigator
    m_pInvestigator = pInvestigator;

    //copy basic attributes
    ReadStatsFromInvestigator();
}

CScriptInvestigator::~CScriptInvestigator()
{
    //copy back basic attributes
    WriteStatsToInvestigator();
}

Variant* CScriptInvestigator::GetMemberVariable( char* pszName )
{
    //return all members
    if( stricmp( pszName, "skillpoints" ) == 0 ) return &m_SkillPoints;
    if( stricmp( pszName, "interestpoints" ) == 0 ) return &m_InterestPoints;
    if( stricmp( pszName, "name" ) == 0 ) return &m_Name;
    if( stricmp( pszName, "occupation" ) == 0 ) return &m_Occupation;
    if( stricmp( pszName, "birthplace" ) == 0 ) return &m_Birthplace;
    if( stricmp( pszName, "colleges" ) == 0 ) return &m_Colleges;
    if( stricmp( pszName, "mentaldisorders" ) == 0 ) return &m_MentalDisorders;
    if( stricmp( pszName, "sex" ) == 0 ) return &m_Sex;
    if( stricmp( pszName, "age" ) == 0 ) return &m_Age;
    if( stricmp( pszName, "app" ) == 0 ) return &m_APP;
    if( stricmp( pszName, "con" ) == 0 ) return &m_CON;
    if( stricmp( pszName, "dex" ) == 0 ) return &m_DEX;
    if( stricmp( pszName, "edu" ) == 0 ) return &m_EDU;
    if( stricmp( pszName, "int" ) == 0 ) return &m_INT;
    if( stricmp( pszName, "pow" ) == 0 ) return &m_POW;
    if( stricmp( pszName, "siz" ) == 0 ) return &m_SIZ;
    if( stricmp( pszName, "str" ) == 0 ) return &m_STR;
    if( stricmp( pszName, "san" ) == 0 ) return &m_SAN;
    if( stricmp( pszName, "hp" ) == 0 ) return &m_HitPoints;
    if( stricmp( pszName, "magic" ) == 0 ) return &m_Magic;
    if( stricmp( pszName, "affiliation" ) == 0 ) return &m_Affiliation;
    if( stricmp( pszName, "codename" ) == 0 ) return &m_CodeName;
    if( stricmp( pszName, "currentdate" ) == 0 ) return &m_CurrentDate;
    if( stricmp( pszName, "insanities" ) == 0 ) return &m_Insanities;
    if( stricmp( pszName, "income" ) == 0 ) return &m_Income;
    if( stricmp( pszName, "spells" ) == 0 ) return &m_Spells;

    if( stricmp( pszName, "residence" ) == 0 ) return &m_Residence;
    if( stricmp( pszName, "personaldescription" ) == 0 ) return &m_PersonalDescription;
    if( stricmp( pszName, "contacts" ) == 0 ) return &m_Contacts;
    if( stricmp( pszName, "wounds" ) == 0 ) return &m_Wounds;
    if( stricmp( pszName, "scars" ) == 0 ) return &m_Scars;
    if( stricmp( pszName, "history" ) == 0 ) return &m_History;
    if( stricmp( pszName, "cashonhand" ) == 0 ) return &m_CashOnHand;
    if( stricmp( pszName, "savings" ) == 0 ) return &m_Savings;
    if( stricmp( pszName, "personalproperty" ) == 0 ) return &m_PersonalProperty;
    if( stricmp( pszName, "realestate" ) == 0 ) return &m_RealEstate;
    if( stricmp( pszName, "possessions" ) == 0 ) return &m_Possessions;
    if( stricmp( pszName, "tomesread" ) == 0 ) return &m_TomesRead;
    if( stricmp( pszName, "artifacts" ) == 0 ) return &m_Artifacts;
    if( stricmp( pszName, "entitiesencountered" ) == 0 ) return &m_EntitiesEncountered;
    
    return NULL;
}

CScriptObject::MEMBERRESULT CScriptInvestigator::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
    if( stricmp( pszName, "hasgotskill" ) == 0 )
    {
        if( nParameters == 1 ) //usage: if( investigator.hasgotskill("art") )...
        {
            //find the skill
            CString strName = LPCSTR(pParameters[0]);
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strName);

            //return it
            returnValue = (pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL) ? TRUE : FALSE;
            return OK;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;

    }

    if( stricmp( pszName, "setskillvalue" ) == 0 )
    {
        if( nParameters == 2 )//usage: investigator.setskillvalue( "name", n );
        {
            int nCurrent = pParameters[1];

            //find the skill
            CString strName = LPCSTR(pParameters[0]);
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strName);

            //return it
            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL  )
                pInvestigatorSkill->m_nCurrent = nCurrent;
            else
            {
                AfxMessageBox( CString("Script Error: Skill ") + strName + CString(" was not found"), MB_ICONEXCLAMATION );
                return UNKNOWN_ERROR;
            }
            return OK;
        }
        else
            return nParameters < 2 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    if( stricmp( pszName, "getskillvalue" ) == 0 )
    {
        if( nParameters == 1 )//usage: skill = investigator.getskillvalue( "name" );
        {
            //find the skill
            CString strName = LPCSTR(pParameters[0]);
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strName);

            //return it
            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL )
                returnValue = pInvestigatorSkill->m_nCurrent;
            else
            {
                AfxMessageBox( CString("Script Error: Skill ") + strName + CString(" was not found"), MB_ICONEXCLAMATION );
                return UNKNOWN_ERROR;
            }
            return OK;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    if( stricmp( pszName, "getskillsubsetcount" ) == 0 )
    {
        if( nParameters == 0 )//usage: investigator.getsubsetcount("name");
        {
            //find the skill
            CString strName = LPCSTR(pParameters[0]);
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strName);

            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
                returnValue = ((CSpecialSkill*)pInvestigatorSkill->m_pSkill)->m_nSkills;
            else
                returnValue = 0;
        }
        else
            return TOO_MANY_PARAMS;
    }
   
    if( stricmp( pszName, "setsubskill" ) == 0 )
    {
        if( nParameters >= 3 && nParameters <= 4 )//usage: investigator.setsubskill( "name", i, "name" [, value] );
        {
            //find the skill
            CString strName = LPCSTR(pParameters[0]);
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strName);

            int nSubSkill = int(pParameters[1]);
            CString strSubName = LPCSTR(pParameters[2]);
            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
            {
                CSpecialSkill* pSpecialSkill = (CSpecialSkill*)(pInvestigatorSkill->m_pSkill);
                if( nSubSkill < 0 || nSubSkill >= pSpecialSkill->m_nSkills )
                {
                    AfxMessageBox( "Subskill index out of range", MB_ICONEXCLAMATION );
                }
                else
                {
                    if( nParameters >= 4 )
                    {
                        CInvestigatorSkill* pInvestigatorSubSkill = m_pInvestigator->FindSubskill(pInvestigatorSkill, nSubSkill );
                        if(pInvestigatorSubSkill)pInvestigatorSubSkill->m_nCurrent = int(pParameters[3]);
                    }
                    pSpecialSkill->m_pSubSkills[nSubSkill]->m_strName = strSubName;
                    return OK;
                }
            }
            else
            {
                AfxMessageBox( CString("Script Error: Skill ") + strName + CString("Does not have any subskills"), MB_ICONEXCLAMATION );
                return UNKNOWN_ERROR;
            }
            return OK;
        }
        else
            return nParameters < 3 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }


    if( stricmp( pszName, "addskill" ) == 0 )//usage: investigator.addSkill( "name", ["0", [0]] );
    {
        //set return value
        returnValue = Variant::GetNULL();

        //make sure we've got enough parameters
        if( nParameters < 1 ) return TOO_FEW_PARAMS;
        if( nParameters > 3 ) return TOO_MANY_PARAMS;

        //get the parameters
        CString strName = pParameters[0];
        CString strBase = nParameters >= 2 ? pParameters[1] : "0";
        int nCurrent    = nParameters >= 3 ? pParameters[2] : 0;

        //see if this skill already exists
        if( m_pInvestigator->FindSkill(strName) != NULL )
        {
            AfxMessageBox( CString("Script Error: Skill ") + strName + CString(" already defined!"), MB_ICONEXCLAMATION );
            return UNKNOWN_ERROR;
        }

        //create this skill
        CSkill* pSkill = NULL;
        if( strName == "*" ) pSkill = new CSkill( ERA_ISALL ); else pSkill = new CSkill( strName, strBase, ERA_ISALL );
        CInvestigatorSkill* pInvestigatorSkill = new CInvestigatorSkill( pSkill, nCurrent, pSkill->EvaluateBase(m_pInvestigator) );

        //add it to the skill lists
        m_pInvestigator->m_AllSkillList.Add(pInvestigatorSkill);
        m_pInvestigator->m_SkillList.Add(pInvestigatorSkill);

        return OK;
    }
    if( stricmp( pszName, "addOccupationalSkill" ) == 0 )//usage: investigator.addOccupationalSkill( "name", ["0", [0]] );
    {
        //set return value
        returnValue = Variant::GetNULL();

        //make sure we've got enough parameters
        if( nParameters < 1 ) return TOO_FEW_PARAMS;
        if( nParameters > 3 ) return TOO_MANY_PARAMS;

        //get the parameters
        CString strName = pParameters[0];
        CString strBase = nParameters >= 2 ? pParameters[1] : "0";
        int nCurrent    = nParameters >= 3 ? pParameters[2] : 0;

        //see if this skill already exists
        if( m_pInvestigator->FindSkill(strName) != NULL )
        {
            AfxMessageBox( CString("Script Error: Skill ") + strName + CString(" already defined!"), MB_ICONEXCLAMATION );
            return UNKNOWN_ERROR;
        }

        //create this skill
        CSkill* pSkill = NULL;
        if( strName == "*" ) pSkill = new CSkill( ERA_ISALL ); else pSkill = new CSkill( strName, strBase, ERA_ISALL );
        CInvestigatorSkill* pInvestigatorSkill = new CInvestigatorSkill( pSkill, nCurrent, pSkill->EvaluateBase(m_pInvestigator) );
        pInvestigatorSkill->m_fOccupational = TRUE;

        //add it to the skill lists
        m_pInvestigator->m_AllSkillList.Add(pInvestigatorSkill);
        m_pInvestigator->m_SkillList.Add(pInvestigatorSkill);

        return OK;
    }


    if( stricmp( pszName, "recalculate" ) == 0 )
    {
        if( nParameters == 0 )
        {
            WriteStatsToInvestigator();
            m_pInvestigator->CalculateCoreStatistics();
            ReadStatsFromInvestigator();
            return OK;
        }
        else
            return TOO_MANY_PARAMS;
    }

    return NOT_FOUND;
}

void CScriptInvestigator::ReadStatsFromInvestigator()
{
	m_SkillPoints =             m_pInvestigator->m_nSkillPoints;
	m_InterestPoints =          m_pInvestigator->m_nInterestPoints;
	m_Name =                    m_pInvestigator->m_strName;
	m_Occupation =              m_pInvestigator->m_strOccupation;
	m_Birthplace =              m_pInvestigator->m_strBirthplace;
    m_Colleges =                m_pInvestigator->m_strColleges;
    m_MentalDisorders =         m_pInvestigator->m_strMentalDisorders;
    m_Sex =                     m_pInvestigator->m_strSex;
	m_Age =                     m_pInvestigator->m_nAge;
	m_APP =                     m_pInvestigator->m_nAPP;
	m_CON =                     m_pInvestigator->m_nCON;
	m_DEX =                     m_pInvestigator->m_nDEX;
	m_EDU =                     m_pInvestigator->m_nEDU;
	m_INT =                     m_pInvestigator->m_nINT;
	m_POW =                     m_pInvestigator->m_nPOW;
	m_SIZ =                     m_pInvestigator->m_nSIZ;
	m_STR =                     m_pInvestigator->m_nSTR;
    m_SAN =                     m_pInvestigator->m_nSAN;
    m_HitPoints =               m_pInvestigator->m_nHitPoints;
    m_Magic =                   m_pInvestigator->m_nMagic;
    m_Affiliation =             m_pInvestigator->m_strAffiliation;
    m_CodeName =                m_pInvestigator->m_strCodeName;
    m_CurrentDate =             m_pInvestigator->m_strCurrentDate;
    m_Insanities =              m_pInvestigator->m_strInsanities;
    m_Income =                  m_pInvestigator->m_strIncome;
    m_Spells =                  m_pInvestigator->m_strSpells;

    m_Residence =               m_pInvestigator->m_strResidence;
    m_PersonalDescription =     m_pInvestigator->m_strPersonalDescription;
    m_Contacts =                m_pInvestigator->m_strContacts;
    m_Wounds =                  m_pInvestigator->m_strWounds;
    m_Scars =                   m_pInvestigator->m_strScars;
    m_History =                 m_pInvestigator->m_strHistory;
    m_CashOnHand =              m_pInvestigator->m_strCashOnHand;
    m_Savings =                 m_pInvestigator->m_strSavings;
    m_PersonalProperty =        m_pInvestigator->m_strPersonalProperty;
    m_RealEstate =              m_pInvestigator->m_strRealEstate;
    m_Possessions =             m_pInvestigator->m_strPossessions;
    m_TomesRead =               m_pInvestigator->m_strTomesRead;
    m_Artifacts =               m_pInvestigator->m_strArtifacts;
    m_EntitiesEncountered =     m_pInvestigator->m_strEntitiesEncountered;
}

void CScriptInvestigator::WriteStatsToInvestigator()
{
	m_pInvestigator->m_nSkillPoints =           m_SkillPoints;
	m_pInvestigator->m_nInterestPoints =        m_InterestPoints;
	m_pInvestigator->m_strName =                m_Name;
	m_pInvestigator->m_strOccupation =          m_Occupation;
	m_pInvestigator->m_strBirthplace =          m_Birthplace;
    m_pInvestigator->m_strColleges =            m_Colleges;
    m_pInvestigator->m_strMentalDisorders =     m_MentalDisorders;
    m_pInvestigator->m_strSex =                 m_Sex;
	m_pInvestigator->m_nAge =                   m_Age;
	m_pInvestigator->m_nAPP =                   m_APP;
	m_pInvestigator->m_nCON =                   m_CON;
	m_pInvestigator->m_nDEX =                   m_DEX;
	m_pInvestigator->m_nEDU =                   m_EDU;
	m_pInvestigator->m_nINT =                   m_INT;
	m_pInvestigator->m_nPOW =                   m_POW;
	m_pInvestigator->m_nSIZ =                   m_SIZ;
	m_pInvestigator->m_nSTR =                   m_STR;
    m_pInvestigator->m_nSAN =                   m_SAN;
    m_pInvestigator->m_nHitPoints =             m_HitPoints;
    m_pInvestigator->m_nMagic =                 m_Magic;
    m_pInvestigator->m_strAffiliation =         m_Affiliation;
    m_pInvestigator->m_strCodeName =            m_CodeName;
    m_pInvestigator->m_strCurrentDate =         m_CurrentDate;
    m_pInvestigator->m_strInsanities =          m_Insanities;
    m_pInvestigator->m_strIncome =              m_Income;
    m_pInvestigator->m_strSpells =              m_Spells;

    m_pInvestigator->m_strResidence =           m_Residence;              
    m_pInvestigator->m_strPersonalDescription = m_PersonalDescription;    
    m_pInvestigator->m_strContacts =            m_Contacts;               
    m_pInvestigator->m_strWounds =              m_Wounds;                 
    m_pInvestigator->m_strScars =               m_Scars;                  
    m_pInvestigator->m_strHistory =             m_History;                
    m_pInvestigator->m_strCashOnHand =          m_CashOnHand;             
    m_pInvestigator->m_strSavings =             m_Savings;                
    m_pInvestigator->m_strPersonalProperty =    m_PersonalProperty;       
    m_pInvestigator->m_strRealEstate =          m_RealEstate;             
    m_pInvestigator->m_strPossessions =         m_Possessions;            
    m_pInvestigator->m_strTomesRead =           m_TomesRead;              
    m_pInvestigator->m_strArtifacts =           m_Artifacts;              
    m_pInvestigator->m_strEntitiesEncountered = m_EntitiesEncountered;    
}


CInvestigatorSkill* CInvestigator::FindSubskill(CInvestigatorSkill * pInvestigatorSpecialSkill, int nSubSkillIndex)
{
    if( pInvestigatorSpecialSkill->m_pSkill->m_SkillType != SPECIALSKILL ) return NULL;
    CSpecialSkill* pSpecialSkill = (CSpecialSkill*)pInvestigatorSpecialSkill->m_pSkill;
    if( nSubSkillIndex < 0 || nSubSkillIndex >= pSpecialSkill->m_nSkills ) return NULL;

    //scan down the main skill list
    for( int n = 0; n < m_SkillList.GetSize(); n++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_SkillList.GetAt(n);
        if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
        {
            CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSkill->m_pSkill;
            if( pSubSkill == pSpecialSkill->m_pSubSkills[nSubSkillIndex] && pSubSkill->m_pSkill == pSpecialSkill ) return pInvestigatorSkill;
        }
    }

    return NULL;
    
}

BOOL CInvestigatorSkill::IsBase(BOOL f5_5Mode )
{
    if( f5_5Mode && *LPCSTR(m_pSkill->m_strBase) != '*' )
        return (m_nMinimum == 0) ? (m_nCurrent <= 1) : (m_nMinimum == m_nCurrent);
    else
        return ( m_nMinimum == m_nCurrent );
}

BOOL CInvestigatorSkill::IsCalculatedBase()
{
    const char* pszBase = LPCSTR(m_pSkill->m_strBase);

    //skip over known, allowable prefixes
    if( *pszBase == 'F' || *pszBase == '*' ) pszBase++;

    //check whole string for non-numbers
    while( *pszBase != '\0' )
    {
        if( isdigit( (int)*pszBase ) == FALSE ) return TRUE;
        pszBase++;
    }

    return FALSE;

}

void CInvestigator::RollNewStats()
{
    //roll a base character
    CDiceRoller Dice;
    m_nSTR = Dice.Roll( "3D6" );
    m_nCON = Dice.Roll( "3D6" );
    m_nPOW = Dice.Roll( "3D6" );
    m_nDEX = Dice.Roll( "3D6" );
    m_nAPP = Dice.Roll( "3D6" );
    m_nSIZ = Dice.Roll( "2D6+6" );
    m_nINT = Dice.Roll( "2D6+6" );
    m_nEDU = Dice.Roll( "3D6+6" );

    //make sure investigator is old enough to be this educated!
    if( m_nAge < m_nEDU + 6) m_nAge = m_nEDU + 6;

    //recalculate things that depend on the rolled values
    CalculateSkillPoints();
    CalculateCoreStatistics();
    RecalculateMinimumSkillValues();
}

void CInvestigator::CalculateCoreStatistics()
{
    m_nSAN = m_nPOW * 5;
    m_nHitPoints = (int)( 0.5 + (((double)m_nCON + (double)m_nSIZ ) / 2.0 ) ); //fraction is rounded up
    m_nMagic = m_nPOW;
}

void CInvestigator::RecalculateMinimumSkillValues()
{
    int iMax = m_AllSkillList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        //calculate new minimum value
        m_AllSkillList[i]->m_nMinimum = m_AllSkillList[i]->m_pSkill->EvaluateBase(this);

        //change current value if it's not been allocated points
        if( m_AllSkillList[i]->m_nCurrent < m_AllSkillList[i]->m_nMinimum ) m_AllSkillList[i]->m_nCurrent = m_AllSkillList[i]->m_nMinimum;
    }
}

void CInvestigator::RunIncomeScript()
{
    //load income script
    CSectionFile SectionFile;
    SectionFile.Open( CByakheeApp::GetDefaultDirectory() + m_strRuleFile );
    CString strScript = SectionFile.GetSection("INCOME");

    //run income script
    CScriptingEngine ScriptEngine;
    ScriptEngine.SetOwnerWindow( AfxGetMainWnd()->m_hWnd );
    if( strScript.IsEmpty() == FALSE && ScriptEngine.SetScript( strScript ) )
    {
        //set script global variables
        Variant era = 1920;
        Variant income = "";
        switch( m_Era )
        {
            case Era_1890: era = 1890; break;
            case Era_1990: era = 1990; break;
        }

        //add the global objects
        ScriptEngine.AddGlobalVariable( "dice", new CScriptDice(), TRUE );
        ScriptEngine.AddGlobalVariable( "output", new CScriptOutput(), TRUE );
        ScriptEngine.AddGlobalVariable( "era", &era, TRUE );
        ScriptEngine.AddGlobalVariable( "income", &income, FALSE );

        //run the script
        CByakheeApp::ShowScriptExecutionQuickInfo();
        if( !ScriptEngine.ExecuteScript() )
            AfxMessageBox( "Unable to execute the income generation script", MB_ICONEXCLAMATION );
        else
            m_strIncome = LPCSTR(income);
    }
}

void CInvestigator::CalculateSkillPoints()
{
    //recalculate core results, taking schooling etc. into account
    if( m_ExtraSchooling == EXTRASCHOOLING_SUCCESS )
    {
        m_nSkillPoints = (m_nEDU-1) * 20;
        m_nInterestPoints = (m_nINT+1) * 10;
    }
    else
    {
        m_nSkillPoints = m_nEDU * 20;
        m_nInterestPoints = m_nINT * 10;
    }
}



void CInvestigatorSkill::SetValue(BOOL f5_5Base, int nValue)
{
    if( f5_5Base && *LPCSTR(m_pSkill->m_strBase) != '*' && m_pSkill->m_strBase.Compare("0") == 0 )
    {
        m_nCurrent = nValue - 1;
        if( m_nCurrent < 0 ) m_nCurrent = 0;
    }
    else
        m_nCurrent = nValue;
}

int CInvestigatorSkill::GetValue(BOOL f5_5Base)
{
    if( f5_5Base && *LPCSTR(m_pSkill->m_strBase) != '*' && m_pSkill->m_strBase.Compare("0") == 0 )
    {
        return m_nCurrent+1;
    }
    else
        return m_nCurrent;
}
