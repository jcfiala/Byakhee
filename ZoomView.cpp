// ZoomView.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "ZoomView.h"

// CZoomView

IMPLEMENT_DYNCREATE(CZoomView, CScrollView)

CZoomView::CZoomView() : CScrollView()
{
	m_pdcMemory = new CDC;
	m_pBitmap   = new CBitmap;
	m_bInitialSize = FALSE;

	m_zoomFactor = 1.0; //0.01f;
	m_minZoomFactor = 0.2f; //0.002f;
	m_maxZoomFactor = 10.0f; //1.0f;
}

CZoomView::~CZoomView()
{
	if (m_pdcMemory != NULL)
	{
		delete m_pdcMemory;
		m_pdcMemory = NULL;
	}
	
	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}


BEGIN_MESSAGE_MAP(CZoomView, CScrollView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CZoomView drawing

void CZoomView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//m_layout.cx = 100;
	//m_layout.cy = 100;

	m_page.cx = m_layout.cx/2;
	m_page.cy = m_layout.cy/2;
	m_line.cx = m_layout.cx/50;
	m_line.cy = m_layout.cy/50;
	SetScrollSizes(MM_TEXT, m_layout, m_page, m_line);

	m_bInitialSize = TRUE;

	if (m_pdcMemory->GetSafeHdc() == NULL)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);	
		m_pdcMemory->CreateCompatibleDC(&dc);

		// makes bitmap same size as display window
		CRect clientRect(0,0,0,0);
		GetClientRect(clientRect);
		if (m_pBitmap != NULL) 
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CBitmap();
        m_pBitmap->CreateCompatibleBitmap(&dc, clientRect.right,
											   clientRect.bottom);
	}
}

// Probably going to let the ScriptSheetView handle drawing...
void CZoomView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CZoomView diagnostics

#ifdef _DEBUG
void CZoomView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CZoomView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CZoomView message handlers

void CZoomView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//////////////////////////////////////////////////////////////////////////
	// Before we can use OnPrepareDC, we must call SetScrollSize before. So,
	// this flag is used to check whether we already call SetScrollSize.
	if (m_bInitialSize == FALSE) 
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	OnPrepareDC(&dc);
	SetMapMode(&dc);

	//////////////////////////
	CPoint viewportOrg = dc.GetViewportOrg();
	m_pdcMemory->SetViewportOrg(viewportOrg);


	CBitmap* pOldBitMap = m_pdcMemory->SelectObject(m_pBitmap);
	CRect rectUpdate(0,0,0,0);
	dc.GetClipBox(&rectUpdate);
	rectUpdate.InflateRect(1,1,1,1);
	m_pdcMemory->SelectClipRgn(NULL);
	m_pdcMemory->IntersectClipRect(&rectUpdate);
	CBrush backgroundBrush((COLORREF) ::GetSysColor(COLOR_WINDOW));
	CBrush* pOldBrush = m_pdcMemory->SelectObject(&backgroundBrush);
	//m_pdcMemory->PatBlt(rectUpdate.left, rectUpdate.top,
	//	rectUpdate.Width(), rectUpdate.Height(), PATCOPY);
	
	/////////////////////////////////////////////
	// draw section
	OnDraw(m_pdcMemory);
	/////////////////////////////////////////////

	dc.BitBlt(rectUpdate.left, rectUpdate.top,
		rectUpdate.Width(), rectUpdate.Height(),
		m_pdcMemory, rectUpdate.left, rectUpdate.top, SRCCOPY);
	m_pdcMemory->SelectObject(pOldBitMap);
	m_pdcMemory->SelectObject(pOldBrush);
}

void CZoomView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	//////////////////////////////////////////////////////////////////////////
	// Before we can use OnPrepareDC, we must call SetScrollSize before. So,
	// this flag is used to check whether we already call SetScrollSize.
	if (m_bInitialSize == FALSE) 
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	CPaintDC dc(this); // device context for painting
	OnPrepareDC(&dc);
	SetMapMode(&dc);

	CRect clientRect(0,0,0,0);
	GetClientRect(&clientRect);

	if (m_pBitmap != NULL)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	m_pBitmap = new CBitmap;
	m_pBitmap->CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	TRACE("Bitmap's size changed: %d, %d\n", clientRect.Width(), clientRect.Height());
	Invalidate(FALSE);
}

BOOL CZoomView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

int CZoomView::SetMapMode(CDC* pDC)
{
	int previousMode = pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(100,100);
	pDC->SetViewportExt(FloatToInt(100 * m_zoomFactor), FloatToInt(100 * m_zoomFactor));

	return previousMode;
}

void CZoomView::SetDisplaySize(void) 
{
	m_displaySize.cx = FloatToInt(m_layout.cx * m_zoomFactor);
	m_displaySize.cy = FloatToInt(m_layout.cy * m_zoomFactor);
	CRect rectMax(CPoint(0,0), m_displaySize);
	SetScrollSizes(MM_TEXT, m_displaySize, m_page, m_line);
	SetMapMode(m_pdcMemory);
}

void CZoomView::SetZoomScale(float fScale)
{
	float oldZoomFactor = m_zoomFactor;
	CPoint centerScrollPosition = GetCenterScrollPosition();

	SetZoomFactor(fScale);
	SetDisplaySize();

	int newXScrollPosition = FloatToInt((centerScrollPosition.x / oldZoomFactor) * m_zoomFactor);
	int newYScrollPosition = FloatToInt((centerScrollPosition.y / oldZoomFactor) * m_zoomFactor);
	CRect clientRect(0,0,0,0);
	GetClientRect(&clientRect);
	newXScrollPosition -= clientRect.right/2;
	newYScrollPosition -= clientRect.bottom/2;
	ScrollToPosition(CPoint(newXScrollPosition, newYScrollPosition));

	Invalidate(FALSE);
}

void CZoomView::SetZoomArea(CRect zoomRect)
{
	CRect clientRect(0,0,0,0);
	GetClientRect(clientRect);

	CRect realRect(zoomRect);
	LPtoDP(&realRect);

	float widthRatio = float(clientRect.Width()) / float(realRect.Width());
	float heightRatio = float(clientRect.Height()) / float(realRect.Height());
	float ratio = (widthRatio < heightRatio) ? widthRatio : heightRatio;
	ratio = ratio * m_zoomFactor;

	CPoint centerScrollPosition = CPoint(0,0);
	centerScrollPosition.x = zoomRect.left + zoomRect.Width() / 2;
	centerScrollPosition.y = zoomRect.top + zoomRect.Height() / 2;

	SetZoomFactor(ratio);
	SetDisplaySize();

	int newXScrollPosition = FloatToInt(centerScrollPosition.x * m_zoomFactor);
	int newYScrollPosition = FloatToInt(centerScrollPosition.y * m_zoomFactor);

	GetClientRect(clientRect);
	newXScrollPosition -= clientRect.Width() / 2;
	newYScrollPosition -= clientRect.Height() / 2;
	ScrollToPosition(CPoint(newXScrollPosition, newYScrollPosition));

	Invalidate(FALSE);
}

CPoint CZoomView::GetCenterScrollPosition(void)
{
	CPoint ScrollPosition = GetScrollPosition();
	CRect ClientRect(0,0,0,0);
	GetClientRect(&ClientRect);
	ScrollPosition.x += ClientRect.right / 2;
	ScrollPosition.y += ClientRect.bottom / 2;
	return ScrollPosition;
}

void CZoomView::LPtoDP(LPPOINT lpPoint) 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.LPtoDP(lpPoint);
}

void CZoomView::LPtoDP(LPRECT lpRect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.LPtoDP(lpRect);
}

void CZoomView::LPtoDP(LPSIZE lpSize)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.LPtoDP(lpSize);
}


void CZoomView::DPtoLP(LPPOINT lpPoint)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.DPtoLP(lpPoint);
}

void CZoomView::DPtoLP(LPRECT lpRect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.DPtoLP(lpRect);
}

void CZoomView::DPtoLP(LPSIZE lpSize)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	SetMapMode(&dc);
	dc.DPtoLP(lpSize);
}

int CZoomView::FloatToInt(float fNumber)
{
	if (fNumber >= 0)
	{
		fNumber += 0.5f;
	}
	else
	{
		fNumber -= 0.5f;
	}
	return int(fNumber);
}

void CZoomView::SetZoomFactor(float fZoomFactor)
{
	if (fZoomFactor <= m_minZoomFactor)
		m_zoomFactor = m_minZoomFactor;
	else if (fZoomFactor >= m_maxZoomFactor)
		m_zoomFactor = m_maxZoomFactor;
	else
		m_zoomFactor = fZoomFactor;
}
