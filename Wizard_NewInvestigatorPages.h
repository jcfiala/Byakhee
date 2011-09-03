// Wizard_NewInvestigatorPages.h : header file
//

#ifndef __WIZARD5F_NEWINVESTIGATORPAGES_H__
#define __WIZARD5F_NEWINVESTIGATORPAGES_H__

#include "Investigator.h"
#include "SkillEditListBox.h"
#include "RandomNameGenerator.h"
#include "SkillInformationPopupWnd.h"
#include "DragSwapEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage1 dialog

class CWizard_NewInvestigatorPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage1)

// Construction
public:
	CWizard_NewInvestigatorPage1();
	~CWizard_NewInvestigatorPage1();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage1)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE1 };
	CStatic	m_wndFrame2;
	CStatic	m_wndFrame1;
	CButton	m_wndReroll;
	CButton	m_wndExtraSchooling;
	CButton	m_wndCanEdit;
	CEdit	m_wndAge;
	CDragSwapEdit	m_wndSTR;
	CDragSwapEdit	m_wndSIZ;
	CDragSwapEdit	m_wndPOW;
	CDragSwapEdit	m_wndINT;
	CDragSwapEdit	m_wndEDU;
	CDragSwapEdit	m_wndDEX;
	CDragSwapEdit	m_wndAPP;
	CDragSwapEdit	m_wndCON;
	CSpinButtonCtrl	m_wndAgeSpin;
	int		m_nAGE;
	int		m_nAPP;
	BOOL	m_fCanEdit;
	int		m_nCON;
	CString	m_strDB;
	int		m_nDEX;
	int		m_nEDU;
	BOOL	m_fExtraSchooling;
	CString	m_strHP;
	CString	m_strIdea;
	int		m_nINT;
	CString	m_strInterestPoints;
	CString	m_strKnow;
	CString	m_strLuck;
	int		m_nPOW;
	CString	m_strSAN;
	int		m_nSIZ;
	CString	m_strSkillPoints;
	int		m_nSTR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage1)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateInvestigatorStats();
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage1)
	afx_msg void OnDeltaposAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusAge();
	afx_msg void OnChangeApp();
	afx_msg void OnChangeCon();
	afx_msg void OnChangeDex();
	afx_msg void OnChangeEdu();
	afx_msg void OnChangeInt();
	afx_msg void OnChangePow();
	afx_msg void OnChangeSiz();
	afx_msg void OnChangeStr();
	afx_msg void OnReroll();
	afx_msg void OnCanEditToggle();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_fChooseSkills;
	BOOL m_fDoneCoreGeneration;
    BOOL m_fShownTipBefore;
	int m_nBaseEDU;

    HICON m_hIcon;
};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage2 dialog

class CWizard_NewInvestigatorPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage2)

// Construction
public:
	CWizard_NewInvestigatorPage2();
	~CWizard_NewInvestigatorPage2();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage2)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE2 };
	CComboBox	m_wndOccupation;
	CButton	m_wndUseCharacteristicModifiers;
	BOOL	m_fUseCharacteristicModifiers;
	int		m_nOccupation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage2)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnUseCharacteristicModifiers();
	afx_msg void OnSelChangeOccupation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage3 dialog

class CWizard_NewInvestigatorPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage3)

// Construction
public:
	CWizard_NewInvestigatorPage3();
	~CWizard_NewInvestigatorPage3();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage3)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE3 };
	CButton	m_wndInformation;
	CListBox	m_wndInvestigatorWeapons;
	CListBox	m_wndAllWeapons;
	BOOL	m_fInformation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage3)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage3)
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkAllWeapons();
	afx_msg void OnDblclkInvestigatorWeapons();
	afx_msg void OnInformation();
	afx_msg void OnSelchangeAllweapons();
	afx_msg void OnSetfocusAllweapons();
	afx_msg void OnSelchangeInvestigatorweapons();
	afx_msg void OnSetfocusInvestigatorweapons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
    CSkillInformationPopupWnd* m_pwndInformationPopup;
};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage4 dialog

class CWizard_NewInvestigatorPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage4)

// Construction
public:
	CWizard_NewInvestigatorPage4();
	~CWizard_NewInvestigatorPage4();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage4)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE4 };
	CButton	m_wndUndo;
	CStatic	m_wndAvailablePoints;
	CSkillEditListBox	m_wndSkillList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage4)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage4)
	virtual BOOL OnInitDialog();
	afx_msg void OnUndo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage5 dialog

class CWizard_NewInvestigatorPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage5)

// Construction
public:
	CWizard_NewInvestigatorPage5();
	~CWizard_NewInvestigatorPage5();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage5)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE5 };
	CButton	m_wndUndo;
	CSkillEditListBox	m_wndSkillList;
	CStatic	m_wndAvailablePoints;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage5)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage5)
	afx_msg void OnUndo();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage6 dialog

class CWizard_NewInvestigatorPage6 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage6)

// Construction
public:
	CWizard_NewInvestigatorPage6();
	~CWizard_NewInvestigatorPage6();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage6)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE6 };
	CComboBox	m_wndCollegesDegrees;
	CButton	m_wndRandom;
	CComboBox	m_wndMentalDisorders;
	CComboBox	m_wndBirthplace;
	CString	m_strMentalDisorders;
	CString	m_strBirthplace;
	CString	m_strName;
	CString	m_strOccupation;
	CString	m_strPortrait;
	CString	m_strSex;
	CString	m_strCollegesDegrees;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage6)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage6)
	afx_msg void OnRandomName();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CRandomNameGenerator* m_pRandomNameGenerator;
	HICON m_hIcon;
};


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage7 dialog

class CWizard_NewInvestigatorPage7 : public CPropertyPage
{
	DECLARE_DYNCREATE(CWizard_NewInvestigatorPage7)

// Construction
public:
	CWizard_NewInvestigatorPage7();
	~CWizard_NewInvestigatorPage7();
    CInvestigator* m_pInvestigator;

// Dialog Data
	//{{AFX_DATA(CWizard_NewInvestigatorPage7)
	enum { IDD = IDD_WIZARD_NEWINVESTIGATOR_PAGE7 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizard_NewInvestigatorPage7)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizard_NewInvestigatorPage7)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};




#endif // __WIZARD5F_NEWINVESTIGATORPAGES_H__
