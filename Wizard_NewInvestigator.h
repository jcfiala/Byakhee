// Wizard_NewInvestigator.h : header file
//
// This class defines custom modal property sheet 
// CWizard_NewInvestigator.

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
