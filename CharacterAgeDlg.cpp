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
