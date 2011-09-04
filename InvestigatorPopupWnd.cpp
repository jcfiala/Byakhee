// InvestigatorPopupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "InvestigatorPopupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorPopupWnd
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

CInvestigatorPopupWnd::CInvestigatorPopupWnd()
{
    m_pInvestigator = NULL;
    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_SHEETTYPE), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CInvestigatorPopupWnd::~CInvestigatorPopupWnd()
{
    DestroyIcon( m_hIcon );
}


BEGIN_MESSAGE_MAP(CInvestigatorPopupWnd, CPopupWnd)
	//{{AFX_MSG_MAP(CInvestigatorPopupWnd)
	ON_WM_PAINT()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInvestigatorPopupWnd message handlers

void CInvestigatorPopupWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    //draw icon
    ::DrawIconEx( dc.GetSafeHdc(), m_nWidth-20, 4, m_hIcon, 0, 0, 0, NULL, DI_NORMAL );

    //display investigator info
    if( m_pInvestigator )
    {
        //set the text colour to the user's chosen tooltip colour
        dc.SetTextColor( ::GetSysColor(COLOR_INFOTEXT) );
        CString strText;

        //change the font to the dialog font
        CFont Font, *pOldFont;
        Font.Attach( ::GetStockObject(ANSI_VAR_FONT) );
        pOldFont = dc.SelectObject( &Font );

        //set up transparent drawing
        dc.SetBkMode(TRANSPARENT);

        //display each field
        int y = 5, x1 = 5, x2 = 5+(m_nMaxCharWidth*5), x3 = 5+(m_nMaxCharWidth*10);
        char* pszEra = NULL;
        switch( m_pInvestigator->m_Era )
        {
            case Era_1890: pszEra = "1890"; break;
            case Era_1920: pszEra = "1920"; break;
            case Era_1990: pszEra = "1990"; break;
            default: pszEra = "INTERNAL ERROR"; break;
        }
        if( m_pInvestigator->m_strOccupation.GetLength() > 0 && m_pInvestigator->m_strOccupation.CompareNoCase("(custom)") != 0 )
            strText.Format( "%ss %s", pszEra, m_pInvestigator->m_strOccupation );
        else
            strText.Format( "%ss Investigator", pszEra ); 
        dc.TextOut( x1, y, strText );

        y += m_nCharHeight + (m_nCharHeight/2);
        strText.Format( "APP: %d", m_pInvestigator->m_nAPP ); dc.TextOut( x1, y, strText );
	    strText.Format( "CON: %d", m_pInvestigator->m_nCON ); dc.TextOut( x2, y, strText );
	    strText.Format( "DEX: %d", m_pInvestigator->m_nDEX ); dc.TextOut( x3, y, strText );

        y += m_nCharHeight;
	    strText.Format( "EDU: %d", m_pInvestigator->m_nEDU ); dc.TextOut( x1, y, strText );
	    strText.Format( "INT: %d", m_pInvestigator->m_nINT ); dc.TextOut( x2, y, strText );
	    strText.Format( "POW: %d", m_pInvestigator->m_nPOW ); dc.TextOut( x3, y, strText );

        y += m_nCharHeight;
	    strText.Format( "SIZ: %d", m_pInvestigator->m_nSIZ ); dc.TextOut( x1, y, strText );
	    strText.Format( "STR: %d", m_pInvestigator->m_nSTR ); dc.TextOut( x2, y, strText );
        strText.Format( "SAN: %d", m_pInvestigator->m_nSAN ); dc.TextOut( x3, y, strText );

        y += m_nCharHeight;
        strText.Format( "HP: %d", m_pInvestigator->m_nHitPoints );  dc.TextOut( x1, y, strText );
        strText.Format( "DB: %s", m_pInvestigator->GetDBString() ); dc.TextOut( x2, y, strText );
        strText.Format( "Magic: %d", m_pInvestigator->m_nMagic );   dc.TextOut( x3, y, strText );

        y += m_nCharHeight + (m_nCharHeight/2);
        switch( m_pInvestigator->m_ExtraSchooling )
        {
            default:
            case CInvestigator::EXTRASCHOOLING_NOTAPPLICABLE: break;
            case CInvestigator::EXTRASCHOOLING_FAILED:  dc.TextOut( x1, y, "(Extra schooling roll failed)" ); break;
            case CInvestigator::EXTRASCHOOLING_SUCCESS: dc.TextOut( x1, y, "(Extra schooling roll succeeded)" ); break;
        }

        //put the old font back
        dc.SelectObject( pOldFont );
        Font.Detach();
   }
}

void CInvestigatorPopupWnd::SetInvestigator( CInvestigator * pInvestigator)
{
    if( m_pInvestigator != NULL && pInvestigator == NULL )
    {
        //save settings if we're being hidden
        CPoint pt(0,0);
        ClientToScreen(&pt);
        AfxGetApp()->WriteProfileInt( "WindowPos", "InvestigatorInfoX", pt.x );
        AfxGetApp()->WriteProfileInt( "WindowPos", "InvestigatorInfoY", pt.y );
    }

    m_pInvestigator = pInvestigator;
    if( m_pInvestigator == NULL )
        ShowWindow(SW_HIDE);
    else
    {
        MoveIntoPosition();
        ShowWindow(SW_SHOWNA);
    }
}

void CInvestigatorPopupWnd::MoveIntoPosition()
{
    //get the various window rectangles
    CRect rcClientArea;
    ::GetWindowRect( ((CMainFrame*)AfxGetMainWnd())->m_hWndMDIClient, &rcClientArea );

    CRect rcScreen( 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) );

    //calculate the size of the font to be used
    CalcFontSizes();
    m_nWidth = m_nMaxCharWidth * 16;
    m_nHeight = m_nCharHeight * 8;

    //set up window size
    int x, y;
    x = rcClientArea.right - m_nWidth - 16;
    y = rcClientArea.bottom - m_nHeight - 16;

    //load previous position
    x = AfxGetApp()->GetProfileInt( "WindowPos", "InvestigatorInfoX", x );
    y = AfxGetApp()->GetProfileInt( "WindowPos", "InvestigatorInfoY", y );

    //don't let it go off the screen
    if( x < 0 ) x = 0;
    if( y < 0 ) y = 0;
    if( x + m_nWidth > rcScreen.right ) x = rcScreen.right - m_nWidth;
    if( y + m_nHeight > rcScreen.bottom ) y = rcScreen.bottom - m_nHeight;

    //set the new window size and position
    CRect rc( 0, 0, m_nWidth, m_nHeight );
    rc.OffsetRect( x, y );

    //move the window
    MoveWindow( rc );
}

void CInvestigatorPopupWnd::OnMove(int x, int y) 
{
	CPopupWnd::OnMove(x, y);
	
    if( IsWindowVisible() )
    {
        CPoint pt(0,0);
        ClientToScreen(&pt);
        AfxGetApp()->WriteProfileInt( "WindowPos", "InvestigatorInfoX", pt.x );
        AfxGetApp()->WriteProfileInt( "WindowPos", "InvestigatorInfoY", pt.y );
    }
}
