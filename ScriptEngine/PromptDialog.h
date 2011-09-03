// PromptDialog.h: interface for the CPromptDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMPTDIALOG_H__C339C0E0_7829_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_PROMPTDIALOG_H__C339C0E0_7829_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#if _MSC_VER > 5000
#define HWINDOW HWND
#else
#define HWINDOW HANDLE
#endif


extern HINSTANCE g_hThisInstance;
extern HWINDOW g_hWndOwner;

#include "Variant.h"

class CPromptDialog  
{
public:
	Variant Prompt();
	CPromptDialog( LPCSTR pszPrompt );
	virtual ~CPromptDialog();

private:
	LPCSTR m_pszPrompt;
};

#endif // !defined(AFX_PROMPTDIALOG_H__C339C0E0_7829_11D2_9FF9_00001C192944__INCLUDED_)
