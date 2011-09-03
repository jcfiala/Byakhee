#if !defined(AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SkillInformationPopupWnd.h : header file
//

#include "Skill.h"
#include "PopupWnd.h"
#include "SkillManager.h"

/////////////////////////////////////////////////////////////////////////////
// CSkillInformationPopupWnd window

class CSkillInformationPopupWnd : public CPopupWnd
{
// Construction
public:
	CSkillInformationPopupWnd( CString strRuleFile );
	virtual ~CSkillInformationPopupWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillInformationPopupWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void MoveIntoPosition();
	void SetSkill( CString strSkill, CInvestigator* pInvestigator  = NULL );

	// Generated message map functions
protected:
	HICON m_hIcon;
	BOOL m_f5_5Mode;
	CString m_strRuleFile;
	CString GetEraDescription( ERA_MASK EraMask );
	CSkill* m_pSkill;
	//{{AFX_MSG(CSkillInformationPopupWnd)
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLINFORMATIONPOPUPWND_H__AC62A902_735F_11D2_9FF9_00001C192944__INCLUDED_)
