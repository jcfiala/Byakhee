// Byakhee.h : main header file for the BYAKHEE application
//

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

#if !defined(AFX_BYAKHEE_H__A9FB21C4_4B45_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_BYAKHEE_H__A9FB21C4_4B45_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CByakheeApp:
// See Byakhee.cpp for the implementation of this class
//

#define MY_APPSETTINGSCHANGE WM_USER+100
#define MAX_UNDO_SIZE 5

class CByakheeApp : public CWinApp
{
    friend class CNewDocumentDlg;

public:
	static void ShowScriptExecutionQuickInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static BOOL FileExists( CString strFile );
	static BOOL BrowseForPicture( CString& strFilename, CWnd* pWndOwner = NULL );
	CByakheeApp();
    static CString GetDefaultDirectory();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CByakheeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CByakheeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsOptions();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BYAKHEE_H__A9FB21C4_4B45_11D2_9FF9_00001C192944__INCLUDED_)
