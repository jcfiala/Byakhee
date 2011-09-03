// Wizard_NewInvestigator.cpp : implementation file
//

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
