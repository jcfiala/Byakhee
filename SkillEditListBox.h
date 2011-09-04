#if !defined(AFX_SKILLEDITLISTBOX_H__4F618460_614E_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SKILLEDITLISTBOX_H__4F618460_614E_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Investigator.h"
#include "NamedOptionManager.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSkillComboEditCtrl window
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

class CSkillComboEditCtrl : public CEdit
{
// Construction
public:
	CSkillComboEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillComboEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_fSubclassed;
	void SetOwnerList( class CSkillEditListBox* pOwnerList );
	virtual ~CSkillComboEditCtrl();

	// Generated message map functions
protected:
	class CSkillEditListBox* m_pOwnerList;
	//{{AFX_MSG(CSkillComboEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrlFrame window

class CSkillEditCtrlFrame : public CStatic
{
// Construction
public:
	CSkillEditCtrlFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillEditCtrlFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkillEditCtrlFrame();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkillEditCtrlFrame)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrl window

class CSkillEditCtrl : public CEdit
{
// Construction
public:
	CSkillEditCtrl();

// Attributes
public:
    CSpinButtonCtrl m_wndSpinButtonCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillEditCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOwnerList( class CSkillEditListBox* pOwnerList );
	virtual ~CSkillEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkillEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	class CSkillEditListBox* m_pOwnerList;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSkillComboCtrl window

class CSkillComboCtrl : public CComboBox
{
// Construction
public:
	CSkillComboCtrl();

// Attributes
public:
	CSkillComboEditCtrl m_wndComboEditCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillComboCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOwnerList( class CSkillEditListBox* pOwnerList );
	virtual ~CSkillComboCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkillComboCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CSkillEditListBox* m_pOwnerList;
};



/////////////////////////////////////////////////////////////////////////////
// CSkillEditListBox window

class CSkillEditListBox : public CListBox
{
// Construction
public:
	CSkillEditListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillEditListBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Initialise();
	void SetInvestigator( BOOL fOccupationalOnly, class CInvestigator* m_pInvestigator );
	void SetPointsDisplayWnd( CStatic* pwndStatic );
	virtual ~CSkillEditListBox();

	void ChangePercentageEditing();
	void BeginPercentageEditing();
	void EndPercentageEditing( BOOL fSaveChanges = TRUE );

	void ChangeSpecialEditing();
	void BeginSpecialEditing();
	void EndSpecialEditing( BOOL fSaveChanges = TRUE );

    BOOL IsPercentageEditing();
    BOOL IsSpecialEditing();

	// Generated message map functions
protected:
	void FillComboWithOptions( CString strOptionName );
	BOOL m_fOccupationalOnly;
	class CInvestigator* m_pInvestigator;
	CStatic* m_pWndStatic;
    CSkillEditCtrl m_wndEditCtrl;
    CSkillComboCtrl m_wndComboCtrl;
    CSkillEditCtrlFrame m_wndEditCtrlFrame;

	//{{AFX_MSG(CSkillEditListBox)
	afx_msg void OnSelChange();
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CNamedOptionManager* m_pNamedOptionManager;
	CInvestigatorSkill* m_pInvestigatorSkill;
	int m_nOldValue;
	BOOL m_fPercentageEditing, m_fSpecialEditing;
    BOOL m_fCtrlInit;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLEDITLISTBOX_H__4F618460_614E_11D2_9FF9_00001C192944__INCLUDED_)
