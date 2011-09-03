// Hyperlink.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "MainFrm.h"
#include "Hyperlink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperlink

CHyperlink::CHyperlink()
{
    m_fMouseOver = FALSE;
    m_fCaptured = FALSE;
    m_fGotFocus = FALSE;
    m_fVisited = FALSE;
}

CHyperlink::~CHyperlink()
{
}


IMPLEMENT_DYNAMIC(CHyperlink, CStatic)

BEGIN_MESSAGE_MAP(CHyperlink, CStatic)
	//{{AFX_MSG_MAP(CHyperlink)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperlink message handlers

void CHyperlink::SetURL(CString strURL)
{
    m_strURL = strURL;
}

CString CHyperlink::GetURL()
{
    return m_strURL;
}

void CHyperlink::SetVisited(BOOL fVisited)
{
    m_fVisited = fVisited;
    Invalidate();
}



void CHyperlink::PreSubclassWindow() 
{
    //get the current window font	
    CFont* pFont = GetFont();	
	
    //set underline style
	if( pFont != NULL )
    {
		LOGFONT lf;
		pFont->GetLogFont( &lf );
		lf.lfUnderline = TRUE;
		if( m_Font.CreateFontIndirect( &lf ) ) CStatic::SetFont(&m_Font);    
	}

    //continue...
	CStatic::PreSubclassWindow();
}

void CHyperlink::SetFont( CFont* pFont )
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(pFont != NULL);

    //create a copy of the font, with the underline bit set
    LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfUnderline = TRUE;
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&lf);

    //pass the SetFont on
	CStatic::SetFont(&m_Font);
}

HBRUSH CHyperlink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    //change colour depending on where the mouse is and what it's doing
    if( m_fCaptured )
        pDC->SetTextColor( RGB(255,0,0) );
    else
        if( m_fMouseOver || m_fGotFocus )
            pDC->SetTextColor( RGB(0,0,255) );
        else
            if( m_fVisited )
                pDC->SetTextColor( RGB(128,0,128) );
            else
                pDC->SetTextColor( RGB(0,0,128) );

    //no background
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CHyperlink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    ::SetCursor( AfxGetApp()->LoadCursor( MAKEINTRESOURCE(IDC_HAND) ) );	
	return TRUE;
}

UINT CHyperlink::OnNcHitTest(CPoint point) 
{
	return HTCLIENT; //quick fix (Paul DiLascia)
}

void CHyperlink::OnLButtonDown(UINT nFlags, CPoint point) 
{
    m_fCaptured = TRUE;
    SetCapture();	
    Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
}

void CHyperlink::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_fCaptured = FALSE;
    ReleaseCapture();
    if( m_fMouseOver )
    {
        GotoURL( m_strURL );
        m_fVisited = TRUE;
    }
    else
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );

    Invalidate();

	CStatic::OnLButtonUp(nFlags, point);
}


void CHyperlink::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
    m_fGotFocus = TRUE;	
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( m_strURL );
    Invalidate();
}

void CHyperlink::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);
	m_fGotFocus	= FALSE;
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
    Invalidate();
}

void CHyperlink::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if( nChar == VK_SPACE )
    {
        GotoURL( m_strURL );	
        m_fVisited = TRUE;
        Invalidate();
    }
    else
	    CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

// "GotoURL" function by Stuart Patterson appeared in the Aug, 1997
// Windows Developer's Journal.
void CHyperlink::GotoURL(CString strURL)
{
    TCHAR szKey[MAX_PATH + MAX_PATH];	

    //first try ShellExecute()
    HINSTANCE hResult = ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL );

    //if it failed, get the .htm regkey and lookup the program
    if((UINT)hResult <= HINSTANCE_ERROR )
    {
        if( GetRegKey( HKEY_CLASSES_ROOT, _T(".htm"), szKey ) == ERROR_SUCCESS )
        {
            lstrcat( szKey, _T("\\shell\\open\\command" ) );
            if( GetRegKey( HKEY_CLASSES_ROOT, szKey, szKey) == ERROR_SUCCESS )
            {
                TCHAR *pos;
                pos = _tcsstr( szKey, _T("\"%1\""));
                if( pos == NULL )
                {
                    //no quotes found
                    pos = strstr( szKey, _T("%1")); //check for %1, without quotes
                    if( pos == NULL ) //no parameter at all...
                        pos = szKey+lstrlen(szKey)-1;
                    else
                        *pos = '\0'; //remove the parameter
                }
                else
                    *pos = '\0'; //remove the parameter

                lstrcat( pos, _T(" ") );
                lstrcat( pos, strURL );
                hResult = (HINSTANCE)WinExec( szKey, SW_SHOWNORMAL );
            }
        }
	}
	
    //analyse result and display errors etc
    if( int(hResult) < HINSTANCE_ERROR )
    {
        AfxMessageBox( CString("Unable to open the requested URL\n") + m_strURL, MB_ICONERROR );
    }
}

LONG CHyperlink::GetRegKey( HKEY hKeyRoot, LPCTSTR szSubkey, LPTSTR szResult )
{
    HKEY hKey;
    LONG lResult = RegOpenKeyEx( hKeyRoot, szSubkey, 0, KEY_QUERY_VALUE, &hKey );

    if( lResult == ERROR_SUCCESS )
    {
        long lSize = MAX_PATH;
		TCHAR szData[MAX_PATH];

		RegQueryValue( hKey, NULL, szData, &lSize );
		lstrcpy( szResult, szData );
		RegCloseKey( hKey );
    }

    return lResult;
}

void CHyperlink::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( !m_fMouseOver )
    {
        m_fMouseOver = TRUE;
        Invalidate();
        SetTimer( 0xFFFF, 100, NULL );

        //show URL in main frame status bar
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( m_strURL );
    }
	
	CStatic::OnMouseMove(nFlags, point);
}

void CHyperlink::OnTimer(UINT nIDEvent) 
{
    //get cursor position
    CPoint pt;
    GetCursorPos(&pt);

    //get control size in screen coordinates
    CRect rc;
    GetClientRect( &rc );
    ClientToScreen( &rc );

    //if mouse is not over control, turn off hover
    if( rc.PtInRect(pt) == FALSE  )
	{
		m_fMouseOver = FALSE;
        Invalidate();
        KillTimer(nIDEvent);

        //clear status message
        if( !m_fCaptured )
        {
            //only wipe the status message if it hasn't changed
            if( ((CMainFrame*)AfxGetMainWnd())->GetStatusBar().GetPaneText(0).Compare( m_strURL ) == 0 ) ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        }
    }
	
	CStatic::OnTimer(nIDEvent);
}
