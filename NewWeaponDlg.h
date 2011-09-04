// NewWeaponDlg.h : header file
//

#ifndef __NEWWEAPONDLG_H__
#define __NEWWEAPONDLG_H__

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CNewWeaponDlgPage1 dialog
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

class CNewWeaponDlgPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewWeaponDlgPage1)

// Construction
public:
	BOOL AddWeapon();
	CNewWeaponDlgPage1();
	~CNewWeaponDlgPage1();

// Dialog Data
	//{{AFX_DATA(CNewWeaponDlgPage1)
	enum { IDD = IDD_PROPSHEET_NEWWEAPON_PAGE1 };
	CButton	m_wndInsertSpecialText;
	CString	m_strBase;
	CString	m_strDamage;
	CString	m_strBullets;
	CString	m_strHitPoints;
	CString	m_strMal;
	CString	m_strRange;
	CString	m_strShots;
	CString	m_strSkillName;
	//}}AFX_DATA

    void SetInvestigator( CInvestigator* pInvestigator ) { m_pInvestigator = pInvestigator; }

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewWeaponDlgPage1)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CInvestigator* m_pInvestigator;
	// Generated message map functions
	//{{AFX_MSG(CNewWeaponDlgPage1)
	afx_msg void OnInsertSpecialText();
	//}}AFX_MSG
    afx_msg void OnBaseStringAPP();
    afx_msg void OnBaseStringCON();
    afx_msg void OnBaseStringDEX();
    afx_msg void OnBaseStringEDU();
    afx_msg void OnBaseStringINT();
    afx_msg void OnBaseStringPOW();
    afx_msg void OnBaseStringSIZ();
    afx_msg void OnBaseStringSTR();
    afx_msg void OnBaseStringLuck();
    afx_msg void OnBaseStringKnow();
    afx_msg void OnBaseStringIdea();
    afx_msg void OnBaseStringSan();
    afx_msg void OnBaseStringHitPoints();
    afx_msg void OnBaseStringMagic();
    afx_msg void OnBaseStringAsSkill();
	DECLARE_MESSAGE_MAP()

    void AddMultiplier(CString strMultiplier);
};


/////////////////////////////////////////////////////////////////////////////
// CNewWeaponDlgPage2 dialog

class CNewWeaponDlgPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CNewWeaponDlgPage2)

// Construction
public:
	BOOL AddWeapon();
	CNewWeaponDlgPage2();
	~CNewWeaponDlgPage2();

// Dialog Data
	//{{AFX_DATA(CNewWeaponDlgPage2)
	enum { IDD = IDD_PROPSHEET_NEWWEAPON_PAGE2 };
	CString	m_strAttacks;
	CString	m_strBase;
	CString	m_strDamage;
	CString	m_strHitPoints;
	CString	m_strSkillName;
	//}}AFX_DATA

    void SetInvestigator( CInvestigator* pInvestigator ) { m_pInvestigator = pInvestigator; }

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNewWeaponDlgPage2)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CInvestigator* m_pInvestigator;
	// Generated message map functions
	//{{AFX_MSG(CNewWeaponDlgPage2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CNewWeaponDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CNewWeaponDlg)

// Construction
public:
	CNewWeaponDlg(CWnd* pWndParent = NULL);

// Attributes
public:
	CNewWeaponDlgPage1 m_Page1;
	CNewWeaponDlgPage2 m_Page2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewWeaponDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetInvestigator( CInvestigator* pInvestigator );
	virtual ~CNewWeaponDlg();

// Generated message map functions
protected:
	CInvestigator* m_pInvestigator;
	//{{AFX_MSG(CNewWeaponDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
};



#endif // __NEWWEAPONDLG_H__
