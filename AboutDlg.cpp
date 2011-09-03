#include "stdafx.h"
#include "byakhee.h"

#include "AboutDlg.h"


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MAILTIPS, m_wndMailTips);
	DDX_Control(pDX, IDC_MAILCIMAGE, m_wndMailCImage);
	DDX_Control(pDX, IDC_MAILARTIST, m_wndMailArtist);
	DDX_Control(pDX, IDC_EMAILAUTHOR, m_wndMailAuthor);
	DDX_Control(pDX, IDC_DELTAGREENWEB, m_wndDeltaGreenWeb);
	DDX_Control(pDX, IDC_CHAOSIUMWEB, m_wndChaosiumWeb);
	DDX_Control(pDX, IDC_AUTHORWEB, m_wndAuthorWeb);
	DDX_Control(pDX, IDC_ARTISTWEB, m_wndArtistWeb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndMailAuthor.SetURL( "mailto:dharvey@btinternet.com" );
	m_wndMailArtist.SetURL( "mailto:yanasikt@superonline.com" );
	m_wndMailCImage.SetURL( "mailto:julian.smart@ukonline.co.uk" ); //FIXME: add http://web.ukonline.co.uk/Members/julian.smart ?
	m_wndMailTips.SetURL( "mailto:christe@sti.com.br" );

	m_wndAuthorWeb.SetURL( "http://www.geocities.com/SiliconValley/Haven/4173/" );
	m_wndArtistWeb.SetURL( "http://www.atacomputer.com.tr/oir/gallery" );
	m_wndChaosiumWeb.SetURL( "http://www.chaosium.com" );
	m_wndDeltaGreenWeb.SetURL( "http://www.delta-green.com" );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
