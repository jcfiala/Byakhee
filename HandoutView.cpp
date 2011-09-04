// ByakheeView.cpp : implementation of the CHandoutView class
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
#include "HandoutDoc.h"
#include "HandoutView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHandoutView

IMPLEMENT_DYNCREATE(CHandoutView, CScriptSheetView)

BEGIN_MESSAGE_MAP(CHandoutView, CScriptSheetView)
	//{{AFX_MSG_MAP(CHandoutView)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandoutView construction/destruction

CHandoutView::CHandoutView()
{
}

CHandoutView::~CHandoutView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CHandoutView diagnostics

#ifdef _DEBUG
void CHandoutView::AssertValid() const
{
	CScriptSheetView::AssertValid();
}

void CHandoutView::Dump(CDumpContext& dc) const
{
	CScriptSheetView::Dump(dc);
}

CHandoutDoc* CHandoutView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHandoutDoc)));
	return (CHandoutDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHandoutView message handlers

void CHandoutView::OnInitialUpdate() 
{
    //get the document
	CHandoutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    //set the script file extension that we're going to use
    SetSheetFileExtension("bhs");
    m_strSheet = pDoc->m_strSheet;

    CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );

    //pass init onto superclass
	CScriptSheetView::OnInitialUpdate();
}

void CHandoutView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    CHandoutDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnUpdateEditUndo( pCmdUI );
    CScriptSheetView::OnUpdateEditUndo(pCmdUI);
}

void CHandoutView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
    CHandoutDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnUpdateEditRedo( pCmdUI );
    CScriptSheetView::OnUpdateEditRedo(pCmdUI);
}

void CHandoutView::OnEditUndo() 
{
    CHandoutDoc* pDoc = GetDocument();
    if( pDoc ) pDoc->OnEditUndo();
    CScriptSheetView::OnEditUndo();
}

void CHandoutView::StoreUndo()
{
    CHandoutDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    pDoc->StoreUndo();
    
    //assume it's been modified as this is called by editing sheet commands
    pDoc->SetModifiedFlag();
}

void CHandoutView::AddAllFields()
{
    CHandoutDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    /* add all text fields */
    int i;
    for( i = 0; i < MAX_HANDOUT_TEXT_FIELDS; i++ )
    {
        CString strName;
        strName.Format( "text%d", i );

        AddField( &pDoc->m_strStrings[i], strName, FIELD_SCRIPTREAD|FIELD_SCRIPTWRITE|FIELD_TABSTOP );
    }

    /* add all picture fields */
    for( i = 0; i < MAX_HANDOUT_PICTURE_FIELDS; i++ )
    {
        CString strName;
        strName.Format( "picture%d", i );

        AddField( &pDoc->m_strPictures[i], strName, FIELD_SCRIPTREAD|FIELD_SCRIPTWRITE|FIELD_PICTURE );
    }


    //call the base class
    CScriptSheetView::AddAllFields();
}

void CHandoutView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
    if( bActivate == TRUE )
    {
        CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );
    }
	
	CScriptSheetView::OnActivateView(bActivate, (CScrollView*)pActivateView, (CScrollView*)pDeactiveView);
}

void CHandoutView::OnSetFocus(CWnd* pOldWnd) 
{
	CScriptSheetView::OnSetFocus(pOldWnd);
    CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), "" );
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );
}
