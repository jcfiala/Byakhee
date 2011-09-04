#if !defined(AFX_NEWDOCUMENTDLG_H__E27D2541_75D2_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_NEWDOCUMENTDLG_H__E27D2541_75D2_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewDocumentDlg.h : header file
//

#include <afxtempl.h>
#include "Skill.h"

/////////////////////////////////////////////////////////////////////////////
// CNewDocumentDlg dialog
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

#define NEWDOC_SCRIPT_RUN               0
#define NEWDOC_SCRIPT_NEWSCRIPT         1
#define NEWDOC_SCRIPT_NEWSHEETSCRIPT    2
#define NEWDOC_SCRIPT_NEWHANDOUTSCRIPT  3

class CNewDocumentDlg : public CDialog
{
// Construction
public:
	CNewDocumentDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CNewDocumentDlg();

    

// Dialog Data
	//{{AFX_DATA(CNewDocumentDlg)
	enum { IDD = IDD_NEWDOCUMENT };
	CButton	m_wndFullWizard;
	CTabCtrl	m_wndNewDocumentTypes;
	CListCtrl	m_wndAvailableTypes;
	BOOL	m_fFullWizard;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDocumentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL CreateNewHandout( CString strHandoutName );
	void BuildRuleFileList();
    void AddAllHandoutFiles();
	static int m_iPage;
	CString m_strInformation;

	// Generated message map functions
	//{{AFX_MSG(CNewDocumentDlg)
	afx_msg void OnDblclkAvailableTypes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeNewDocumentTypes(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
    afx_msg void OnViewLargeIcons();
    afx_msg void OnViewSmallIcons();
    afx_msg void OnViewList();
	afx_msg void OnClickAvailabletypes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedAvailabletypes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFullWizard();
	afx_msg void OnViewInformation();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
    afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnToolbarDropDown( NMHDR* lpnmt, LRESULT* pResult );
    afx_msg void OnFileMRU( UINT uID );
	DECLARE_MESSAGE_MAP()


private:
	void UpdateDescription();
    CDocTemplate* GetDocTemplate(CString strDocName);
	void InitialisePage( int iPage );
    CImageList m_ImageList;
    CImageList m_ImageListSmall;

    struct NewInvestigatorInfo {
        CString strRuleFile;
        CString strName;
        Era era;
    };
    CArray<NewInvestigatorInfo*, NewInvestigatorInfo*> m_NewInvestigatorInfoArray;

    enum WhatToCreate { CREATE_NONE, CREATE_INVESTIGATOR, WIZARD_INVESTIGATOR };
    WhatToCreate m_WhatToCreate;

	BOOL CreateNewScriptDocument( CString strTemplateName, const char* pszDefaultText, CString strTitle );
	BOOL CreateNewInvestigator( NewInvestigatorInfo* pNewInvestigatorInfo, BOOL fUseWizard );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDOCUMENTDLG_H__E27D2541_75D2_11D2_9FF9_00001C192944__INCLUDED_)
