// HandoutView.h : interface of the CHandoutView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDOUTVIEW_H__50F06AD1_F7F0_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_HANDOUTVIEW_H__50F06AD1_F7F0_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptSheetView.h"
#include "FieldEdit.h"
#include "HandoutDoc.h"

class CHandoutView : public CScriptSheetView
{
protected: // create from serialization only
	CHandoutView();
	DECLARE_DYNCREATE(CHandoutView)

// Attributes
public:
	CHandoutDoc* GetDocument();
    virtual void StoreUndo();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandoutView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHandoutView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	virtual void AddAllFields();

	//{{AFX_MSG(CHandoutView)
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Dummy MFC AppView.cpp
inline CHandoutDoc* CHandoutView::GetDocument()
   { return (CHandoutDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDOUTVIEW_H__50F06AD1_F7F0_11D1_89C0_00001C192944__INCLUDED_)
