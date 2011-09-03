%token FUNCTION IF ELSE FOR WHILE RETURN BREAK CONTINUE NEW STRING VAR OCTAL_DIGIT FLOAT_DIGIT HEX_DIGIT DECIMAL_DIGIT IDENTIFIER ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN ANDASSIGN ORASSIGN XORASSIGN SHLASSIGN SSHRASSIGN LOGOR LOGAND BITOR BITAND BITXOR ISEQUALS ISNOTEQUALS ISLTHAN ISGTHAN ISLTHANEQUALS ISGTHANEQUALS SHL SSHR ADD MUL DIV MOD INCREMENT DECREMENT 

%start script

%{

#include <string.h>
#include <stdio.h>
#include "ParseTree.h"

extern int g_nLineNumber;

/* global script data pointers ************************************************************/

LPNODE g_pScriptNode = NULL;			//pointer to script's root node
LPFUNCTION g_pFunctionList = NULL;		//pointer to script's function list
LPNODE g_pNodeList = NULL;              //pointer to linear script node list


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

    //link it in to the global list
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

    //link it in to the global list
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

    //link it in to the global list
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

    //link it in to the global list
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

	//allocate the argument structure and fill it in
	LPARGUMENT pNew = new ARGUMENT;
	pNew->pNode = pNodeName;
	pNew->next = NULL;

	//link it to the list (at the end - the arguments must be in order)
	if( pArguments == NULL )
		//this is the first item
		return pNew;
	else
	{
		//a list exists already - add this one at the end
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

	//allocate the function stucture and fill it in
	LPFUNCTION pNew = new FUNCTION;
	pNew->pFunctionNameLeaf = pFunctionNameLeaf;
	pNew->pFunctionBodyNode = pFunctionBodyNode;
	pNew->pArguments = pArguments;
	
	//link it to the list (at the start - the function order does not matter)
	pNew->next = g_pFunctionList;
	g_pFunctionList = pNew;

	return pFunctionNameLeaf;
}

LPARGUMENT NewActualArgument( LPARGUMENT pArguments, LPNODE pNodeValue )
{
	//allocate the argument structure and fill it in
	LPARGUMENT pNew = new ARGUMENT;
	pNew->pNode = pNodeValue;
	pNew->next = NULL;

	//link it to the list (at the end - the arguments must be in order)
	if( pArguments == NULL )
		//this is the first item
		return pNew;
	else
	{
		//a list exists already - add this one at the end
		LPARGUMENT pEndOfList = pArguments;
		while( pEndOfList->next != NULL ) pEndOfList = pEndOfList->next;
		
		pEndOfList->next = pNew;
		return pArguments;
	}
}



int DecodeOctal( char* pszOctal )
{
	//it's in the C form of 0NNNNN
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
	//it's in the C form of 0xNNNNN
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

    //delete this node/leaf
    if( pNode->fLeaf == TRUE )
    {
        if( pNode->fString == TRUE )
        {
            //delete the strings
            delete[] pNode->Left.strToken;
            delete[] pNode->Right.strToken;
            delete[] pNode->Other1.strToken;
            delete[] pNode->Other2.strToken;
        }
    }
    else
    {
        //delete child nodes
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

    //delete this node
    delete pNode;

    return pReturn;
}

void FreeParseInformation( LPNODE pNodeList, LPFUNCTION pFunctionList )
{
    //delete the parse tree
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


%}

%union {
	LPNODE pNode;
	LPARGUMENT pFormalArgs;
	LPARGUMENT pActualArgs;
    NODETYPE NodeType;
}


%type <pNode> script
%type <pNode> stmtsfuncs
%type <pNode> function functionBody vars varDeclarations varDeclaration
%type <pNode> statementBlock statements statement
%type <pNode> ifStatement forStatement whileStatement returnStatement breakStatement continueStatement
%type <pNode> expression optionalExpression assign_expression conditional_expression or_expression and_expression bitor_expression xor_expression bitand_expression equality_expression relational_expression shift_expression add_expression mult_expression unary_expression postfix_expression primary_expression 
%type <pNode> objectMethodCall functionCall constructor
%type <pNode> objectMemberAccess arrayDereference leftVariable variable string number identifier
%type <NodeType> assign_operator
%type <pFormalArgs> args
%type <pActualArgs> params

%%


/* main script rules ************************************************************************/

script				: vars stmtsfuncs			{ g_pScriptNode = NewNode( ntSTATEMENT, $1, $2 ); }
					| stmtsfuncs				{ g_pScriptNode = NewNode( ntSTATEMENT, $1, NULL ); }
					;

stmtsfuncs			: function stmtsfuncs       { $$ = $2; }
					| statement stmtsfuncs      { $$ = NewNode( ntSTATEMENT, $1, $2 ); }
					|                           { $$ = NULL; }
					;

/* functions ********************************************************************************/

function            : FUNCTION identifier '(' args ')' functionBody { $$ = NewFunction( $6, $4, $2 ); }
		            ;

args		        : args ',' identifier       { $$ = NewFormalArgument( $1, $3 ); }
					| identifier                { $$ = NewFormalArgument( NULL, $1 ); }
					|                           { $$ = NULL; }
		            ;

functionBody        : '{' vars statements '}'   { $$ = NewNode( ntSTATEMENT, $2, $3 ); }
                    | '{' vars '}'              { $$ = $2; }
                    | statementBlock            { $$ = $1; }
                    ;

/* variable declarations ********************************************************************/

vars                : VAR varDeclarations ';' vars { $$ = NewNode( ntSTATEMENT, $2, $4 ); }
                    | VAR varDeclarations ';'   { $$ = $2; }
                    ;

varDeclarations     : varDeclaration ',' varDeclarations { $$ = NewNode( ntVARIABLELIST, $1, $3 ); }
                    | varDeclaration            { $$ = NewNode( ntVARIABLELIST, $1, NULL ); }
                    ;

varDeclaration      : identifier ASSIGN conditional_expression { $$ = NewNode( ntVARIABLE, $1, $3 ); }
                    | identifier                { $$ = NewNode( ntVARIABLE, $1, NULL ); }
                    ;

/* statements *******************************************************************************/

statementBlock      : '{' statements '}'        { $$ = $2; }
                    | '{' '}'                   { $$ = NULL; }
                    | statement                 { $$ = NewNode( ntSTATEMENT, $1, NULL ); }
		            ;

statements	        : statement statements      { $$ = NewNode( ntSTATEMENT, $1, $2 ); }
		            | statement                 { $$ = NewNode( ntSTATEMENT, $1, NULL ); }
		            ;


statement	        : ifStatement               { $$ = $1; }
		            | forStatement              { $$ = $1; }
		            | whileStatement            { $$ = $1; }
		            | returnStatement ';'       { $$ = $1; }
		            | breakStatement ';'        { $$ = $1; }
		            | continueStatement ';'     { $$ = $1; }
                    | expression ';'            { $$ = $1; }
                    | ';'                       { $$ = NULL; }
                    ;

ifStatement	        : IF '(' expression ')' statementBlock  { $$ = NewNode( ntIFSTATEMENT, $3, $5 ); }
		            | IF '(' expression ')' statementBlock ELSE statementBlock  { $$ = NewNode( ntIFSTATEMENT, $3, $5, $7 ); }
		            ;


forStatement	    : FOR '(' optionalExpression ';' optionalExpression ';' optionalExpression ')' statementBlock  { $$ = NewNode( ntFORSTATEMENT, $3, $5, $7, $9 ); }
		            ;


whileStatement	    : WHILE '(' expression ')' statementBlock  { $$ = NewNode( ntWHILESTATEMENT, $3, $5 ); }
		            ;


returnStatement	    : RETURN expression         { $$ = NewNode( ntRETURNSTATEMENT, $2, NULL ); }
		            | RETURN                    { $$ = NewNode( ntRETURNSTATEMENT, NULL, NULL ); }
		            ;


breakStatement	    : BREAK                     { $$ = NewNode( ntBREAKSTATEMENT, NULL, NULL ); }
		            ;


continueStatement   : CONTINUE                  { $$ = NewNode( ntCONTINUESTATEMENT, NULL, NULL ); }
		            ;


/* expressions *******************************************************************************/

optionalExpression    : expression              { $$ = $1; }
                      |                         { $$ = NULL; }
                      ;

expression            : assign_expression       { $$ = $1; }
                      | expression ',' assign_expression { $$ = NewNode( ntEXPRESSION, $1, $3 ); }
                      ;

assign_expression     : conditional_expression  { $$ = $1; }
                      | leftVariable assign_operator assign_expression  { $$ = NewNode( $2, $1, $3 ); }
                      ;

assign_operator       : ASSIGN                 { $$ = ntASSIGN; }
                      | ADDASSIGN              { $$ = ntADDASSIGN; }
                      | SUBASSIGN              { $$ = ntSUBASSIGN; }
                      | MULASSIGN              { $$ = ntMULASSIGN; }
                      | DIVASSIGN              { $$ = ntDIVASSIGN; }
                      | MODASSIGN              { $$ = ntMODASSIGN; }
                      | ANDASSIGN              { $$ = ntANDASSIGN; }
                      | ORASSIGN               { $$ = ntORASSIGN; }
                      | XORASSIGN              { $$ = ntXORASSIGN; }
                      | SHLASSIGN              { $$ = ntSHLASSIGN; }
                      | SSHRASSIGN             { $$ = ntSSHRASSIGN; }
                      ;

conditional_expression : or_expression      { $$ = $1; }
                      | or_expression '?' expression ':' conditional_expression { $$ = NewNode( ntQUESTIONMARKCOLON, $1, $3, $5 ); }
                      ;

or_expression         : and_expression      { $$ = $1; }
                      | or_expression LOGOR and_expression   { $$ = NewNode( ntLOGOR, $1, $3 ); }
                      ;

and_expression        : bitor_expression    { $$ = $1; }
                      | and_expression LOGAND bitor_expression    { $$ = NewNode( ntLOGAND, $1, $3 ); }
                      ;

bitor_expression      : xor_expression      { $$ = $1; }
                      | bitor_expression BITOR xor_expression     { $$ = NewNode( ntBITOR, $1, $3 ); }
                      ;

xor_expression        : bitand_expression   { $$ = $1; }
                      | xor_expression BITXOR bitand_expression    { $$ = NewNode( ntBITXOR, $1, $3 ); }
                      ;

bitand_expression     : equality_expression { $$ = $1; }
                      | bitand_expression BITAND equality_expression   { $$ = NewNode( ntBITAND, $1, $3 ); }
                      ;

equality_expression   : relational_expression { $$ = $1; }
                      | equality_expression ISEQUALS relational_expression  { $$ = NewNode( ntISEQUALS, $1, $3 ); }
                      | equality_expression ISNOTEQUALS relational_expression  { $$ = NewNode( ntISNOTEQUALS, $1, $3 ); }
                      ;

relational_expression : shift_expression    { $$ = $1; }
                     | relational_expression ISLTHAN shift_expression   { $$ = NewNode( ntISLTHAN, $1, $3 ); }
                     | relational_expression ISGTHAN shift_expression   { $$ = NewNode( ntISGTHAN, $1, $3 ); }
                     | relational_expression ISLTHANEQUALS shift_expression  { $$ = NewNode( ntISLTHANEQUALS, $1, $3 ); }
                     | relational_expression ISGTHANEQUALS shift_expression  { $$ = NewNode( ntISGTHANEQUALS, $1, $3 ); }
                     ;

shift_expression     : add_expression       { $$ = $1; }
                     | shift_expression SHL add_expression         { $$ = NewNode( ntSHL, $1, $3 ); }
                     | shift_expression SSHR add_expression         { $$ = NewNode( ntSSHR, $1, $3 ); }
                     ;

add_expression       : mult_expression      { $$ = $1; }
                     | add_expression ADD mult_expression           { $$ = NewNode( ntADD, $1, $3 ); }
                     | add_expression '-' mult_expression           { $$ = NewNode( ntSUB, $1, $3 ); }
                     ;

mult_expression      : unary_expression     { $$ = $1; }
                     | mult_expression MUL unary_expression         { $$ = NewNode( ntMUL, $1, $3 ); }
                     | mult_expression DIV unary_expression         { $$ = NewNode( ntDIV, $1, $3 ); }
                     | mult_expression MOD unary_expression         { $$ = NewNode( ntMOD, $1, $3 ); }
                     ;

unary_expression     : postfix_expression   { $$ = $1; }
                     | INCREMENT unary_expression                        { $$ = NewNode( ntPREINCREMENT, $2, NULL ); }
                     | DECREMENT unary_expression                        { $$ = NewNode( ntPREDECREMENT, $2, NULL ); }
                     | '-' unary_expression                         { $$ = NewNode( ntNEGATE, $2, NULL ); }
                     | '!' unary_expression                         { $$ = NewNode( ntLOGICALNOT, $2, NULL ); }
                     | '~' unary_expression                         { $$ = NewNode( ntCOMPLIMENT, $2, NULL ); }
                     ;


postfix_expression   : primary_expression   { $$ = $1; }
                     | postfix_expression INCREMENT                      { $$ = NewNode( ntPOSTINCREMENT, $1, NULL ); }
                     | postfix_expression DECREMENT                      { $$ = NewNode( ntPOSTDECREMENT, $1, NULL ); }
                     | identifier                                   { $$ = $1; }
                     ;

primary_expression   : '(' expression ')'                           { $$ = $2; }
                     | objectMethodCall                             { $$ = $1; }
                     | objectMemberAccess                           { $$ = $1; }
                     | functionCall                                 { $$ = $1; }
                     | string                                       { $$ = $1; }
                     | number                                       { $$ = $1; }
                     | NEW constructor                              { $$ = NewNode( ntNEW, $2, NULL ); }
                     | arrayDereference                             { $$ = $1; }
                     ;

/* function calls ****************************************************************************/

functionCall	    : identifier '(' params ')'                     { $$ = NewNode( ntFUNCTIONCALL, $1, (LPNODE)$3 ); }
		            ;


constructor	        : identifier '(' params ')'                     { $$ = NewNode( ntCONSTRUCTOR, $1, (LPNODE)$3 ); }
					| identifier									{ $$ = NewNode( ntCONSTRUCTOR, $1, NULL ); }
		            ;


objectMethodCall    : postfix_expression '.' identifier '(' params ')' { $$ = NewNode( ntOBJECTMETHODCALL, $1, $3, (LPNODE)$5 ); }
		            ;


objectMemberAccess  : postfix_expression '.' identifier             { $$ = NewNode( ntOBJECTMEMBERACCESS, $1, $3 ); }
    	            ;


params		        : params ',' assign_expression  { $$ = NewActualArgument( $1, $3 ); }
		            | assign_expression             { $$ = NewActualArgument( NULL, $1 ); }
		            |                               { $$ = NULL; }
		            ;


/* misc left and right hand sides ************************************************************/

arrayDereference    : postfix_expression '[' expression ']' { $$ = NewNode( ntARRAYDEREFERENCE, $1, $3 ); }
		            ;


leftVariable        : objectMemberAccess        { $$ = $1; }
		            | arrayDereference          { $$ = $1; }
		            | variable                  { $$ = $1; }
		            ;


variable	        : identifier                { $$ = $1; }
		            ;


string 		        : STRING 
					  { 
						  //strip the quotes from the string
						  char* pszCloseQuote = &yytext[strlen(yytext)-1];
						  *pszCloseQuote = '\0';

                          //now search & replace escape codes
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

						  $$ = NewLeaf( ltSTRING, yytext ); 
                      }
		            ;


number		        : OCTAL_DIGIT
						{ $$ = NewLeaf( ltNUMBER, DecodeOctal(yytext) ); }
		            | FLOAT_DIGIT
						{ $$ = NewLeaf( ltFLOAT, DecodeFloat(yytext) ); }
		            | HEX_DIGIT
						{ $$ = NewLeaf( ltNUMBER, DecodeHex(yytext) ); }
		            | DECIMAL_DIGIT
						{ $$ = NewLeaf( ltNUMBER, DecodeDecimal(yytext) ); }
		            ;


identifier	        : IDENTIFIER 
						{ 
                            if( strlen( yytext ) >= MAX_IDENTIFIER )
                            {
                                char szBuffer[128];
                                sprintf( szBuffer, "Identifier length exceeds %d characters", MAX_IDENTIFIER );
                                yyerror( szBuffer );
                                return 0;
                            }
                            $$ = NewLeaf( ltIDENTIFIER, yytext ); 
                        }
		            ;


%%


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
