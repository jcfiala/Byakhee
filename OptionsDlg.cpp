// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg
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

IMPLEMENT_DYNAMIC(COptionsDlg, CPropertySheet)

COptionsDlg::COptionsDlg(CWnd* pWndParent)
	 : CPropertySheet(IDS_CAPTION_OPTIONSDLG, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	//AddPage(&m_Page3);
	//AddPage(&m_Page4);
	//AddPage(&m_Page5);
	//AddPage(&m_Page6);

    m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

COptionsDlg::~COptionsDlg()
{
}


BEGIN_MESSAGE_MAP(COptionsDlg, CPropertySheet)
	//{{AFX_MSG_MAP(COptionsDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers



BOOL COptionsDlg::ShowRulePicker()
{
	//remove all pages
	int nMax = GetPageCount();
	for( int i = 0; i < nMax; i++ ) RemovePage(0);

	//add rule pick page
	AddPage(&m_Page1);

	return( DoModal() == IDOK );
}
