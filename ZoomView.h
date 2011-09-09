#pragma once



// CZoomView view

class CZoomView : public CScrollView
{
	DECLARE_DYNCREATE(CZoomView)

protected:
	CZoomView();           // protected constructor used by dynamic creation
	virtual ~CZoomView();

// Attributes
public:
	CDC*		m_pdcMemory;
	CBitmap*	m_pBitmap;
	BOOL		m_bInitialSize;

protected:
	// Zoom Attributes
	float		m_zoomFactor;
	float		m_minZoomFactor;
	float		m_maxZoomFactor;
	CSize		m_line;
	CSize		m_page;
	CSize		m_layout;
	CSize		m_displaySize;

// Operations
public:
	// Display Operation //////////////////////////////////
	int		SetMapMode(CDC* pDC);
	void	SetDisplaySize(void);
	void	SetZoomScale(float fScale);
	void	SetZoomArea(CRect zoomRect);
	CPoint	GetCenterScrollPosition(void);
	CSize	GetDisplaySize() {return m_displaySize;};
	void LPtoDP(LPPOINT lpPoint);
	void LPtoDP(LPRECT lpRect);
	void LPtoDP(LPSIZE lpSize);
	void DPtoLP(LPPOINT lpPoint);
	void DPtoLP(LPRECT lpRect);
	void DPtoLP(LPSIZE lpSize);

	///////////////////////////////////////////////////////

	int FloatToInt(float fNumber);

protected:
	void SetZoomFactor(float fZoomFactor);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


