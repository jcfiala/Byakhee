#ifndef lint
static char const yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
extern "C" { char *getenv(const char *); }
#else
extern char *getenv();
extern int yylex();
extern int yyparse();
#endif
#define YYPREFIX "yy"
#line 6 "javascript.y"

#include <string.h>
#include <stdio.h>
#include "ParseTree.h"

extern int g_nLineNumber;

/* global script data pointers ************************************************************/

LPNODE g_pScriptNode = NULL;			/*pointer to script's root node*/
LPFUNCTION g_pFunctionList = NULL;		/*pointer to script's function list*/
LPNODE g_pNodeList = NULL;              /*pointer to linear script node list*/


/* item creation functions ****************************************************************/
void yyerror( char *s );

LPNODE NewNode( NODETYPE type, LPNODE pLeft, LPNODE pRight, LPNODE pOther1 = NULL, LPNODE pOther2 = NULL )
{
	if( type == ntINVALID ) return NULL;

    LPNODE pNew = new NODE;

    pNew->NodeType = type;
    pNew->Left.pNode = pLeft;
    pNew->Right.pNode = pRight;
	pNew->Other1.pNode = pOther1;
	pNew->Other2.pNode = pOther2;
    pNew->fLeaf = FALSE;

    pNew->nLineNumber = g_nLineNumber;

    /*link it in to the global list*/
    pNew->next = g_pNodeList;
    g_pNodeList = pNew;

    return pNew;
}

LPNODE NewLeaf( NODETYPE type, int nValue )
{
	if( type == ntINVALID ) return NULL;

    LPNODE pNew = new NODE;

	pNew->NodeType = type;
	pNew->Left.nValue = nValue;
	pNew->Right.nValue = 0;
	pNew->Other1.nValue = 0;
	pNew->Other2.nValue = 0;
	pNew->fLeaf = TRUE;
    pNew->fString = FALSE;

    pNew->nLineNumber = g_nLineNumber;

    /*link it in to the global list*/
    pNew->next = g_pNodeList;
    g_pNodeList = pNew;

	return pNew;
}

LPNODE NewLeaf( NODETYPE type, double dValue )
{
	if( type == ntINVALID ) return NULL;

    LPNODE pNew = new NODE;

	pNew->NodeType = type;
	pNew->Left.dValue = dValue;
	pNew->Right.dValue = 0;
	pNew->Other1.dValue = 0;
	pNew->Other2.dValue = 0;
	pNew->fLeaf = TRUE;
    pNew->fString = FALSE;

    pNew->nLineNumber = g_nLineNumber;

    /*link it in to the global list*/
    pNew->next = g_pNodeList;
    g_pNodeList = pNew;

	return pNew;
}


LPNODE NewLeaf( NODETYPE type, char* pszString = NULL )
{
	if( type == ntINVALID ) return NULL;

    LPNODE pNew = new NODE;

	pNew->NodeType = type;
	pNew->Left.strToken = pszString == NULL ? NULL : strdup(pszString);
	pNew->Right.strToken = NULL;
	pNew->Other1.strToken = NULL;
	pNew->Other2.strToken = NULL;
	pNew->fLeaf = TRUE;
    pNew->fString = TRUE;

    pNew->nLineNumber = g_nLineNumber;

    /*link it in to the global list*/
    pNew->next = g_pNodeList;
    g_pNodeList = pNew;


	return pNew;
}



LPARGUMENT NewFormalArgument( LPARGUMENT pArguments, LPNODE pNodeName )
{
	if( pArguments )
	{
		LPARGUMENT pList = pArguments;
		while( pList )
		{
			if( strcmp( pList->pNode->Left.strToken, pNodeName->Left.strToken ) == 0 )
            {
                yyerror( "Argument already exists!" );
				return NULL;
            }
			pList = pList->next;
		}
	}

	/*allocate the argument structure and fill it in*/
	LPARGUMENT pNew = new ARGUMENT;
	pNew->pNode = pNodeName;
	pNew->next = NULL;

	/*link it to the list (at the end - the arguments must be in order)*/
	if( pArguments == NULL )
		/*this is the first item*/
		return pNew;
	else
	{
		/*a list exists already - add this one at the end*/
		LPARGUMENT pEndOfList = pArguments;
		while( pEndOfList->next != NULL ) pEndOfList = pEndOfList->next;
		
		pEndOfList->next = pNew;
		return pArguments;
	}
}

LPNODE NewFunction( LPNODE pFunctionBodyNode, LPARGUMENT pArguments, LPNODE pFunctionNameLeaf )
{
	if( pArguments )
	{
		LPFUNCTION pList = g_pFunctionList;
		while( pList )
		{
			if( strcmp( pList->pFunctionNameLeaf->Left.strToken, pFunctionNameLeaf->Left.strToken ) == 0 )
            {
				yyerror( "Function already exists" );
				return NULL;
            }
			pList = pList->next;
		}
	}

	/*allocate the function stucture and fill it in*/
	LPFUNCTION pNew = new FUNCTION;
	pNew->pFunctionNameLeaf = pFunctionNameLeaf;
	pNew->pFunctionBodyNode = pFunctionBodyNode;
	pNew->pArguments = pArguments;
	
	/*link it to the list (at the start - the function order does not matter)*/
	pNew->next = g_pFunctionList;
	g_pFunctionList = pNew;

	return pFunctionNameLeaf;
}

LPARGUMENT NewActualArgument( LPARGUMENT pArguments, LPNODE pNodeValue )
{
	/*allocate the argument structure and fill it in*/
	LPARGUMENT pNew = new ARGUMENT;
	pNew->pNode = pNodeValue;
	pNew->next = NULL;

	/*link it to the list (at the end - the arguments must be in order)*/
	if( pArguments == NULL )
		/*this is the first item*/
		return pNew;
	else
	{
		/*a list exists already - add this one at the end*/
		LPARGUMENT pEndOfList = pArguments;
		while( pEndOfList->next != NULL ) pEndOfList = pEndOfList->next;
		
		pEndOfList->next = pNew;
		return pArguments;
	}
}



int DecodeOctal( char* pszOctal )
{
	/*it's in the C form of 0NNNNN*/
	int n = 0;
	sscanf( pszOctal+1, "%o", &n );

	return n;
}

double DecodeFloat( char* pszFloat )
{
	return atof( pszFloat );
}

int DecodeHex( char* pszHex )
{
	/*it's in the C form of 0xNNNNN*/
	int n = 0;
	sscanf( pszHex+2, "%x", &n );

	return n;
}

int DecodeDecimal( char* pszDecimal )
{
	return atoi( pszDecimal );
}

/* tree destruction function **************************************************************/

void DeleteArguments( LPARGUMENT pArg )
{
	LPARGUMENT pTemp;
	while( pArg )
	{
		pTemp = pArg;
		pArg = pArg->next;
		delete pTemp;
	}
}

LPNODE DeleteNode( LPNODE pNode )
{
    if( pNode == NULL ) return NULL;
    LPNODE pReturn = pNode->next;

    /*delete this node/leaf*/
    if( pNode->fLeaf == TRUE )
    {
        if( pNode->fString == TRUE )
        {
            /*delete the strings*/
            delete[] pNode->Left.strToken;
            delete[] pNode->Right.strToken;
            delete[] pNode->Other1.strToken;
            delete[] pNode->Other2.strToken;
        }
    }
    else
    {
        /*delete child nodes*/
        switch( pNode->NodeType )
        {
            case ntOBJECTMETHODCALL: 
                DeleteArguments( (LPARGUMENT)pNode->Other1.pNode ); 
                break;
            case ntFUNCTIONCALL:
            case ntCONSTRUCTOR:      
                DeleteArguments( (LPARGUMENT)pNode->Right.pNode ); 
                break;
        }
    }

    /*delete this node*/
    delete pNode;

    return pReturn;
}

void FreeParseInformation( LPNODE pNodeList, LPFUNCTION pFunctionList )
{
    /*delete the parse tree*/
    while( pNodeList ) pNodeList = DeleteNode( pNodeList );

    LPFUNCTION pTemp;
    while( pFunctionList )
    {
		DeleteArguments( pFunctionList->pArguments );
        
		pTemp = pFunctionList;
        pFunctionList = pFunctionList->next;
        delete pTemp;
    }
}


#line 305 "javascript.y"
typedef union {
	LPNODE pNode;
	LPARGUMENT pFormalArgs;
	LPARGUMENT pActualArgs;
    NODETYPE NodeType;
} YYSTYPE;
#line 333 "javascript.cpp"
#define FUNCTION 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define RETURN 262
#define BREAK 263
#define CONTINUE 264
#define NEW 265
#define STRING 266
#define VAR 267
#define OCTAL_DIGIT 268
#define FLOAT_DIGIT 269
#define HEX_DIGIT 270
#define DECIMAL_DIGIT 271
#define IDENTIFIER 272
#define ASSIGN 273
#define ADDASSIGN 274
#define SUBASSIGN 275
#define MULASSIGN 276
#define DIVASSIGN 277
#define MODASSIGN 278
#define ANDASSIGN 279
#define ORASSIGN 280
#define XORASSIGN 281
#define SHLASSIGN 282
#define SSHRASSIGN 283
#define LOGOR 284
#define LOGAND 285
#define BITOR 286
#define BITAND 287
#define BITXOR 288
#define ISEQUALS 289
#define ISNOTEQUALS 290
#define ISLTHAN 291
#define ISGTHAN 292
#define ISLTHANEQUALS 293
#define ISGTHANEQUALS 294
#define SHL 295
#define SSHR 296
#define ADD 297
#define MUL 298
#define DIV 299
#define MOD 300
#define INCREMENT 301
#define DECREMENT 302
#define YYERRCODE 256
const short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    2,   44,   44,   44,    3,
    3,    3,    4,    4,    5,    5,    6,    6,    7,    7,
    7,    8,    8,    9,    9,    9,    9,    9,    9,    9,
    9,   10,   10,   11,   12,   13,   13,   14,   15,   17,
   17,   16,   16,   18,   18,   43,   43,   43,   43,   43,
   43,   43,   43,   43,   43,   43,   19,   19,   20,   20,
   21,   21,   22,   22,   23,   23,   24,   24,   25,   25,
   25,   26,   26,   26,   26,   26,   27,   27,   27,   28,
   28,   28,   29,   29,   29,   29,   30,   30,   30,   30,
   30,   30,   31,   31,   31,   31,   32,   32,   32,   32,
   32,   32,   32,   32,   34,   35,   35,   33,   36,   45,
   45,   45,   37,   38,   38,   38,   39,   40,   41,   41,
   41,   41,   42,
};
const short yylen[] = {                                         2,
    2,    1,    2,    2,    0,    6,    3,    1,    0,    4,
    3,    1,    4,    3,    3,    1,    3,    1,    3,    2,
    1,    2,    1,    1,    1,    1,    2,    2,    2,    2,
    1,    5,    7,    9,    5,    2,    1,    1,    1,    1,
    0,    1,    3,    1,    3,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    5,    1,    3,
    1,    3,    1,    3,    1,    3,    1,    3,    1,    3,
    3,    1,    3,    3,    3,    3,    1,    3,    3,    1,
    3,    3,    1,    3,    3,    3,    1,    2,    2,    2,
    2,    2,    1,    2,    2,    1,    3,    1,    1,    1,
    1,    1,    2,    1,    4,    4,    1,    6,    3,    3,
    1,    0,    4,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
const short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,   38,   39,    0,  118,    0,
  119,  120,  121,  122,  123,    0,    0,    0,   31,    0,
    0,    0,    0,    2,    0,    0,    0,   24,   25,   26,
    0,    0,    0,    0,   42,   44,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   83,    0,   93,   98,
  100,    0,    0,    0,  116,  101,  102,    0,    0,    0,
    0,    0,    0,  103,    0,    0,    0,    0,   88,   99,
  104,    0,   89,    0,   90,   91,   92,    3,    1,    4,
   27,   28,   29,    0,   30,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   94,   95,    0,    0,   46,   47,
   48,   49,   50,   51,   52,   53,   54,   55,   56,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   97,   43,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   84,   85,
   86,    0,    0,   45,  111,    0,    8,    0,    0,    0,
    0,    0,   13,   15,   17,    0,    0,  113,  105,    0,
    0,    0,    0,    0,   21,    0,   35,  106,   58,    0,
  110,    0,    6,   12,    7,   20,    0,    0,    0,    0,
  108,    0,   19,   22,   33,    0,   11,    0,    0,   10,
   34,
};
const short yydgoto[] = {                                      23,
   24,   25,  183,   26,   66,   67,  174,  187,  175,   28,
   29,   30,   31,   32,   33,   34,  125,   35,   36,   37,
   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
   48,   49,   50,   51,   64,   52,   53,   54,   55,   56,
   57,   58,  120,  158,  156,
};
const short yysindex[] = {                                     63,
 -243,    9,   11,   29,  251,    0,    0, -243,    0, -243,
    0,    0,    0,    0,    0,  251,  251,  251,    0,  251,
  251,  251,    0,    0,  124,  124,  124,    0,    0,    0,
   15,   25,   40,  -31,    0,    0,  -53, -197, -193, -178,
 -162, -253, -228, -167,  -43, -194,    0,  -46,    0,    0,
    0,    0,    0, -129,    0,    0,    0,   96,  127,  251,
  251,  251,  118,    0,  128,  111,  129, -102,    0,    0,
    0,   96,    0,   26,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  251,    0,  251,  251,  251,  251,  251,
  251,  251,  251,  251,  251,  251,  251,  251,  251,  251,
  251,  251,  251,  251,    0,    0, -243,  251,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  251,
  251, -243,   34,  118,  113,   38,  251,  -93, -243,  251,
    0,    0, -197,  -28, -193, -178, -162, -253, -228, -228,
 -167, -167, -167, -167,  -43,  -43, -194, -194,    0,    0,
    0,  136,  -32,    0,    0,   39,    0,   48,  169,  251,
  169,   54,    0,    0,    0,  251,  251,    0,    0,  251,
  184, -243,  338,  -82,    0,  123,    0,    0,    0,   71,
    0,   78,    0,    0,    0,    0,   59,  428,  169,  251,
    0,  358,    0,    0,    0,  145,    0,   62,  169,    0,
    0,
};
const short yyrindex[] = {                                    190,
    0,    0,    0,    0,  132,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  190,  190,  190,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -6,  167,  175,   14,
  -37, 1055,   27,  122,  979,  923,    0,  903,    0,    0,
    0,  466,  497,    0,    0,    0,    0,  590,    0,    0,
  137,    0,  140,    0,  520,    0,  141,  -19,    0,    0,
    0,  883,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   73,   80,    0,  -17,    0,    0,   73,    1,    0,    0,
    0,    0,  311,    0, 1317, 1093, 1068, 1062,   72,  134,
  433, 1018, 1029, 1042,  992, 1005,  939,  959,    0,    0,
    0,  621,    0,    0,    0,    0,    0,    0,    0,  137,
    0,    0,    0,    0,    0,    0,   73,    0,    0,    0,
    0,    0,    0,   17,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   70,    0,  160,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
const short yygindex[] = {                                      0,
  107,    0,    0, -123,   76,    0,  -80, -165,    6,    0,
    0,    0,    0,    0,    0,  484, -151,  -73, -122,    0,
  120,  119,  121,  130,  131,   66,   44,   57,   60,   -2,
    0,    0,    0,    0,    0, 1348, 1465,    0,    0,    0,
    0, 1373,    0,    0, -124,
};
#define YYTABLESIZE 1631
const short yytable[] = {                                     107,
   14,  101,  162,   65,  163,   27,   65,  165,  176,   87,
  132,   84,   84,   69,   73,   84,   32,   75,   76,   77,
   65,   65,  194,   40,   18,   65,  198,   85,   15,  166,
   27,   27,   27,   14,   57,   92,   93,   57,  196,   18,
   14,   40,  180,  179,  108,   14,  154,  155,   60,   32,
   61,   57,   57,  155,   63,   65,   32,   63,  192,   14,
  168,   32,   94,   95,   96,   97,  131,   69,   62,   84,
   69,   63,   63,   81,  159,   32,   63,   84,  161,  169,
  177,   84,  170,   82,   69,   69,   57,   88,  171,   69,
  184,  172,   89,  155,  178,   21,  181,  170,   83,  149,
  150,  151,   18,  102,  103,  104,   63,   20,  195,   90,
   21,  191,   70,  112,  170,   70,  112,   18,  201,   69,
    9,   19,   20,    9,   91,   14,   14,   98,   99,   70,
   70,   78,   79,   80,   70,  121,   19,  141,  142,  143,
  144,   32,   32,  109,  110,  111,  112,  113,  114,  115,
  116,  117,  118,  119,  145,  146,   21,  139,  140,  147,
  148,   84,   72,   18,   70,   72,  122,  127,   20,  128,
  130,  160,  129,   10,   71,  167,  189,   71,  188,   72,
   72,  190,   19,  193,   72,  199,  200,  188,   22,    5,
   37,   71,   71,  188,   23,   41,   71,  188,   36,   16,
   41,   21,  186,   22,  164,  133,  135,   59,   18,  136,
   59,    0,    0,   20,   72,   61,   21,    0,   61,  137,
    0,  138,    0,   18,   59,   59,   71,   19,   20,   59,
   86,    0,   61,   61,    0,    0,    0,   61,    0,    0,
    0,    0,   19,    0,    0,    0,   65,   65,   65,   22,
   65,    0,    0,  100,  105,  106,    0,   14,   14,   59,
   14,   14,   14,   14,   14,   14,   14,   61,   14,   14,
   14,   14,   14,   32,   32,    0,   32,   32,   32,   32,
   32,   32,   32,   21,   32,   32,   32,   32,   32,    0,
   18,  173,    0,    0,   22,   20,    0,   63,   63,   63,
    0,   14,   14,    0,    0,    0,  182,    0,    0,   22,
   69,   69,   69,   69,   69,   69,   69,   32,   32,    1,
    2,    0,    3,    4,    5,    6,    7,    8,    9,   10,
   11,   12,   13,   14,   15,    2,    0,    3,    4,    5,
    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
    0,   60,    0,    0,   60,   70,   70,   70,   70,   70,
   70,   70,    0,   16,   17,    0,    0,    0,   60,   60,
   21,    0,    0,   60,    0,    0,   22,   18,   16,   17,
    1,    2,   20,    3,    4,    5,    6,    7,    8,    9,
   21,   11,   12,   13,   14,   15,   19,   18,    0,    0,
    0,    0,   20,   60,    0,   72,   72,   72,   72,   72,
   72,   72,   72,   72,   72,   72,   19,   71,   71,   71,
   71,   71,   71,   71,   16,   17,    2,    0,    3,    4,
    5,    6,    7,    8,    9,    0,   11,   12,   13,   14,
   15,    2,    0,    3,    4,    5,    6,    7,    8,    9,
   59,   11,   12,   13,   14,   15,    0,    0,   61,   61,
   21,    0,  186,   22,    0,    0,    0,   18,    0,   16,
   17,    0,   20,   73,    0,    0,   73,    0,    0,    0,
    0,    0,  197,   22,   16,   17,   19,    0,   63,    0,
   73,   73,    0,    0,    0,   73,    0,    0,    0,    0,
    0,   74,    0,    0,    0,    0,   99,    0,    0,   99,
   99,   99,    0,    0,    0,    8,    9,    0,   11,   12,
   13,   14,   15,   99,   99,   73,    0,    0,   99,    0,
    0,    0,    0,    0,    0,    0,    0,  104,    0,    0,
  104,  104,  104,  123,  124,  126,    0,    0,    0,    0,
    0,   16,   17,   22,  104,  104,   99,    0,   99,  104,
  107,    0,    0,  107,  107,  107,    0,    0,    0,    0,
  134,    0,    0,    0,    0,    0,    0,  107,  107,    0,
    0,    0,  107,    0,    0,    0,    0,  104,    0,  104,
    0,  153,    0,    0,   60,    2,    0,    3,    4,    5,
    6,    7,    8,    9,    0,   11,   12,   13,   14,   15,
  107,    0,  107,    0,    0,    2,    0,    3,    4,    5,
    6,    7,    8,    9,    0,   11,   12,   13,   14,   15,
   96,    0,    0,   96,   96,   96,    0,    0,   16,   17,
    0,    0,    0,  124,    0,    0,    0,   96,   96,    0,
    0,    0,   96,    0,    0,    0,    0,    0,   16,   17,
    0,  109,    0,    0,  109,  109,  109,    0,    0,    0,
    0,    0,    0,  124,    0,    0,    0,    0,  109,  109,
   96,    0,   96,  109,    0,    2,    0,    3,    4,    5,
    6,    7,    8,    9,    0,   11,   12,   13,   14,   15,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  109,    0,  109,    0,    0,   73,   73,   73,   73,
   73,   73,   73,   73,   73,   73,   73,    0,   16,   17,
    0,    0,    0,    0,    0,    0,    0,    0,  114,  114,
  114,  114,  114,  114,  114,  114,  114,  114,  114,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,    0,  115,
  115,  115,  115,  115,  115,  115,  115,  115,  115,  115,
  104,  104,  104,  104,  104,  104,  104,  104,  104,  104,
  104,  104,  104,  104,  104,  104,  104,  104,  104,    0,
    0,    0,    0,  107,  107,  107,  107,  107,  107,  107,
  107,  107,  107,  107,  107,  107,  107,  107,  107,  107,
  107,  107,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  117,  117,  117,  117,  117,  117,  117,  117,
  117,  117,  117,   96,   96,   96,   96,   96,   96,   96,
   96,   96,   96,   96,   96,   96,   96,   96,   96,   96,
   96,   96,    0,  109,  109,  109,  109,  109,  109,  109,
  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,
  109,  109,  109,  109,  109,  109,  109,  109,  109,  109,
  109,  109,  109,   96,    0,    0,   96,   96,   96,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   96,   96,    0,   87,    0,   96,   87,   87,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   87,   87,    0,   80,    0,   87,   80,   80,    0,    0,
    0,    0,    0,   96,    0,   96,    0,    0,    0,   81,
   80,   80,   81,   81,    0,   80,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   87,   81,   81,    0,   82,
    0,   81,   82,   82,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   80,   82,   82,    0,   77,
    0,   82,   77,    0,    0,    0,    0,    0,    0,    0,
    0,   81,   78,    0,    0,   78,   77,   77,    0,    0,
    0,   77,    0,    0,    0,   79,    0,    0,   79,   78,
   78,   82,    0,    0,   78,    0,    0,    0,   74,    0,
    0,   74,   79,   79,    0,    0,    0,   79,    0,   75,
    0,   77,   75,    0,    0,   74,   74,    0,    0,    0,
   74,    0,   76,    0,   78,   76,   75,   75,    0,    0,
    0,   75,    0,    0,    0,   67,    0,   79,   67,   76,
   76,    0,   68,    0,   76,   68,    0,    0,   66,    0,
   74,   66,   67,   67,    0,    0,    0,   67,    0,   68,
   68,   75,    0,    0,   68,   66,   66,    0,    0,    0,
   66,    0,    0,   64,   76,    0,   64,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   67,    0,    0,
   64,   64,    0,    0,   68,   64,    0,    0,    0,    0,
   66,    0,    0,    0,    0,    0,   96,   96,   96,   96,
   96,   96,   96,   96,   96,   96,   96,   96,   96,   96,
   96,   96,   96,   96,   96,   64,   87,   87,   87,   87,
   87,   87,   87,   87,   87,   87,   87,   87,   87,   87,
   87,   87,   87,    0,    0,    0,   80,   80,   80,   80,
   80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
    0,    0,   81,   81,   81,   81,   81,   81,   81,   81,
   81,   81,   81,   81,   81,   81,    0,    0,    0,    0,
    0,    0,   82,   82,   82,   82,   82,   82,   82,   82,
   82,   82,   82,   82,   82,   82,    0,    0,    0,    0,
    0,    0,   77,   77,   77,   77,   77,   77,   77,   77,
   77,   77,   77,   77,   77,   78,   78,   78,   78,   78,
   78,   78,   78,   78,   78,   78,   78,   78,   79,   79,
   79,   79,   79,   79,   79,   79,   79,   79,   79,   79,
   79,   74,   74,   74,   74,   74,   74,   74,   74,   74,
   74,   74,   75,   75,   75,   75,   75,   75,   75,   75,
   75,   75,   75,    0,    0,   76,   76,   76,   76,   76,
   76,   76,   76,   76,   76,   76,    0,    0,   67,   67,
   67,   67,   67,    0,    0,   68,   68,   68,   68,   68,
    0,   66,   66,   66,    0,   66,    0,   62,    0,    0,
   62,    0,    0,   70,   70,    0,    0,   70,   70,   70,
    0,    0,    0,   59,   62,   62,   64,   64,   64,   62,
   65,    0,   68,    0,    0,    0,    0,    0,   72,   72,
    0,    0,   72,   72,   72,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   62,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   70,    0,   70,   70,   70,   70,   70,
   70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
   70,   70,    0,    0,    0,    0,    0,    0,   72,    0,
   72,   72,   72,   72,   72,   72,   72,   72,   72,   72,
   72,   72,   72,   72,   72,   72,   72,   70,    0,  152,
   71,   71,    0,    0,   71,   71,   71,    0,    0,    0,
    0,    0,    0,    0,  157,    0,    0,    0,    0,    0,
    0,   68,   72,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   70,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   72,    0,
    0,    0,    0,    0,  185,    0,    0,    0,    0,    0,
   71,    0,   71,   71,   71,   71,   71,   71,   71,   71,
   71,   71,   71,   71,   71,   71,   71,   71,   71,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   71,    0,    0,    0,    0,    0,
   62,   62,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   71,
};
const short yycheck[] = {                                      46,
    0,   45,  127,   41,  128,    0,   44,  130,  160,   63,
   84,   44,   44,   16,   17,   44,    0,   20,   21,   22,
   58,   59,  188,   41,   44,   63,  192,   59,  272,   58,
   25,   26,   27,   33,   41,  289,  290,   44,  190,   59,
   40,   59,  167,  166,   91,   45,  120,  121,   40,   33,
   40,   58,   59,  127,   41,   93,   40,   44,  182,   59,
   93,   45,  291,  292,  293,  294,   41,   41,   40,   44,
   44,   58,   59,   59,   41,   59,   63,   44,   41,   41,
  161,   44,   44,   59,   58,   59,   93,  285,   41,   63,
  171,   44,  286,  167,   41,   33,  170,   44,   59,  102,
  103,  104,   40,  298,  299,  300,   93,   45,  189,  288,
   33,   41,   41,   41,   44,   44,   44,   40,  199,   93,
   41,   59,   45,   44,  287,  125,  126,  295,  296,   58,
   59,   25,   26,   27,   63,   40,   59,   94,   95,   96,
   97,  125,  126,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,  283,   98,   99,   33,   92,   93,  100,
  101,   44,   41,   40,   93,   44,   40,   40,   45,   59,
  273,   59,   44,  267,   41,   40,  259,   44,  173,   58,
   59,   59,   59,  125,   63,   41,  125,  182,  126,    0,
   59,   58,   59,  188,  125,   59,   63,  192,   59,   59,
   41,   33,  125,  126,  129,   86,   88,   41,   40,   89,
   44,   -1,   -1,   45,   93,   41,   33,   -1,   44,   90,
   -1,   91,   -1,   40,   58,   59,   93,   59,   45,   63,
  284,   -1,   58,   59,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,  284,  285,  286,  126,
  288,   -1,   -1,  297,  301,  302,   -1,  257,  258,   93,
  260,  261,  262,  263,  264,  265,  266,   93,  268,  269,
  270,  271,  272,  257,  258,   -1,  260,  261,  262,  263,
  264,  265,  266,   33,  268,  269,  270,  271,  272,   -1,
   40,  123,   -1,   -1,  126,   45,   -1,  284,  285,  286,
   -1,  301,  302,   -1,   -1,   -1,  123,   -1,   -1,  126,
  284,  285,  286,  287,  288,  289,  290,  301,  302,  257,
  258,   -1,  260,  261,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  258,   -1,  260,  261,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   -1,   41,   -1,   -1,   44,  284,  285,  286,  287,  288,
  289,  290,   -1,  301,  302,   -1,   -1,   -1,   58,   59,
   33,   -1,   -1,   63,   -1,   -1,  126,   40,  301,  302,
  257,  258,   45,  260,  261,  262,  263,  264,  265,  266,
   33,  268,  269,  270,  271,  272,   59,   40,   -1,   -1,
   -1,   -1,   45,   93,   -1,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,   59,  284,  285,  286,
  287,  288,  289,  290,  301,  302,  258,   -1,  260,  261,
  262,  263,  264,  265,  266,   -1,  268,  269,  270,  271,
  272,  258,   -1,  260,  261,  262,  263,  264,  265,  266,
  284,  268,  269,  270,  271,  272,   -1,   -1,  284,  285,
   33,   -1,  125,  126,   -1,   -1,   -1,   40,   -1,  301,
  302,   -1,   45,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,  125,  126,  301,  302,   59,   -1,    5,   -1,
   58,   59,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   18,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   45,   46,   -1,   -1,   -1,  265,  266,   -1,  268,  269,
  270,  271,  272,   58,   59,   93,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   45,   46,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  301,  302,  126,   58,   59,   91,   -1,   93,   63,
   41,   -1,   -1,   44,   45,   46,   -1,   -1,   -1,   -1,
   87,   -1,   -1,   -1,   -1,   -1,   -1,   58,   59,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   91,   -1,   93,
   -1,  108,   -1,   -1,  284,  258,   -1,  260,  261,  262,
  263,  264,  265,  266,   -1,  268,  269,  270,  271,  272,
   91,   -1,   93,   -1,   -1,  258,   -1,  260,  261,  262,
  263,  264,  265,  266,   -1,  268,  269,  270,  271,  272,
   41,   -1,   -1,   44,   45,   46,   -1,   -1,  301,  302,
   -1,   -1,   -1,  160,   -1,   -1,   -1,   58,   59,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  301,  302,
   -1,   41,   -1,   -1,   44,   45,   46,   -1,   -1,   -1,
   -1,   -1,   -1,  190,   -1,   -1,   -1,   -1,   58,   59,
   91,   -1,   93,   63,   -1,  258,   -1,  260,  261,  262,
  263,  264,  265,  266,   -1,  268,  269,  270,  271,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   93,   -1,   -1,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,   -1,  301,  302,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,  274,
  275,  276,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,  302,   -1,  273,
  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,
  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,  302,   -1,
   -1,   -1,   -1,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  302,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,  274,  275,  276,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  302,   -1,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,  283,  284,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  302,   41,   -1,   -1,   44,   45,   46,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   59,   -1,   41,   -1,   63,   44,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   59,   -1,   41,   -1,   63,   44,   45,   -1,   -1,
   -1,   -1,   -1,   91,   -1,   93,   -1,   -1,   -1,   41,
   58,   59,   44,   45,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   93,   58,   59,   -1,   41,
   -1,   63,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   93,   58,   59,   -1,   41,
   -1,   63,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   93,   41,   -1,   -1,   44,   58,   59,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   41,   -1,   -1,   44,   58,
   59,   93,   -1,   -1,   63,   -1,   -1,   -1,   41,   -1,
   -1,   44,   58,   59,   -1,   -1,   -1,   63,   -1,   41,
   -1,   93,   44,   -1,   -1,   58,   59,   -1,   -1,   -1,
   63,   -1,   41,   -1,   93,   44,   58,   59,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   41,   -1,   93,   44,   58,
   59,   -1,   41,   -1,   63,   44,   -1,   -1,   41,   -1,
   93,   44,   58,   59,   -1,   -1,   -1,   63,   -1,   58,
   59,   93,   -1,   -1,   63,   58,   59,   -1,   -1,   -1,
   63,   -1,   -1,   41,   93,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   93,   -1,   -1,
   58,   59,   -1,   -1,   93,   63,   -1,   -1,   -1,   -1,
   93,   -1,   -1,   -1,   -1,   -1,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,  301,  302,   93,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,   -1,   -1,   -1,  284,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  295,  296,  297,
   -1,   -1,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  295,  296,  297,   -1,   -1,   -1,   -1,
   -1,   -1,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  295,  296,  297,   -1,   -1,   -1,   -1,
   -1,   -1,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,  295,  296,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,  295,  296,  284,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
  296,  284,  285,  286,  287,  288,  289,  290,  291,  292,
  293,  294,  284,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,   -1,   -1,  284,  285,  286,  287,  288,
  289,  290,  291,  292,  293,  294,   -1,   -1,  284,  285,
  286,  287,  288,   -1,   -1,  284,  285,  286,  287,  288,
   -1,  284,  285,  286,   -1,  288,   -1,   41,   -1,   -1,
   44,   -1,   -1,   16,   17,   -1,   -1,   20,   21,   22,
   -1,   -1,   -1,    1,   58,   59,  284,  285,  286,   63,
    8,   -1,   10,   -1,   -1,   -1,   -1,   -1,   16,   17,
   -1,   -1,   20,   21,   22,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   93,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   86,   -1,   88,   89,   90,   91,   92,
   93,   94,   95,   96,   97,   98,   99,  100,  101,  102,
  103,  104,   -1,   -1,   -1,   -1,   -1,   -1,   86,   -1,
   88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
   98,   99,  100,  101,  102,  103,  104,  130,   -1,  107,
   16,   17,   -1,   -1,   20,   21,   22,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  122,   -1,   -1,   -1,   -1,   -1,
   -1,  129,  130,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  166,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  166,   -1,
   -1,   -1,   -1,   -1,  172,   -1,   -1,   -1,   -1,   -1,
   86,   -1,   88,   89,   90,   91,   92,   93,   94,   95,
   96,   97,   98,   99,  100,  101,  102,  103,  104,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  130,   -1,   -1,   -1,   -1,   -1,
  284,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  166,
};
#define YYFINAL 23
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 302
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,0,0,0,"'('","')'",0,0,"','","'-'","'.'",0,0,0,0,0,0,0,0,0,0,0,"':'",
"';'",0,0,0,"'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,
"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",
"'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,"FUNCTION","IF","ELSE","FOR","WHILE","RETURN","BREAK",
"CONTINUE","NEW","STRING","VAR","OCTAL_DIGIT","FLOAT_DIGIT","HEX_DIGIT",
"DECIMAL_DIGIT","IDENTIFIER","ASSIGN","ADDASSIGN","SUBASSIGN","MULASSIGN",
"DIVASSIGN","MODASSIGN","ANDASSIGN","ORASSIGN","XORASSIGN","SHLASSIGN",
"SSHRASSIGN","LOGOR","LOGAND","BITOR","BITAND","BITXOR","ISEQUALS",
"ISNOTEQUALS","ISLTHAN","ISGTHAN","ISLTHANEQUALS","ISGTHANEQUALS","SHL","SSHR",
"ADD","MUL","DIV","MOD","INCREMENT","DECREMENT",
};
const char * const yyrule[] = {
"$accept : script",
"script : vars stmtsfuncs",
"script : stmtsfuncs",
"stmtsfuncs : function stmtsfuncs",
"stmtsfuncs : statement stmtsfuncs",
"stmtsfuncs :",
"function : FUNCTION identifier '(' args ')' functionBody",
"args : args ',' identifier",
"args : identifier",
"args :",
"functionBody : '{' vars statements '}'",
"functionBody : '{' vars '}'",
"functionBody : statementBlock",
"vars : VAR varDeclarations ';' vars",
"vars : VAR varDeclarations ';'",
"varDeclarations : varDeclaration ',' varDeclarations",
"varDeclarations : varDeclaration",
"varDeclaration : identifier ASSIGN conditional_expression",
"varDeclaration : identifier",
"statementBlock : '{' statements '}'",
"statementBlock : '{' '}'",
"statementBlock : statement",
"statements : statement statements",
"statements : statement",
"statement : ifStatement",
"statement : forStatement",
"statement : whileStatement",
"statement : returnStatement ';'",
"statement : breakStatement ';'",
"statement : continueStatement ';'",
"statement : expression ';'",
"statement : ';'",
"ifStatement : IF '(' expression ')' statementBlock",
"ifStatement : IF '(' expression ')' statementBlock ELSE statementBlock",
"forStatement : FOR '(' optionalExpression ';' optionalExpression ';' optionalExpression ')' statementBlock",
"whileStatement : WHILE '(' expression ')' statementBlock",
"returnStatement : RETURN expression",
"returnStatement : RETURN",
"breakStatement : BREAK",
"continueStatement : CONTINUE",
"optionalExpression : expression",
"optionalExpression :",
"expression : assign_expression",
"expression : expression ',' assign_expression",
"assign_expression : conditional_expression",
"assign_expression : leftVariable assign_operator assign_expression",
"assign_operator : ASSIGN",
"assign_operator : ADDASSIGN",
"assign_operator : SUBASSIGN",
"assign_operator : MULASSIGN",
"assign_operator : DIVASSIGN",
"assign_operator : MODASSIGN",
"assign_operator : ANDASSIGN",
"assign_operator : ORASSIGN",
"assign_operator : XORASSIGN",
"assign_operator : SHLASSIGN",
"assign_operator : SSHRASSIGN",
"conditional_expression : or_expression",
"conditional_expression : or_expression '?' expression ':' conditional_expression",
"or_expression : and_expression",
"or_expression : or_expression LOGOR and_expression",
"and_expression : bitor_expression",
"and_expression : and_expression LOGAND bitor_expression",
"bitor_expression : xor_expression",
"bitor_expression : bitor_expression BITOR xor_expression",
"xor_expression : bitand_expression",
"xor_expression : xor_expression BITXOR bitand_expression",
"bitand_expression : equality_expression",
"bitand_expression : bitand_expression BITAND equality_expression",
"equality_expression : relational_expression",
"equality_expression : equality_expression ISEQUALS relational_expression",
"equality_expression : equality_expression ISNOTEQUALS relational_expression",
"relational_expression : shift_expression",
"relational_expression : relational_expression ISLTHAN shift_expression",
"relational_expression : relational_expression ISGTHAN shift_expression",
"relational_expression : relational_expression ISLTHANEQUALS shift_expression",
"relational_expression : relational_expression ISGTHANEQUALS shift_expression",
"shift_expression : add_expression",
"shift_expression : shift_expression SHL add_expression",
"shift_expression : shift_expression SSHR add_expression",
"add_expression : mult_expression",
"add_expression : add_expression ADD mult_expression",
"add_expression : add_expression '-' mult_expression",
"mult_expression : unary_expression",
"mult_expression : mult_expression MUL unary_expression",
"mult_expression : mult_expression DIV unary_expression",
"mult_expression : mult_expression MOD unary_expression",
"unary_expression : postfix_expression",
"unary_expression : INCREMENT unary_expression",
"unary_expression : DECREMENT unary_expression",
"unary_expression : '-' unary_expression",
"unary_expression : '!' unary_expression",
"unary_expression : '~' unary_expression",
"postfix_expression : primary_expression",
"postfix_expression : postfix_expression INCREMENT",
"postfix_expression : postfix_expression DECREMENT",
"postfix_expression : identifier",
"primary_expression : '(' expression ')'",
"primary_expression : objectMethodCall",
"primary_expression : objectMemberAccess",
"primary_expression : functionCall",
"primary_expression : string",
"primary_expression : number",
"primary_expression : NEW constructor",
"primary_expression : arrayDereference",
"functionCall : identifier '(' params ')'",
"constructor : identifier '(' params ')'",
"constructor : identifier",
"objectMethodCall : postfix_expression '.' identifier '(' params ')'",
"objectMemberAccess : postfix_expression '.' identifier",
"params : params ',' assign_expression",
"params : assign_expression",
"params :",
"arrayDereference : postfix_expression '[' expression ']'",
"leftVariable : objectMemberAccess",
"leftVariable : arrayDereference",
"leftVariable : variable",
"variable : identifier",
"string : STRING",
"number : OCTAL_DIGIT",
"number : FLOAT_DIGIT",
"number : HEX_DIGIT",
"number : DECIMAL_DIGIT",
"identifier : IDENTIFIER",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 634 "javascript.y"


/* scanner declarations - DOS and Windows (MFC) *******************************************/
#ifndef _WIN32
    #include "lex.yy.c"
    void yyerror( char *s ) { printf( "\nLine %d : %s\n%s\n\n", g_nLineNumber, s, yytext ); }
    main() { yyparse(); }
#else
    #include <io.h>
    #define isatty _isatty
    #include "lex.yy.c"
    extern HWND g_hWndOwner;
    void yyerror( char *s )
    {
        char* pszMessage = new char[strlen(s)+strlen(yytext)+256];
        sprintf( pszMessage, "Line %d : %s\n%s", g_nLineNumber, s, yytext );
        MessageBox( g_hWndOwner, pszMessage, "Script Error", MB_ICONEXCLAMATION|MB_TASKMODAL );
        delete[] pszMessage;
    }

    void ResetParser()
    {
    	yychar = -1;
    	yyerrflag = 0;
    	yynerrs = 0;
    
        YY_FLUSH_BUFFER;
    }

	void CleanupParser()
	{
		yy_delete_buffer( yy_current_buffer );
        free( yyss );
        free( yyvs );

		yy_current_buffer = 0;
		yyss = 0;
		yyvs = 0;
	}

#endif
#line 1042 "javascript.cpp"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    if ((newss = (short*)realloc(yyss, newsize * sizeof *newss)) == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    if ((newvs = (YYSTYPE*)realloc(yyvs, newsize * sizeof *newvs)) == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 330 "javascript.y"
{ g_pScriptNode = NewNode( ntSTATEMENT, yyvsp[-1].pNode, yyvsp[0].pNode ); }
break;
case 2:
#line 331 "javascript.y"
{ g_pScriptNode = NewNode( ntSTATEMENT, yyvsp[0].pNode, NULL ); }
break;
case 3:
#line 334 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 4:
#line 335 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[-1].pNode, yyvsp[0].pNode ); }
break;
case 5:
#line 336 "javascript.y"
{ yyval.pNode = NULL; }
break;
case 6:
#line 341 "javascript.y"
{ yyval.pNode = NewFunction( yyvsp[0].pNode, yyvsp[-2].pFormalArgs, yyvsp[-4].pNode ); }
break;
case 7:
#line 344 "javascript.y"
{ yyval.pFormalArgs = NewFormalArgument( yyvsp[-2].pFormalArgs, yyvsp[0].pNode ); }
break;
case 8:
#line 345 "javascript.y"
{ yyval.pFormalArgs = NewFormalArgument( NULL, yyvsp[0].pNode ); }
break;
case 9:
#line 346 "javascript.y"
{ yyval.pFormalArgs = NULL; }
break;
case 10:
#line 349 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[-2].pNode, yyvsp[-1].pNode ); }
break;
case 11:
#line 350 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 12:
#line 351 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 13:
#line 356 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 14:
#line 357 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 15:
#line 360 "javascript.y"
{ yyval.pNode = NewNode( ntVARIABLELIST, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 16:
#line 361 "javascript.y"
{ yyval.pNode = NewNode( ntVARIABLELIST, yyvsp[0].pNode, NULL ); }
break;
case 17:
#line 364 "javascript.y"
{ yyval.pNode = NewNode( ntVARIABLE, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 18:
#line 365 "javascript.y"
{ yyval.pNode = NewNode( ntVARIABLE, yyvsp[0].pNode, NULL ); }
break;
case 19:
#line 370 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 20:
#line 371 "javascript.y"
{ yyval.pNode = NULL; }
break;
case 21:
#line 372 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[0].pNode, NULL ); }
break;
case 22:
#line 375 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[-1].pNode, yyvsp[0].pNode ); }
break;
case 23:
#line 376 "javascript.y"
{ yyval.pNode = NewNode( ntSTATEMENT, yyvsp[0].pNode, NULL ); }
break;
case 24:
#line 380 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 25:
#line 381 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 26:
#line 382 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 27:
#line 383 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 28:
#line 384 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 29:
#line 385 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 30:
#line 386 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 31:
#line 387 "javascript.y"
{ yyval.pNode = NULL; }
break;
case 32:
#line 390 "javascript.y"
{ yyval.pNode = NewNode( ntIFSTATEMENT, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 33:
#line 391 "javascript.y"
{ yyval.pNode = NewNode( ntIFSTATEMENT, yyvsp[-4].pNode, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 34:
#line 395 "javascript.y"
{ yyval.pNode = NewNode( ntFORSTATEMENT, yyvsp[-6].pNode, yyvsp[-4].pNode, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 35:
#line 399 "javascript.y"
{ yyval.pNode = NewNode( ntWHILESTATEMENT, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 36:
#line 403 "javascript.y"
{ yyval.pNode = NewNode( ntRETURNSTATEMENT, yyvsp[0].pNode, NULL ); }
break;
case 37:
#line 404 "javascript.y"
{ yyval.pNode = NewNode( ntRETURNSTATEMENT, NULL, NULL ); }
break;
case 38:
#line 408 "javascript.y"
{ yyval.pNode = NewNode( ntBREAKSTATEMENT, NULL, NULL ); }
break;
case 39:
#line 412 "javascript.y"
{ yyval.pNode = NewNode( ntCONTINUESTATEMENT, NULL, NULL ); }
break;
case 40:
#line 418 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 41:
#line 419 "javascript.y"
{ yyval.pNode = NULL; }
break;
case 42:
#line 422 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 43:
#line 423 "javascript.y"
{ yyval.pNode = NewNode( ntEXPRESSION, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 44:
#line 426 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 45:
#line 427 "javascript.y"
{ yyval.pNode = NewNode( yyvsp[-1].NodeType, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 46:
#line 430 "javascript.y"
{ yyval.NodeType = ntASSIGN; }
break;
case 47:
#line 431 "javascript.y"
{ yyval.NodeType = ntADDASSIGN; }
break;
case 48:
#line 432 "javascript.y"
{ yyval.NodeType = ntSUBASSIGN; }
break;
case 49:
#line 433 "javascript.y"
{ yyval.NodeType = ntMULASSIGN; }
break;
case 50:
#line 434 "javascript.y"
{ yyval.NodeType = ntDIVASSIGN; }
break;
case 51:
#line 435 "javascript.y"
{ yyval.NodeType = ntMODASSIGN; }
break;
case 52:
#line 436 "javascript.y"
{ yyval.NodeType = ntANDASSIGN; }
break;
case 53:
#line 437 "javascript.y"
{ yyval.NodeType = ntORASSIGN; }
break;
case 54:
#line 438 "javascript.y"
{ yyval.NodeType = ntXORASSIGN; }
break;
case 55:
#line 439 "javascript.y"
{ yyval.NodeType = ntSHLASSIGN; }
break;
case 56:
#line 440 "javascript.y"
{ yyval.NodeType = ntSSHRASSIGN; }
break;
case 57:
#line 443 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 58:
#line 444 "javascript.y"
{ yyval.pNode = NewNode( ntQUESTIONMARKCOLON, yyvsp[-4].pNode, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 59:
#line 447 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 60:
#line 448 "javascript.y"
{ yyval.pNode = NewNode( ntLOGOR, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 61:
#line 451 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 62:
#line 452 "javascript.y"
{ yyval.pNode = NewNode( ntLOGAND, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 63:
#line 455 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 64:
#line 456 "javascript.y"
{ yyval.pNode = NewNode( ntBITOR, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 65:
#line 459 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 66:
#line 460 "javascript.y"
{ yyval.pNode = NewNode( ntBITXOR, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 67:
#line 463 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 68:
#line 464 "javascript.y"
{ yyval.pNode = NewNode( ntBITAND, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 69:
#line 467 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 70:
#line 468 "javascript.y"
{ yyval.pNode = NewNode( ntISEQUALS, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 71:
#line 469 "javascript.y"
{ yyval.pNode = NewNode( ntISNOTEQUALS, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 72:
#line 472 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 73:
#line 473 "javascript.y"
{ yyval.pNode = NewNode( ntISLTHAN, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 74:
#line 474 "javascript.y"
{ yyval.pNode = NewNode( ntISGTHAN, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 75:
#line 475 "javascript.y"
{ yyval.pNode = NewNode( ntISLTHANEQUALS, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 76:
#line 476 "javascript.y"
{ yyval.pNode = NewNode( ntISGTHANEQUALS, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 77:
#line 479 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 78:
#line 480 "javascript.y"
{ yyval.pNode = NewNode( ntSHL, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 79:
#line 481 "javascript.y"
{ yyval.pNode = NewNode( ntSSHR, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 80:
#line 484 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 81:
#line 485 "javascript.y"
{ yyval.pNode = NewNode( ntADD, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 82:
#line 486 "javascript.y"
{ yyval.pNode = NewNode( ntSUB, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 83:
#line 489 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 84:
#line 490 "javascript.y"
{ yyval.pNode = NewNode( ntMUL, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 85:
#line 491 "javascript.y"
{ yyval.pNode = NewNode( ntDIV, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 86:
#line 492 "javascript.y"
{ yyval.pNode = NewNode( ntMOD, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 87:
#line 495 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 88:
#line 496 "javascript.y"
{ yyval.pNode = NewNode( ntPREINCREMENT, yyvsp[0].pNode, NULL ); }
break;
case 89:
#line 497 "javascript.y"
{ yyval.pNode = NewNode( ntPREDECREMENT, yyvsp[0].pNode, NULL ); }
break;
case 90:
#line 498 "javascript.y"
{ yyval.pNode = NewNode( ntNEGATE, yyvsp[0].pNode, NULL ); }
break;
case 91:
#line 499 "javascript.y"
{ yyval.pNode = NewNode( ntLOGICALNOT, yyvsp[0].pNode, NULL ); }
break;
case 92:
#line 500 "javascript.y"
{ yyval.pNode = NewNode( ntCOMPLIMENT, yyvsp[0].pNode, NULL ); }
break;
case 93:
#line 504 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 94:
#line 505 "javascript.y"
{ yyval.pNode = NewNode( ntPOSTINCREMENT, yyvsp[-1].pNode, NULL ); }
break;
case 95:
#line 506 "javascript.y"
{ yyval.pNode = NewNode( ntPOSTDECREMENT, yyvsp[-1].pNode, NULL ); }
break;
case 96:
#line 507 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 97:
#line 510 "javascript.y"
{ yyval.pNode = yyvsp[-1].pNode; }
break;
case 98:
#line 511 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 99:
#line 512 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 100:
#line 513 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 101:
#line 514 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 102:
#line 515 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 103:
#line 516 "javascript.y"
{ yyval.pNode = NewNode( ntNEW, yyvsp[0].pNode, NULL ); }
break;
case 104:
#line 517 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 105:
#line 522 "javascript.y"
{ yyval.pNode = NewNode( ntFUNCTIONCALL, yyvsp[-3].pNode, (LPNODE)yyvsp[-1].pActualArgs ); }
break;
case 106:
#line 526 "javascript.y"
{ yyval.pNode = NewNode( ntCONSTRUCTOR, yyvsp[-3].pNode, (LPNODE)yyvsp[-1].pActualArgs ); }
break;
case 107:
#line 527 "javascript.y"
{ yyval.pNode = NewNode( ntCONSTRUCTOR, yyvsp[0].pNode, NULL ); }
break;
case 108:
#line 531 "javascript.y"
{ yyval.pNode = NewNode( ntOBJECTMETHODCALL, yyvsp[-5].pNode, yyvsp[-3].pNode, (LPNODE)yyvsp[-1].pActualArgs ); }
break;
case 109:
#line 535 "javascript.y"
{ yyval.pNode = NewNode( ntOBJECTMEMBERACCESS, yyvsp[-2].pNode, yyvsp[0].pNode ); }
break;
case 110:
#line 539 "javascript.y"
{ yyval.pActualArgs = NewActualArgument( yyvsp[-2].pActualArgs, yyvsp[0].pNode ); }
break;
case 111:
#line 540 "javascript.y"
{ yyval.pActualArgs = NewActualArgument( NULL, yyvsp[0].pNode ); }
break;
case 112:
#line 541 "javascript.y"
{ yyval.pActualArgs = NULL; }
break;
case 113:
#line 547 "javascript.y"
{ yyval.pNode = NewNode( ntARRAYDEREFERENCE, yyvsp[-3].pNode, yyvsp[-1].pNode ); }
break;
case 114:
#line 551 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 115:
#line 552 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 116:
#line 553 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 117:
#line 557 "javascript.y"
{ yyval.pNode = yyvsp[0].pNode; }
break;
case 118:
#line 562 "javascript.y"
{ 
						  /*strip the quotes from the string*/
						  char* pszCloseQuote = &yytext[strlen(yytext)-1];
						  *pszCloseQuote = '\0';

                          /*now search & replace escape codes*/
                          char* pszScan = &yytext[1], *pszWrite = yytext;
                          while( *pszScan != '\0' )
                          {
                              if( *pszScan == '\\' )
								  switch( pszScan[1] )
								  {
									  case 'a':  *pszWrite = '\a'; pszWrite++; pszScan+=2; break;
									  case 'b':  *pszWrite = '\b'; pszWrite++; pszScan+=2; break;
									  case 'f':  *pszWrite = '\f'; pszWrite++; pszScan+=2; break;
									  case 'n':  *pszWrite = '\n'; pszWrite++; pszScan+=2; break;
									  case 'r':  *pszWrite = '\r'; pszWrite++; pszScan+=2; break;
									  case 't':  *pszWrite = '\t'; pszWrite++; pszScan+=2; break;
									  case 'v':  *pszWrite = '\v'; pszWrite++; pszScan+=2; break;
									  case '?':  *pszWrite = '\?'; pszWrite++; pszScan+=2; break;
									  case '\'': *pszWrite = '\''; pszWrite++; pszScan+=2; break;
									  case '\"': *pszWrite = '\"'; pszWrite++; pszScan+=2; break;
									  case '\\': *pszWrite = '\\'; pszWrite++; pszScan+=2; break;
									  case '0':  *pszWrite = '\0'; pszWrite++; pszScan+=2; break;
									  default:
									  {
										  char szBuffer[128];
										  sprintf( szBuffer, "Invalid escape code: %c", pszScan[1] );
										  yyerror( szBuffer );
										  return 0;
									  }
								  }
							  else
							  {
								  *pszWrite = *pszScan;
								  pszWrite++;
								  pszScan++;
							  }
                          }
						  *pszWrite = '\0';

						  yyval.pNode = NewLeaf( ltSTRING, yytext ); 
                      }
break;
case 119:
#line 609 "javascript.y"
{ yyval.pNode = NewLeaf( ltNUMBER, DecodeOctal(yytext) ); }
break;
case 120:
#line 611 "javascript.y"
{ yyval.pNode = NewLeaf( ltFLOAT, DecodeFloat(yytext) ); }
break;
case 121:
#line 613 "javascript.y"
{ yyval.pNode = NewLeaf( ltNUMBER, DecodeHex(yytext) ); }
break;
case 122:
#line 615 "javascript.y"
{ yyval.pNode = NewLeaf( ltNUMBER, DecodeDecimal(yytext) ); }
break;
case 123:
#line 620 "javascript.y"
{ 
                            if( strlen( yytext ) >= MAX_IDENTIFIER )
                            {
                                char szBuffer[128];
                                sprintf( szBuffer, "Identifier length exceeds %d characters", MAX_IDENTIFIER );
                                yyerror( szBuffer );
                                return 0;
                            }
                            yyval.pNode = NewLeaf( ltIDENTIFIER, yytext ); 
                        }
break;
#line 1754 "javascript.cpp"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
