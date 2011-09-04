// PopupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "PopupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupWnd
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

CString CPopupWnd::m_strClassName;

CPopupWnd::CPopupWnd()
{
	m_nCharHeight = 15;
    m_nMaxCharWidth = 8;
    m_nWidth = 190;
    m_nHeight = 120;
}

CPopupWnd::~CPopupWnd()
{
}


BEGIN_MESSAGE_MAP(CPopupWnd, CWnd)
	//{{AFX_MSG_MAP(CPopupWnd)
	ON_WM_NCHITTEST()
	ON_WM_DEVMODECHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPopupWnd message handlers

LRESULT CPopupWnd::OnNcHitTest(CPoint point) 
{
    //this allows the window to be moved just by dragging it around from anywhere
    return HTCAPTION;
}

void CPopupWnd::Create( CWnd* pWndParent )
{
    //set up window size
    CRect rc( 0, 0, m_nWidth, m_nHeight );

    //create the window
    if( m_strClassName.IsEmpty() ) m_strClassName = AfxRegisterWndClass(CS_OWNDC, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_INFOBK) );
    CWnd::CreateEx( 0, m_strClassName, "", WS_BORDER|WS_POPUP, rc, pWndParent, 0 );
}

void CPopupWnd::OnDevModeChange(LPTSTR lpDeviceName) 
{
	CWnd::OnDevModeChange(lpDeviceName);
	MoveIntoPosition();
}

void CPopupWnd::CalcFontSizes()
{
    //prepare the device and font
    CDC* pDC = GetDC();
    CFont Font, *pOldFont;
    Font.Attach( ::GetStockObject(ANSI_VAR_FONT) );
    pOldFont = pDC->SelectObject( &Font );

    //get the text metrics
    TEXTMETRIC tm;
    pDC->GetOutputTextMetrics( &tm );

    //store what we want
    m_nCharHeight = tm.tmHeight;
    m_nMaxCharWidth = tm.tmMaxCharWidth;

    //reset the device
    pDC->SelectObject( pOldFont );
    ReleaseDC( pDC );
}
