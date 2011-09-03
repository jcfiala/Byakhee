// CharacterAgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "CharacterAgeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterAgeDlg dialog


CCharacterAgeDlg::CCharacterAgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharacterAgeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharacterAgeDlg)
	m_nWhichStatistic = -1;
	//}}AFX_DATA_INIT
}


void CCharacterAgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharacterAgeDlg)
	DDX_Radio(pDX, IDC_CHARACTERISTIC1, m_nWhichStatistic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharacterAgeDlg, CDialog)
	//{{AFX_MSG_MAP(CCharacterAgeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterAgeDlg message handlers

BOOL CCharacterAgeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_nWhichStatistic = rand() % 4;
    UpdateData(UD_SETDATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
