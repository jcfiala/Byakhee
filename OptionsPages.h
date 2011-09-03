// OptionsPages.h : header file
//

#ifndef __OPTIONSPAGES_H__
#define __OPTIONSPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// COptionsPage1 dialog

class COptionsPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage1)

// Construction
public:
	COptionsPage1();
	~COptionsPage1();

// Dialog Data
	//{{AFX_DATA(COptionsPage1)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE1 };
	CSpinButtonCtrl	m_wndTopMarginSpin;
	CSpinButtonCtrl	m_wndLeftMarginSpin;
	double	m_dLeftMargin;
	double	m_dTopMargin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage1)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionsPage2 dialog

class COptionsPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage2)

// Construction
public:
	COptionsPage2();
	~COptionsPage2();

// Dialog Data
	//{{AFX_DATA(COptionsPage2)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE2 };
	CListBox	m_wndComponentList;
	BOOL	m_fFlatToolbar;
	BOOL	m_fAnimatedFieldEdit;
	BOOL	m_fTipOfTheDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage2)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ChangeColour( CString strSection, CString strEntry, COLORREF crDefault );
	void ChangeFont( CString strSection, CString strEntry, CString strDefaultFace, int nDefaultSize = -1 );
	// Generated message map functions
	//{{AFX_MSG(COptionsPage2)
	afx_msg void OnDblclkComponentList();
	afx_msg void OnChange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/*

/////////////////////////////////////////////////////////////////////////////
// COptionsPage3 dialog

class COptionsPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage3)

// Construction
public:
	COptionsPage3();
	~COptionsPage3();

// Dialog Data
	//{{AFX_DATA(COptionsPage3)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage3)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionsPage4 dialog

class COptionsPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage4)

// Construction
public:
	COptionsPage4();
	~COptionsPage4();

// Dialog Data
	//{{AFX_DATA(COptionsPage4)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE4 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage4)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionsPage5 dialog

class COptionsPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage5)

// Construction
public:
	COptionsPage5();
	~COptionsPage5();

// Dialog Data
	//{{AFX_DATA(COptionsPage5)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE5 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage5)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// COptionsPage6 dialog

class COptionsPage6 : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsPage6)

// Construction
public:
	COptionsPage6();
	~COptionsPage6();

// Dialog Data
	//{{AFX_DATA(COptionsPage6)
	enum { IDD = IDD_PROPSHEET_OPTIONS_PAGE6 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsPage6)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsPage6)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
*/


#endif // __OPTIONSPAGES_H__
