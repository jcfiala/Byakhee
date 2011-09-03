// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9FB21C6_4B45_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_STDAFX_H__A9FB21C6_4B45_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//special macros to make UpdateData() easier to read
//note: I know this is an awful hack, but I always get confused
//with the crappy UpdateData() function... 
#define UD_SETDATA FALSE
#define UD_GETDATA  TRUE


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9FB21C6_4B45_11D2_9FF9_00001C192944__INCLUDED_)

// Suggested at http://support.microsoft.com/kb/309801 to handle unresolved external symbol errors.
template class __declspec(dllimport) CStringT<TCHAR, StrTraitMFC<TCHAR, ChTraitsCRT<TCHAR> > >;
template class __declspec(dllimport) CSimpleStringT<TCHAR>;