// Wizard_NewInvestigator.h : header file
//
// This class defines custom modal property sheet 
// CWizard_NewInvestigator.
 
#ifndef __WIZARD5F_NEWINVESTIGATOR_H__
#define __WIZARD5F_NEWINVESTIGATOR_H__

#include "Wizard_NewInvestigatorPages.h"
#include "Investigator.h"
#include "InvestigatorPopupWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigator

class CWizard_NewInvestigator : public CPropertySheet
{
	DECLARE_DYNAMIC(CWizard_NewInvestigator)

// Construction
public:
	CWizard_NewInvestigator( CInvestigator* pInvestigator, CWnd* pWndParent = NULL);

// Attributes
public:

	CWizard_NewInvestigatorPage1 m_Page1;
	CWizard_NewInvestigatorPage2 m_Page2;
	CWizard_NewInvestigatorPage3 m_Page3;
	CWizard_NewInvestigatorPage4 m_Page4;
	CWizard_NewInvestigatorPage5 m_Page5;
	CWizard_NewInvestigatorPage6 m_Page6;
	CWizard_NewInvestigatorPage7 m_Page7;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigator)
	//}}AFX_VIRTUAL

// Implementation
public:
	CInvestigatorPopupWnd m_wndInvestigatorPopup;
	void Undo();
	void StoreUndo();
	int m_nSkillPoints;
    int m_nPersonalInterestPoints;
	virtual ~CWizard_NewInvestigator();

// Generated message map functions
protected:
	//{{AFX_MSG(CWizard_NewInvestigator)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CInvestigator* m_pInvestigator;
	CInvestigator m_InvestigatorUndo;
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __WIZARD5F_NEWINVESTIGATOR_H__
