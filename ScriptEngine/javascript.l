%{

#ifdef LEXER
    #define _RETURN(x) return((int)#x)
#else
    #define _RETURN(x) return(x)
#endif



int g_nLineNumber = 1;

#ifdef _WIN32
    extern void yyerror( char *s );
    void Unexpected()
    {
        yyerror( "Unexpected symbol" );
    }
#else
    void Unexpected()
    {
        printf( "Line %d : Unexpected %s!\n", g_nLineNumber, yytext );
    }
#endif

%}

C       [^\"\n]
STR     {C}*
NUM     [0-9]+

%%

"/*"                            {	/* skip over comments in the input stream */
									int c1 = 0, c2 = yyinput();
									while(1)
									{
										if( c2 == EOF ) break;
										if( c1 == '*' && c2 == '/' ) break;
										if( c2 == '\n' ) g_nLineNumber++;
										c1 = c2;
										c2 = yyinput();
									}
								}
"//".*\n                        g_nLineNumber++;
function			            _RETURN( FUNCTION );
if				                _RETURN( IF );
else				            _RETURN( ELSE );
for				                _RETURN( FOR );
while				            _RETURN( WHILE );
return				            _RETURN( RETURN );
break				            _RETURN( BREAK );
continue			            _RETURN( CONTINUE );
var                             _RETURN( VAR );

"<<"                            _RETURN( SHL );
">>"                            _RETURN( SSHR );

"++"                            _RETURN( INCREMENT );
"--"                            _RETURN( DECREMENT );

"+"                             _RETURN( ADD );
"*"                             _RETURN( MUL );
"/"                             _RETURN( DIV );
"%"                             _RETURN( MOD );


"="                             _RETURN( ASSIGN );
"+="                            _RETURN( ADDASSIGN );
"-="                            _RETURN( SUBASSIGN );
"*="                            _RETURN( MULASSIGN );
"/="                            _RETURN( DIVASSIGN );
"%="                            _RETURN( MODASSIGN );
"&="                            _RETURN( ANDASSIGN );
"|="                            _RETURN( ORASSIGN );
"^="                            _RETURN( XORASSIGN );
"<<="                           _RETURN( SHLASSIGN );
">>="                           _RETURN( SSHRASSIGN );

[-!~(),.{};?:\[\]]              _RETURN( yytext[0] );

"||"                            _RETURN( LOGOR );
"&&"                            _RETURN( LOGAND );

"|"                             _RETURN( BITOR );
"&"                             _RETURN( BITAND );
"^"                             _RETURN( BITXOR );

"=="                            _RETURN( ISEQUALS );
"!="                            _RETURN( ISNOTEQUALS );

"<"                             _RETURN( ISLTHAN );
">"                             _RETURN( ISGTHAN );
"<="                            _RETURN( ISLTHANEQUALS );
">="                            _RETURN( ISGTHANEQUALS );

new				                _RETURN( NEW );
\"{STR}\"                       _RETURN( STRING );
\"{STR}$                        { yyerror( "Newline in string or missing \"" ); return 0; }
0[0-7]+                         _RETURN( OCTAL_DIGIT );
0x[0-9A-Fa-f]+                  _RETURN( HEX_DIGIT );
0X[0-9A-Fa-f]+			        _RETURN( HEX_DIGIT );
{NUM}"."{NUM}                   _RETURN( FLOAT_DIGIT );
{NUM}                           _RETURN( DECIMAL_DIGIT );
[a-zA-Z_][a-zA-Z_0-9]*		    _RETURN( IDENTIFIER );
[ \t]                           ;
\n				                g_nLineNumber++;
.				                { Unexpected(); return 0; }

%%

#ifdef LEXER
#ifndef _WIN32
main()
{
    char* s;
    while( s = (char*)yylex() )
        printf( "Got: %s \"%s\"\n", s, yytext );
}
#endif
#endif
