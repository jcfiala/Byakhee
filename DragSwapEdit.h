#if !defined(AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragSwapEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragSwapEdit window
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

class CDragSwapEdit : public CEdit
{
// Construction
public:
	CDragSwapEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragSwapEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSwapRect( CRect rc );
	virtual ~CDragSwapEdit();

	// Generated message map functions
protected:
	CEdit* GetEditControlAtPoint( CPoint point );
	HCURSOR m_hCurDragDrop;
	BOOL m_fMouseDown;
	BOOL m_fDragging;
    CRect m_rcSwapVisual;
	//{{AFX_MSG(CDragSwapEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_)
