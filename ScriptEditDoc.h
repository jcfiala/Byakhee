// ScriptEditDoc.h : interface of the CScriptEditDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTEDITDOC_H__3A33A330_B695_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_SCRIPTEDITDOC_H__3A33A330_B695_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CScriptEditDoc : public CDocument
{
protected: // create from serialization only
	CScriptEditDoc();
	DECLARE_DYNCREATE(CScriptEditDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScriptEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CScriptEditDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTEDITDOC_H__3A33A330_B695_11D1_89C0_00001C192944__INCLUDED_)
