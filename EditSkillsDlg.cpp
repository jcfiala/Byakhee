// EditSkillsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "AddSkillDlg.h"
#include "EditSkillDlg.h"
#include "EditSkillsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSkillsDlg dialog
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


CEditSkillsDlg::CEditSkillsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSkillsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditSkillsDlg)
	m_fInformation = FALSE;
	//}}AFX_DATA_INIT

    m_pInvestigator = NULL;
    m_pwndInformationPopup = NULL;
    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INFO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CEditSkillsDlg::~CEditSkillsDlg()
{
    delete m_pwndInformationPopup;
    DestroyIcon( m_hIcon );
}

void CEditSkillsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditSkillsDlg)
	DDX_Control(pDX, IDC_INFORMATION, m_wndInformation);
	DDX_Control(pDX, IDC_SKILLLIST, m_wndSkillList);
	DDX_Check(pDX, IDC_INFORMATION, m_fInformation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSkillsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditSkillsDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_INFORMATION, OnInformation)
	ON_LBN_SELCHANGE(IDC_SKILLLIST, OnSelchangeSkillList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSkillsDlg message handlers

BOOL CEditSkillsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    ASSERT(m_pInvestigator);

    //initialise the info window
    m_wndInformation.SetIcon( m_hIcon );
    m_fInformation = AfxGetApp()->GetProfileInt( "WindowPos", "SkillInfo", FALSE );

    m_pwndInformationPopup = new CSkillInformationPopupWnd(m_pInvestigator->m_strRuleFile);
    m_pwndInformationPopup->Create( this );
    m_pwndInformationPopup->MoveIntoPosition();
    if( m_fInformation ) m_pwndInformationPopup->ShowWindow( SW_SHOWNA );

    //redraw the skill list
    AddInvestigatorSkillsToListBox();

    UpdateData(UD_SETDATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditSkillsDlg::OnAdd() 
{
	CAddSkillDlg AddSkillDlg;
    AddSkillDlg.SetInvestigator( m_pInvestigator );

    //display the add skill dialog and redraw the skill list
    if( AddSkillDlg.DoModal() == IDOK ) AddInvestigatorSkillsToListBox();
}

void CEditSkillsDlg::OnDelete() 
{
	int iSel = m_wndSkillList.GetCurSel();
    if( iSel != LB_ERR )
    {
        CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)m_wndSkillList.GetItemData(iSel);

        if( pInvestigatorSkill )
        {
            //remove the skill
            m_pInvestigator->RemoveSkill(pInvestigatorSkill);

            //redraw the list
            AddInvestigatorSkillsToListBox();
        }
    }
}

void CEditSkillsDlg::OnEdit() 
{
	int iSel = m_wndSkillList.GetCurSel();
    if( iSel != LB_ERR )
    {
        CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)m_wndSkillList.GetItemData(iSel);

        if( pInvestigatorSkill )
        {
            //edit the skill
            CEditSkillDlg EditSkillDlg;
            EditSkillDlg.SetInvestigator( m_pInvestigator );
            EditSkillDlg.SetSkill( pInvestigatorSkill );

            //display the edit dialog and redraw the list
            if( EditSkillDlg.DoModal() == IDOK ) AddInvestigatorSkillsToListBox();
        }
    }
}

void CEditSkillsDlg::SetInvestigator(CInvestigator * pInvestigator)
{
    m_pInvestigator = pInvestigator;
}

void CEditSkillsDlg::AddInvestigatorSkillsToListBox()
{
    //remove all current items
    m_wndSkillList.ResetContent();
    m_pwndInformationPopup->SetSkill( "", m_pInvestigator );

    //zip down the main investigator skill list and add each item
    int iMax = m_pInvestigator->m_SkillList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->m_SkillList[i];
        if( pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL && pInvestigatorSkill->m_pSkill->m_SkillType != ANYSKILL && pInvestigatorSkill->m_pSkill->m_SkillType != NO_TYPE )
        {
            const char* pszName = pInvestigatorSkill->m_pSkill->m_strName;
            if( *pszName == '*' ) pszName++;

            int iPos = m_wndSkillList.AddString( pszName );
            if( iPos != LB_ERR ) m_wndSkillList.SetItemData( iPos, (DWORD)pInvestigatorSkill );
        }
    }
}

void CEditSkillsDlg::OnInformation() 
{
    UpdateData(UD_GETDATA);
    if( m_fInformation )
        m_pwndInformationPopup->ShowWindow( SW_SHOWNA );
    else
        m_pwndInformationPopup->ShowWindow( SW_HIDE );

    AfxGetApp()->WriteProfileInt( "WindowPos", "SkillInfo", m_fInformation );
}

void CEditSkillsDlg::OnSelchangeSkillList() 
{
    CString strSkill;
    int i = m_wndSkillList.GetCurSel();
    if( i != LB_ERR && m_wndSkillList.GetCount() > 0 ) m_wndSkillList.GetText(i, strSkill );
    m_pwndInformationPopup->SetSkill( strSkill, m_pInvestigator );
}
