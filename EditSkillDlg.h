#if !defined(AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditSkillDlg.h : header file
//

#include "Investigator.h"

/////////////////////////////////////////////////////////////////////////////
// CEditSkillDlg dialog

class CEditSkillDlg : public CDialog
{
// Construction
public:
	void SetSkill( CInvestigatorSkill* pInvestigatorSkill );
	void SetInvestigator( CInvestigator* pInvestigator );
	CEditSkillDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditSkillDlg)
	enum { IDD = IDD_EDITSKILL };
	CString	m_strBase;
	CString	m_strSkillName;
	BOOL	m_fListWithMainSkills;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSkillDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CInvestigatorSkill* m_pInvestigatorSkill;
	CInvestigator* m_pInvestigator;

	// Generated message map functions
	//{{AFX_MSG(CEditSkillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSKILLDLG_H__1EFA32C6_8707_11D2_9FF9_00001C192944__INCLUDED_)
