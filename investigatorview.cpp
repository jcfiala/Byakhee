// InvestigatorView.cpp : implementation of the CInvestigatorView class
//
/**
 * Copyright (C) 1998  David Harvey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "stdafx.h"
#include <math.h>
#include "Byakhee.h"
#include "MainFrm.h"
#include "InvestigatorDoc.h"
#include "InvestigatorView.h"
#include "ScriptEngine/ScriptObject.h"
#include "ScriptEngine/Variant.h"
#include "SectionFile.h"
#include "ParseBuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CInvestigatorView

BOOL CInvestigatorView::m_fDrawV5_5Bases = TRUE;
BOOL CInvestigatorView::m_fShowAllSkillPercentiles = FALSE;
BOOL CInvestigatorView::m_fCustomSkillsInUserText = TRUE;
BOOL CInvestigatorView::m_fTrimToMinimum = TRUE;

IMPLEMENT_DYNCREATE(CInvestigatorView, CScriptSheetView)

BEGIN_MESSAGE_MAP(CInvestigatorView, CScriptSheetView)
	//{{AFX_MSG_MAP(CInvestigatorView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_VIEW_LIKECTHULHU5_5, OnViewLikeCthulhu55)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKECTHULHU5_5, OnUpdateViewLikeCthulhu55)
	ON_COMMAND(ID_VIEW_CUSTOMSKILLSINUSERTEXT, OnViewCustomskillsInUserText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_VIEW_ALLSKILLPERCENTILES, OnViewAllSkillPercentiles)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALLSKILLPERCENTILES, OnUpdateViewAllSkillPercentiles)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CUSTOMSKILLSINUSERTEXT, OnUpdateViewCustomSkillsInUserText)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLIPSKILLPOINTS, OnUpdateToolsClipSkillPoints)
	ON_COMMAND(ID_TOOLS_CLIPSKILLPOINTS, OnToolsClipSkillPoints)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorView construction/destruction

CInvestigatorView::CInvestigatorView()
{
    m_fCustomSkillsInUserText = AfxGetApp()->GetProfileInt( "Sheet", "CustSkillField", m_fCustomSkillsInUserText );

    //load view settings
    m_fDrawV5_5Bases = AfxGetApp()->GetProfileInt( "Sheet", "V5.5Base", m_fDrawV5_5Bases );
    m_fShowAllSkillPercentiles = AfxGetApp()->GetProfileInt( "Sheet", "AllPercent", m_fShowAllSkillPercentiles );
    m_fTrimToMinimum = AfxGetApp()->GetProfileInt( "Sheet", "TrimSkills", m_fTrimToMinimum );

    //add field parameters to the sheet
    AddSheetParameter( "shortsheet",       SHEETPARAM_SHORT );
    AddSheetParameter( "showcorefirearms", SHEETPARAM_SHOWCOREFIREARMS );
    AddSheetParameter( "showbasepercent",  SHEETPARAM_SHOWBASEPERCENT );

    //load the abbreviations
    CSectionFile SectionFile;
    SectionFile.Open( CByakheeApp::GetDefaultDirectory() + "options.lst" );

    //build the abbreviations list
    CParseBuffer ParseBuffer( SectionFile.GetSection( "Abbreviations" ) );
    while( ParseBuffer.IsEmpty() == FALSE )
    {
        Abbreviation a;
        a.strOriginal = ParseBuffer.ExtractQuotedString();
        a.strShort = ParseBuffer.ExtractQuotedString();
        if( !ParseBuffer.IsParseError() && !ParseBuffer.IsUnderflow() ) m_Abbreviations.Add( a );
    }            
}

CInvestigatorView::~CInvestigatorView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorView diagnostics

#ifdef _DEBUG
void CInvestigatorView::AssertValid() const
{
	CScriptSheetView::AssertValid();
}

void CInvestigatorView::Dump(CDumpContext& dc) const
{
	CScriptSheetView::Dump(dc);
}

CInvestigatorDoc* CInvestigatorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInvestigatorDoc)));
	return (CInvestigatorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorView member functions

void CInvestigatorView::OnInitialUpdate() 
{
    //get the document
	CInvestigatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    //initialise the editing controls
    m_wndFieldEdit.SetNamedOptionManager( pDoc->GetNamedOptionManager() );
    
    //get the investigator
    CInvestigator& Investigator = pDoc->m_Investigator;

    //set the script file extension that we're going to use
    SetSheetFileExtension("bis");
    m_strSheet = SkillManager.GetKeyPairValue( Investigator.m_strRuleFile, "DefaultScript", "" );
    if( pDoc->m_strLastSheet.IsEmpty() == FALSE ) m_strSheet = pDoc->m_strLastSheet;

    //display initial skill points in status bar
    pDoc->UpdateStatusBar();

    //pass init onto superclass
	CScriptSheetView::OnInitialUpdate();
}


LPCSTR CInvestigatorView::Abbreviate(const char * pszSkillName)
{
    int iMax = m_Abbreviations.GetSize();
    for( int i = 0; i < iMax; i++ ) if( m_Abbreviations[i].strOriginal.CompareNoCase(pszSkillName) == 0 ) return LPCSTR(m_Abbreviations[i].strShort);

    return pszSkillName;
}

void CInvestigatorView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    CInvestigatorDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnUpdateEditUndo( pCmdUI );
    CScriptSheetView::OnUpdateEditUndo(pCmdUI);
}

void CInvestigatorView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
    CInvestigatorDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnUpdateEditRedo( pCmdUI );
    CScriptSheetView::OnUpdateEditRedo(pCmdUI);
}


void CInvestigatorView::OnEditUndo() 
{
    CInvestigatorDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnEditUndo();
    CScriptSheetView::OnEditUndo();
}

void CInvestigatorView::StoreUndo()
{
    CInvestigatorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    pDoc->StoreUndo();
    
    //assume it's been modified as this is called by editing sheet commands
    pDoc->SetModifiedFlag();
}

void CInvestigatorView::OnViewLikeCthulhu55() 
{
    m_fDrawV5_5Bases	= !m_fDrawV5_5Bases;
    AfxGetApp()->WriteProfileInt( "Sheet", "V5.5Base", m_fDrawV5_5Bases );

    GetDocument()->UpdateAllViews(NULL);
}

void CInvestigatorView::OnViewCustomskillsInUserText() 
{
	m_fCustomSkillsInUserText = !m_fCustomSkillsInUserText;
    AfxGetApp()->WriteProfileInt( "Sheet", "CustSkillField", m_fCustomSkillsInUserText );

    GetDocument()->UpdateAllViews(NULL);
}

void CInvestigatorView::OnViewAllSkillPercentiles() 
{
    m_fShowAllSkillPercentiles = !m_fShowAllSkillPercentiles;
    AfxGetApp()->WriteProfileInt( "Sheet", "AllPercent", m_fShowAllSkillPercentiles );

    GetDocument()->UpdateAllViews(NULL);
}

void CInvestigatorView::OnToolsClipSkillPoints() 
{
    m_fTrimToMinimum = !m_fTrimToMinimum;
    AfxGetApp()->WriteProfileInt( "Sheet", "TrimSkills", m_fTrimToMinimum );

    GetDocument()->UpdateAllViews(NULL);
}



void CInvestigatorView::OnUpdateViewLikeCthulhu55(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck( m_fDrawV5_5Bases );
}

void CInvestigatorView::OnUpdateViewCustomSkillsInUserText(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck( m_fCustomSkillsInUserText );
}

void CInvestigatorView::OnUpdateViewAllSkillPercentiles(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck( m_fShowAllSkillPercentiles );
}

void CInvestigatorView::OnUpdateToolsClipSkillPoints(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck( m_fTrimToMinimum );
}

void CInvestigatorView::AddAllFields()
{
    CInvestigatorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CInvestigator& I = pDoc->m_Investigator;

    /* add all fields */

    //first, the easy ones
    AddField( &I.m_strName,                 "name",                 FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strOccupation,           "occupation",           FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strBirthplace,           "birthplace",           FIELD_SCRIPTREAD|FIELD_TABSTOP, "birthplace" );
    AddField( &I.m_strColleges,             "collegesDegrees",      FIELD_SCRIPTREAD|FIELD_TABSTOP, "CollegesDegrees" );
    AddField( &I.m_strMentalDisorders,      "mentalDisorders",      FIELD_SCRIPTREAD|FIELD_TABSTOP, "Phobias" );
    AddField( &I.m_strSex,                  "sex",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nAge,                    "age",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nAPP,                    "app",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nCON,                    "con",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nDEX,                    "dex",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nEDU,                    "edu",                  FIELD_SCRIPTREAD|FIELD_TABSTOP|FIELD_CALCRELIANT );
    AddField( &I.m_nINT,                    "int",                  FIELD_SCRIPTREAD|FIELD_TABSTOP|FIELD_CALCRELIANT );
    AddField( &I.m_nPOW,                    "pow",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nSIZ,                    "siz",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nSTR,                    "str",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nSAN,                    "san",                  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nHitPoints,              "hitPoints",            FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_nMagic,                  "magic",                FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strAffiliation,          "affiliation",          FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strCodeName,             "codeName",             FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strCurrentDate,          "currentDate",          FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strInsanities,           "insanities",           FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strIncome,               "income",               FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strSpells,               "spells",               FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strResidence,            "residence",            FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strPersonalDescription,  "personalDescription",  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strContacts,             "contacts",             FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strWounds,               "wounds",               FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strScars,                "scars",                FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strHistory,              "history",              FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strCashOnHand,           "cashonhand",           FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strSavings,              "savings",              FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strPersonalProperty,     "personalProperty",     FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strRealEstate,           "realEstate",           FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strPossessions,          "possessions",          FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strTomesRead,            "tomesRead",            FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strArtifacts,            "artifacts",            FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strEntitiesEncountered,  "entitiesEncountered",  FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( &I.m_strPlayerName,           "playerName",           FIELD_SCRIPTREAD|FIELD_TABSTOP );

    //and the portrait (we could use &I.m_strPortrait, but the document loads one as well, which is cleaner)
    AddField( FIELDTYPE_PORTRAIT,           "portrait",             FIELD_SCRIPTREAD|FIELD_PICTURE );

    //now the calculated ones...
    AddField( FIELDTYPE_MAXSAN,             "maxsan",               FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( FIELDTYPE_CTHULHUMYTHOS,      "cthulhuMythos",        FIELD_SCRIPTREAD|FIELD_TABSTOP );
    AddField( FIELDTYPE_ERA,                "era",                  FIELD_SCRIPTREAD );
    AddField( FIELDTYPE_STARTINGSAN,        "startingSan",          FIELD_SCRIPTREAD|FIELD_READONLY );
    AddField( FIELDTYPE_LUCK,               "luck",                 FIELD_SCRIPTREAD|FIELD_READONLY );
    AddField( FIELDTYPE_KNOW,               "know",                 FIELD_SCRIPTREAD|FIELD_READONLY );
    AddField( FIELDTYPE_IDEA,               "idea",                 FIELD_SCRIPTREAD|FIELD_READONLY );
    AddField( FIELDTYPE_DAMAGEBONUS,        "damageBonus",          FIELD_SCRIPTREAD|FIELD_READONLY );

    //now the skills...
    AddField( FIELDTYPE_SKILL,              "skill",                FIELD_READONLY|FIELD_NOTACTIVE );
    AddField( FIELDTYPE_SKILL_CHECK,        "skill_check",          FIELD_BOOLEAN );
    AddField( FIELDTYPE_SKILL_NAME,         "skill_name",           FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_SKILL_PERCENT,      "skill_percent",        FIELD_NORMAL|FIELD_TABSTOP );

    //now the firearm skills....
    AddField( FIELDTYPE_FIREARM,            "firearm",              FIELD_READONLY|FIELD_NOTACTIVE );
    AddField( FIELDTYPE_FIREARM_CHECK,      "firearm_check",        FIELD_BOOLEAN );
    AddField( FIELDTYPE_FIREARM_NAME,       "firearm_name",         FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_PERCENT,    "firearm_percent",      FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_DAMAGE,     "firearm_damage",       FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_RANGE,      "firearm_range",        FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_SHOTS,      "firearm_shots",        FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_BULLETS,    "firearm_bullets",      FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_HITPOINTS,  "firearm_hitpoints",    FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_FIREARM_MAL,        "firearm_mal",          FIELD_NORMAL|FIELD_TABSTOP );

    //melee skills..
    AddField( FIELDTYPE_MELEE,              "melee",                FIELD_READONLY|FIELD_NOTACTIVE );
    AddField( FIELDTYPE_MELEE_CHECK,        "melee_check",          FIELD_BOOLEAN );
    AddField( FIELDTYPE_MELEE_NAME,         "melee_name",           FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_MELEE_PERCENT,      "melee_percent",        FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_MELEE_DAMAGE,       "melee_damage",         FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_MELEE_ATTACKS,      "melee_attacks",        FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_MELEE_HITPOINTS,    "melee_hitpoints",      FIELD_NORMAL|FIELD_TABSTOP );

    //and finally, the melee and/or firearm skills...
    AddField( FIELDTYPE_WEAPON,             "weapon",               FIELD_READONLY|FIELD_NOTACTIVE );
    AddField( FIELDTYPE_WEAPON_CHECK,       "weapon_check",         FIELD_BOOLEAN );
    AddField( FIELDTYPE_WEAPON_NAME,        "weapon_name",          FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_PERCENT,     "weapon_percent",       FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_DAMAGE,      "weapon_damage",        FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_HITPONTS,    "weapon_hitpoints",     FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_BULLETS,     "weapon_bullets",       FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_RANGE,       "weapon_range",         FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_ATTACKS,     "weapon_attacks",       FIELD_NORMAL|FIELD_TABSTOP );
    AddField( FIELDTYPE_WEAPON_MAL,         "weapon_mal",           FIELD_NORMAL|FIELD_TABSTOP );

    //call the base class
    CScriptSheetView::AddAllFields();
}

Variant CInvestigatorView::GetFieldValue( CField* pField, CFieldSheetCommand* pFieldSheetCommand /*NULL*/  )
{
    CInvestigatorSkill* pInvestigatorSkill = pFieldSheetCommand ? ((CInvestigatorSkill*)pFieldSheetCommand->m_dwParam1) : NULL;
    switch( int(pField->m_Code) )
    {
        //portrait
        case FIELDTYPE_PORTRAIT:
            return LPCSTR(GetDocument()->m_Investigator.m_strPortrait);
            break;

        //skill types
        case FIELDTYPE_SKILL_CHECK:     
        case FIELDTYPE_FIREARM_CHECK:
        case FIELDTYPE_MELEE_CHECK:
        case FIELDTYPE_WEAPON_CHECK:        if( pInvestigatorSkill ) return pInvestigatorSkill->m_fChecked; break;

        case FIELDTYPE_SKILL_NAME:      
        case FIELDTYPE_FIREARM_NAME:
        case FIELDTYPE_MELEE_NAME:
        case FIELDTYPE_WEAPON_NAME:         
            if( pInvestigatorSkill ) 
            {
                const char* pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
                return pszName;
                break;
            }

        case FIELDTYPE_SKILL_PERCENT:   
        case FIELDTYPE_FIREARM_PERCENT:
        case FIELDTYPE_MELEE_PERCENT:
        case FIELDTYPE_WEAPON_PERCENT:      
            if( pInvestigatorSkill ) return pInvestigatorSkill->GetValue( m_fDrawV5_5Bases );
                /*
            {
                if( pInvestigatorSkill->m_nCurrent == 0 && m_fDrawV5_5Bases && *LPCSTR(pInvestigatorSkill->m_pSkill->m_strBase) != '*' )
                    return 1;
                else
                    return pInvestigatorSkill->m_nCurrent;  
            }*/
            break;

        case FIELDTYPE_FIREARM_DAMAGE:      if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg);  break;
        case FIELDTYPE_FIREARM_RANGE:       if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strRange);  break;
        case FIELDTYPE_FIREARM_SHOTS:       if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strShots);  break;
        case FIELDTYPE_FIREARM_BULLETS:     if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strBullets);  break;
        case FIELDTYPE_FIREARM_HITPOINTS:   if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strHP);  break;
        case FIELDTYPE_FIREARM_MAL:         if( pInvestigatorSkill ) return LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strMAL);  break;

        case FIELDTYPE_MELEE_DAMAGE:        if( pInvestigatorSkill ) return LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg); break;
        case FIELDTYPE_MELEE_ATTACKS:       if( pInvestigatorSkill ) return LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strAttacks);  break;
        case FIELDTYPE_MELEE_HITPOINTS:     if( pInvestigatorSkill ) return LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strHP);  break;

        case FIELDTYPE_WEAPON_DAMAGE:       if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg) : LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg); break;
        case FIELDTYPE_WEAPON_HITPONTS:     if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strHP) : LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strHP); break;
        case FIELDTYPE_WEAPON_BULLETS:      if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strBullets) : ""; break;
        case FIELDTYPE_WEAPON_RANGE:        if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strRange) : ""; break;
        case FIELDTYPE_WEAPON_ATTACKS:      if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strShots) : LPCSTR(((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strAttacks); break;
        case FIELDTYPE_WEAPON_MAL:          if( pInvestigatorSkill ) return pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ? LPCSTR(((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strMAL) : ""; break;

        //others
        case FIELDTYPE_MAXSAN:
            pInvestigatorSkill = GetDocument()->m_Investigator.FindSkill("Cthulhu Mythos");
            return int(99 - (pInvestigatorSkill ? pInvestigatorSkill->m_nCurrent : 0) );

        case FIELDTYPE_CTHULHUMYTHOS:
            pInvestigatorSkill = GetDocument()->m_Investigator.FindSkill("Cthulhu Mythos");
            return int(pInvestigatorSkill ? pInvestigatorSkill->m_nCurrent : 0);

        case FIELDTYPE_STARTINGSAN:         return int(GetDocument()->m_Investigator.m_nPOW * 5);
        case FIELDTYPE_LUCK:                return int(GetDocument()->m_Investigator.m_nPOW * 5);
        case FIELDTYPE_KNOW:                return int(GetDocument()->m_Investigator.m_nEDU * 5);
        case FIELDTYPE_IDEA:                return int(GetDocument()->m_Investigator.m_nINT * 5);
        case FIELDTYPE_DAMAGEBONUS:         return LPCSTR(GetDocument()->m_Investigator.GetDBString());
        case FIELDTYPE_ERA:
            switch( GetDocument()->m_Investigator.m_Era )
            {
                case Era_1890: return "1890";
                default:
                case Era_1920: return "1920";
                case Era_1990: return "1990";
            }
            break;
    }

    return CScriptSheetView::GetFieldValue(pField, pFieldSheetCommand );
}

void CInvestigatorView::SetFieldValue( CField* pField, Variant value, CFieldSheetCommand* pFieldSheetCommand /*NULL*/ )
{
    CInvestigatorSkill* pInvestigatorSkill = pFieldSheetCommand ? ((CInvestigatorSkill*)pFieldSheetCommand->m_dwParam1) : NULL;
    switch( int(pField->m_Code) )
    {
        //portrait
        case FIELDTYPE_PORTRAIT:
            GetDocument()->SetPortrait( LPCSTR(value) );
            break;

        //skill types
        case FIELDTYPE_SKILL_CHECK:     
        case FIELDTYPE_FIREARM_CHECK:
        case FIELDTYPE_MELEE_CHECK:
        case FIELDTYPE_WEAPON_CHECK:        
            if( pInvestigatorSkill ) pInvestigatorSkill->m_fChecked = int(value); break;

        case FIELDTYPE_SKILL_NAME:      
        case FIELDTYPE_FIREARM_NAME:
        case FIELDTYPE_MELEE_NAME:
        case FIELDTYPE_WEAPON_NAME:         
            if( pInvestigatorSkill ) pInvestigatorSkill->m_pSkill->m_strName = LPCSTR(value); break;

        case FIELDTYPE_SKILL_PERCENT:
        case FIELDTYPE_FIREARM_PERCENT:
        case FIELDTYPE_MELEE_PERCENT:
        case FIELDTYPE_WEAPON_PERCENT:
            if( pInvestigatorSkill )
            {
                //recalculate minimum value now
                pInvestigatorSkill->m_nMinimum = pInvestigatorSkill->m_pSkill->EvaluateBase( &GetDocument()->m_Investigator );

                //see if the new value is less than the (new) minimum value
                int nValue = int(value);
                if( m_fTrimToMinimum && nValue < pInvestigatorSkill->m_nMinimum ) nValue = pInvestigatorSkill->m_nMinimum;
                //pInvestigatorSkill->m_nCurrent = nValue;
                pInvestigatorSkill->SetValue( m_fDrawV5_5Bases, nValue );
                break;
            }

        case FIELDTYPE_FIREARM_DAMAGE:      if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg = LPCSTR(value);  break;
        case FIELDTYPE_FIREARM_RANGE:       if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strRange = LPCSTR(value);  break;
        case FIELDTYPE_FIREARM_SHOTS:       if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strShots = LPCSTR(value);  break;
        case FIELDTYPE_FIREARM_BULLETS:     if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strBullets = LPCSTR(value);  break;
        case FIELDTYPE_FIREARM_HITPOINTS:   if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strHP = LPCSTR(value);  break;
        case FIELDTYPE_FIREARM_MAL:         if( pInvestigatorSkill ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strMAL = LPCSTR(value);  break;

        case FIELDTYPE_MELEE_DAMAGE:        if( pInvestigatorSkill ) ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg = LPCSTR(value); break;
        case FIELDTYPE_MELEE_ATTACKS:       if( pInvestigatorSkill ) ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strAttacks = LPCSTR(value);  break;
        case FIELDTYPE_MELEE_HITPOINTS:     if( pInvestigatorSkill ) ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strHP = LPCSTR(value);  break;

        case FIELDTYPE_WEAPON_DAMAGE:       if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg = LPCSTR(value); else ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg = LPCSTR(value); break;
        case FIELDTYPE_WEAPON_HITPONTS:     if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strHP = LPCSTR(value); else ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strHP = LPCSTR(value); break;
        case FIELDTYPE_WEAPON_BULLETS:      if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strBullets = LPCSTR(value); break;
        case FIELDTYPE_WEAPON_RANGE:        if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strRange = LPCSTR(value); break;
        case FIELDTYPE_WEAPON_ATTACKS:      if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strShots = LPCSTR(value); else ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strAttacks = LPCSTR(value); break;
        case FIELDTYPE_WEAPON_MAL:          if( pInvestigatorSkill ) if( pInvestigatorSkill->m_pSkill->m_SkillType == FIREARM ) ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strMAL = LPCSTR(value); break;

        //others
        case FIELDTYPE_CTHULHUMYTHOS:
            pInvestigatorSkill = GetDocument()->m_Investigator.FindSkill("Cthulhu Mythos");
            if( pInvestigatorSkill ) pInvestigatorSkill->m_nCurrent = int(value);
            break;

        case FIELDTYPE_MAXSAN:
            pInvestigatorSkill = GetDocument()->m_Investigator.FindSkill("Cthulhu Mythos");
            if( pInvestigatorSkill ) 
            {
                int nMaxSan = int(value);
                pInvestigatorSkill->m_nCurrent = 99-nMaxSan;
                if( GetDocument()->m_Investigator.m_nSAN > nMaxSan ) GetDocument()->m_Investigator.m_nSAN = nMaxSan;
            }
            break;

        case FIELDTYPE_ERA:
        {
            CString strEra = LPCSTR(value);
            if( strEra.Compare("1890") == 0 ) GetDocument()->m_Investigator.m_Era = Era_1890;
            else if( strEra.Compare("1990") == 0 ) GetDocument()->m_Investigator.m_Era = Era_1990;
                else if( strEra.Compare("1920") == 0 ) GetDocument()->m_Investigator.m_Era = Era_1920;
        }
    }

    CScriptSheetView::SetFieldValue(pField, value, pFieldSheetCommand );
}

void CInvestigatorView::InitialiseSheetCommands()
{
    //get the investigator to recalculate it's minimum stat values
    CInvestigatorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    pDoc->m_Investigator.RecalculateMinimumSkillValues();

    CInvestigator& Investigator = pDoc->m_Investigator;

    //reset the skill drawing lists etc.
    int iMaxAllSkills = Investigator.m_AllSkillList.GetSize();
    int iMaxSkills = Investigator.m_SkillList.GetSize();
    int iMaxFirearms = Investigator.m_FirearmList.GetSize();
    int iMaxMelee = Investigator.m_MeleeList.GetSize();
    int iWeapon, iSkill, iFirearm, iMelee;

    m_fShowBasePercent = TRUE; //show a percentage sign in the skill name?
    m_fDrawTruncated = GetSheetParameter(SHEETPARAM_SHORT, FALSE); //show all skills in full?
    m_fShowFirearms = GetSheetParameter(SHEETPARAM_SHOWCOREFIREARMS, TRUE); //show "Handgun" etc. ?

    //find the number of skills to be displayed
    int nSkillFields = 0;
    int iMaxSheetCommands = m_Fields.m_HotFieldList.GetSize(), iSheetCommands;
    for( iSheetCommands = 0; iSheetCommands < iMaxSheetCommands; iSheetCommands++ ) if( int(m_Fields.m_HotFieldList[iSheetCommands]->m_pField->m_Code) == FIELDTYPE_SKILL ) nSkillFields++;

    //find the number of firearm and non firearm skills this investigator has (where base[0] == 'F' or not)
    int nBaseFirearms = 0, nNonFirearmSkills = 0;
    for( iSkill = 0; iSkill < iMaxSkills; iSkill++ ) 
        if( Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase.IsEmpty() == FALSE && Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase[0] == 'F' ) nBaseFirearms++; else nNonFirearmSkills++;
    
    //should we display firearms at all?
    if( nBaseFirearms == 0 ) m_fShowFirearms = FALSE; //don't show "firearms" if there aren't any
 
    //tie the skills etc. to the relevent FieldSheetCommand pointer
    BOOL fDrawnMainSkills = FALSE, fDrawnFirearms = FALSE, fDrawingFirearmCaption = FALSE, fDrawingSkills = FALSE;
    iWeapon = iSkill = iFirearm = iMelee = -1;
    int nTotalSkillCount = 0;

    for( iSheetCommands = 0; iSheetCommands < iMaxSheetCommands; iSheetCommands++ )
    {
        //empty pointer value and enable fields by default
        m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = 0;
        m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 1;
        m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = TRUE;

        switch( int(m_Fields.m_HotFieldList[iSheetCommands]->m_pField->m_Code) )
        {
            case FIELDTYPE_SKILL:
                if( fDrawnMainSkills == FALSE )
                {
                    while( ++iSkill < iMaxSkills )
                    {
                        if( Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase.IsEmpty() == FALSE && Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase[0] != 'F' )
                        {
                            if( m_fDrawTruncated ) //ignore SPECIALSKILL types if we're drawing truncated
                            {
                                if( Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SKILL || ( Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SUBSKILL && Investigator.m_SkillList[iSkill]->m_pSkill->m_strName.IsEmpty() == FALSE ) ) 
                                    break;
                            }
                            else
                            {
                                if( Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SKILL || Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SUBSKILL || Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SPECIALSKILL ) 
                                    break;
                            }
                        }
                    }

                    //not found
                    nTotalSkillCount++;
                    if( (!fDrawnMainSkills && m_fShowFirearms && nTotalSkillCount >= nSkillFields-nBaseFirearms ) || iSkill >= iMaxSkills ) 
                    {
                        fDrawingSkills = FALSE;
                        fDrawnMainSkills = TRUE;
                        iSkill = 0;

                        //see if we need to draw the firearms caption now (no blank spaces)
                        if( nTotalSkillCount >= nSkillFields-nBaseFirearms )
                        {
                            fDrawingFirearmCaption = m_fShowFirearms;
                        }
                    }
                    else
                        fDrawingSkills = TRUE;

                }
                else
                {
                    if( m_fShowFirearms && nTotalSkillCount >= (nSkillFields-nBaseFirearms)-1 )
                    {
                        if( fDrawingSkills == FALSE && fDrawingFirearmCaption == FALSE )
                        {
                            fDrawingFirearmCaption = TRUE;
                        }
                        else
                        {
                            fDrawingFirearmCaption = FALSE;
                            if( fDrawnFirearms == FALSE )
                            {
                                while( ++iSkill < iMaxSkills )
                                {
                                    if( Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase.IsEmpty() == FALSE && Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase[0] == 'F' )
                                        break;
                                }

                                //not found
                                if( iSkill >= iMaxSkills ) 
                                {
                                    fDrawingSkills = FALSE;
                                    fDrawnFirearms = TRUE;
                                    iSkill = 0;
                                }
                                else
                                    fDrawingSkills = TRUE;
                            }
                        }
                    }

                    nTotalSkillCount++;
                }
                break;

            case FIELDTYPE_SKILL_CHECK:
                if( fDrawingFirearmCaption ) //firearms caption
                {
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 0;
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                }
                else
                    if( iSkill < 0 || iSkill > iMaxSkills || fDrawingSkills == FALSE ) //out-of-range skill - just for show
                        m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                    else
                    {
                        //see if it should be drawn at all
                        if( *LPCSTR(Investigator.m_SkillList[iSkill]->m_pSkill->m_strBase) == '*' || Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SPECIALSKILL )
                        {
                            m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 0;
                            m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                        }
                        m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_SkillList[iSkill];
                    }
                break;

            case FIELDTYPE_SKILL_NAME:
                if( fDrawingFirearmCaption ) //firearms caption
                {
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 2;
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                }
                else
                    if( iSkill < 0 || iSkill > iMaxSkills || fDrawingSkills == FALSE ) //out-of-range skill - just for show
                        m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                    else
                    {
                        //store a pointer to this one
                        m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_SkillList[iSkill];

                        //disable it if it's not a subskill
                        if( Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType != SUBSKILL )
                        {
                            m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                        }
                        else
                        {
                            //it's a subskill - set it's option list name

                            //remove asterisks from the parent name
                            const char* pszParentName = LPCSTR(((CSubSkill*)Investigator.m_SkillList[iSkill]->m_pSkill)->m_pSkill->m_strName); while( *pszParentName == '*' ) pszParentName++;

                            //get a CString copy and translate spaces into underscores
                            CString strOption = pszParentName; char* pszName = (char*)LPCSTR(strOption);
                            while( *pszName != '\0' ) { if( *pszName == ' ' ) *pszName = '_'; pszName++; }
                            m_Fields.m_HotFieldList[iSheetCommands]->m_strOptionName = strOption;
                        }
                    }
                break;

            case FIELDTYPE_SKILL_PERCENT:
                if( fDrawingFirearmCaption ) //firearms caption
                {
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 0;
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                }
                else
                    if( iSkill < 0 || iSkill > iMaxSkills || fDrawingSkills == FALSE ) //out-of-range skill - just for show
                        m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                    else
                    {
                        //see if it should be drawn at all
                        if( Investigator.m_SkillList[iSkill]->m_pSkill->m_SkillType == SPECIALSKILL )
                        {
                            m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam2 = 0;
                            m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                        }
                        m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_SkillList[iSkill];
                    }
                break;

            case FIELDTYPE_FIREARM:
                iFirearm++;
                break;

            case FIELDTYPE_FIREARM_CHECK:
            case FIELDTYPE_FIREARM_NAME:
            case FIELDTYPE_FIREARM_PERCENT:
            case FIELDTYPE_FIREARM_DAMAGE:
            case FIELDTYPE_FIREARM_RANGE:
            case FIELDTYPE_FIREARM_SHOTS:
            case FIELDTYPE_FIREARM_BULLETS:
            case FIELDTYPE_FIREARM_HITPOINTS:
            case FIELDTYPE_FIREARM_MAL:
                if( iFirearm >= 0 && iFirearm < iMaxFirearms ) 
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_FirearmList[iFirearm];
                else
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                break;

            case FIELDTYPE_MELEE:
                iMelee++;

                //skip over core melee weapons (fist/punch etc.) if we're drawing a truncated sheet
                if( m_fDrawTruncated ) while( iMelee < iMaxMelee && *LPCSTR(Investigator.m_MeleeList[iMelee]->m_pSkill->m_strName) == '*' ) iMelee++;
                break;

            case FIELDTYPE_MELEE_NAME:
                if( iMelee >= 0 && iMelee < iMaxMelee ) if( m_fDrawTruncated == FALSE && *LPCSTR(Investigator.m_MeleeList[iMelee]->m_pSkill->m_strName) == '*' ) m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                //drop through...

            case FIELDTYPE_MELEE_CHECK:
            case FIELDTYPE_MELEE_PERCENT:
            case FIELDTYPE_MELEE_DAMAGE:
            case FIELDTYPE_MELEE_ATTACKS:
            case FIELDTYPE_MELEE_HITPOINTS:
                if( iMelee >= 0 && iMelee < iMaxMelee) 
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_MeleeList[iMelee];
                else
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                break;

            case FIELDTYPE_WEAPON:
                if( iWeapon < iMaxAllSkills )
                {
                    iWeapon++;
                    if( m_fDrawTruncated )
                        while( iWeapon < iMaxAllSkills && (*LPCSTR(Investigator.m_AllSkillList[iWeapon]->m_pSkill->m_strName) == '*' || (Investigator.m_AllSkillList[iWeapon]->m_pSkill->m_SkillType != FIREARM && Investigator.m_AllSkillList[iWeapon]->m_pSkill->m_SkillType != MELEE ) ) ) iWeapon++;
                    else
                        while( iWeapon < iMaxAllSkills && Investigator.m_AllSkillList[iWeapon]->m_pSkill->m_SkillType != FIREARM && Investigator.m_AllSkillList[iWeapon]->m_pSkill->m_SkillType != MELEE ) iWeapon++;
                }
                break;

            case FIELDTYPE_WEAPON_CHECK:
            case FIELDTYPE_WEAPON_NAME:
            case FIELDTYPE_WEAPON_PERCENT:
            case FIELDTYPE_WEAPON_DAMAGE:
            case FIELDTYPE_WEAPON_HITPONTS:
            case FIELDTYPE_WEAPON_BULLETS:
            case FIELDTYPE_WEAPON_RANGE:
            case FIELDTYPE_WEAPON_ATTACKS:
            case FIELDTYPE_WEAPON_MAL:
                if( iWeapon >= 0 && iWeapon < iMaxAllSkills )
                    m_Fields.m_HotFieldList[iSheetCommands]->m_dwParam1 = (DWORD)Investigator.m_AllSkillList[iWeapon];
                else
                    m_Fields.m_HotFieldList[iSheetCommands]->m_fActive = FALSE;
                break;
        }
    }

    CScriptSheetView::InitialiseSheetCommands();
}


void CInvestigatorView::ExecuteSheetCommand( CSheetCommand* pSheetCommand, CSheetDrawInfo* pDrawInfo )
{
    ASSERT(pSheetCommand);
    ASSERT(pDrawInfo);

    switch( pSheetCommand->m_Type )
    {
        case CommandType_Field:
        {
            CFieldSheetCommand* pFieldSheetCommand = (CFieldSheetCommand*)pSheetCommand;
            switch( int(pFieldSheetCommand->m_pField->m_Code) )
            {
                case FIELDTYPE_PORTRAIT:
                    pDrawInfo->DrawImage( GetDocument()->GetPortrait(), pFieldSheetCommand->m_rc );
                    break;

                case FIELDTYPE_SKILL_CHECK:
                    if( pFieldSheetCommand->m_dwParam2 ) CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
                    break;

                case FIELDTYPE_SKILL_NAME:
                    switch( pFieldSheetCommand->m_dwParam2 )
                    {
                        case 1: //draw normally
                        {
                            CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)pFieldSheetCommand->m_dwParam1;
                            if( pInvestigatorSkill )
                            {
                                //get skill name without it's leading asterisks
                                const char* pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                                while( *pszName == '*' ) pszName++;

                                //as CFieldSheetCommand::Draw() isn't being called, we've got to store the last used info ourselves
                                pFieldSheetCommand->m_pLastUsedFont = pDrawInfo->m_pLastUsedFont;
                                pFieldSheetCommand->m_crFontColour = pDrawInfo->m_crText;
                                pFieldSheetCommand->m_dwFieldStyle = pDrawInfo->m_dwFieldFormatting;
                                pFieldSheetCommand->m_nCurrentUserTextSize = pDrawInfo->m_nCurrentUserTextSize;
                                pFieldSheetCommand->m_nCurrentUserTextAngle = pDrawInfo->m_nCurrentUserTextAngle;

                                //determine how to draw the skill
                                switch( pInvestigatorSkill->m_pSkill->m_SkillType )
                                {
                                    case SKILL:
                                    {
                                        //build name string
                                        CString strText;
                                        strText.Format( "%s (%s)", m_fDrawTruncated ? Abbreviate(pszName) : pszName, pInvestigatorSkill->m_pSkill->GetBaseString( m_fDrawV5_5Bases, m_fShowBasePercent ) );

                                        //display it (in usertext if the name doesn't begin with a *)
                                        DWORD dwFormat = pDrawInfo->m_dwFieldFormatting & ~DWORD(TEXTSTYLE_FIELDUNDERLINE|TEXTSTYLE_USERTEXT);
                                        if( *LPCSTR(pInvestigatorSkill->m_pSkill->m_strName) != '*' && m_fCustomSkillsInUserText ) dwFormat |= TEXTSTYLE_USERTEXT;
                                        pDrawInfo->DrawText( strText, pFieldSheetCommand->m_rc, dwFormat );
                                        break;
                                    }

                                    case SPECIALSKILL:
                                    {
                                        //build name string (notice the colon after the (%s) !!
                                        CString strText;
                                        strText.Format( "%s (%s):", pszName, pInvestigatorSkill->m_pSkill->GetBaseString( m_fDrawV5_5Bases, m_fShowBasePercent ) );

                                        //display it
                                        pDrawInfo->DrawText( strText, pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting & ~DWORD(TEXTSTYLE_FIELDUNDERLINE|TEXTSTYLE_USERTEXT) );
                                        break;
                                    }

                                    case SUBSKILL:
                                    {
                                        if( m_fDrawTruncated ) //draw it as Art (dance)
                                        {
                                            //get asterisk-less parent name too
                                            const char* pszParentName = LPCSTR(((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName);
                                            while( *pszParentName == '*' ) pszParentName++;

                                            //build name string
                                            CString strText;
                                            strText.Format( "%s (%s) (%s)", Abbreviate(pszParentName), pszName, pInvestigatorSkill->m_pSkill->GetBaseString( m_fDrawV5_5Bases, m_fShowBasePercent ) );

                                            //display it
                                            pDrawInfo->DrawText( strText, pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting & ~DWORD(TEXTSTYLE_FIELDUNDERLINE|TEXTSTYLE_USERTEXT) );
                                        }
                                        else//draw it as just _dance_ ("Art" is shown above it)
                                            pDrawInfo->DrawText( pszName, pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting|TEXTSTYLE_USERTEXT );
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                //draw a blank field
                                pDrawInfo->DrawText( "", pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting );
                            }
                            break;
                        }

                        case 2: //special case: draw firearms caption
                        {
                            //create a font based on the current one, but with bold toggled
                            LOGFONT lf; pDrawInfo->m_pFont->GetObject( sizeof(LOGFONT), &lf );
                            if( lf.lfWeight == FW_NORMAL ) lf.lfWeight = FW_BOLD; else lf.lfWeight = FW_NORMAL;
                            CFont NewFont; NewFont.CreateFontIndirect( &lf );

                            //select it into the device context and display the "Firearms" text using current formatting, except for no field underline and not in usertext
                            CFont* pOldFont = pDrawInfo->m_pDC->SelectObject( &NewFont );
                            pDrawInfo->DrawText( "Firearms", pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting & ~DWORD(TEXTSTYLE_FIELDUNDERLINE|TEXTSTYLE_USERTEXT) );

                            //clean up everything
                            pDrawInfo->m_pDC->SelectObject( pOldFont );
                            NewFont.DeleteObject();
                            break;
                        }
                    }
                    break;

                case FIELDTYPE_SKILL_PERCENT:
                    if( pFieldSheetCommand->m_dwParam2 == 0 ) break;
                    //otherwise, drop through
                case FIELDTYPE_FIREARM_PERCENT:
                case FIELDTYPE_MELEE_PERCENT:
                case FIELDTYPE_WEAPON_PERCENT:
                    if( m_fShowAllSkillPercentiles )
                        //draw all base percentiles
                        CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
                    else
                    {
                        //only draw those that are calculated or non-base
                        CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)pFieldSheetCommand->m_dwParam1;
                        if( pInvestigatorSkill && (pInvestigatorSkill->IsBase(m_fDrawV5_5Bases) == FALSE || pInvestigatorSkill->IsCalculatedBase() == TRUE ) )
                            CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
                        else
                        {
                            //as CFieldSheetCommand::Draw() isn't being called, we've got to store the last used info ourselves
                            pFieldSheetCommand->m_pLastUsedFont = pDrawInfo->m_pLastUsedFont;
                            pFieldSheetCommand->m_crFontColour = pDrawInfo->m_crText;
                            pFieldSheetCommand->m_dwFieldStyle = pDrawInfo->m_dwFieldFormatting;
                            pFieldSheetCommand->m_nCurrentUserTextSize = pDrawInfo->m_nCurrentUserTextSize;
                            pFieldSheetCommand->m_nCurrentUserTextAngle = pDrawInfo->m_nCurrentUserTextAngle;

                            pDrawInfo->DrawText( "", pFieldSheetCommand->m_rc, pDrawInfo->m_dwFieldFormatting );
                        }
                    }
                    break;

                case FIELDTYPE_SKILL:
                case FIELDTYPE_FIREARM:
                case FIELDTYPE_MELEE:
                case FIELDTYPE_WEAPON:
                    //these field types are just placeholders to move onto the next skill when initialising sheet commands
                    break;

                case FIELDTYPE_MELEE_NAME:
                {
                    CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)pFieldSheetCommand->m_dwParam1;
                    if( pInvestigatorSkill && *LPCSTR(pInvestigatorSkill->m_pSkill->m_strName) == '*' )
                    {
                        //as CFieldSheetCommand::Draw() isn't being called, we've got to store the last used info ourselves
                        pFieldSheetCommand->m_pLastUsedFont = pDrawInfo->m_pLastUsedFont;
                        pFieldSheetCommand->m_crFontColour = pDrawInfo->m_crText;
                        pFieldSheetCommand->m_dwFieldStyle = pDrawInfo->m_dwFieldFormatting;
                        pFieldSheetCommand->m_nCurrentUserTextSize = pDrawInfo->m_nCurrentUserTextSize;
                        pFieldSheetCommand->m_nCurrentUserTextAngle = pDrawInfo->m_nCurrentUserTextAngle;

                        //get skill name without asterisks
                        const char* pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                        while( *pszName == '*' ) pszName++;

                        //build name string
                        CString strText;
                        strText.Format( "%s (%s)", m_fDrawTruncated ? Abbreviate(pszName) : pszName, pInvestigatorSkill->m_pSkill->GetBaseString( m_fDrawV5_5Bases, m_fShowBasePercent ) );

                        //display it (in usertext if the name doesn't begin with a *)
                        DWORD dwFormat = pDrawInfo->m_dwFieldFormatting & ~DWORD(TEXTSTYLE_FIELDUNDERLINE|TEXTSTYLE_USERTEXT);
                        pDrawInfo->DrawText( strText, pFieldSheetCommand->m_rc, dwFormat );
                    }
                    else
                        CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
                    break;
                }

                default:
                    CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
                    break;
            }
            break;
        }

        case CommandType_SetSheetParameter:
        {
            int value = ((CDrawSheetCommand*)pSheetCommand)->m_nValue2;
            switch( ((CDrawSheetCommand*)pSheetCommand)->m_nValue )
            {
                case SHEETPARAM_SHORT:
                case SHEETPARAM_SHOWCOREFIREARMS:
                    //these two are one-time only settings (see InitialiseSheetCommands)
                    break;

                case SHEETPARAM_SHOWBASEPERCENT:
                    m_fShowBasePercent = value;
                    break;
            }
            break;
        }

        default:
            CScriptSheetView::ExecuteSheetCommand( pSheetCommand, pDrawInfo );
            break;
    }
}


void CInvestigatorView::RedrawFields( CFieldSheetCommand* pFieldChanged )
{
    //calculate a new point value if any factors have been changed
    if( pFieldChanged )
    {
        if( pFieldChanged->m_pField->m_dwFlags & FIELD_CALCRELIANT ) GetDocument()->m_Investigator.CalculateSkillPoints();
    }

    //update the status bar with the new point value
    GetDocument()->UpdateStatusBar();

    //continue to redraw the fields
    CScriptSheetView::RedrawFields( pFieldChanged );
}

void CInvestigatorView::OnActivateView(BOOL bActivate, CScrollView* pActivateView, CScrollView* pDeactiveView) 
{
    if( bActivate == FALSE )
    {
	    CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
    }
    else
        GetDocument()->UpdateStatusBar();

	CScriptSheetView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CInvestigatorView::SheetChanged()
{
    CInvestigatorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    pDoc->m_strLastSheet = m_strSheet;
}

void CInvestigatorView::OnSetFocus(CWnd* pOldWnd) 
{
	CScriptSheetView::OnSetFocus(pOldWnd);
    GetDocument()->UpdateStatusBar();
}
