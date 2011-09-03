#ifndef _PARSETREE_H
#define _PARSETREE_H

#include <windows.h>

/* type sizes *****************************************************************************/
#define MAX_IDENTIFIER 64


/* node type definitions ******************************************************************/
enum NODETYPE { ntINVALID,
				ntVARIABLELIST, ntVARIABLE, ntSTATEMENT, ntIFSTATEMENT, ntFORSTATEMENT, ntWHILESTATEMENT, 
				ntRETURNSTATEMENT, ntBREAKSTATEMENT, ntCONTINUESTATEMENT,
				ntEXPRESSION, 
				ntNEGATE, ntLOGICALNOT, ntCOMPLIMENT,
				ntASSIGN, ntADDASSIGN, ntSUBASSIGN, ntMULASSIGN, ntDIVASSIGN, ntMODASSIGN, ntANDASSIGN, ntORASSIGN, ntXORASSIGN, ntSHLASSIGN, ntSSHRASSIGN, ntUSHRASSIGN,
				ntPREINCREMENT, ntPREDECREMENT, ntPOSTINCREMENT, ntPOSTDECREMENT,
				ntISEQUALS, ntISNOTEQUALS, ntISLTHAN, ntISLTHANEQUALS, ntISGTHANEQUALS, ntISGTHAN,
				ntADD, ntSUB, ntMUL, ntDIV, ntMOD,
				ntBITAND, ntBITOR, ntBITXOR,
				ntLOGAND, ntLOGOR,
				ntSHL, ntSSHR, ntUSHR,
				ntQUESTIONMARKCOLON,
				ntNEW,
				ntFUNCTIONCALL, ntCONSTRUCTOR,
				ntOBJECTMETHODCALL, ntOBJECTMEMBERACCESS,
				ntPARAM,
				ntARRAYDEREFERENCE,

				ltSTRING, ltNUMBER, ltFLOAT, ltIDENTIFIER
              };




/* struture definitions for Parse tree ****************************************************/

#ifndef _WIN32
	typedef unsigned char BOOL;
	#define TRUE (1)
	#define FALSE (0)
#endif


typedef union tagNODE_OR_LEAF {
    struct tagNODE* pNode;
	char* strToken;
	int nValue;	
	double dValue;
} NODE_OR_LEAF, *LPNODE_OR_LEAF;

typedef struct tagNODE {
    NODETYPE NodeType;
    BOOL     fLeaf;
	BOOL	 fString;
    NODE_OR_LEAF Left, Right, Other1, Other2;

    int nLineNumber;

	struct tagNODE* next;
} NODE, *LPNODE;

typedef struct tagARGUMENT {
	LPNODE pNode;
	struct tagARGUMENT* next;
} ARGUMENT, *LPARGUMENT;

typedef struct tagFUNCTION {
	LPNODE pFunctionNameLeaf;
	LPNODE pFunctionBodyNode;
	LPARGUMENT pArguments;
	struct tagFUNCTION* next;
} FUNCTION, *LPFUNCTION;


/* global script variable declarations ****************************************************/
extern LPNODE g_pScriptNode;		//pointer to script's root node
extern LPFUNCTION g_pFunctionList;	//pointer to script's function list
extern void FreeParseInformation( LPNODE pNodeList, LPFUNCTION pFunctionList );
extern int g_nLineNumber;

extern LPNODE g_pNodeList;			//pointer to linear script node list

#endif //_PARSETREE_H
