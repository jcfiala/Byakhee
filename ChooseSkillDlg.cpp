// ChooseSkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "ChooseSkillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChooseSkillDlg dialog
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


CChooseSkillDlg::CChooseSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseSkillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseSkillDlg)
	m_strCustomPrompt = _T("");
	//}}AFX_DATA_INIT

    m_pInvestigator = NULL;
    m_fSetAsOccupational = FALSE;
    CInvestigatorSkill* m_pInvestigatorSkill = NULL;
}


void CChooseSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseSkillDlg)
	DDX_Control(pDX, IDC_SKILLS, m_wndSkills);
	DDX_Text(pDX, IDC_CUSTOMPROMPT, m_strCustomPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseSkillDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseSkillDlg)
	ON_LBN_DBLCLK(IDC_SKILLS, OnDblclkSkills)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseSkillDlg message handlers

void CChooseSkillDlg::OnDblclkSkills() 
{
    OnOK();
}

BOOL CChooseSkillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    ASSERT( m_pInvestigator );

    //set the tab stops
	int nBaseUnits = GetDialogBaseUnits();
	int nTabStops[] = { 10*nBaseUnits };

	m_wndSkills.SetTabStops( sizeof(nTabStops)/sizeof(int), nTabStops );
	
    //add the skills
    if( m_fSetAsOccupational )
    {
        for( int i = 0; i < m_pInvestigator->m_AllSkillList.GetSize(); i++ )
	    {
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->m_AllSkillList.GetAt(i);
            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL && pInvestigatorSkill->m_pSkill->m_SkillType != ANYSKILL && pInvestigatorSkill->m_fOccupational == FALSE && pInvestigatorSkill->m_pSkill->m_strBase[0] != '*' /*prefix * == read only*/ && pInvestigatorSkill->m_pSkill->IsEra(m_pInvestigator->m_Era) )
            {
                const char * pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
			    CString strText;
			    strText.Format( "%s\t%d%%", pszName, pInvestigatorSkill->m_nCurrent );
                int i = m_wndSkills.AddString( strText );
			    m_wndSkills.SetItemData( i, (DWORD)pInvestigatorSkill );
            }
	    }
    }
    else
    {
        for( int i = 0; i < m_pInvestigator->m_SkillList.GetSize(); i++ )
	    {
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->m_SkillList.GetAt(i);
            if( pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL && pInvestigatorSkill->m_pSkill->m_SkillType != ANYSKILL && pInvestigatorSkill->m_pSkill->m_SkillType != FIREARM && pInvestigatorSkill->m_pSkill->m_SkillType != MELEE && pInvestigatorSkill->m_pSkill->IsEra(m_pInvestigator->m_Era) )
            {
                const char * pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
			    CString strText;
			    strText.Format( "%s\t%d%%", pszName, pInvestigatorSkill->m_nCurrent );
                int i = m_wndSkills.AddString( strText );
			    m_wndSkills.SetItemData( i, (DWORD)pInvestigatorSkill );
            }
	    }
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChooseSkillDlg::OnOK() 
{
	int i = m_wndSkills.GetCurSel();
	if( i != LB_ERR )
    {
        m_pInvestigatorSkill = (CInvestigatorSkill*)m_wndSkills.GetItemData(i);

        if( m_fSetAsOccupational )
        {
            m_pInvestigatorSkill->m_fOccupational = TRUE;

            //set the occupational flag on all child skills too
            if( m_pInvestigatorSkill->m_pSkill->m_SkillType == SPECIALSKILL )
            {
                CSpecialSkill* pSpecialSkill = (CSpecialSkill*)m_pInvestigatorSkill->m_pSkill;
                for( int n = 0; n < m_pInvestigator->m_SkillList.GetSize(); n++ )
                {
                    CInvestigatorSkill* pSubInvestigatorSkill = m_pInvestigator->m_SkillList.GetAt(n);
                    if( pSubInvestigatorSkill && pSubInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
                    {
                        //get a pointer to it and see if it is a subskill of the selected special skill
                        CSubSkill* pSubSkill = (CSubSkill*)pSubInvestigatorSkill->m_pSkill;
                        if( pSubSkill->m_pSkill == pSpecialSkill ) pSubInvestigatorSkill->m_fOccupational = TRUE;
                    }
                }
            }
        }
    }
	
	CDialog::OnOK();
}

void CChooseSkillDlg::SetAsOccupational(BOOL fSetAsOccupational)
{
    m_fSetAsOccupational = fSetAsOccupational;
}
