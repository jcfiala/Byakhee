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
