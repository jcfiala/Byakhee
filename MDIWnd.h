// MDIWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIWnd window

class CMDIWnd : public CWnd
{
// Construction
public:
    CMDIWnd();
    virtual ~CMDIWnd();

// Attributes
public:
	FILE* m_pStream;
    BOOL m_fTipsLoadedOK;
    CSize m_sizeBitmap;
	CString	m_strTip;
	void HideTipWindow();
	void ShowNextTip();
	void ShowTip();
    int m_iFade;

    void GetNextTipString(CString& strNext);
	void DrawTipWindow( HDC hDC, int x, int y );
    HBITMAP m_hBmBackground;

    BOOL m_fStartup;
    BOOL m_fTipVisible;
    
// Operations
public:

// Implementation
public:
    // Generated message map functions
protected:
	CRect GetTipRect();

    //{{AFX_MSG(CMDIWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	void DrawTipData( HDC hDC, CRect rc  );
};

/////////////////////////////////////////////////////////////////////////////
