#if !defined(AFX_OPTIONALSKILLSDLG_H__DB0F6A62_66B5_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_OPTIONALSKILLSDLG_H__DB0F6A62_66B5_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionalSkillsDlg.h : header file
//

#include "SkillManager.h"
#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// COptionalSkillsDlg dialog
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

class COptionalSkillsDlg : public CDialog
{
// Construction
public:
	COptionalSkillsDlg(CInvestigator* pInvestigator, COccupation* pOccupation, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionalSkillsDlg)
	enum { IDD = IDD_OPTIONALSKILLS };
	CListBox	m_wndSkills;
	CString	m_strNumberOfOptions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionalSkillsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionalSkillsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkSkills();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    CInvestigator* m_pInvestigator;
    COccupation* m_pOccupation;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONALSKILLSDLG_H__DB0F6A62_66B5_11D2_9FF9_00001C192944__INCLUDED_)
