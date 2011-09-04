#if !defined(AFX_INVESTIGATORPOPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_INVESTIGATORPOPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InvestigatorPopupWnd.h : header file
//

#include "Investigator.h"
#include "PopupWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorPopupWnd window
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

class CInvestigatorPopupWnd : public CPopupWnd
{
// Construction
public:
	CInvestigatorPopupWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestigatorPopupWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetInvestigator( CInvestigator* pInvestigator );
	virtual ~CInvestigatorPopupWnd();
	virtual void MoveIntoPosition();

	// Generated message map functions
protected:
    HICON m_hIcon;

	//{{AFX_MSG(CInvestigatorPopupWnd)
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CInvestigator* m_pInvestigator;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVESTIGATORPOPUPWND_H__A8F72D82_6EAB_11D2_9FF9_00001C192944__INCLUDED_)
