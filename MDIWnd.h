// MDIWnd.h : header file
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
// CMDIWnd window

class CMDIWnd : public CWnd
{
// Construction
public:
    CMDIWnd();
    virtual ~CMDIWnd();

// Attributes
public:
	FILE* m_pStream;
    BOOL m_fTipsLoadedOK;
    CSize m_sizeBitmap;
	CString	m_strTip;
	void HideTipWindow();
	void ShowNextTip();
	void ShowTip();
    int m_iFade;

    void GetNextTipString(CString& strNext);
	void DrawTipWindow( HDC hDC, int x, int y );
    HBITMAP m_hBmBackground;

    BOOL m_fStartup;
    BOOL m_fTipVisible;
    
// Operations
public:

// Implementation
public:
    // Generated message map functions
protected:
	CRect GetTipRect();

    //{{AFX_MSG(CMDIWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	void DrawTipData( HDC hDC, CRect rc  );
};

/////////////////////////////////////////////////////////////////////////////
