// Wizard_NewInvestigator.cpp : implementation file
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
#include "MainFrm.h"
#include "Wizard_NewInvestigator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigator

IMPLEMENT_DYNAMIC(CWizard_NewInvestigator, CPropertySheet)

CWizard_NewInvestigator::CWizard_NewInvestigator( CInvestigator* pInvestigator, CWnd* pWndParent )
	 : CPropertySheet(IDS_CAPTION_NEWINVESTIGATOR, pWndParent)
{
    //clear point logs
	m_nSkillPoints = 0;
    m_nPersonalInterestPoints = 0;

    //add all pages
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);

    //give each page a pointer to the investigator
    m_pInvestigator = pInvestigator;
    m_Page1.m_pInvestigator =  m_pInvestigator;
    m_Page2.m_pInvestigator =  m_pInvestigator;
    m_Page3.m_pInvestigator =  m_pInvestigator;
    m_Page4.m_pInvestigator =  m_pInvestigator;
    m_Page5.m_pInvestigator =  m_pInvestigator;
    m_Page6.m_pInvestigator =  m_pInvestigator;
    m_Page7.m_pInvestigator =  m_pInvestigator;

    //change mode
	SetWizardMode();
}

CWizard_NewInvestigator::~CWizard_NewInvestigator()
{
    //clear the status bar
    CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigator, CPropertySheet)
	//{{AFX_MSG_MAP(CWizard_NewInvestigator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWizard_NewInvestigator::StoreUndo()
{
    m_InvestigatorUndo = *m_pInvestigator;
}

void CWizard_NewInvestigator::Undo()
{
    *m_pInvestigator = m_InvestigatorUndo;
}
