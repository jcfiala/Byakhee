// ScriptingEngine.h: interface for the CScriptingEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTINGENGINE_H__A02EDE61_85C6_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_SCRIPTINGENGINE_H__A02EDE61_85C6_11D1_89C0_00001C192944__INCLUDED_

#include "ScriptObject.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#if _MSC_VER > 5000
#define HWINDOW HWND
#else
#define HWINDOW HANDLE
#endif

class CScriptingEngine  
{
public:
    void SetOwnerWindow( HWINDOW hWndOwner );

	BOOL SetScript( const char* pszScript );
	CScriptingEngine();
	virtual ~CScriptingEngine();

	BOOL LoadScript( const char* pszFilename );
	BOOL ExecuteScript( int* pnReturnValue = NULL );
    void TerminateScript();

    BOOL AddClass( char* pszName, PFNCONSTRUCTOR pConstructor );
    BOOL AddGlobalVariable( char* pszName, Variant value, BOOL fReadOnly /*FALSE*/ );

private:
	class CScript* m_pScript;
};

#endif // !defined(AFX_SCRIPTINGENGINE_H__A02EDE61_85C6_11D1_89C0_00001C192944__INCLUDED_)
