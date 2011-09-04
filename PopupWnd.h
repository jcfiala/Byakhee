#if !defined(AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PopupWnd.h : header file
//

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupWnd window
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

class CPopupWnd : public CWnd
{
// Construction
public:
	CPopupWnd();
	virtual ~CPopupWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Create( CWnd* pWndParent );
	virtual void MoveIntoPosition() = 0;

	// Generated message map functions
protected:
    static CString m_strClassName;
	int m_nCharHeight;
    int m_nMaxCharWidth;
    int m_nWidth, m_nHeight;
	void CalcFontSizes();


	//{{AFX_MSG(CPopupWnd)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
