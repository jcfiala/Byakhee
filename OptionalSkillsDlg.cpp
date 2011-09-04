// OptionalSkillsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "OptionalSkillsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionalSkillsDlg dialog
//void CInvestigator::AddOccupationalSkill(CString strSkill)
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


COptionalSkillsDlg::COptionalSkillsDlg( CInvestigator* pInvestigator, COccupation* pOccupation, CWnd* pParent /*=NULL*/)
	: CDialog(COptionalSkillsDlg::IDD, pParent)
{
    m_pInvestigator = pInvestigator;
    m_pOccupation = pOccupation;

    ASSERT(pInvestigator);
    ASSERT(pOccupation);

    //{{AFX_DATA_INIT(COptionalSkillsDlg)
	//}}AFX_DATA_INIT
	m_strNumberOfOptions.Format( "%d", pOccupation->m_nOptional );
}


void COptionalSkillsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionalSkillsDlg)
	DDX_Control(pDX, IDC_SKILLS, m_wndSkills);
	DDX_Text(pDX, IDC_NUMBEROFOPTIONS, m_strNumberOfOptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionalSkillsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionalSkillsDlg)
	ON_LBN_DBLCLK(IDC_SKILLS, OnDblclkSkills)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionalSkillsDlg message handlers

BOOL COptionalSkillsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    for( int n = 0; n < m_pOccupation->m_OptionalSkillArray.GetSize(); n++ )
    {
        CString strSkill = m_pOccupation->m_OptionalSkillArray[n];

        //terminate at the $
        int nDollarPos = strSkill.Find( '$' );
        if( nDollarPos != -1 ) strSkill = strSkill.Left( nDollarPos );

        //try and find it
        CSkill* pSkill = SkillManager.FindSkill( m_pInvestigator->m_strRuleFile, strSkill );

        //only add it if it's the right era, or it doesn't exist (FIXME?)
        if( ( pSkill && pSkill->IsEra( m_pInvestigator->m_Era ) ) || pSkill == NULL )
        {
            m_wndSkills.AddString( strSkill );
        }
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionalSkillsDlg::OnOK() 
{
	//make sure that there isn't too many selected
	if( m_wndSkills.GetSelCount() > atoi(m_strNumberOfOptions) )
	{
		CString str;
		str.Format( "Sorry - You can only select up to %s skills for this occupation", m_strNumberOfOptions );
		AfxMessageBox( str, MB_ICONEXCLAMATION );
		return;
	}

	//make sure they've actually selected *something*
	if( m_wndSkills.GetSelCount() == 0 )
	{
		if( AfxMessageBox( "Are you sure you don't want to select any of these skills?", MB_YESNO|MB_ICONQUESTION ) == IDYES )
			CDialog::OnOK();

		return;
	}

    //enumerate all selected skills and set them as occupational
    int nMax = m_wndSkills.GetCount();
    for( int n = 0; n < nMax; n++ )
    {
        //see if this skill is selected
        if( m_wndSkills.GetSel(n) > 0 )
        {
            //get the item's caption
            CString strSkillName;
            m_wndSkills.GetText( n, strSkillName );

            //make it an occupational skill
            m_pInvestigator->AddOccupationalSkill( strSkillName );
        }
    }

	CDialog::OnOK();
}

void COptionalSkillsDlg::OnDblclkSkills() 
{
    UpdateData(UD_GETDATA);
    if( atoi(m_strNumberOfOptions) == 1 ) OnOK();
}
