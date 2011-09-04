#if !defined(AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditSkillDlg.h : header file
//

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CEditSkillDlg dialog
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

class CEditSkillDlg : public CDialog
{
// Construction
public:
	void SetSkill( CInvestigatorSkill* pInvestigatorSkill );
	void SetInvestigator( CInvestigator* pInvestigator );
	CEditSkillDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditSkillDlg)
	enum { IDD = IDD_EDITSKILL };
	CString	m_strBase;
	CString	m_strSkillName;
	BOOL	m_fListWithMainSkills;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSkillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CInvestigatorSkill* m_pInvestigatorSkill;
	CInvestigator* m_pInvestigator;

	// Generated message map functions
	//{{AFX_MSG(CEditSkillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_)
