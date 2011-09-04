// MDIWnd.cpp : implementation file
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

#include "stdafx.h"
#include "Byakhee.h"
#include "MDIWnd.h"

#include <sys\stat.h>
#include <sys\types.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFLEN 1000

#define ID_TIMER_TIMETOGO        0x7771
#define ID_TIMER_FADEREQUEST     0x7772

#define MAX_FADE_FRAMES 10
#define FADE_TIMER_FREQUENCY 100
#define TIMETOGO_TIMER_FREQUENCY 10000

static const TCHAR szSection[] = _T("Tips");
static const TCHAR szIntStartup[] = _T("StartUp");
static const TCHAR szIntFilePos[] = _T("FilePos");
static const TCHAR szTimeStamp[] = _T("TimeStamp");


/////////////////////////////////////////////////////////////////////////////
// CMDIWnd

CMDIWnd::CMDIWnd()
{
    //empty member variables
    m_fTipsLoadedOK = TRUE;
	m_fStartup = !AfxGetApp()->GetProfileInt(szSection, szIntStartup, FALSE );
    m_iFade = 0;
    m_fTipVisible = FALSE;

    //read the file position
	UINT uFilePos = AfxGetApp()->GetProfileInt( szSection, szIntFilePos, 0 );

	//try to open the tips file
	CString strTipsFile = CByakheeApp::GetDefaultDirectory() + CString("tips.lst");
	m_pStream = fopen(strTipsFile, "r");
	if( m_pStream == NULL )
	{
		m_strTip = "Cannot find tips file";
        m_fTipsLoadedOK = FALSE;
		return;
	} 

    //if the saved timestamp differs from that of the file, the tips file has been modifed, so start again
	struct _stat buf;
	_fstat(_fileno(m_pStream), &buf);
	CString strCurrentTime = ctime(&buf.st_ctime);
	strCurrentTime.TrimRight();
	CString strStoredTime = AfxGetApp()->GetProfileString(szSection, szTimeStamp, NULL);
	if( strCurrentTime != strStoredTime )
	{
		uFilePos = 0;
		AfxGetApp()->WriteProfileString(szSection, szTimeStamp, strCurrentTime);
	}

    //seek to the new file position
	if( fseek( m_pStream, uFilePos, SEEK_SET ) != 0 )
	{
        //seek failed - disable tips
		AfxMessageBox("The tips file is corrupt. Cthulhu's thought of the day will not be displayed");
        AfxGetApp()->WriteProfileInt(szSection, szIntStartup, FALSE );
        m_fTipsLoadedOK = FALSE;
	}
	else 
	{
        //load the background bitmap
        m_hBmBackground = ::LoadBitmap( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_TIPOFTHEDAY) );

        //get the bitmap size
        BITMAP bm;
        GetObject( m_hBmBackground, sizeof(BITMAP), &bm );
        m_sizeBitmap.cx = bm.bmWidth;
        m_sizeBitmap.cy = bm.bmHeight;

        //get the next tip string from the file
        GetNextTipString(m_strTip);
	}
}

CMDIWnd::~CMDIWnd()
{
    //save file position
	if( m_pStream != NULL )
	{
		AfxGetApp()->WriteProfileInt( szSection, szIntFilePos, ftell(m_pStream) );
		fclose( m_pStream );
	}

    //delete the background bitmap
    ::DeleteObject( m_hBmBackground );
}

BOOL CMDIWnd::OnEraseBkgnd(CDC* pDC) 
{
    //get the size of the client area on the parent window
    CRect rc;
    GetClientRect( &rc );


    HBRUSH hBrBack = ::GetSysColorBrush(COLOR_APPWORKSPACE);
    HDC hDC = pDC->GetSafeHdc();
    if( m_fTipVisible && m_fTipsLoadedOK )
    {
        //erase around the tip
        CRect rcTip = GetTipRect();
        ::FillRect( hDC, CRect( 0, 0, rc.right, rcTip.top), hBrBack );
        ::FillRect( hDC, CRect( 0, rcTip.top, rcTip.left, rc.bottom), hBrBack );
        ::FillRect( hDC, CRect( rcTip.left, rcTip.bottom, rc.right, rc.bottom), hBrBack );
        ::FillRect( hDC, CRect( rcTip.right, rcTip.top, rc.right, rcTip.bottom), hBrBack );

        //display tip dlg?
        DrawTipWindow( pDC->GetSafeHdc(), /*rc.right - (m_sizeBitmap.cx +*/ 16/*)*/, rc.bottom - (m_sizeBitmap.cy + 16) );
    }
    else
    {
        ::FillRect( hDC, &rc, hBrBack );
    }

    return TRUE;
}


void CMDIWnd::GetNextTipString(CString& strNext)
{
	LPTSTR lpsz = strNext.GetBuffer(MAX_BUFLEN);

    //get the next tip from the tips file
	BOOL fStop = FALSE;
	while( !fStop )
	{
		if( _fgetts(lpsz, MAX_BUFLEN, m_pStream ) == NULL )
		{
            //EOF or error - go back to start
			if( fseek(m_pStream, 0, SEEK_SET) != 0 ) AfxMessageBox("The tips file is corrupt!" );
		} 
		else 
		{
            //don't use lines with whitespace or semicolons at the start, or blank lines
			if (*lpsz != ' ' && *lpsz != '\t' && *lpsz != '\n' && *lpsz != ';')  fStop = TRUE;
		}
	}

    //release the CString's buffer
	strNext.ReleaseBuffer();
}


BEGIN_MESSAGE_MAP(CMDIWnd, CWnd)
    //{{AFX_MSG_MAP(CMDIWnd)
	ON_WM_ERASEBKGND()
    ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIWnd message handlers

void CMDIWnd::OnTimer(UINT nIDEvent) 
{
    switch( nIDEvent )
    {
        case ID_TIMER_TIMETOGO:
        {
            //delete the timer
            KillTimer(ID_TIMER_TIMETOGO);

            //hide the window
            HideTipWindow();
            break;
        }

        case ID_TIMER_FADEREQUEST:
        {
            CRect rc; GetClientRect( &rc );
            HDC hDC = ::GetDC(m_hWnd);
            DrawTipWindow( hDC, /*rc.right - (m_sizeBitmap.cx +*/ 16/*)*/, rc.bottom - (m_sizeBitmap.cy + 16)  );
            ::ReleaseDC( m_hWnd, hDC );

            if( m_iFade++ >= MAX_FADE_FRAMES )
            {
                //stop this timer and hide the window
                KillTimer(ID_TIMER_FADEREQUEST);
                m_fTipVisible = FALSE;
                RedrawWindow( GetTipRect(), NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW );

                //reset fade counter
                m_iFade = 0;
            }
            break;
        }
    }


	CWnd::OnTimer(nIDEvent);
}

void CMDIWnd::DrawTipWindow( HDC hDC, int x, int y )
{
    //note: the only reason this is in WinAPI rather than MFC is because memory leaks were occuring and I couldn't track them down.

    //get client rect
    CRect rc( 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy );

    //prepare an off-screen buffer
    HDC hDCOff = ::CreateCompatibleDC(hDC);
    HBITMAP hBmOff, hBmOld;

    if( m_iFade == 0 )
    {
        hBmOff = ::CreateCompatibleBitmap( hDC, m_sizeBitmap.cx, m_sizeBitmap.cy );
        hBmOld = (HBITMAP)::SelectObject( hDCOff, hBmOff );
        DrawTipData( hDCOff, rc );
    }
    else
    {
        //get the components of the colour we're going to blend with
        COLORREF crMerge = GetSysColor(COLOR_APPWORKSPACE);
        unsigned char chMergeR = GetRValue(crMerge);
        unsigned char chMergeG = GetGValue(crMerge);
        unsigned char chMergeB = GetBValue(crMerge);

        //prepare the bitmap info structure
        BITMAPINFOHEADER bmi = { sizeof(BITMAPINFOHEADER), m_sizeBitmap.cx, m_sizeBitmap.cy, 1, 24, BI_RGB, 0, 0, 0, 0, 0 };

        //create the DIB section and select it into the device context
        unsigned char* pBits = NULL;
        hBmOff = ::CreateDIBSection( NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (VOID**)&pBits, NULL, 0 );
        hBmOld = (HBITMAP)::SelectObject( hDCOff, hBmOff );

        DrawTipData( hDCOff, rc );

        //merge all colour values
        unsigned char* pMerge = pBits;
        double dMerge = double(m_iFade) / double(MAX_FADE_FRAMES);
        for( int ii = 0; ii < rc.right*rc.bottom; ii++ )
        {
            //note: it is still BGR in the buffer!
            *pMerge = unsigned char((double(*pMerge) * (1.0-dMerge)) + (double(chMergeB) * dMerge)); pMerge++;
            *pMerge = unsigned char((double(*pMerge) * (1.0-dMerge)) + (double(chMergeG) * dMerge)); pMerge++;
            *pMerge = unsigned char((double(*pMerge) * (1.0-dMerge)) + (double(chMergeR) * dMerge)); pMerge++;
        }
    }

    //copy off screen buffer to display
    ::BitBlt( hDC, x, y, m_sizeBitmap.cx, m_sizeBitmap.cy, hDCOff, 0, 0, SRCCOPY );

    //clean up
    ::SelectObject( hDCOff, hBmOld );
    ::DeleteDC( hDCOff );
    ::DeleteObject( hBmOff );
}

void CMDIWnd::HideTipWindow()
{
    //we can only hide the tip window if it's there in the first place
    if( ::IsWindow(m_hWnd) && IsWindowVisible() )
    {
        //cancel automatic close request
        KillTimer( ID_TIMER_TIMETOGO ); 

        //do something cheezy if it's not a slow machine
        if( !GetSystemMetrics(SM_SLOWMACHINE) )
        {
            if( m_iFade == 0 ) SetTimer(ID_TIMER_FADEREQUEST, FADE_TIMER_FREQUENCY, NULL );
        }
        else
        {
            //hide the window
            m_fTipVisible = FALSE;
            RedrawWindow( GetTipRect(), NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW );
        }
    }
}

void CMDIWnd::ShowTip()
{
    if( m_fStartup && m_fTipsLoadedOK )
    {
        //show the window
        m_fTipVisible = TRUE;
        RedrawWindow( GetTipRect(), NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW );
        SetTimer( ID_TIMER_TIMETOGO, TIMETOGO_TIMER_FREQUENCY, NULL );
    } 
}

void CMDIWnd::ShowNextTip()
{
    //advance to the next tip
	GetNextTipString(m_strTip);

    //stop fading now!
    KillTimer(ID_TIMER_FADEREQUEST);
    m_iFade = 0;

    //(re) show the window
    m_fTipVisible = TRUE;
    RedrawWindow( GetTipRect(), NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW );
    SetTimer( ID_TIMER_TIMETOGO, TIMETOGO_TIMER_FREQUENCY, NULL );
}


void CMDIWnd::DrawTipData( HDC hDC, CRect rc )
{
    //draw background
    HDC hDCBackground = ::CreateCompatibleDC(hDC);
    HBITMAP hBmBkOld = (HBITMAP)::SelectObject( hDCBackground, m_hBmBackground );
    ::BitBlt( hDC, 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy, hDCBackground, 0, 0, SRCCOPY );
    ::SelectObject( hDCBackground, hBmBkOld );
    ::DeleteDC( hDCBackground );

    //draw border
    ::DrawEdge( hDC, &rc, EDGE_RAISED, BF_RECT|BF_SOFT );

	//prepare caption rectangle
    CRect rcText = rc;
    rcText.top += 8;
    rcText.left += 8;
    CRect rcTitle = rcText;

    //create the dialog font
    HFONT hFont = (HFONT)::GetStockObject(ANSI_VAR_FONT);
    HFONT hFontOld = (HFONT)::SelectObject( hDC, hFont );

    //draw out the tip
    rcText.right -= 8;
    rcText.top += 16;
    rcText.bottom -= 6;
    ::SetBkMode( hDC, TRANSPARENT);
    ::DrawText( hDC, m_strTip, -1, &rcText, DT_TOP|DT_WORDBREAK );
    
    //create caption font
    LOGFONT lf;
    ::GetObject( hFont, sizeof(LOGFONT), &lf );
    lf.lfWeight = FW_BOLD;
    
    HFONT hCaptionFont = ::CreateFontIndirect(&lf);
    ::SelectObject( hDC, hCaptionFont );

    //display caption
    ::DrawText( hDC, "Cthulhu's Thought of the Day", -1, &rcTitle, DT_TOP|DT_SINGLELINE);

    //put old font back
    ::SelectObject( hDC, hFontOld );
    ::DeleteObject( hCaptionFont );
}

CRect CMDIWnd::GetTipRect()
{
    CRect rc;
    GetClientRect( &rc );
    int x = /*rc.right - (m_sizeBitmap.cx +*/ 16/*)*/, y = rc.bottom - (m_sizeBitmap.cy + 16);

    return CRect( x, y, x + m_sizeBitmap.cx, y + m_sizeBitmap.cy );
}

void CMDIWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if( m_fTipVisible && GetTipRect().PtInRect(point) ) HideTipWindow();
	CWnd::OnLButtonUp(nFlags, point);
}

void CMDIWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN );
}
