// OptionsDlg.h : header file
//
// This class defines custom modal property sheet 
// COptionsDlg.
 
#ifndef __OPTIONSDLG_H__
#define __OPTIONSDLG_H__

#include "OptionsPages.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

class COptionsDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionsDlg)

// Construction
public:
	COptionsDlg(CWnd* pWndParent = NULL);

// Attributes
public:
	COptionsPage1 m_Page1;
	COptionsPage2 m_Page2;
	//COptionsPage3 m_Page3;
	//COptionsPage4 m_Page4;
	//COptionsPage5 m_Page5;
	//COptionsPage6 m_Page6;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ShowRulePicker();
	virtual ~COptionsDlg();

// Generated message map functions
protected:
	//{{AFX_MSG(COptionsDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OPTIONSDLG_H__
