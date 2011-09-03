#if !defined(AFX_HYPERLINK_H__53F9DB63_A2F0_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_HYPERLINK_H__53F9DB63_A2F0_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Hyperlink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHyperlink window

class CHyperlink : public CStatic
{
	DECLARE_DYNAMIC(CHyperlink)
   
// Construction
public:
	CHyperlink();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperlink)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetVisited( BOOL fVisited );
	CString GetURL();
	void SetURL( CString strURL );
    virtual void SetFont( CFont* pFont );
	virtual ~CHyperlink();

	// Generated message map functions
protected:
	BOOL m_fVisited;
	BOOL m_fGotFocus;
	CFont m_Font;
	BOOL m_fCaptured;
	BOOL m_fMouseOver;
	void GotoURL( CString strURL );
	CString m_strURL;
	//{{AFX_MSG(CHyperlink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    LONG GetRegKey( HKEY hKey, LPCTSTR szSubkey, LPTSTR szResult );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINK_H__53F9DB63_A2F0_11D2_A50C_00105A4ACC2C__INCLUDED_)
