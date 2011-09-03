#if !defined(AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragSwapEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragSwapEdit window

class CDragSwapEdit : public CEdit
{
// Construction
public:
	CDragSwapEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragSwapEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSwapRect( CRect rc );
	virtual ~CDragSwapEdit();

	// Generated message map functions
protected:
	CEdit* GetEditControlAtPoint( CPoint point );
	HCURSOR m_hCurDragDrop;
	BOOL m_fMouseDown;
	BOOL m_fDragging;
    CRect m_rcSwapVisual;
	//{{AFX_MSG(CDragSwapEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGSWAPEDIT_H__923EDF20_A664_11D2_A50C_00105A4ACC2C__INCLUDED_)
