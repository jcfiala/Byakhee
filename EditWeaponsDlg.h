#if !defined(AFX_EDITWEAPONSDLG_H__1EFA32C7_8707_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_EDITWEAPONSDLG_H__1EFA32C7_8707_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditWeaponsDlg.h : header file
//

#include "Investigator.h"
#include "SkillInformationPopupWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CEditWeaponsDlg dialog
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

class CEditWeaponsDlg : public CDialog
{
// Construction
public:
	CEditWeaponsDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CEditWeaponsDlg();

// Dialog Data
	//{{AFX_DATA(CEditWeaponsDlg)
	enum { IDD = IDD_EDITWEAPONS };
	CListBox	m_wndInvestigatorWeapons;
	CButton	m_wndInformation;
	CListBox	m_wndAllWeapons;
	BOOL	m_fInformation;
	//}}AFX_DATA

    void SetInvestigator( CInvestigator* pInvestigator ) { m_pInvestigator = pInvestigator; };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditWeaponsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddWeaponsToListBoxes();
    CInvestigator* m_pInvestigator;
    HICON m_hIcon;
    CSkillInformationPopupWnd* m_pwndInformationPopup;

	// Generated message map functions
	//{{AFX_MSG(CEditWeaponsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnSelchangeAllweapons();
	afx_msg void OnDblclkAllweapons();
	afx_msg void OnInformation();
	afx_msg void OnDblclkInvestigatorweapons();
	afx_msg void OnSelchangeInvestigatorWeapons();
	afx_msg void OnNew();
	afx_msg void OnRemove();
	afx_msg void OnSetfocusAllweapons();
	afx_msg void OnSetfocusInvestigatorweapons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWEAPONSDLG_H__1EFA32C7_8707_11D2_9FF9_00001C192944__INCLUDED_)
