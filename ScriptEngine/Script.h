// Script.h: interface for the CScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPT_H__A02EDE64_85C6_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_SCRIPT_H__A02EDE64_85C6_11D1_89C0_00001C192944__INCLUDED_

#include "ParseTree.h"
#include "Variant.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAX_STACK_SIZE 400 //maximum level of function call depth in script
//#define MAX_PARSER_STACK_SIZE (1024*1024*8) //maximum size of stack in parser thread - 8MB [should be more than enough...]


/* main script class definition */
class CScript {
public:
	CScript( const char* pszFilename );
	virtual ~CScript();
	BOOL Start( int* pnReturnValue = NULL );

	void TerminateScript();
    inline BOOL IsError() { return m_fScriptError; };
    BOOL AddClass( char* pszName, PFNCONSTRUCTOR pConstructor );
	BOOL AddGlobalVariable( char* pszName, Variant value, BOOL fReadOnly = FALSE );

	const char* GetFilename() { return m_szFilename; };

protected:


private:
	inline int DisplayScriptException( LPNODE pNode, signed int e );
	char m_szFilename[MAX_PATH+1];

/* Script Parse Tree Interpretation Members *******************************/

	//structures
	typedef struct tagVARIABLE {
		char szName[MAX_IDENTIFIER];
		Variant value;
	} VARIABLE, *LPVARIABLE;

	typedef struct tagVARIABLELIST {
		char szName[MAX_IDENTIFIER];
		Variant value;
		struct tagVARIABLELIST* next;
	} VARIABLELIST, *LPVARIABLELIST;

	typedef struct tagFUNCTIONINFORMATION {
		LPFUNCTION pFunction;
		LPVARIABLELIST pLocalVariables;
		LPVARIABLE pParameters;
		int nParameters;
		BOOL fGlobalScope;
		struct tagFUNCTIONINFORMATION* next;
	} FUNCTIONINFORMATION, *LPFUNCTIONINFORMATION;

	typedef struct tagVARIABLELISTSTACK {
		LPVARIABLELIST variables;
		LPFUNCTIONINFORMATION pScope;
		Variant* parameters;
		struct tagVARIABLELISTSTACK* next;
	} VARIABLELISTSTACK, *LPVARIABLELISTSTACK;


	//member pointers to parsing results
	LPNODE m_pScriptNode;		//pointer to script's root node
	LPNODE m_pNodeList;			//pointer to script node list
	LPFUNCTIONINFORMATION m_pFunctions;
	LPFUNCTION m_pFunctionNodes;
	FUNCTIONINFORMATION m_GlobalScope;

	//stack info
	int m_nStackSize;
	LPVARIABLELISTSTACK m_pStack;
	void PushScope( LPFUNCTIONINFORMATION pScope );
	LPFUNCTIONINFORMATION PopScope();

	//script helper functions
	inline int ScriptError( LPNODE pNodeError, char* pszErrorMessage = NULL, ... );
	int ParseTree();

	//parse tree decode functions
	BOOL ExecuteGlobalFunction( LPNODE pNode, char * pszName, Variant& returnValue, Variant* pParameters, int nParameters, LPFUNCTIONINFORMATION pThisFunction );
	Variant ExecuteStatements( LPNODE pNodeStatements, LPFUNCTIONINFORMATION pThisFunction );
	Variant ExecuteObjectMethodCall( LPNODE pNode, Variant object, char* pszMemberFuncName, LPARGUMENT pArgs, LPFUNCTIONINFORMATION pScope );

	//parse tree decode helper functions
	Variant CallFunction( LPNODE pNode, LPFUNCTIONINFORMATION pThisFunction );
	LPFUNCTIONINFORMATION FindFunction( char* pszFunctionName );
	Variant* GetParameters( LPARGUMENT pArgs, int nArgs, LPFUNCTIONINFORMATION pThisFunction );
	Variant* GetVariable( LPFUNCTIONINFORMATION pScope, LPNODE pLHS, BOOL fCheckGlobal = TRUE );
	BOOL AddVariable( LPNODE pNode, LPFUNCTIONINFORMATION pScope, char* pszName, Variant value );
	int CountParameters( LPARGUMENT pArgs );

	//tree decode variables
	int m_nWhileCount;
	int m_nForCount;
	int m_nBreakCount;
	int m_nReturnCount;
	int m_nContinueCount;

    //script error flag
    BOOL m_fScriptError;

/* Script Object Creation Members *****************************************/

    typedef struct tagCLASSLIST {
        PFNCONSTRUCTOR pConstructor;
        char szName[MAX_IDENTIFIER];

        struct tagCLASSLIST* next;
    } CLASSLIST, *LPCLASSLIST;

    LPCLASSLIST m_pClassList;

    PFNCONSTRUCTOR FindClass( char* pszName );
	Variant InstantiateClass( LPNODE pNode, char* pszName, LPARGUMENT pArgs, LPFUNCTIONINFORMATION pScope );

};

#endif // !defined(AFX_SCRIPT_H__A02EDE64_85C6_11D1_89C0_00001C192944__INCLUDED_)
