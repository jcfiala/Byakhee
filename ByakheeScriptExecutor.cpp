// ByakheeScriptExecutor.cpp: implementation of the CByakheeScriptExecutor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "ByakheeScriptExecutor.h"
#include "DiceRoller.h"
#include "Output.h"
#include "ScriptEngine/ScriptingEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CByakheeScriptExecutor::CByakheeScriptExecutor( CString strScriptFileName )
{
    m_strScriptFileName = strScriptFileName;
}

CByakheeScriptExecutor::~CByakheeScriptExecutor()
{

}

BOOL CByakheeScriptExecutor::Execute()
{
    CScriptingEngine ScriptEngine;
    ScriptEngine.SetOwnerWindow( AfxGetMainWnd()->m_hWnd );
    if( ScriptEngine.LoadScript(m_strScriptFileName) )
    {
        //add the global objects
        ScriptEngine.AddGlobalVariable( "dice", new CScriptDice(), TRUE );
        ScriptEngine.AddGlobalVariable( "output", new CScriptOutput(), TRUE );

        //FIXME: add ourselves as a creator of handouts, monsters and investigators,
        //and make CreateHandout, CreateMonster and CreateInvestigator functions
        //
        //...or something so that new() can be used!

        //run the script
        int nReturnValue = 0;
        CByakheeApp::ShowScriptExecutionQuickInfo();
        if( ScriptEngine.ExecuteScript(&nReturnValue) == FALSE ) return FALSE;

        if( nReturnValue == 1 ) return TRUE;
    }

    return FALSE;
}
