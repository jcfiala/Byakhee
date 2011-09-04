#if !defined(AFX_SPELLBOOKDLG_H__4F2F3565_8790_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SPELLBOOKDLG_H__4F2F3565_8790_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpellbookDlg.h : header file
//
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
/////////////////////////////////////////////////////////////////////////////
// CSpellbookDlg dialog

class CSpellbookDlg : public CDialog
{
// Construction
public:
	CString m_strInvestigatorSpells;
	CSpellbookDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpellbookDlg)
	enum { IDD = IDD_SPELLBOOK };
	CListBox	m_wndInvestigatorSpells;
	CTreeCtrl	m_wndSpellList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellbookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ExpandTreeItem( HTREEITEM hTreeItem );
	void AddSpellBook( const char* pszFileName );
	CImageList m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CSpellbookDlg)
	afx_msg void OnDblclkSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnReturnSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpandingSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRemove();
	virtual void OnOK();
	afx_msg void OnDblclkInvestigatorspells();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLBOOKDLG_H__4F2F3565_8790_11D2_9FF9_00001C192944__INCLUDED_)
