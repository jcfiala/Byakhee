// DragSwapEdit.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "DragSwapEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EDIT_DRAG_DELAY 200

/////////////////////////////////////////////////////////////////////////////
// CDragSwapEdit

CDragSwapEdit::CDragSwapEdit()
{
    m_fDragging = FALSE;
    m_fMouseDown = FALSE;
    m_hCurDragDrop = AfxGetApp()->LoadCursor(IDC_DRAGDROP);
}

CDragSwapEdit::~CDragSwapEdit()
{
    DeleteObject( m_hCurDragDrop );
}


BEGIN_MESSAGE_MAP(CDragSwapEdit, CEdit)
	//{{AFX_MSG_MAP(CDragSwapEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragSwapEdit message handlers

void CDragSwapEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
    m_fMouseDown = TRUE;
    SetCapture();
    SetTimer( 0xFFFF, EDIT_DRAG_DELAY, NULL );
	CEdit::OnLButtonDown(nFlags, point);
}

void CDragSwapEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_fMouseDown = FALSE;
    ReleaseCapture();

    if( m_fDragging )
    {
        //erase the box around the controls that we really should drop into
        CDC* pDC = GetParent()->GetDC();
        pDC->DrawFocusRect( &m_rcSwapVisual );
        GetParent()->ReleaseDC( pDC );

        //convert the point into parent coordinates
        ClientToScreen(&point);
        GetParent()->ScreenToClient(&point);

        //get the edit control it's pointing to
        CEdit* pEdit = GetEditControlAtPoint(point);
        if( pEdit && pEdit != this )
        {
            //swap strings over
            CString str1, str2;
            GetWindowText( str1 );
            pEdit->GetWindowText( str2 );
            SetWindowText( str2 );
            pEdit->SetWindowText( str1 );

            //set the focus to the new control
            pEdit->SetSel( 0, -1 );
            pEdit->SetFocus();
        }

        m_fDragging = FALSE;
    }

	CEdit::OnLButtonUp(nFlags, point);
}

void CDragSwapEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
    if( m_fDragging )
    {
        //convert the point into parent coordinates
        ClientToScreen(&point);
        GetParent()->ScreenToClient(&point);

        //get the edit control it's pointing to
        CEdit* pEdit = GetEditControlAtPoint(point);
        if( pEdit )
        {
            //change the cursor to a drag-drop cursor or a drag-swap cursor, depending on who we're pointing to
            ::SetCursor( m_hCurDragDrop );
            
            //move the cursor in the edit control we're pointing to
            GetParent()->ClientToScreen(&point);
            pEdit->ScreenToClient(&point);
            int nCaretPos = LOWORD(pEdit->CharFromPos(point));
            pEdit->SetSel(nCaretPos, nCaretPos);

            //give the focus to the control we're pointing to
            pEdit->SetFocus();
        }
        else //display no-drop cursor
            ::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_NO) );
    }
	
	CEdit::OnMouseMove(nFlags, point);
}

void CDragSwapEdit::OnTimer(UINT nIDEvent) 
{
    //kill this timer immediately
    KillTimer(nIDEvent);

    //if the mouse button is still down...
    if( m_fMouseDown )
    {
        //draw a box around the controls that we really should drop into
        CDC* pDC = GetParent()->GetDC();
        pDC->DrawFocusRect( &m_rcSwapVisual );
        GetParent()->ReleaseDC( pDC );

        //begin dragging
        m_fDragging = TRUE;
        ::SetCursor(m_hCurDragDrop);
        SetSel( 0, -1 );
    }
	
	CEdit::OnTimer(nIDEvent);
}

CEdit* CDragSwapEdit::GetEditControlAtPoint(CPoint point)
{
    //find the window that is being pointed to
    CWnd* pWnd = GetParent()->ChildWindowFromPoint(point, CWP_SKIPINVISIBLE );

    //see if it's an edit control
    if( pWnd )
    {
        /*
        note: if this function appears to be messing up and finding a window of type "BUTTON"
        or something, make sure that the tab index of a group box isn't messing things up. Group
        boxes have a class of type "BUTTON" - it's safer to use an etched frame static control.
        */
        if( pWnd == this ) //we know that we're an edit control
            return this;
        else
        {
            //check it's class name to see if it's a standard edit control
            char szClassName[6]; GetClassName( pWnd->GetSafeHwnd(), szClassName, 5 );
            if( stricmp(szClassName, "EDIT") == 0 ) return (CEdit*)pWnd;
        }
    }

    return NULL;
}

void CDragSwapEdit::SetSwapRect(CRect rc)
{
    m_rcSwapVisual = rc;
}
