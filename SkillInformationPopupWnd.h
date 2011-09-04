#if !defined(AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SkillInformationPopupWnd.h : header file
//

#include "Skill.h"
#include "PopupWnd.h"
#include "SkillManager.h"

/////////////////////////////////////////////////////////////////////////////
// CSkillInformationPopupWnd window
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
class CSkillInformationPopupWnd : public CPopupWnd
{
// Construction
public:
	CSkillInformationPopupWnd( CString strRuleFile );
	virtual ~CSkillInformationPopupWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillInformationPopupWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void MoveIntoPosition();
	void SetSkill( CString strSkill, CInvestigator* pInvestigator  = NULL );

	// Generated message map functions
protected:
	HICON m_hIcon;
	BOOL m_f5_5Mode;
	CString m_strRuleFile;
	CString GetEraDescription( ERA_MASK EraMask );
	CSkill* m_pSkill;
	//{{AFX_MSG(CSkillInformationPopupWnd)
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_)
