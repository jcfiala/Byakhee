// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
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

#if !defined(AFX_MAINFRM_H__A9FB21C8_4B45_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_MAINFRM_H__A9FB21C8_4B45_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Output.h"
#include "MDIWnd.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
    CMDIWnd m_wndMDIClientArea;

// Operations
public:
    //this allows dialogs to pass their tooltip handler to us
    afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult) { return CMDIFrameWnd::OnToolTipText(id,pNMHDR,pResult); }

    COutput& GetOutputWindow() { return m_wndOutput; }
    CToolBar& GetToolbar() { return m_wndToolBar; }
    CStatusBar& GetStatusBar() { return m_wndStatusBar; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetStatusMessage( CString strMessage );
	CComboBox m_wndSheetType;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	COutput m_wndOutput;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewSheetStyle();
	afx_msg void OnClose();
	afx_msg void OnViewHideOutput();
	//}}AFX_MSG
	afx_msg void OnInitMenu(CMenu* pMenu);
    afx_msg void OnTipOfTheDay();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A9FB21C8_4B45_11D2_9FF9_00001C192944__INCLUDED_)
