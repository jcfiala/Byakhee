#if !defined(AFX_EDITSKILLSDLG_H__1EFA32C5_8707_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_EDITSKILLSDLG_H__1EFA32C5_8707_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditSkillsDlg.h : header file
//

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

#include "Investigator.h"
#include "SkillInformationPopupWnd.h"

class CEditSkillsDlg : public CDialog
{
// Construction
public:
	void SetInvestigator( CInvestigator* pInvestigator );
	CEditSkillsDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CEditSkillsDlg();

// Dialog Data
	//{{AFX_DATA(CEditSkillsDlg)
	enum { IDD = IDD_EDITSKILLS };
	CButton	m_wndInformation;
	CListBox	m_wndSkillList;
	BOOL	m_fInformation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSkillsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddInvestigatorSkillsToListBox();
	CInvestigator* m_pInvestigator;
    CSkillInformationPopupWnd* m_pwndInformationPopup;
    HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEditSkillsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnInformation();
	afx_msg void OnSelchangeSkillList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSKILLSDLG_H__1EFA32C5_8707_11D2_9FF9_00001C192944__INCLUDED_)
