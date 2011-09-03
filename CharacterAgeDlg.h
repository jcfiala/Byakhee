#if !defined(AFX_CHARACTERAGEDLG_H__2DBEC6A2_647E_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_CHARACTERAGEDLG_H__2DBEC6A2_647E_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CharacterAgeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterAgeDlg dialog

class CCharacterAgeDlg : public CDialog
{
// Construction
public:
	CCharacterAgeDlg(CWnd* pParent = NULL);   // standard constructor

    enum WhatToRot { ROT_APP = 0, ROT_DEX = 1, ROT_CON = 2, ROT_STR = 3 };

// Dialog Data
	//{{AFX_DATA(CCharacterAgeDlg)
	enum { IDD = IDD_CHARACTERAGE };
	int		m_nWhichStatistic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterAgeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCharacterAgeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERAGEDLG_H__2DBEC6A2_647E_11D2_9FF9_00001C192944__INCLUDED_)
