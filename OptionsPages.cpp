// OptionsPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#include "OptionsPages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COptionsPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(COptionsPage2, CPropertyPage)
//IMPLEMENT_DYNCREATE(COptionsPage3, CPropertyPage)
//IMPLEMENT_DYNCREATE(COptionsPage4, CPropertyPage)
//IMPLEMENT_DYNCREATE(COptionsPage5, CPropertyPage)
//IMPLEMENT_DYNCREATE(COptionsPage6, CPropertyPage)

static unsigned int g_crCustomColours[16] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                                 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };


/////////////////////////////////////////////////////////////////////////////
// COptionsPage1 property page

COptionsPage1::COptionsPage1() : CPropertyPage(COptionsPage1::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage1)
	m_dLeftMargin = 0.0;
	m_dTopMargin = 0.0;
	//}}AFX_DATA_INIT
}

COptionsPage1::~COptionsPage1()
{
}

void COptionsPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage1)
	DDX_Control(pDX, IDC_TOPMARGINSPIN, m_wndTopMarginSpin);
	DDX_Control(pDX, IDC_LEFTMARGINSPIN, m_wndLeftMarginSpin);
	DDX_Text(pDX, IDC_LEFTMARGIN, m_dLeftMargin);
	DDX_Text(pDX, IDC_TOPMARGIN, m_dTopMargin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage1, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionsPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    //set spinner ranges
    m_wndTopMarginSpin.SetRange( 0, 999 );
    m_wndLeftMarginSpin.SetRange( 0, 999 );

    //set margin settings
    m_dTopMargin = double(AfxGetApp()->GetProfileInt( "Sheet", "TopMargin", 185 )) / 10.0;
    m_dLeftMargin = double(AfxGetApp()->GetProfileInt( "Sheet", "LeftMargin", 0 )) / 10.0;

    //initialise the dialog
    UpdateData(UD_SETDATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsPage1::OnOK() 
{
    UpdateData(UD_GETDATA);

    //save margin settings
    AfxGetApp()->WriteProfileInt( "Sheet", "TopMargin",  int(m_dTopMargin * 10.0) );
    AfxGetApp()->WriteProfileInt( "Sheet", "LeftMargin", int(m_dLeftMargin * 10.0) );
	
	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// COptionsPage2 property page

COptionsPage2::COptionsPage2() : CPropertyPage(COptionsPage2::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage2)
	m_fFlatToolbar = FALSE;
	m_fAnimatedFieldEdit = FALSE;
	m_fTipOfTheDay = FALSE;
	//}}AFX_DATA_INIT
}

COptionsPage2::~COptionsPage2()
{
}

void COptionsPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage2)
	DDX_Control(pDX, IDC_COMPONENTLIST, m_wndComponentList);
	DDX_Check(pDX, IDC_FLATTOOLBAR, m_fFlatToolbar);
	DDX_Check(pDX, IDC_ANIMATEDFIELDEDIT, m_fAnimatedFieldEdit);
	DDX_Check(pDX, IDC_SHOWTIPS, m_fTipOfTheDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage2, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage2)
	ON_LBN_DBLCLK(IDC_COMPONENTLIST, OnDblclkComponentList)
	ON_BN_CLICKED(IDC_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionsPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //load settings
    m_fFlatToolbar = AfxGetApp()->GetProfileInt( "Display", "FlatToolbar", TRUE );
    m_fAnimatedFieldEdit = AfxGetApp()->GetProfileInt( "Sheet", "FieldAnimation", TRUE );
    m_fTipOfTheDay = !AfxGetApp()->GetProfileInt( "Tips", "StartUp", FALSE );	
	
    //NOTE: if the order of these change, the order must also be changed in OnChange()
	m_wndComponentList.AddString( "Output Window Font" );
    m_wndComponentList.AddString( "Output Window Background Colour" );
    m_wndComponentList.AddString( "Output Window Foreground Colour" );
    m_wndComponentList.AddString( "User Text Font" );
    m_wndComponentList.AddString( "User Text Colour" );
    m_wndComponentList.AddString( "Script Edit Window Font" );
    m_wndComponentList.SetCurSel(0);


    UpdateData(UD_SETDATA);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsPage2::OnChange() 
{
    switch( m_wndComponentList.GetCurSel() )
    {
        case 0: ChangeFont( "Output", "Font", "MS Sans Serif", -10 ); break;
        case 1: ChangeColour( "Output", "BkColour", GetSysColor(COLOR_WINDOW) ); break; 
        case 2: ChangeColour( "Output", "TxColour", GetSysColor(COLOR_WINDOWTEXT) ); break;
        case 3: ChangeFont( "Sheet", "UserText", "Comic Sans MS" ); break;
        case 4: ChangeColour( "Sheet", "UserTextcolour", RGB(0,0,128) ); break;
        case 5: ChangeFont( "Edit", "Font", "Courier", 15 ); break;
    }
}

void COptionsPage2::OnDblclkComponentList() 
{
    OnChange();	
}

void COptionsPage2::ChangeFont(CString strSection, CString strEntry, CString strDefaultFace, int nDefaultSize /*-1*/)
{
    //empty the logfont structure
    LOGFONT lf;
    ZeroMemory( &lf, sizeof(LOGFONT) );

    //read current setting
    lf.lfItalic = AfxGetApp()->GetProfileInt( strSection, strEntry + "Italic", 0 );
    lf.lfWeight = AfxGetApp()->GetProfileInt( strSection, strEntry + "Weight", 0 );
    strcpy( lf.lfFaceName, AfxGetApp()->GetProfileString( strSection, strEntry + "Face", strDefaultFace ) );
    if( nDefaultSize != -1 )
        lf.lfHeight = AfxGetApp()->GetProfileInt( strSection, strEntry + "Size", nDefaultSize );
    else
        lf.lfHeight = 13;

    //display font dialog
    CFontDialog FontDlg( &lf, CF_SCREENFONTS|CF_NOSCRIPTSEL|(nDefaultSize==-1?CF_LIMITSIZE:0) );
    if( nDefaultSize == -1 ) FontDlg.m_cf.nSizeMin = FontDlg.m_cf.nSizeMax = 10;
    if( FontDlg.DoModal() == IDOK )
    {
        AfxGetApp()->WriteProfileInt( strSection, strEntry + "Italic", lf.lfItalic );
        AfxGetApp()->WriteProfileInt( strSection, strEntry + "Weight", lf.lfWeight );
        AfxGetApp()->WriteProfileString( strSection, strEntry + "Face", lf.lfFaceName );
        if( nDefaultSize != -1 ) AfxGetApp()->WriteProfileInt( strSection, strEntry + "Size", lf.lfHeight );
    }
}

void COptionsPage2::ChangeColour(CString strSection, CString strEntry, COLORREF crDefault)
{
    //load current setting
    COLORREF crCurrent = AfxGetApp()->GetProfileInt( strSection, strEntry, crDefault );

    //display dialog
    CColorDialog ColourDlg( crCurrent, CC_ANYCOLOR|CC_RGBINIT );
    ColourDlg.m_cc.lpCustColors = (COLORREF*)g_crCustomColours;
    if( ColourDlg.DoModal() == IDOK )
    {
        AfxGetApp()->WriteProfileInt( strSection, strEntry, ColourDlg.GetColor() );
    }
}

void COptionsPage2::OnOK() 
{
    UpdateData(UD_GETDATA);

    //save settings
    AfxGetApp()->WriteProfileInt( "Display", "FlatToolbar", m_fFlatToolbar );
    AfxGetApp()->WriteProfileInt( "Sheet", "FieldAnimation", m_fAnimatedFieldEdit );
    AfxGetApp()->WriteProfileInt( "Tips", "StartUp", !m_fTipOfTheDay );

    //apply settings
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    if( pMainFrame )
    {
        pMainFrame->GetToolbar().GetToolBarCtrl().ModifyStyle( m_fFlatToolbar?0:TBSTYLE_FLAT, m_fFlatToolbar?TBSTYLE_FLAT:0 );
        pMainFrame->GetToolbar().RedrawWindow();
    }
    
    CPropertyPage::OnOK();
}


/*

/////////////////////////////////////////////////////////////////////////////
// COptionsPage3 property page

COptionsPage3::COptionsPage3() : CPropertyPage(COptionsPage3::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsPage3::~COptionsPage3()
{
}

void COptionsPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage3, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsPage4 property page

COptionsPage4::COptionsPage4() : CPropertyPage(COptionsPage4::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsPage4::~COptionsPage4()
{
}

void COptionsPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage4, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage4)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsPage5 property page

COptionsPage5::COptionsPage5() : CPropertyPage(COptionsPage5::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsPage5::~COptionsPage5()
{
}

void COptionsPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage5)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage5, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage5)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsPage6 property page

COptionsPage6::COptionsPage6() : CPropertyPage(COptionsPage6::IDD)
{
	//{{AFX_DATA_INIT(COptionsPage6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionsPage6::~COptionsPage6()
{
}

void COptionsPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsPage6)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsPage6, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsPage6)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/