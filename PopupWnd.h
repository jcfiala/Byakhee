#if !defined(AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PopupWnd.h : header file
//

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CPopupWnd window

class CPopupWnd : public CWnd
{
// Construction
public:
	CPopupWnd();
	virtual ~CPopupWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void Create( CWnd* pWndParent );
	virtual void MoveIntoPosition() = 0;

	// Generated message map functions
protected:
    static CString m_strClassName;
	int m_nCharHeight;
    int m_nMaxCharWidth;
    int m_nWidth, m_nHeight;
	void CalcFontSizes();


	//{{AFX_MSG(CPopupWnd)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
