// ScriptEditView.h : interface of the CScriptEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTEDITVIEW_H__3A33A332_B695_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_SCRIPTEDITVIEW_H__3A33A332_B695_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CScriptEditView : public CEditView
{
protected: // create from serialization only
	CScriptEditView();
	DECLARE_DYNCREATE(CScriptEditView)

// Attributes
public:
	CScriptEditDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEditView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateFont();
	virtual ~CScriptEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void UpdateLineNumber();
	//{{AFX_MSG(CScriptEditView)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
    afx_msg void OnAppSettingsChange();
	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
};

#ifndef _DEBUG  // debug version in ScriptEditView.cpp
inline CScriptEditDoc* CScriptEditView::GetDocument()
   { return (CScriptEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTEDITVIEW_H__3A33A332_B695_11D1_89C0_00001C192944__INCLUDED_)
