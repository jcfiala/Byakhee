// ScriptEditDoc.cpp : implementation of the CScriptEditDoc class
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

#include "stdafx.h"
#include "Byakhee.h"
#include "ScriptEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEditDoc

IMPLEMENT_DYNCREATE(CScriptEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CScriptEditDoc, CDocument)
	//{{AFX_MSG_MAP(CScriptEditDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEditDoc construction/destruction

CScriptEditDoc::CScriptEditDoc()
{
}

CScriptEditDoc::~CScriptEditDoc()
{
}

BOOL CScriptEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CScriptEditDoc serialization

void CScriptEditDoc::Serialize(CArchive& ar)
{
	CEditView* pView = (CEditView*)m_viewList.GetHead();
	ASSERT_KINDOF(CEditView, pView);
	pView->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CScriptEditDoc diagnostics

#ifdef _DEBUG
void CScriptEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScriptEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
