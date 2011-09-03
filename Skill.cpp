// Skill.cpp: implementation of the CSkill class hierarchy.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "Skill.h"
#include "Investigator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill( CString strName, CString strBase, ERA_MASK EraMask )
{
    m_strName = strName;
    m_strBase = strBase;
    m_EraMask = EraMask;
    m_SkillType = SKILL;
}

CSkill::CSkill()
{
    m_EraMask = ERA_ISNONE;
    m_SkillType = NO_TYPE;
}

CSkill::CSkill( ERA_MASK EraMask )
{
    m_EraMask = EraMask;
    m_SkillType = ANYSKILL;
}

CSkill::~CSkill()
{

}

CSkill* CSkill::GetCopy()
{
    switch( m_SkillType )
    {
        case SKILL: return new CSkill( m_strName, m_strBase, m_EraMask );
        case ANYSKILL: return new CSkill( m_EraMask );
        default: return new CSkill();
    }        
}

int CSkill::EvaluateBase(CInvestigator * pInvestigator)
{
	const char* pszCode = LPCSTR(m_strBase);
	int nResult = 0;

    BOOL fGotStatMultiplier = FALSE;
    ASSERT(pInvestigator);

	while( pszCode && *pszCode != '\0' )
	{
		switch( *pszCode )
		{
			case '^': //'^' == skill stat
			{
				//get the stat on the end of the '^'
				CString strSkill = "";
				pszCode++;
				while( isalpha(*pszCode) && *pszCode != '\0' ) 
				{
					strSkill = strSkill + *pszCode;
					pszCode++;
				}

                int nStat = pInvestigator->GetAttributeByName( strSkill );
                if( nStat == -666 ) return -666;
				nResult += nStat;
                fGotStatMultiplier = TRUE;
				break;	
			}

			case '*':  //'*' == Can't modify
            case 'F':  //'F' == core firearm
				pszCode++; 
				break;

			case '%':
			{
				//read in the skill
				CString strSkill = "";
				pszCode++;
				while( (*pszCode) != '%' && *pszCode != '\0' ) 
				{
					strSkill = strSkill + *pszCode;
					pszCode++;
				}
				pszCode++;

                CInvestigatorSkill* pInvestigatorSkill = pInvestigator->FindSkill( strSkill );
                if( pInvestigatorSkill == NULL )
                {
	                CString str;
	                str.Format( "Unknown skill name: %s", strSkill );
	                AfxMessageBox( str, MB_ICONEXCLAMATION );
                    return -666;
                }
                else
				    nResult += pInvestigatorSkill->m_nCurrent;
				break;	
			}

			case '0':
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
				//rip out the number
				CString strValue = "";
				while( isdigit(*pszCode) && *pszCode != '\0' ) 
				{
					strValue = strValue + *pszCode;
					pszCode++;
				}

				if( strValue.GetLength() > 0 )
					if( fGotStatMultiplier )
						nResult *= atoi( LPCSTR(strValue) );
					else
						nResult = atoi( LPCSTR(strValue) );
				break;	
			}

			default:
				pszCode++;
				break;

		}
	}

	return nResult;
}


BOOL CSkill::IsEra(Era era)
{
    switch( era )
    {
        case Era_1890: return ( m_EraMask & ERA_IS1890 );
        case Era_1920: return ( m_EraMask & ERA_IS1920 );
        case Era_1990: return ( m_EraMask & ERA_IS1990 );
        default: 
            TRACE0("Warning: IsEra() - Unknown era\n" );
            return FALSE;
    }
}




CFirearmSkill::CFirearmSkill( CString strName, CString strBase, ERA_MASK EraMask, CString strDmg, CString strRange, CString strShots, CString strBullets, CString strHP, CString strMAL )
{
    m_strName = strName;
    m_strBase = strBase;
    m_EraMask = EraMask;
    m_strDmg = strDmg;
    m_strRange = strRange;
    m_strShots = strShots;
    m_strBullets = strBullets;
    m_strHP = strHP;
    m_strMAL = strMAL;
    m_SkillType = FIREARM;
}

CFirearmSkill::CFirearmSkill( ERA_MASK EraMask )
{
    m_EraMask = EraMask;
    m_SkillType = FIREARM;
}

CFirearmSkill::~CFirearmSkill()
{
}

CSkill* CFirearmSkill::GetCopy()
{
    return new CFirearmSkill( m_strName, m_strBase, m_EraMask, m_strDmg, m_strRange, m_strShots, m_strBullets, m_strHP, m_strMAL );
}





CMeleeSkill::CMeleeSkill( CString strName, CString strBase, ERA_MASK EraMask, CString strDmg, CString strAttacks, CString strHP )
{
    m_strName = strName;
    m_strBase = strBase;
    m_EraMask = EraMask;
    m_strDmg = strDmg;
    m_strAttacks = strAttacks;
    m_strHP = strHP;
    m_SkillType = MELEE;

}

CMeleeSkill::CMeleeSkill( ERA_MASK EraMask )
{
    m_EraMask = EraMask;
    m_SkillType = MELEE;
}

CMeleeSkill::~CMeleeSkill()
{
}

CSkill* CMeleeSkill::GetCopy()
{
    return new CMeleeSkill( m_strName, m_strBase, m_EraMask, m_strDmg, m_strAttacks, m_strHP );
}




CSpecialSkill::CSpecialSkill( CString strName, CString strBase, ERA_MASK EraMask, int nSubSkills, BOOL fCreateSubskills/*TRUE*/ )
{
    //initialise members
    m_strName = strName;
    m_strBase = strBase;
    m_EraMask = EraMask;
    m_nSkills = nSubSkills;
    m_SkillType = SPECIALSKILL;

    //create the subskill array
    m_pSubSkills = new CSubSkill*[m_nSkills];

    m_fManageSubskills = fCreateSubskills;
    if( m_fManageSubskills )
    {
        //create all subskills
        for( int i = 0; i < m_nSkills; i++ ) m_pSubSkills[i] = new CSubSkill( this, strBase, EraMask );
    }
    else
    {
        //set all subskills as blank
        for( int i = 0; i < m_nSkills; i++ ) m_pSubSkills[i] = NULL;
    }
}

CSpecialSkill::~CSpecialSkill()
{
    //delete all subskills
    if( m_fManageSubskills )
    {
        for( int i = 0; i < m_nSkills; i++ ) if( m_pSubSkills[i] ) delete m_pSubSkills[i];
    }

    //delete the subskill array
    delete[] m_pSubSkills;
}



CSkill* CSpecialSkill::GetCopy()
{
    //create a copy of ourselves
    CSpecialSkill* pSpecialSkill = new CSpecialSkill( m_strName, m_strBase, m_EraMask, m_nSkills, FALSE );

    //copy all subskills too and link them to the new skill
    for( int n = 0; n < m_nSkills; n++ )
    {
        pSpecialSkill->m_pSubSkills[n] = (CSubSkill*)m_pSubSkills[n]->GetCopy();
        pSpecialSkill->m_pSubSkills[n]->m_pSkill = pSpecialSkill;
    }

    //let the copy manage it's subskills
    pSpecialSkill->m_fManageSubskills = TRUE;

    return pSpecialSkill;
}

CSpecialSkill* CSpecialSkill::GetEmptyCopy()
{
    //create a raw copy of ourselves
    CSpecialSkill* pSpecialSkill = new CSpecialSkill( m_strName, m_strBase, m_EraMask, m_nSkills, FALSE );
    return pSpecialSkill;
}

int CSpecialSkill::GetSubSkillIndex(const CSubSkill * pSubSkill)
{
    for( int i = 0; i < m_nSkills; i++ ) if( m_pSubSkills[i] == pSubSkill ) return i;

    return -1;
}

CSubSkill::CSubSkill( CSpecialSkill* pSkill, CString strBase, ERA_MASK EraMask )
{
    m_pSkill = pSkill;
    m_strBase = strBase;
    m_EraMask = EraMask;
    m_strName = "";
    m_SkillType = SUBSKILL;
}

CSubSkill::~CSubSkill()
{

}

CSkill* CSubSkill::GetCopy()
{
    CSubSkill* pSubSkill = new CSubSkill( NULL, m_strBase, m_EraMask );
    pSubSkill->m_strName = m_strName;
    return pSubSkill;
}



CString CSkill::GetBaseString( BOOL f5_5Mode, BOOL fShowPercent /*TRUE*/ )
{
	const char* pszCode = LPCSTR(m_strBase);
	CString strResult = "";

    BOOL fNeedPercent = fShowPercent;

	while( pszCode && *pszCode != '\0' )
	{
		switch( *pszCode )
		{
			case '^': //'^' == skill stat
			{
				//get the stat on the end of the '^'
				CString strSkill = "";
				pszCode++;
				while( isalpha(*pszCode) && *pszCode != '\0' ) 
				{
					strSkill = strSkill + *pszCode;
					pszCode++;
				}

				if( strResult.GetLength() > 0 ) strResult += "+";
				strResult += strSkill;
                fNeedPercent = FALSE;
				break;	
			}

			case '*':  //'*' == Can't modify
            case 'F':  //'F' == core firearm
				pszCode++; 
				break;

			case '%':
			{
				//read in the skill
				CString strSkill = "";
				pszCode++;
				while( (*pszCode) != '%' && *pszCode != '\0' ) 
				{
                    char chCode = *pszCode; if( chCode == '_' ) chCode = ' ';
					strSkill = strSkill + chCode;
					pszCode++;
				}
				pszCode++;

				strResult += strSkill;
                fNeedPercent = FALSE;
				break;	
			}

			case '0':
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
				//rip out the number
                BOOL fDoneLeadingZero = (*pszCode != '0');
				CString strValue = "";
				while( isdigit(*pszCode) && *pszCode != '\0' ) 
				{
                    if( *pszCode != '0' ) fDoneLeadingZero = TRUE;
                    if( fDoneLeadingZero ) strValue = strValue + *pszCode;
					pszCode++;
				}

				if( strResult.GetLength() > 0 )
				{
					strResult += "x";
					if( strValue != "1" ) strResult += strValue;
                    fNeedPercent = FALSE;
				}
				else
                {
                    //slap on a leading 0
                    if( atoi( LPCSTR(strValue)) < 10 ) strValue = "0" + strValue;
                    strResult += strValue;
                    fNeedPercent = TRUE;
                }
				break;	
			}

			default:
				strResult += *pszCode;
				pszCode++;
                fNeedPercent = FALSE;
				break;

		}
	}

    //final checks
    if( strResult.Compare("0") == 0 )
    {
        strResult = f5_5Mode && *LPCSTR(m_strBase) != '*' ? "01" : "00"; 
        fNeedPercent = f5_5Mode; 
    }
    else
        if( strResult.Compare("00") == 0 ) fNeedPercent = FALSE;
    
    if( fNeedPercent && fShowPercent ) strResult += "%";

	return strResult;
}
