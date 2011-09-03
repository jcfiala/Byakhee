// Script.cpp: implementation of the CScript class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "Script.h"
#include "ParseTree.h"
#include "VariantArray.h"
#include "PromptDialog.h"

//pull in yaccing functions and variables
extern int yyparse(void);
extern FILE *yyin, *yyout;
extern void ResetParser();
extern void CleanupParser();

/*
The following class ensures that CleanupParser is called at system closedown
*/
class __ParserCleanup {
public:
    ~__ParserCleanup() { CleanupParser(); }
};
__ParserCleanup _ParserCleanup;

/***********************************************************************************************************/
/* Constructor/Destructor **********************************************************************************/

/* CScript() - constructor for the individual script objects */
CScript::CScript( const char* pszFilename )
{
	strcpy( m_szFilename, pszFilename );

	m_pScriptNode = NULL;
	m_pNodeList = NULL;
	m_pFunctions = NULL;
	m_pFunctionNodes = NULL;

	m_fScriptError = FALSE;

    m_pClassList = NULL;

	m_nStackSize = 0;
	m_pStack = NULL;

	//initialise global scope
	m_GlobalScope.pFunction = NULL;
	m_GlobalScope.pLocalVariables = NULL;
	m_GlobalScope.pParameters = NULL;
	m_GlobalScope.nParameters = 0;
	m_GlobalScope.next = NULL;
	m_GlobalScope.fGlobalScope = TRUE;
}



/* ~CScript() - destructor for the individual script objects */
CScript::~CScript()
{
	//delete the function information list
	while( m_pFunctions )
	{
		LPFUNCTIONINFORMATION pTemp = m_pFunctions;
		m_pFunctions = m_pFunctions->next;

		//free the variable list
		LPVARIABLELIST pVars = pTemp->pLocalVariables;
		while( pVars )
		{
			LPVARIABLELIST pTmp = pVars;
			pVars = pVars->next;
			delete pTmp;
		}

		//delete the parameter list
		delete[] pTemp->pParameters;

		//delete the item
		delete pTemp;
	}

	//delete the global scope's variables
	LPVARIABLELIST pVars = m_GlobalScope.pLocalVariables;
	while( pVars )
	{
		LPVARIABLELIST pTmp = pVars;
		pVars = pVars->next;
		delete pTmp;
	}

	//free the parse tree information and the function table
	if( m_pNodeList || m_pFunctionNodes )
		FreeParseInformation( m_pNodeList, m_pFunctionNodes );

    //free the class list
    while( m_pClassList )
    {
        LPCLASSLIST pList = m_pClassList;
        m_pClassList = m_pClassList->next;

        delete pList;
    }

	//clear up the stack
	while( m_pStack ) PopScope();
}











/***********************************************************************************************************/
/* Script Environment Setup functions **********************************************************************/

/* AddGlobalVariable() - adds a variable to global scope */
BOOL CScript::AddGlobalVariable( char* pszName, Variant value, BOOL fReadOnly /*FALSE*/ )
{
	value.SetReadOnly( fReadOnly );
	return AddVariable( NULL, &m_GlobalScope, pszName, value );
}



/* AddClass() - adds the given class to the script */
BOOL CScript::AddClass( char* pszName, PFNCONSTRUCTOR pConstructor )
{
    //make sure it doesn't exist already
    if( FindClass(pszName) ) return FALSE;

    //validate everything
    if( strlen( pszName ) >= MAX_IDENTIFIER ) return FALSE;

    //create a new class list item for this class
    LPCLASSLIST pNew = new CLASSLIST;

    strcpy( pNew->szName, pszName );
    pNew->pConstructor = pConstructor;
    pNew->next = m_pClassList;
    m_pClassList = pNew;

    return TRUE;
}







/***********************************************************************************************************/
/* Script Execution Start/Stop functions *******************************************************************/

/* Start() - execute the script */
BOOL CScript::Start( int* pnReturnValue /*NULL*/ )
{
	//initialise the global parse tree functions
	g_pScriptNode = NULL;
	g_pFunctionList = NULL;
	g_pNodeList = NULL;
	g_nLineNumber = 1;

    if( pnReturnValue ) *pnReturnValue = 0;

	//build the parse tree
    FILE* file = fopen( m_szFilename, "rt" );
    fflush(NULL);
    if( file != NULL )
    {
		//reset yacc
        ResetParser();

		//do parse
		fseek( file, 0, SEEK_SET );
        yyin = file;
        yyparse();

        //clean up after parser
        fclose( file );
        yyin = stdin;
    }
    else 
	{
        MessageBox( g_hWndOwner, "Couldn't open file", "Script Error", MB_ICONEXCLAMATION|MB_TASKMODAL );
		return FALSE;
	}

	m_pScriptNode = g_pScriptNode;
	m_pFunctionNodes = g_pFunctionList;
	m_pNodeList = g_pNodeList;

    //run the parse tree
	if( m_pScriptNode )
	{
	    //build the function information list
	    LPFUNCTION pFuncNode = m_pFunctionNodes;
	    m_pFunctions = NULL;
	    while( pFuncNode )
	    {
		    LPFUNCTIONINFORMATION pNew = new FUNCTIONINFORMATION;

		    //count the number of parameters
		    pNew->nParameters = 0;
		    LPARGUMENT pArg = pFuncNode->pArguments;
		    while( pArg )
		    {
			    pNew->nParameters++;
			    pArg = pArg->next;
		    }
		    pNew->pParameters = ( pNew->nParameters > 0 ) ? new VARIABLE[pNew->nParameters+1] : NULL;
		    pNew->pFunction = pFuncNode;
		    pNew->pLocalVariables = NULL;
		    pNew->fGlobalScope = FALSE;

		    pArg = pFuncNode->pArguments;
		    int n = 0;
		    while( pArg )
		    {
			    strcpy( pNew->pParameters[n].szName, pArg->pNode->Left.strToken );
			    pNew->pParameters[n].value = 0;
			    pArg = pArg->next;
			    n++;
		    }

		    pNew->next = m_pFunctions;
		    m_pFunctions = pNew;
	    
		    pFuncNode = pFuncNode->next;
	    }

	    //interpret the tree
	    try 
        {
			int nReturnValue = ParseTree();
            if( pnReturnValue ) *pnReturnValue = nReturnValue;
	    } 
        catch( ... )
		{
            //report the exception
			ScriptError( NULL, "Unhandled exception:\nThis is probably due to a stack overflow. This program does not handle deep recursive functions very well - try rewriting the script without recursion.\nThis exception may also be caused by an internal parser error." );
            m_fScriptError = TRUE;

			//re-throw the exception to report it in debug mode...
            #ifdef _DEBUG
			throw;
            #endif
		}

        //all OK...
        return !m_fScriptError;
    }
    else
        return FALSE;
}



/* TerminateScript() - stops script execution as soon as possible */
void CScript::TerminateScript()
{
	//tell the parser to shut down ASAP
	m_fScriptError = TRUE;
}










/***********************************************************************************************************/
/* Script Parse Tree Decode Functions **********************************************************************/

/* ParseTree() - begins evaluation of the parse tree */
int CScript::ParseTree()
{
	//initialise the statement depth counters
	m_nWhileCount = 0;
	m_nForCount = 0;
	m_nBreakCount = 0;
	m_nReturnCount = 0;
	m_nContinueCount = 0;

    //reset pause and break keystates
    GetAsyncKeyState(VK_CANCEL);

	//start execution at the top of the program
	return ExecuteStatements( m_pScriptNode, &m_GlobalScope );
}




/* ExecuteStatements() - main parse tree node decoding function */
Variant CScript::ExecuteStatements( LPNODE pNode, LPFUNCTIONINFORMATION pThisFunction )
{
	if( m_fScriptError ) return 0;

    //check for control+break
    if( GetAsyncKeyState(VK_CANCEL) )
    {
        //build break notification message
        char szMessage[1024];
        if( pNode )
            sprintf( szMessage, "Break detected at line %d\nStop script execuction?", pNode->nLineNumber );
        else
            strcpy( szMessage, "Break detected\nStop script execution?" );

        //display message (ask first, for two reasons... 1) they might have pressed it by accident and 2) IDOK message boxes are terminated by the break before they are displayed!! :(
        if( MessageBox( g_hWndOwner, szMessage, "Break", MB_ICONQUESTION|MB_TASKMODAL|MB_YESNO ) == IDYES )
        {
            //terminate the script
            TerminateScript();
            return 0;
        }
    }

	Variant returnValue;
	while( pNode && !(m_nBreakCount || m_nReturnCount || m_nContinueCount || m_fScriptError) )
	{
		try {
				switch( pNode->NodeType )
				{
					case ntSTATEMENT: 
						returnValue = ExecuteStatements( pNode->Left.pNode, pThisFunction );
						pNode = pNode->Right.pNode;
						break;
							

					case ntEXPRESSION:
					{
						returnValue = ExecuteStatements( pNode->Left.pNode, pThisFunction );
						if( pNode->Right.pNode ) 
							returnValue = ExecuteStatements( pNode->Right.pNode, pThisFunction );

						return returnValue;
					}

					case ntVARIABLELIST:
						while( pNode && m_fScriptError == FALSE )
						{
							char* pszName = pNode->Left.pNode->Left.pNode->Left.strToken;
							AddVariable( pNode, pThisFunction, pszName, ExecuteStatements(pNode->Left.pNode->Right.pNode, pThisFunction) );
							pNode = pNode->Right.pNode;
						}
						return 0;

					case ntIFSTATEMENT:
					{
						if( int(ExecuteStatements( pNode->Left.pNode, pThisFunction )) != FALSE )
							returnValue = ExecuteStatements( pNode->Right.pNode, pThisFunction );
						else
							if( pNode->Other1.pNode != NULL )
								returnValue = ExecuteStatements( pNode->Other1.pNode, pThisFunction );
						return returnValue;
					}

					case ntFORSTATEMENT:
					{
						m_nForCount++;
						for( ExecuteStatements( pNode->Left.pNode, pThisFunction ); ExecuteStatements(pNode->Right.pNode, pThisFunction) != Variant(0) ; ExecuteStatements( pNode->Other1.pNode, pThisFunction ) )
						{
							returnValue = ExecuteStatements( pNode->Other2.pNode, pThisFunction );
							if( m_nBreakCount )
							{
								m_nBreakCount--;
								break;
							}
							if( m_fScriptError ) return 0; 
							if( m_nContinueCount ) m_nContinueCount--;
						}
						m_nForCount--;
						return returnValue;
					}

					case ntWHILESTATEMENT:
					{
						m_nWhileCount++;
						while( int(ExecuteStatements( pNode->Left.pNode, pThisFunction )) != FALSE )
						{
							returnValue = ExecuteStatements( pNode->Right.pNode, pThisFunction );
							if( m_nBreakCount )
							{
								m_nBreakCount--;
								break;
							}
							if( m_fScriptError ) return 0; 
							if( m_nContinueCount ) m_nContinueCount--;
						}
						m_nWhileCount--;
						return returnValue;
					}

					case ntRETURNSTATEMENT:
					{
						returnValue = ExecuteStatements( pNode->Left.pNode, pThisFunction );
						m_nReturnCount++;
						return returnValue;
					}

					case ntBREAKSTATEMENT:
						m_nBreakCount++;
						return 0;

					case ntCONTINUESTATEMENT:
						m_nContinueCount++;
						return 0;

					case ntFUNCTIONCALL:
						return CallFunction( pNode, pThisFunction );
						break;

					case ntOBJECTMETHODCALL:
						return ExecuteObjectMethodCall( pNode, ExecuteStatements( pNode->Left.pNode, pThisFunction ), pNode->Right.pNode->Left.strToken, (LPARGUMENT)pNode->Other1.pNode, pThisFunction );

					case ntNEW:	
						return InstantiateClass( pNode, pNode->Left.pNode->Left.pNode->Left.strToken, (LPARGUMENT)pNode->Left.pNode->Right.pNode, pThisFunction );

					case ntNEGATE:			 return -ExecuteStatements( pNode->Left.pNode, pThisFunction );
					case ntLOGICALNOT:		 return !ExecuteStatements( pNode->Left.pNode, pThisFunction );
					case ntCOMPLIMENT:		 return ~ExecuteStatements( pNode->Left.pNode, pThisFunction );

					case ntASSIGN:			 return *GetVariable( pThisFunction, pNode->Left.pNode ) = ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntADDASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) += ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSUBASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) -= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntMULASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) *= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntDIVASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) /= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntMODASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) %= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntANDASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) &= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntORASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) |= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntXORASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) ^= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSHLASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) <<= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSSHRASSIGN:		 return *GetVariable( pThisFunction, pNode->Left.pNode ) >>= ExecuteStatements( pNode->Right.pNode, pThisFunction );

					case ntPREINCREMENT:	 return ++ExecuteStatements( pNode->Left.pNode, pThisFunction );
					case ntPREDECREMENT:     return --ExecuteStatements( pNode->Left.pNode, pThisFunction );
					case ntPOSTINCREMENT:    return ExecuteStatements( pNode->Left.pNode, pThisFunction )++;
					case ntPOSTDECREMENT:    return ExecuteStatements( pNode->Left.pNode, pThisFunction )--;

					case ntISEQUALS:		 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) == ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntISNOTEQUALS:		 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) != ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntISLTHAN:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) < ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntISLTHANEQUALS:	 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) <= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntISGTHANEQUALS:	 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) >= ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntISGTHAN:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) > ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntLOGAND:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) && ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntLOGOR:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) || ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSHL:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) << ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSSHR:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) >> ExecuteStatements( pNode->Right.pNode, pThisFunction );

					case ntADD:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) + ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntSUB:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) - ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntMUL:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) * ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntDIV:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) / ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntMOD:				 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) % ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntBITAND:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) & ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntBITOR:            return ExecuteStatements( pNode->Left.pNode, pThisFunction ) | ExecuteStatements( pNode->Right.pNode, pThisFunction );
					case ntBITXOR:			 return ExecuteStatements( pNode->Left.pNode, pThisFunction ) ^ ExecuteStatements( pNode->Right.pNode, pThisFunction );

					case ntQUESTIONMARKCOLON:
						if( int(ExecuteStatements( pNode->Left.pNode, pThisFunction )) )
							return ExecuteStatements( pNode->Right.pNode, pThisFunction );
						else
							return ExecuteStatements( pNode->Other1.pNode, pThisFunction );

					case ntOBJECTMEMBERACCESS:
						return GetVariable( pThisFunction, pNode );

					case ntARRAYDEREFERENCE: return GetVariable( pThisFunction, pNode );
					case ltSTRING:			 return Variant(pNode->Left.strToken);
					case ltFLOAT:			 return Variant(pNode->Left.dValue);
					case ltNUMBER:			 return Variant(pNode->Left.nValue);
					case ltIDENTIFIER:		 return GetVariable( pThisFunction, pNode );

					default:				 return ScriptError( pNode, "Unexpected node type in parse tree" );
				}
		} catch( signed int e )
			{

				//catch all of the expressions that the variant class might throw
				return DisplayScriptException( pNode, e );
			}
	}

	return returnValue;
}







/***********************************************************************************************************/
/* Variable Handling Functions *****************************************************************************/

/* GetVariable() - locate a variable and returns a pointer to it */
Variant* CScript::GetVariable( LPFUNCTIONINFORMATION pScope, LPNODE pLHS, BOOL fCheckGlobal /*TRUE*/ )
{
	static Variant dummy;
	dummy = 0;

    switch( pLHS->NodeType )
	{
		case ltIDENTIFIER:
		{
			//check the scope parameters
			for( int i = 0; i < pScope->nParameters; i++ )
			{
				if( strcmp(pScope->pParameters[i].szName, pLHS->Left.strToken ) == 0 )
                    return &(pScope->pParameters[i].value);
			}

			//check the local scope
			LPVARIABLELIST pVar = pScope->pLocalVariables;
			while( pVar )
			{
				if( strcmp(pVar->szName, pLHS->Left.strToken ) == 0 ) 
                    return &(pVar->value);
				pVar = pVar->next;
			}

			//check global scope
			if( fCheckGlobal )
			{
				LPVARIABLELIST pVar = m_GlobalScope.pLocalVariables;
				while( pVar )
				{
					if( strcmp(pVar->szName, pLHS->Left.strToken ) == 0 ) return &pVar->value;
					pVar = pVar->next;
				}
			}

			ScriptError( pLHS, "Variable \"%s\" not found", pLHS->Left.strToken );
			break;
		}

		case ntOBJECTMEMBERACCESS:
		{
			Variant variant = ExecuteStatements( pLHS->Left.pNode, pScope );
			if( variant.IsObject() == FALSE )
				{ ScriptError( pLHS, "LHS is not an object" ); break; }

			CScriptObject* pObject = LPCSCRIPTOBJECT(variant);
			if( pObject )
			{
				Variant* pVar = pObject->GetMemberVariable( pLHS->Right.pNode->Left.strToken );
				if( pVar ) 
					return pVar;
				else
					ScriptError( pLHS, "Member variable \"%s\" not found", pLHS->Right.pNode->Left.strToken );
			}
			else ScriptError( pLHS, "LHS is not an object" ); 
			break;
		}

		case ntARRAYDEREFERENCE:
        {
            unsigned int uIndex = unsigned int(ExecuteStatements( pLHS->Right.pNode, pScope ) );
            Variant v = ExecuteStatements( pLHS->Left.pNode, pScope );
            return v[uIndex];
			break;
        }

		default:
			ScriptError( pLHS, "Invalid LValue" );
			break;

	}

	return &dummy;
}



/* AddVariable() - adds a variable to the given scope */
BOOL CScript::AddVariable( LPNODE pNode, LPFUNCTIONINFORMATION pScope, char* pszName, Variant value )
{
	//see if it already exists
	BOOL fExists = FALSE;

	//check the scope parameters
	for( int i = 0; i < pScope->nParameters; i++ )
	{
		if( strcmp(pScope->pParameters[i].szName, pszName ) == 0 ) { fExists = TRUE; break; }
	}

	//check the local scope
	if( !fExists )
	{
		LPVARIABLELIST pVar = pScope->pLocalVariables;
		while( pVar )
		{
			if( strcmp(pVar->szName, pszName ) == 0 ) { fExists = TRUE; break; }
			pVar = pVar->next;
		}
	}
	if( fExists )
	{
		ScriptError( pNode, "Variable \"%s\" has already been declared!", pszName );
		return FALSE;
	}

	//create the new list object and initialise it
	LPVARIABLELIST pNew = new VARIABLELIST;
	strcpy( pNew->szName, pszName );
	pNew->value.Become( value );

	//link it at the start of this scope's local variable list
	pNew->next = pScope->pLocalVariables;
	pScope->pLocalVariables = pNew;

	return TRUE;
}











/***********************************************************************************************************/
/* Function Call Handling Functions ************************************************************************/

/* PushScope() - pushes the current scope onto the stack */
void CScript::PushScope( LPFUNCTIONINFORMATION pScope )
{
	LPVARIABLELISTSTACK pNewStackItem = new VARIABLELISTSTACK;
	pNewStackItem->variables = pScope->pLocalVariables;
	pNewStackItem->parameters = new Variant[ pScope->nParameters ];
	pNewStackItem->pScope = pScope;
	for( int i = 0; i < pScope->nParameters; i++ )
		pNewStackItem->parameters[i] = pScope->pParameters[i].value;

	pNewStackItem->next = m_pStack;
	m_pStack = pNewStackItem;
}

/* PopScope() - removes the topmost scope from the stack and returns it */
CScript::LPFUNCTIONINFORMATION CScript::PopScope()
{
	//remove top item from stack
	LPVARIABLELISTSTACK pStackTop = m_pStack;
	m_pStack = m_pStack->next;

	//prepare return scope

	//delete current local variables
	LPFUNCTIONINFORMATION pScope = pStackTop->pScope;
	while( pScope->pLocalVariables )
	{
		LPVARIABLELIST pTmp = pScope->pLocalVariables;
		pScope->pLocalVariables = pScope->pLocalVariables->next;
		delete pTmp;
	}
	pScope->pLocalVariables = pStackTop->variables;

	//reset parameters
	for( int i = 0; i < pScope->nParameters; i++ )
		pScope->pParameters[i].value = pStackTop->parameters[i];
	
	delete[] pStackTop->parameters;
	delete pStackTop;

	return pScope;
}

/* CallFunction() - calls a function. This could be global or local */
Variant CScript::CallFunction( LPNODE pNode, LPFUNCTIONINFORMATION pThisFunction )
{
	m_nStackSize++;
	if( m_nStackSize > MAX_STACK_SIZE )
		return ScriptError( pNode, "Stack Overflow" );

	Variant value = 0;

	char* pszFuncName = pNode->Left.pNode->Left.strToken;

	LPFUNCTIONINFORMATION pFunc = FindFunction( pszFuncName );
	if( pFunc )
	{
		//store current
		if( pThisFunction->fGlobalScope == FALSE )
			PushScope( pThisFunction );

		//plug the variables into the function's parameter list
		int n;
		LPARGUMENT pArg = (LPARGUMENT)pNode->Right.pNode;
		for( n = 0; n < pFunc->nParameters; n++ )
		{
			if( pArg )
			{
				Variant v = ExecuteStatements( pArg->pNode, pThisFunction );
				pFunc->pParameters[n].value = v;
				pArg = pArg->next;
			}
			else ScriptError( pNode, "Too few parameters for function call to \"%s\"", pszFuncName );
		}

		if( pArg ) //still arguments in the list...
			ScriptError( pNode, "Too many parameters for function call to \"%s\"", pszFuncName );

		//clear current local variables
		if( pThisFunction->fGlobalScope == FALSE )
			pThisFunction->pLocalVariables = NULL;

		//execute the function
		value = ExecuteStatements( pFunc->pFunction->pFunctionBodyNode, pFunc );

		//delete function's allocated local variables
		while( pFunc->pLocalVariables )
		{
			LPVARIABLELIST pTmp = pFunc->pLocalVariables;
			pFunc->pLocalVariables = pFunc->pLocalVariables->next;
			delete pTmp;
		}

		//restore current
		if( pThisFunction->fGlobalScope == FALSE ) PopScope();

	}
	else
	{
		//count the parameters
		int nParameters = CountParameters( (LPARGUMENT)pNode->Right.pNode );
        Variant* pParameters = GetParameters( (LPARGUMENT)pNode->Right.pNode, nParameters, pThisFunction );
		BOOL fSuccess;

		//check for global scope function
		fSuccess = ExecuteGlobalFunction( pNode, pszFuncName, value, pParameters, nParameters, pThisFunction );
		delete[] pParameters;

		if( !fSuccess )
		{
			//function not found in script or global scope
			ScriptError( pNode, "Function \"%s\" has not been declared!", pszFuncName );
		}
	}

	m_nStackSize--;

	//decrement the 'return' counter return the return value from the function
	if( m_nReturnCount ) m_nReturnCount--;
	return value;
}


/* FindFunction() - finds the function information for a given function */
CScript::LPFUNCTIONINFORMATION CScript::FindFunction( char* pszFunctionName )
{
	LPFUNCTIONINFORMATION pFunctions = m_pFunctions;
	while( pFunctions )
	{
		if( pFunctions->pFunction->pFunctionNameLeaf && strcmp( pFunctions->pFunction->pFunctionNameLeaf->Left.strToken, pszFunctionName ) == 0 )
			return pFunctions;

		pFunctions = pFunctions->next;
	}

	return NULL;
}


/* CountParameters() - Counts the number of parameters in the given list */
int CScript::CountParameters( LPARGUMENT pArgs )
{
	int nParameters = 0;
	while( pArgs ) { nParameters++; pArgs = pArgs->next; };

	return nParameters;
}


/* GetParameters() - evaluates the given list of arguments */
Variant* CScript::GetParameters(LPARGUMENT pArgs, int nArgs, LPFUNCTIONINFORMATION pThisFunction )
{
	Variant* pParams = new Variant[nArgs];

	for( int n = 0; n < nArgs; n++ )
	{
		pParams[n] = ExecuteStatements( pArgs->pNode, pThisFunction );
		pArgs = pArgs->next;
	}
	
	return pParams;
}


/* ExecuteGlobalFunction() - executes the given global function */
BOOL CScript::ExecuteGlobalFunction( LPNODE pNode, char * pszName, Variant& returnValue, Variant* pParameters, int nParameters, LPFUNCTIONINFORMATION pThisFunction )
{
	returnValue = 0;

	/* check the function name against *all* of the possible global functions */

	//note: this could be optimised very simply by storing a value in pNode's Other1 and Other2 pointers...

	if( strcmp( pszName, "alert" ) == 0 )
	{
		if( nParameters == 1 )
		{
            MessageBox( g_hWndOwner, LPCSTR(pParameters[0]), "Message", MB_ICONEXCLAMATION|MB_TASKMODAL );
			return TRUE;
		}
		else
		{
			ScriptError( pNode, "alert() takes one parameter" );
			return FALSE;
		}
	}

	if( strcmp( pszName, "confirm" ) == 0 )
	{
		if( nParameters == 1 )
		{
			int nResult = MessageBox( g_hWndOwner, LPCSTR(pParameters[0]), "Confirm", MB_ICONEXCLAMATION|MB_YESNO );
			if( nResult == IDYES )
				returnValue = TRUE;
			else
				returnValue = FALSE;

			return TRUE;
		}
		else
		{
			ScriptError( pNode, "confirm() takes one parameter" );
			return FALSE;
		}
	}

	if( strcmp( pszName, "prompt" ) == 0 && g_hWndOwner != NULL )
	{
		if( nParameters == 1 )
		{
            CPromptDialog PromptDialog( pParameters[0] );
            returnValue = PromptDialog.Prompt();
			return TRUE;
		}
		else
		{
			ScriptError( pNode, "prompt() takes one parameter" );
			return FALSE;
		}
	}

	return FALSE;
}



/* ExecuteObjectMethodCall() - executes the given member function call on the given object */
Variant CScript::ExecuteObjectMethodCall( LPNODE pNode, Variant object, char* pszName, LPARGUMENT pArgs, LPFUNCTIONINFORMATION pScope )
{
	CScriptObject* pObject;

	//make sure we've got an object
	if( object.IsObject() == FALSE || (pObject = LPCSCRIPTOBJECT(object)) == NULL )
	{
		ScriptError( pNode, "Member function call %s on non-object", pszName );
		return 0;
	}

	//build the parameter list
	int nArgs = CountParameters( pArgs );
	CScriptObject::MEMBERRESULT result;
	Variant returnValue;
	Variant* pParams = GetParameters( pArgs, nArgs, pScope );
	if( !m_fScriptError )
	{
	    //call the member function
    	result = pObject->ExecuteMemberFunction( pszName, pParams, nArgs, returnValue );
    }
    else result = CScriptObject::OK; //don't display func. execution error as well

	//delete the parameters and check the result of execution
	delete[] pParams;	

	switch( result )
	{
		case CScriptObject::OK: break;
		case CScriptObject::NOT_FOUND:		 ScriptError( pNode, "Member function %s not found!", pszName ); break;
		case CScriptObject::TOO_FEW_PARAMS:  ScriptError( pNode, "%s() : too few parameters", pszName ); break;
		case CScriptObject::TOO_MANY_PARAMS: ScriptError( pNode, "%s() : too many parameters", pszName ); break;
        case CScriptObject::UNKNOWN_ERROR:   ScriptError( pNode ); break;
		default:						     ScriptError( pNode, "failed to call member function %s", pszName ); break;
	}

	return returnValue;
}







/***********************************************************************************************************/
/* Class Handling Functions ********************************************************************************/


/* FindClass() - locates the given class */
PFNCONSTRUCTOR CScript::FindClass( char* pszName )
{
    LPCLASSLIST pList = m_pClassList;

    while( pList )
    {
        if( stricmp( pszName, pList->szName ) == 0 )
            return pList->pConstructor;

        pList = pList->next;
    }

    return NULL;
}



/* InstantiateClass() - creates a new instance of the given class */
Variant CScript::InstantiateClass( LPNODE pNode, char* pszName, LPARGUMENT pArgs, LPFUNCTIONINFORMATION pScope )
{
    Variant newClass;

    PFNCONSTRUCTOR pConstructor = FindClass( pszName );
    if( pConstructor == NULL )
    {
        ScriptError( pNode, "Class %s not defined", pszName );
        return 0;
    }

	int nArgs = CountParameters( pArgs );
	CScriptObject::MEMBERRESULT result;

    Variant* pParams = GetParameters( pArgs, nArgs, pScope );
	if( !m_fScriptError )
    {
        result = pConstructor( pParams, nArgs, newClass );
        delete[] pParams;
        switch( result )
        {
		    case CScriptObject::OK: break;
		    case CScriptObject::NOT_FOUND:		 ScriptError( pNode, "Member function %s not found!", pszName ); return 0;
		    case CScriptObject::TOO_FEW_PARAMS:  ScriptError( pNode, "%s() : too few parameters", pszName ); return 0;
		    case CScriptObject::TOO_MANY_PARAMS: ScriptError( pNode, "%s() : too many parameters", pszName ); return 0;
            case CScriptObject::UNKNOWN_ERROR:   ScriptError( pNode ); return 0;
		    default:						     ScriptError( pNode, "failed to call member function %s", pszName ); return 0;
        }
    }
    else
    {
        delete[] pParams;
        return 0;
    }

    return newClass;
}







/***********************************************************************************************************/
/* Error Handling Functions ********************************************************************************/

/* DisplayScriptException() - displays the error message related to the given exception number */
int CScript::DisplayScriptException( LPNODE pNode, signed int e )
{
    switch( e )
    {
		case INVALID_OPERATOR_STRING:
			return ScriptError( pNode, "Invalid operator for strings" );

		case INVALID_OPERATOR_UINT:
		case INVALID_OPERATOR_INT:
			return ScriptError( pNode, "Invalid operator for integers" );

		case INVALID_OPERATOR_FLOAT:
			return ScriptError( pNode, "Invalid operator for floating point numbers" );

        case INVALID_OPERATOR_OBJECT:
            return ScriptError( pNode, "Object does not support this operator" );

        case ARRAY_INDEX_OUT_OF_BOUNDS:
            return ScriptError( pNode, "Array access out of bounds" );

        case VALUE_IS_READ_ONLY:
            return ScriptError( pNode, "Value cannot be modified" );

		case OUT_OF_MEMORY:
			return ScriptError( pNode, "Out of memory" );

		case DIVIDE_BY_ZERO:
			return ScriptError( pNode, "Divide by zero" );

		default:
			return ScriptError( pNode, "Internal Parser Error: 0x%X", e );
    }
}


/* ScriptError() - displays a given error message and sets various error flags */
int CScript::ScriptError( LPNODE pNodeError, char* pszErrorMessage /*NULL*/, ... )
{
    if( pszErrorMessage )
    {
	    //display the error message
	    char szBuffer[1024];
	    va_list pArguments;
	    va_start( pArguments, pszErrorMessage );
	    vsprintf( szBuffer, pszErrorMessage, pArguments );
		va_end( pArguments );

        int nLineNumber = 0;
        if( pNodeError )
        {
            //get the line number from the node
            nLineNumber = pNodeError->nLineNumber;
            char* pszMessage = new char[strlen(szBuffer)+1024];
            sprintf( pszMessage, "Line %d : %s", nLineNumber, szBuffer );
            MessageBox( g_hWndOwner, pszMessage, "Script Error", MB_ICONEXCLAMATION|MB_TASKMODAL );
            delete[] pszMessage;
        }
        else
        {
            //the error does not include a node - just show the error
            MessageBox( g_hWndOwner, szBuffer, "Script Error", MB_ICONEXCLAMATION|MB_TASKMODAL );
        }
    }

	//shut everything down after an error
	m_fScriptError = TRUE;
	return 0;
}
