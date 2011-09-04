// QuickInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "QuickInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuickInfoDlg dialog
/**
 * Copyright (C) 1998  David Harvey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


CQuickInfoDlg::CQuickInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickInfoDlg)
	m_fDontTellMeAgain = FALSE;
	//}}AFX_DATA_INIT
}


void CQuickInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickInfoDlg)
	DDX_Control(pDX, IDC_TEXTAREA, m_wndTextArea);
	DDX_Control(pDX, IDC_DLGICON, m_wndIcon);
	DDX_Check(pDX, IDC_DONTTELLMEAGAIN, m_fDontTellMeAgain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CQuickInfoDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickInfoDlg message handlers

BOOL CQuickInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //change the icon in the dialog to the standard info icon
	m_wndIcon.SetIcon( AfxGetApp()->LoadStandardIcon(IDI_INFORMATION) );

    //update the dialog
    UpdateData(UD_SETDATA);

    //change the value in the text area
    //note: this isn't done through a DDX_TEXT because the word-wrapping does not appear to work
    m_wndTextArea.SetWindowText(m_strText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuickInfoDlg::OnDestroy() 
{
    UpdateData(UD_GETDATA);

    //if the user does not want this quickinfo any more, save the value in the registry
    if( m_fDontTellMeAgain ) AfxGetApp()->WriteProfileInt( "QuickInfo", m_strTipName, TRUE );

	CDialog::OnDestroy();
}

BOOL CQuickInfoDlg::Show(CString strTipName, CString strTip)
{
    //store these values
    m_strTipName = strTipName;
    m_strText = strTip;

    //see if the user wants this info again
    m_fDontTellMeAgain = AfxGetApp()->GetProfileInt( "QuickInfo", strTipName, FALSE );

    //if they do, display the dialog
    if( m_fDontTellMeAgain == FALSE )
    {
        DoModal();
        return TRUE;
    }
    else
        return FALSE;
}
