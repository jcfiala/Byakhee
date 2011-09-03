// ByakheeScriptExecutor.cpp: implementation of the CByakheeScriptExecutor class.
//
//////////////////////////////////////////////////////////////////////

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
