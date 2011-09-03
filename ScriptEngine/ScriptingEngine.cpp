// ScriptingEngine.cpp: implementation of the CScriptingEngine class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include "ScriptingEngine.h"
#include "ScriptObject.h"
#include "Script.h"
#include "VariantArray.h"
#include "Script_Math.h"
#include "PromptDialog.h"

char szTempFileName[MAX_PATH+1];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptingEngine::CScriptingEngine()
{
    //initialise members
	m_pScript = NULL;

    //clear the temp. file name
    char szTempPath[MAX_PATH+1];
    strcpy( szTempPath, "" );
    GetTempPath( MAX_PATH, szTempPath );
    GetTempFileName( szTempPath, "BYK", 0, szTempFileName );
}

CScriptingEngine::~CScriptingEngine()
{
    delete m_pScript;
    DeleteFile( szTempFileName );
}

BOOL CScriptingEngine::LoadScript( const char* pszFilename )
{
    //remove existing script, if any
    delete m_pScript;

	//create the new script object
	m_pScript = new CScript( pszFilename );
	return (m_pScript != NULL);
}

BOOL CScriptingEngine::ExecuteScript( int* pnReturnValue /*NULL*/ )
{
#ifdef _DEBUG
    OutputDebugString( "Using DEBUG version of Scripting Engine library\n" );
#endif
    if( m_pScript )
    {
		//add constants
		m_pScript->AddGlobalVariable( "null", Variant::GetNULL(), TRUE );
		m_pScript->AddGlobalVariable( "true", Variant(TRUE), TRUE );
		m_pScript->AddGlobalVariable( "false", Variant(FALSE), TRUE );

		//add global objects
		m_pScript->AddGlobalVariable( "math", new CMath(), TRUE );

        //add global classes
        m_pScript->AddClass( "array", VariantArray::Constructor );

        //execute the script
        return m_pScript->Start( pnReturnValue );
    }
    else
        return FALSE;
}

void CScriptingEngine::TerminateScript()
{
    if( m_pScript )
        m_pScript->TerminateScript();
}

BOOL CScriptingEngine::AddClass( char* pszName, PFNCONSTRUCTOR pConstructor )
{
    if( m_pScript )
        return m_pScript->AddClass( pszName, pConstructor );
    else
        return FALSE;
}

BOOL CScriptingEngine::AddGlobalVariable( char* pszName, Variant value, BOOL fReadOnly = FALSE )
{
    if( m_pScript )
        return m_pScript->AddGlobalVariable( pszName, value, fReadOnly );
    else
        return FALSE;
}

BOOL CScriptingEngine::SetScript(const char * pszScript)
{
    //create the temp. file
    HANDLE hFile = CreateFile( szTempFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_WRITE_THROUGH, NULL );
    if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

    //write the scriptlet to it
    DWORD dwWritten;
    BOOL fWriteResult = WriteFile( hFile, pszScript, strlen(pszScript), &dwWritten, NULL );
    CloseHandle( hFile );

    //fail if the write failed
    if( fWriteResult == FALSE ) return FALSE;
    
    //try and load it
    return LoadScript( szTempFileName );
}

void CScriptingEngine::SetOwnerWindow( HWINDOW hWndOwner )
{
    g_hWndOwner = hWndOwner;
}
