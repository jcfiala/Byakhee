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
#if !defined(AFX_ADDSKILLDLG_H__0D286B45_84B7_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_ADDSKILLDLG_H__0D286B45_84B7_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AddSkillDlg.h : header file
//

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CAddSkillDlg dialog

class CAddSkillDlg : public CDialog
{
// Construction
public:
	void SetInvestigator( CInvestigator* pInvestigator );
	CAddSkillDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddSkillDlg)
	enum { IDD = IDD_ADDSKILL };
	CButton	m_wndListWithMainSkills;
	CButton	m_wndAdvanced;
	CButton	m_wndSkillType2;
	CButton	m_wndSkillType1;
	CEdit	m_wndNumberOfSubskills;
	CButton	m_wndInsertSpecialText;
	CStatic	m_wndDivider;
	CSpinButtonCtrl	m_wndNumberOfSubskillsSpin;
	CString	m_strBase;
	int		m_nNumberOfSubSkills;
	CString	m_strSkillName;
	int		m_nSkillType;
	BOOL	m_fListWithMainSkills;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSkillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetPrefix( CString strPrefix );
	void AddMultiplier( CString strMultiplier );
	void ShowHideAdvanced();

	// Generated message map functions
	//{{AFX_MSG(CAddSkillDlg)
	afx_msg void OnAdvanced();
	afx_msg void OnInsertSpecialText();
	afx_msg void OnSkilltype1();
	afx_msg void OnSkilltype2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
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
    afx_msg void OnBaseStringNoModify();
    afx_msg void OnBaseStringCoreFirearm();
    afx_msg void OnBaseStringNothingSpecial();
    DECLARE_MESSAGE_MAP()

protected:
    CInvestigator* m_pInvestigator;
private:
	int m_nExpandedHeight;
};

#endif // !defined(AFX_ADDSKILLDLG_H__0D286B45_84B7_11D2_9FF9_00001C192944__INCLUDED_)
