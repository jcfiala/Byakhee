#if !defined(AFX_CHOOSESKILLDLG_H__9D77F0A4_6863_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_CHOOSESKILLDLG_H__9D77F0A4_6863_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChooseSkillDlg.h : header file
//

#include "Investigator.h"


/////////////////////////////////////////////////////////////////////////////
// CChooseSkillDlg dialog

class CChooseSkillDlg : public CDialog
{
// Construction
public:
	void SetAsOccupational( BOOL fSetAsOccupational = TRUE );
	CInvestigator* m_pInvestigator;
	CChooseSkillDlg(CWnd* pParent = NULL);   // standard constructor
    CInvestigatorSkill* m_pInvestigatorSkill;

// Dialog Data
	//{{AFX_DATA(CChooseSkillDlg)
	enum { IDD = IDD_CHOOSESKILL };
	CListBox	m_wndSkills;
	CString	m_strCustomPrompt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseSkillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
    BOOL m_fSetAsOccupational;

	// Generated message map functions
	//{{AFX_MSG(CChooseSkillDlg)
	afx_msg void OnDblclkSkills();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSESKILLDLG_H__9D77F0A4_6863_11D2_9FF9_00001C192944__INCLUDED_)
