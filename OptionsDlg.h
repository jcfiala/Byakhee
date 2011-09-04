// OptionsDlg.h : header file
//
// This class defines custom modal property sheet 
// COptionsDlg.
 
#ifndef __OPTIONSDLG_H__
#define __OPTIONSDLG_H__

#include "OptionsPages.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg
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
