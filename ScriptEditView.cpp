// ScriptEditView.cpp : implementation of the CScriptEditView class
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
#include "MainFrm.h"
#include "ScriptEditDoc.h"
#include "ScriptEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView

IMPLEMENT_DYNCREATE(CScriptEditView, CEditView)

BEGIN_MESSAGE_MAP(CScriptEditView, CEditView)
	//{{AFX_MSG_MAP(CScriptEditView)
	ON_WM_CREATE()	
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
    ON_MESSAGE(MY_APPSETTINGSCHANGE, OnAppSettingsChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView construction/destruction

CScriptEditView::CScriptEditView()
{
}

CScriptEditView::~CScriptEditView()
{
}

BOOL CScriptEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView diagnostics

#ifdef _DEBUG
void CScriptEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CScriptEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CScriptEditDoc* CScriptEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScriptEditDoc)));
	return (CScriptEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptEditView message handlers

int CScriptEditView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CEditView::OnCreate(lpcs) != 0) return -1;

    UpdateFont();
	SetTabStops( 4*4 );
	return 0;
}

void CScriptEditView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CEditView::OnKeyDown( nChar, nRepCnt, nFlags );
    UpdateLineNumber();
}


void CScriptEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
    UpdateLineNumber();
	CEditView::OnMouseMove( nFlags, point );
}


void CScriptEditView::UpdateFont()
{
    LOGFONT lf;
    ZeroMemory( &lf, sizeof(LOGFONT) );

    CString strSection = "Edit";
	strcpy( lf.lfFaceName, AfxGetApp()->GetProfileString( strSection, "FontFace", "Courier" ) );
	lf.lfHeight = AfxGetApp()->GetProfileInt( strSection, "FontSize", 15 );
    lf.lfWeight = AfxGetApp()->GetProfileInt( strSection, "FontWeight", FW_NORMAL );
    lf.lfItalic = AfxGetApp()->GetProfileInt( strSection, "FontItalic", 0 );

	m_font.DeleteObject();
	m_font.CreateFontIndirect( &lf );
	SetFont( &m_font, TRUE );
}

void CScriptEditView::UpdateLineNumber()
{
	char szLine[32];
	wsprintf( szLine, "Line %d", GetEditCtrl().LineFromChar( GetEditCtrl().LineIndex() ) + 1 );
    ((CMainFrame*)AfxGetMainWnd())->GetStatusBar().SetPaneText( ((CMainFrame*)AfxGetMainWnd())->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), szLine );
}

LRESULT CScriptEditView::OnAppSettingsChange(WPARAM w, LPARAM l)
{
    UpdateFont();	
    RedrawWindow();
	return 0L;
}

void CScriptEditView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
    if( bActivate )
        UpdateLineNumber();
    else
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );


	//do default stuff
	CEditView::OnActivateView( bActivate, pActivateView, pDeactiveView );
}

void CScriptEditView::OnSetFocus(CWnd* pOldWnd) 
{
	CEditView::OnSetFocus(pOldWnd);
    UpdateLineNumber();
}
