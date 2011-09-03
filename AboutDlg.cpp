#include "stdafx.h"
#include "byakhee.h"

#include "AboutDlg.h"

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
