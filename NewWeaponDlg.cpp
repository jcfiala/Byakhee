// NewWeaponDlg.cpp : implementation file
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
#include "resource.h"
#include "NewWeaponDlg.h"
#include "ChooseSkillDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CNewWeaponDlgPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(CNewWeaponDlgPage2, CPropertyPage)
IMPLEMENT_DYNAMIC(CNewWeaponDlg, CPropertySheet)

//FIXME: add the popup format helper menu to the second page - maybe split the dialog so that the top
//half is the same on each and only the bottom bit changes, or start with a subclass??

CNewWeaponDlg::CNewWeaponDlg(CWnd* pWndParent)
	 : CPropertySheet(IDS_CAPTION_NEWWEAPONDLG, pWndParent)
{
	AddPage(&m_Page1);
	AddPage(&m_Page2);

    m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CNewWeaponDlg::~CNewWeaponDlg()
{
}


BEGIN_MESSAGE_MAP(CNewWeaponDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CNewWeaponDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CNewWeaponDlg::SetInvestigator(CInvestigator * pInvestigator)
{
    m_pInvestigator = pInvestigator;
    m_Page1.SetInvestigator(pInvestigator);
    m_Page2.SetInvestigator(pInvestigator);
}


/////////////////////////////////////////////////////////////////////////////
// CNewWeaponDlgPage1 property page

CNewWeaponDlgPage1::CNewWeaponDlgPage1() : CPropertyPage(CNewWeaponDlgPage1::IDD)
{
	//{{AFX_DATA_INIT(CNewWeaponDlgPage1)
	m_strBase = _T("");
	m_strDamage = _T("");
	m_strBullets = _T("");
	m_strHitPoints = _T("");
	m_strMal = _T("");
	m_strRange = _T("");
	m_strShots = _T("");
	m_strSkillName = _T("");
	//}}AFX_DATA_INIT
}

CNewWeaponDlgPage1::~CNewWeaponDlgPage1()
{
}

void CNewWeaponDlgPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewWeaponDlgPage1)
	DDX_Control(pDX, IDC_INSERTSPECIALTEXT, m_wndInsertSpecialText);
	DDX_Text(pDX, IDC_BASE, m_strBase);
	DDX_Text(pDX, IDC_DAMAGE, m_strDamage);
	DDX_Text(pDX, IDC_BULLETS, m_strBullets);
	DDX_Text(pDX, IDC_HITPOINTS, m_strHitPoints);
	DDX_Text(pDX, IDC_MAL, m_strMal);
	DDX_Text(pDX, IDC_RANGE, m_strRange);
	DDX_Text(pDX, IDC_SHOTS, m_strShots);
	DDX_Text(pDX, IDC_SKILLNAME, m_strSkillName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewWeaponDlgPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CNewWeaponDlgPage1)
	ON_BN_CLICKED(IDC_INSERTSPECIALTEXT, OnInsertSpecialText)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_BASESTRINGPOPUP_APP, OnBaseStringAPP )
    ON_COMMAND(ID_BASESTRINGPOPUP_CON, OnBaseStringCON )
    ON_COMMAND(ID_BASESTRINGPOPUP_DEX, OnBaseStringDEX )
    ON_COMMAND(ID_BASESTRINGPOPUP_EDU, OnBaseStringEDU )
    ON_COMMAND(ID_BASESTRINGPOPUP_INT, OnBaseStringINT )
    ON_COMMAND(ID_BASESTRINGPOPUP_POW, OnBaseStringPOW )
    ON_COMMAND(ID_BASESTRINGPOPUP_SIZ, OnBaseStringSIZ )
    ON_COMMAND(ID_BASESTRINGPOPUP_STR, OnBaseStringSTR )
    ON_COMMAND(ID_BASESTRINGPOPUP_LUCK, OnBaseStringLuck )
    ON_COMMAND(ID_BASESTRINGPOPUP_KNOW, OnBaseStringKnow )
    ON_COMMAND(ID_BASESTRINGPOPUP_IDEA, OnBaseStringIdea )
    ON_COMMAND(ID_BASESTRINGPOPUP_SAN, OnBaseStringSan )
    ON_COMMAND(ID_BASESTRINGPOPUP_HP, OnBaseStringHitPoints )
    ON_COMMAND(ID_BASESTRINGPOPUP_MAGIC, OnBaseStringMagic )
    ON_COMMAND(ID_BASESTRINGPOPUP_BASE_AS_SKILL, OnBaseStringAsSkill )
END_MESSAGE_MAP()


void CNewWeaponDlgPage1::OnInsertSpecialText() 
{
    CMenu Menu; 
    VERIFY(Menu.LoadMenu(IDR_WEAPONBASESTRINGPOPUP));

    CMenu* pPopup = Menu.GetSubMenu(0);
    ASSERT(pPopup != NULL);

    RECT rc;
    m_wndInsertSpecialText.GetWindowRect( &rc );
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rc.right, rc.top, this);
}

void CNewWeaponDlgPage1::OnBaseStringAPP()
{
    AddMultiplier( "APP" );
}

void CNewWeaponDlgPage1::OnBaseStringCON()
{
    AddMultiplier( "CON" );
}

void CNewWeaponDlgPage1::OnBaseStringDEX()
{
    AddMultiplier( "DEX" );
}

void CNewWeaponDlgPage1::OnBaseStringEDU()
{
    AddMultiplier( "EDU" );
}

void CNewWeaponDlgPage1::OnBaseStringINT()
{
    AddMultiplier( "INT" );
}

void CNewWeaponDlgPage1::OnBaseStringPOW()
{
    AddMultiplier( "POW" );
}

void CNewWeaponDlgPage1::OnBaseStringSIZ()
{
    AddMultiplier( "SIZ" );
}

void CNewWeaponDlgPage1::OnBaseStringSTR()
{
    AddMultiplier( "STR" );
}

void CNewWeaponDlgPage1::OnBaseStringLuck()
{
    AddMultiplier( "LUCK" );
}

void CNewWeaponDlgPage1::OnBaseStringKnow()
{
    AddMultiplier( "KNOW" );
}

void CNewWeaponDlgPage1::OnBaseStringIdea()
{
    AddMultiplier( "IDEA" );
}

void CNewWeaponDlgPage1::OnBaseStringSan()
{
    AddMultiplier( "SAN" );
}

void CNewWeaponDlgPage1::OnBaseStringHitPoints()
{
    AddMultiplier( "HP" );
}

void CNewWeaponDlgPage1::OnBaseStringMagic()
{
    AddMultiplier( "MAGIC" );
}

void CNewWeaponDlgPage1::OnBaseStringAsSkill()
{
    UpdateData(UD_GETDATA);

    //prompt for the skill to use
	CChooseSkillDlg ChooseSkillDlg;
	ChooseSkillDlg.m_strCustomPrompt = "Please select a skill to use as this weapon's base";
	ChooseSkillDlg.m_pInvestigator = m_pInvestigator;
 	if( ChooseSkillDlg.DoModal() == IDOK && ChooseSkillDlg.m_pInvestigatorSkill ) 
    {
        const char* pszName = ChooseSkillDlg.m_pInvestigatorSkill->m_pSkill->m_strName;
        while( *pszName == '*' ) pszName++;

        //set the base to %skillname%
        m_strBase.Format( "%%%s%%", pszName );

        //change base on dialog
        UpdateData(UD_SETDATA);
    }
}

void CNewWeaponDlgPage1::AddMultiplier(CString strMultiplier)
{
    UpdateData(UD_GETDATA);
    
    //bulid new base
    m_strBase = CString("^") + strMultiplier + m_strBase;
    UpdateData(UD_SETDATA);
}


BOOL CNewWeaponDlgPage1::AddWeapon()
{
    UpdateData(UD_GETDATA);

    //make sure it's OK
    if( m_strSkillName.IsEmpty() || m_strSkillName.Compare( "*" ) == 0 )
    {
        AfxMessageBox( "This is not a valid skill name", MB_ICONEXCLAMATION );
        return FALSE;
    }

    //see if this skill already exists
    CInvestigatorSkill* pInvestigatorSkillFind = m_pInvestigator->FindSkill(m_strSkillName);
    if( pInvestigatorSkillFind != NULL )
    {
        AfxMessageBox( CString("This investigator already has a skill called ") + m_strSkillName + CString("\nPlease choose a different name"), MB_ICONEXCLAMATION );
        return FALSE;
    }

    //check to see if they're trying to create a 5.5 skill
    if( m_strBase.Compare( "1" ) == 0 )
    {
        switch( AfxMessageBox( "Note: You've chosen to set the base string to \"1\". If you want to display investigator skills so that they're compatible with Call of Cthulhu versions 5.5 upwards, click on View|Cthulhu 5.5 Bases in the main sheet window\nDo you want to set this base back to zero and use this option?", MB_YESNOCANCEL ) )
        {
            case IDYES: m_strBase = "0"; break;
            case IDCANCEL: return FALSE;
        }
    }

    //create this skill
    CFirearmSkill* pFirearmSkill = new CFirearmSkill( m_strSkillName, m_strBase, ERA_ISALL, m_strDamage, m_strRange, m_strShots, m_strBullets, m_strHitPoints, m_strMal );

    //evaluate the base to make sure it's OK
    int nMinimum = pFirearmSkill->EvaluateBase(m_pInvestigator);
    if( nMinimum == -666 ) { delete pFirearmSkill; return FALSE; }//malformed

    //add it to the skill lists
    m_pInvestigator->AddSkill(pFirearmSkill);
    m_pInvestigator->SortSkillList();

    UpdateData(UD_SETDATA);
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CNewWeaponDlgPage2 property page

CNewWeaponDlgPage2::CNewWeaponDlgPage2() : CPropertyPage(CNewWeaponDlgPage2::IDD)
{
	//{{AFX_DATA_INIT(CNewWeaponDlgPage2)
	m_strAttacks = _T("");
	m_strBase = _T("");
	m_strDamage = _T("");
	m_strHitPoints = _T("");
	m_strSkillName = _T("");
	//}}AFX_DATA_INIT
}

CNewWeaponDlgPage2::~CNewWeaponDlgPage2()
{
}

void CNewWeaponDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewWeaponDlgPage2)
	DDX_Text(pDX, IDC_ATTACKS, m_strAttacks);
	DDX_Text(pDX, IDC_BASE, m_strBase);
	DDX_Text(pDX, IDC_DAMAGE, m_strDamage);
	DDX_Text(pDX, IDC_HITPOINTS, m_strHitPoints);
	DDX_Text(pDX, IDC_SKILLNAME, m_strSkillName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewWeaponDlgPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CNewWeaponDlgPage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CNewWeaponDlgPage2::AddWeapon()
{
    UpdateData(UD_GETDATA);

    //make sure it's OK
    if( m_strSkillName.IsEmpty() || m_strSkillName.Compare( "*" ) == 0 )
    {
        AfxMessageBox( "This is not a valid skill name", MB_ICONEXCLAMATION );
        return FALSE;
    }

    //see if this skill already exists
    CInvestigatorSkill* pInvestigatorSkillFind = m_pInvestigator->FindSkill(m_strSkillName);
    if( pInvestigatorSkillFind != NULL )
    {
        AfxMessageBox( CString("This investigator already has a skill called ") + m_strSkillName + CString("\nPlease choose a different name"), MB_ICONEXCLAMATION );
        return FALSE;
    }

    //check to see if they're trying to create a 5.5 skill
    if( m_strBase.Compare( "1" ) == 0 )
    {
        switch( AfxMessageBox( "Note: You've chosen to set the base string to \"1\". If you want to display investigator skills so that they're compatible with Call of Cthulhu versions 5.5 upwards, click on View|Cthulhu 5.5 Bases in the main sheet window\nDo you want to set this base back to zero and use this option?", MB_YESNOCANCEL ) )
        {
            case IDYES: m_strBase = "0"; break;
            case IDCANCEL: return FALSE;
        }
    }

    //create this skill
    CMeleeSkill* pMeleeSkill = new CMeleeSkill( m_strSkillName, m_strBase, ERA_ISALL, m_strDamage, m_strAttacks, m_strHitPoints );

    //evaluate the base to make sure it's OK
    int nMinimum = pMeleeSkill->EvaluateBase(m_pInvestigator);
    if( nMinimum == -666 ) { delete pMeleeSkill; return FALSE; }//malformed

    //add it to the skill lists
    m_pInvestigator->AddSkill(pMeleeSkill);
    m_pInvestigator->SortSkillList();

    UpdateData(UD_SETDATA);
    return TRUE;
}

void CNewWeaponDlgPage1::OnOK() 
{
    if( ((CPropertySheet*)GetParent())->GetActivePage() == this )
    {
        if( !AddWeapon() ) return;
    }

	CPropertyPage::OnOK();
}

void CNewWeaponDlgPage2::OnOK() 
{
    if( ((CPropertySheet*)GetParent())->GetActivePage() == this )
    {
        if( !AddWeapon() ) return;
    }

	CPropertyPage::OnOK();
}

