// Output.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "MainFrm.h"
#include "Output.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutput dialog
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


COutput::COutput()
{
}

COutput::~COutput()
{
}

BEGIN_MESSAGE_MAP(COutput, CSizingControlBar)
	//{{AFX_MSG_MAP(COutput)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutput message handlers

void COutput::AddString(CString strText)
{
    //add this item to the end
    int iPos = m_wndText.AddString(strText);
    m_wndText.SetCurSel(iPos);
    
    //remove top item if it's too full
    if( m_wndText.GetCount() > 50 ) m_wndText.DeleteString(0);
}

void COutput::Clear()
{
    //empty the list box
    m_wndText.ResetContent();
}

int COutput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

    //create the list box
    if( !m_wndText.Create( LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL|WS_BORDER|WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 0xFFFF )) return -1;
    m_wndText.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

    //change the list box font
    LoadDisplaySettings();
	return 0;
}

void COutput::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBar::OnSize(nType, cx, cy);

    CRect rc;
    GetClientRect( rc );

    //resize the list box
    m_wndText.SetWindowPos( NULL, 4, 4, rc.right-8, rc.bottom-8, SWP_NOZORDER );
}

void COutput::LoadDisplaySettings()
{
    //delete current font and brush
    m_Font.DeleteObject();

    //initialise LOGFONT structure
    LOGFONT lf;
    ZeroMemory( &lf, sizeof(LOGFONT) );

    //load settings
    CString strSection = "Output";
    strcpy( lf.lfFaceName,  AfxGetApp()->GetProfileString( strSection, "FontFace", "MS Sans Serif" ) );
    lf.lfHeight =           AfxGetApp()->GetProfileInt( strSection, "FontSize", -10 );
    lf.lfItalic =           AfxGetApp()->GetProfileInt( strSection, "FontItalic", 0 );
    lf.lfWeight =           AfxGetApp()->GetProfileInt( strSection, "FontWeight", 0 );
    COLORREF crBackground = AfxGetApp()->GetProfileInt( strSection, "BkColour", GetSysColor(COLOR_WINDOW) );
    COLORREF crForeground = AfxGetApp()->GetProfileInt( strSection, "TxColour", GetSysColor(COLOR_WINDOWTEXT) );

    //apply settings
    m_wndText.SetColours( crForeground, crBackground );
    m_Font.CreateFontIndirect( &lf );
    m_wndText.SetFont( &m_Font, TRUE );
}

/******************************************************************************************************************/


CScriptObject::MEMBERRESULT CScriptOutput::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
    COutput& wndOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWindow();

    if( stricmp(pszName, "write") == 0 )
    {
        if( nParameters == 1 )
        {
            //FIXME: add multiple lines for each \r and/or \n 
            wndOutput.AddString( LPCSTR(pParameters[0]) );
            wndOutput.m_wndText.RedrawWindow();
            return OK;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    if( stricmp(pszName, "show") == 0 )
    {
        if( nParameters == 0 )
        {
            ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(&wndOutput, TRUE, FALSE);
            wndOutput.m_wndText.RedrawWindow();
            return OK;
        }
        else
            return TOO_MANY_PARAMS;
    }

    if( stricmp(pszName, "hide") == 0 )
    {
        if( nParameters == 0 )
        {
            ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(&wndOutput, FALSE, FALSE);
            return OK;
        }
        else
            return TOO_MANY_PARAMS;
    }

    if( stricmp(pszName, "clear") == 0 )
    {
        if( nParameters == 0 )
        {
            wndOutput.Clear();
            wndOutput.m_wndText.RedrawWindow();
            return OK;
        }
        else
            return TOO_MANY_PARAMS;
    }

    return NOT_FOUND;
}
