#if !defined(AFX_FIELDEDIT_H__018A0803_83DD_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_FIELDEDIT_H__018A0803_83DD_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FieldEdit.h : header file
//

#include "NamedOptionManager.h"
#include "Skill.h"
#include "SheetCommand.h"
#include "SheetDrawInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CFieldEditPopup window

class CFieldEditPopup : public CWnd
{
// Construction
public:
	CFieldEditPopup();

    void SetFieldEditBox( class CFieldEdit* pEdit ) { m_pEdit = pEdit; }


// Attributes
public:
    static CString m_strClassName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldEditPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFieldEditPopup();

	// Generated message map functions
protected:
    class CFieldEdit* m_pEdit;
	//{{AFX_MSG(CFieldEditPopup)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFieldListBox window

class CFieldListBox : public CListBox 
{
public:
    CFieldListBox() { m_pEdit = NULL; }
	//{{AFX_VIRTUAL(CFieldListBox)
	//}}AFX_VIRTUAL

    void SetFieldEditBox( class CFieldEdit* pEdit ) { m_pEdit = pEdit; }

protected:
	//{{AFX_MSG(CFieldListBox)
	//}}AFX_MSG
    afx_msg void OnSelChange();
    afx_msg void OnDblClk();
    afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );

    class CFieldEdit* m_pEdit;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFieldEdit window

class CFieldEdit : public CEdit
{
    friend class CFieldListBox;

// Construction
public:
	CFieldEdit();
    CFieldManager* m_pOwner;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void BeginEditing( CFieldSheetCommand* pFieldSheetCommand );
	void EndEditing( BOOL fSaveChanges = TRUE );
	BOOL IsEditing();

	void SetOwner( CFieldManager* pOwner );
	void SetNamedOptionManager( CNamedOptionManager* pNamedOptionManager );

	virtual ~CFieldEdit();
    CFieldListBox* m_pwndListBox; 

	// Generated message map functions
protected:
	COLORREF m_crText;
    DWORD m_dwAdditionalStyle;
	void CreateControl();
	void BeginEditing(CRect rc);
	//{{AFX_MSG(CFieldEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEditUndo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	//}}AFX_MSG

    CFieldEditPopup* m_pwndPopup;
    CNamedOptionManager* m_pNamedOptionManager;
	CFont* m_pFont;

    CFieldSheetCommand* m_pFieldSheetCommand;

    BOOL m_fUserTextItalic;
    int m_nUserTextWeight;
	COLORREF m_crUserText;
	CString m_strUserTextFace;

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDEDIT_H__018A0803_83DD_11D2_9FF9_00001C192944__INCLUDED_)
