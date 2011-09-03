#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#pragma once

#include "Hyperlink.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperlink	m_wndMailTips;
	CHyperlink	m_wndMailCImage;
	CHyperlink	m_wndMailArtist;
	CHyperlink	m_wndMailAuthor;
	CHyperlink	m_wndDeltaGreenWeb;
	CHyperlink	m_wndChaosiumWeb;
	CHyperlink	m_wndAuthorWeb;
	CHyperlink	m_wndArtistWeb;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__ABOUTDLG_H__