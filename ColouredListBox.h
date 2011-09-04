#if !defined(AFX_COLOUREDLISTBOX_H__B2436904_A23E_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_COLOUREDLISTBOX_H__B2436904_A23E_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColouredListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColouredListBox window
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

class CColouredListBox : public CListBox
{
// Construction
public:
	CColouredListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColouredListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual void SetFont( CFont* pFont, BOOL fRedraw = TRUE );
	void SetColours( COLORREF crForeground, COLORREF crBackground );
	virtual ~CColouredListBox();

	// Generated message map functions
protected:
	int GetFontHeight();
	HBRUSH m_hBrBackground;
	COLORREF m_crForeground;
	COLORREF m_crBackground;
	//{{AFX_MSG(CColouredListBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOUREDLISTBOX_H__B2436904_A23E_11D2_A50C_00105A4ACC2C__INCLUDED_)
