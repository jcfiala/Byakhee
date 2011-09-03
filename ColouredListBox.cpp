// ColouredListBox.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "ColouredListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColouredListBox

CColouredListBox::CColouredListBox()
{
    m_hBrBackground = NULL;
}

CColouredListBox::~CColouredListBox()
{
    if( m_hBrBackground ) ::DeleteObject(m_hBrBackground);
}


BEGIN_MESSAGE_MAP(CColouredListBox, CListBox)
	//{{AFX_MSG_MAP(CColouredListBox)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColouredListBox message handlers

HBRUSH CColouredListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    return m_hBrBackground;
}

void CColouredListBox::SetColours(COLORREF crForeground, COLORREF crBackground)
{
    //store new colours
	m_crForeground = crForeground;
	m_crBackground = crBackground;

    //recreate background brush
    if( m_hBrBackground ) ::DeleteObject(m_hBrBackground);
    m_hBrBackground = ::CreateSolidBrush(m_crBackground);

    //redraw the window
    RedrawWindow();
}

void CColouredListBox::DrawItem(LPDRAWITEMSTRUCT lpdis) 
{
    //set colours
    COLORREF crBk, crTx;
    HBRUSH hBrBk;
    if( lpdis->itemState & ODS_SELECTED )
    {
        //use system highlight colour if it's selected
        crBk = ::GetSysColor(COLOR_HIGHLIGHT);
        crTx = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
        hBrBk = (HBRUSH)(COLOR_HIGHLIGHT+1);
    }
    else
    {
        //use current colour settings
        crBk = m_crBackground; 
        crTx = m_crForeground;
        hBrBk = m_hBrBackground;
    }

    //draw the item background
    ::FillRect( lpdis->hDC, &lpdis->rcItem, hBrBk );
    ::SetBkColor( lpdis->hDC, crBk );
    ::SetTextColor( lpdis->hDC, crTx );
    ::SetBkMode( lpdis->hDC, TRANSPARENT );

    //draw the text
    if( lpdis->itemID != -1 )
    {
        CString strText; GetText( lpdis->itemID, strText );
        ::TextOut( lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, strText, strText.GetLength() );
    }

    //draw the focus rectangle
    if( lpdis->itemState & ODS_FOCUS ) ::DrawFocusRect( lpdis->hDC, &lpdis->rcItem );
}

void CColouredListBox::SetFont( CFont* pFont, BOOL fRedraw/*TRUE*/ )
{
    //change font
    CListBox::SetFont( pFont, fRedraw );

    //set height on all items
    int iMax = GetCount();
    int nHeight = GetFontHeight();
    for( int i = 0; i < iMax; i++ ) SetItemHeight( i, nHeight );
}

void CColouredListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
    lpMeasureItemStruct->itemHeight = GetFontHeight();
}

int CColouredListBox::GetFontHeight()
{
    //get the text metrics of the current window font
    TEXTMETRIC tm;
    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject( GetFont() );
    pDC->GetTextMetrics( &tm );
    pDC->SelectObject( pOldFont );

    return tm.tmHeight;
}
