#if !defined(AFX_SCRIPTSHEETVIEW_H__1C2033E3_7BE7_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SCRIPTSHEETVIEW_H__1C2033E3_7BE7_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScriptSheetView.h : header file
//

#include <afxtempl.h>
#include "ScriptEngine/Variant.h"
#include "ScriptEngine/ScriptObject.h"
#include "FieldEdit.h"
#include "Field.h"
#include "ZoomView.h"

#define SHEETPARAM_SHADOWOFFSET 1

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView view
/**
 * Copyright (C) 1998  David Harvey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

class CScriptSheetView : public CZoomView, public CScriptObject, public CFieldManager
{
protected:
	CScriptSheetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScriptSheetView)

// Attributes
public:
	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );

    virtual Variant GetFieldValue( CField* pField, CFieldSheetCommand* pFieldSheetCommand /*NULL*/ );
    virtual void SetFieldValue( CField* pField, Variant value, CFieldSheetCommand* pFieldSheetCommand /*NULL*/ );
    virtual void RedrawFields( CFieldSheetCommand* pFieldChanged = NULL );
    virtual CWnd* GetFieldManagerWindow();
    virtual void FieldToWindow( CRect& rc );

// Operations
public:
	void EnsureRectVisible( CRect rc );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptSheetView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnActivateView(BOOL bActivate, CScrollView* pActivateView, CScrollView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iLastEditedFieldSheetCommand;
	int GetSheetParameter( int nCode, int nDefaultValue );
	virtual void InitialiseSheetCommands();
	CFieldEdit m_wndFieldEdit;
	inline int ScaleToSheet( double dValue );
	virtual void ExecuteSheetCommand( CSheetCommand* pSheetCommand, CSheetDrawInfo* pDrawInfo );
	void ClearSheetCommands();
	void ResetFieldList();

    CBitmap* m_pbmSheet;

	void AddSheetParameter( CString strName, int nCode );
    struct SheetParameter {
        CString strName;
        Variant Code;
    };
    CArray<SheetParameter*, SheetParameter*> m_SheetParameters;

    virtual void AddAllFields() {};
    CFieldScriptObject m_Fields;
    CField* FindField(int nFieldCode);
	BOOL AddField( CString* pstrValue, CString strName, DWORD dwFlags = FIELD_NORMAL, const char* pszOptionName = NULL );
	BOOL AddField( int* pnValue, CString strName, DWORD dwFlags = FIELD_NORMAL );
	BOOL AddField( int nFieldCode, CString strName, DWORD dwFlags = FIELD_NORMAL, const char* pszOptionName = NULL );

    CFieldSheetCommand* HotFieldFromPoint(CPoint pt, int* pnIndex = NULL );

    void AddDummySheetCommands( const char* pszMessage = NULL );

	BOOL m_fLeftButtonClick;

	void SetSheetFileExtension( CString strFileExtension );

    void WindowToSheet( CPoint& point );
    void SheetToWindow( CPoint& point );
    void WindowToSheet( CRect& rect );
    void SheetToWindow( CRect& rect );

	//CSize m_SheetSize;
    CString m_strSheet;

    virtual void SheetChanged() {};

    virtual ~CScriptSheetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
    afx_msg void OnSheetStyleChange();
	//{{AFX_MSG(CScriptSheetView)
	afx_msg void OnViewRefreshSheet();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditNextField();
	afx_msg void OnEditPrevField();
	//}}AFX_MSG
    afx_msg long OnAppSettingsChange(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

private:
    CSheetCommand* AddHotArea(int nFieldCode, CRect rcField);
    CSheetCommand* AddTableHotArea( int nFieldCode, CRect rcField, Variant value );
    CString FindFont( const char * pszFontFaceName );

    int m_nFieldID;
    CString GetFirstSheetFile();

    BOOL SetSheet(CString strSheetFileName  = "" );
    BOOL ReloadSheet();

    void AddAllSheetTypes();

	CString m_strSheetFileExtension;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTSHEETVIEW_H__1C2033E3_7BE7_11D2_9FF9_00001C192944__INCLUDED_)
