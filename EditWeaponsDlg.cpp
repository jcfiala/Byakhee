// EditWeaponsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "EditWeaponsDlg.h"
#include "SkillManager.h"
#include "NewWeaponDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditWeaponsDlg dialog
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


CEditWeaponsDlg::CEditWeaponsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditWeaponsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditWeaponsDlg)
	m_fInformation = FALSE;
	//}}AFX_DATA_INIT

    m_pInvestigator = NULL;
    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INFO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
    m_pwndInformationPopup = NULL;
}

CEditWeaponsDlg::~CEditWeaponsDlg()
{
    delete m_pwndInformationPopup;
    DestroyIcon( m_hIcon );
}

void CEditWeaponsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditWeaponsDlg)
	DDX_Control(pDX, IDC_INVESTIGATORWEAPONS, m_wndInvestigatorWeapons);
	DDX_Control(pDX, IDC_INFORMATION, m_wndInformation);
	DDX_Control(pDX, IDC_ALLWEAPONS, m_wndAllWeapons);
	DDX_Check(pDX, IDC_INFORMATION, m_fInformation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditWeaponsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditWeaponsDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_LBN_SELCHANGE(IDC_ALLWEAPONS, OnSelchangeAllweapons)
	ON_LBN_DBLCLK(IDC_ALLWEAPONS, OnDblclkAllweapons)
	ON_BN_CLICKED(IDC_INFORMATION, OnInformation)
	ON_LBN_DBLCLK(IDC_INVESTIGATORWEAPONS, OnDblclkInvestigatorweapons)
	ON_LBN_SELCHANGE(IDC_INVESTIGATORWEAPONS, OnSelchangeInvestigatorWeapons)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SETFOCUS(IDC_ALLWEAPONS, OnSetfocusAllweapons)
	ON_LBN_SETFOCUS(IDC_INVESTIGATORWEAPONS, OnSetfocusInvestigatorweapons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditWeaponsDlg message handlers

BOOL CEditWeaponsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pInvestigator);

    //build list
    AddWeaponsToListBoxes();

    //initialise the info window
    m_wndInformation.SetIcon( m_hIcon );
    m_fInformation = AfxGetApp()->GetProfileInt( "WindowPos", "SkillInfo", FALSE );

    m_pwndInformationPopup = new CSkillInformationPopupWnd(m_pInvestigator->m_strRuleFile);
    m_pwndInformationPopup->Create( this );
    m_pwndInformationPopup->MoveIntoPosition();
    if( m_fInformation ) m_pwndInformationPopup->ShowWindow( SW_SHOWNA );

    UpdateData(UD_SETDATA);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditWeaponsDlg::OnAdd() 
{
    int nSelected = m_wndAllWeapons.GetCurSel();
    if( nSelected == LB_ERR || m_wndAllWeapons.GetCount() <= 0 ) return;
    UpdateData(UD_GETDATA);

    //make sure we've got something to add
    CString strWeaponToAdd;
    m_wndAllWeapons.GetText( nSelected, strWeaponToAdd );
    if( strWeaponToAdd.GetLength() > 0 )
    {
        //remove it from the available weapons list
        int n = m_wndAllWeapons.FindStringExact( -1, strWeaponToAdd );
        if( n != LB_ERR )
            m_wndAllWeapons.DeleteString( n );
        else
        {
            AfxMessageBox( "Error: Unable to locate weapon!", MB_ICONERROR );
            return;
        }

        //add it to the investigator
        CSkill* pSkill = SkillManager.FindSkill( m_pInvestigator->m_strRuleFile, strWeaponToAdd );
        if( pSkill )
        {
            //make sure it's a weapon!
            if( pSkill->m_SkillType == FIREARM || pSkill->m_SkillType == MELEE )
            {
                //create a copy of the skill
                m_pInvestigator->AddSkill( pSkill->GetCopy() );
                m_wndInvestigatorWeapons.AddString( strWeaponToAdd );
            }
        }
    }
}

void CEditWeaponsDlg::OnSelchangeAllweapons() 
{
    CString strWeapon;
    int i = m_wndAllWeapons.GetCurSel();
    if( i != LB_ERR && m_wndAllWeapons.GetCount() > 0 ) m_wndAllWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill( strWeapon, m_pInvestigator );
}

void CEditWeaponsDlg::OnDblclkAllweapons() 
{
	OnAdd();
}

void CEditWeaponsDlg::OnInformation() 
{
    UpdateData(UD_GETDATA);
    if( m_fInformation )
        m_pwndInformationPopup->ShowWindow( SW_SHOWNA );
    else
        m_pwndInformationPopup->ShowWindow( SW_HIDE );

    AfxGetApp()->WriteProfileInt( "WindowPos", "SkillInfo", m_fInformation );
}

void CEditWeaponsDlg::OnDblclkInvestigatorweapons() 
{
    OnRemove();
}

void CEditWeaponsDlg::OnSelchangeInvestigatorWeapons() 
{
    CString strWeapon;
    int i = m_wndInvestigatorWeapons.GetCurSel();
    if( i != LB_ERR && m_wndInvestigatorWeapons.GetCount() > 0 ) m_wndInvestigatorWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill( strWeapon, m_pInvestigator );
}

void CEditWeaponsDlg::OnRemove() 
{
    int nSelected = m_wndInvestigatorWeapons.GetCurSel();
    if( nSelected == LB_ERR || m_wndInvestigatorWeapons.GetCount() <= 0 ) return;
    UpdateData(UD_GETDATA);

    //make sure we've got something to remove
    CString strWeaponToRemove;
    m_wndInvestigatorWeapons.GetText( nSelected, strWeaponToRemove );
    if( strWeaponToRemove.GetLength() > 0 )
    {
        //tell the investigator to remove it
        if( m_pInvestigator->RemoveSkill(strWeaponToRemove) == TRUE )
            AddWeaponsToListBoxes();
        else
            AfxMessageBox( "Error: Could not remove skill", MB_ICONERROR );
    }
}

void CEditWeaponsDlg::OnSetfocusAllweapons() 
{
    CString strWeapon;
    int i = m_wndAllWeapons.GetCurSel();
    if( i != LB_ERR && m_wndAllWeapons.GetCount() > 0 ) m_wndAllWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill( strWeapon, m_pInvestigator );
}

void CEditWeaponsDlg::OnSetfocusInvestigatorweapons() 
{
    CString strWeapon;
    int i = m_wndInvestigatorWeapons.GetCurSel();
    if( i != LB_ERR && m_wndInvestigatorWeapons.GetCount() > 0 ) m_wndInvestigatorWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill( strWeapon, m_pInvestigator );
}

void CEditWeaponsDlg::AddWeaponsToListBoxes()
{
    m_wndInvestigatorWeapons.ResetContent();
    m_wndAllWeapons.ResetContent();

    //get the investigator's era
    Era era = m_pInvestigator->m_Era;

    //add all melee weapons
    int i;
    const CRuleFile* pTempRuleFile = SkillManager.GetTempRuleFile(m_pInvestigator->m_strRuleFile);
    if( pTempRuleFile != NULL )
    {
        for( i = 0; i < pTempRuleFile->m_MeleeArray.GetSize(); i++ )
        {
            CSkill* pSkill = pTempRuleFile->m_MeleeArray[i];

            //only add melee skills of this era which aren't core melee weapons
            if( pSkill->m_SkillType == MELEE && pSkill->IsEra(era) && pSkill->m_strName[0] != '*' )
            {
                //see which list to add it to
                if( m_pInvestigator->FindSkill( pSkill->m_strName ) == NULL ) m_wndAllWeapons.AddString( pSkill->m_strName );
            }
        }

        //add all firearms
        for( i = 0; i < pTempRuleFile->m_FirearmArray.GetSize(); i++ )
        {
            CSkill* pSkill = pTempRuleFile->m_FirearmArray[i];

            //only add melee skills of this era which aren't core melee weapons
            if( pSkill->m_SkillType == FIREARM && pSkill->IsEra(era) && pSkill->m_strName[0] != '*' )
            {
                //see which list to add it to
                if( m_pInvestigator->FindSkill( pSkill->m_strName ) == NULL ) m_wndAllWeapons.AddString( pSkill->m_strName );
            }
        }
    }

    //add this investigator's weapons
    int iMax;
    iMax = m_pInvestigator->m_FirearmList.GetSize();
    for( i = 0; i < iMax; i++ ) 
        if( m_pInvestigator->m_FirearmList[i]->m_pSkill->m_strName.IsEmpty() == FALSE )
            m_wndInvestigatorWeapons.AddString( m_pInvestigator->m_FirearmList[i]->m_pSkill->m_strName );

    iMax = m_pInvestigator->m_MeleeList.GetSize();
    for( i = 0; i < iMax; i++ )
        if( m_pInvestigator->m_MeleeList[i]->m_pSkill->m_strName.IsEmpty() == FALSE && m_pInvestigator->m_MeleeList[i]->m_pSkill->m_strName[0] != '*' )
            m_wndInvestigatorWeapons.AddString( m_pInvestigator->m_MeleeList[i]->m_pSkill->m_strName );


}

void CEditWeaponsDlg::OnNew() 
{
	CNewWeaponDlg NewWeaponDlg;
    NewWeaponDlg.SetInvestigator( m_pInvestigator );
    NewWeaponDlg.DoModal();
    AddWeaponsToListBoxes();
}

