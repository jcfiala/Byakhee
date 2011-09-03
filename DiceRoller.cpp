// DiceRoller.cpp: implementation of the CDiceRoller class.
//
//////////////////////////////////////////////////////////////////////

/*

This is a generic dice rolling class. It is capable of parsing strings such as:
D4 1D6 3D6*4 1D10+20
It supports addition, subtraction, multiplication and division operators

The RollNSidedDice function is overridable should the need arrise to provide
an alternative dice rolling algorithm (i.e. a true random number generator is
invented, or you want to make a CLoadedDiceRoller class ;-) )
*/

#include "stdafx.h"
#include "Byakhee.h"
#include "DiceRoller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiceRoller::CDiceRoller()
{
}

CDiceRoller::~CDiceRoller()
{

}


//////////////////////////////////////////////////////////////////////
// Public Function
//////////////////////////////////////////////////////////////////////
int CDiceRoller::Roll( CString strRoll )
{
    //get a pointer to the string data
    LPCSTR pszRoll = LPCSTR(strRoll);
    if( pszRoll == NULL || *pszRoll == '\0' ) return 0;

    //roll first dice
    int nTotal = ExtractAndRoll( pszRoll );

    //keep rolling out the string until the end
    while( pszRoll && *pszRoll != '\0' )
    {
        char chOperator = *pszRoll;
        switch( chOperator )
        {
            //check for legal operators - continue to parse
            case '+': nTotal += ExtractAndRoll( ++pszRoll ); break;
            case '-': nTotal -= ExtractAndRoll( ++pszRoll ); break;
            case '*': nTotal *= ExtractAndRoll( ++pszRoll ); break;
            case '/': 
            {
                int nRoll = ExtractAndRoll( ++pszRoll );
                if( nRoll != 0 )  //check for divide by zero
                    nTotal /= nRoll; 
                else 
                {
                    //ka-boom...
                    CString str;
                    str.Format( "Malformed Dice Roll Expression: %s\nDivide by zero", strRoll );
                    AfxMessageBox( str );
                    return 0;
                }
                break;
            }

            //skip whitespace
            case ' ':
            case '\t':
                pszRoll++;
                break;

            //oops... unexpected token
            default:
            {
                CString str;
                str.Format( "Malformed Dice Roll Expression: %s\n\"%c\" - Invalid Symbol", strRoll, *pszRoll );
                AfxMessageBox( str );
                return 0;
            }
        }
    }

    //return the result
    return nTotal;
}


//////////////////////////////////////////////////////////////////////
// Protected Function (overridable)
//////////////////////////////////////////////////////////////////////

int CDiceRoller::RollNSidedDice( int n )
{
    //make sure it's at least nearly a sensible roll
    if( n <= 0 ) 
    {
        TRACE0( "Warning: Tried to roll dice with less than one side!!!\n" );
        return 0;
    }

    //perform roll
    return ( rand() % n ) + 1;
}

//////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////

//ExtractAndRoll - extracts a singular dice roll expression* and rolls it out
//returns the result of the roll (if required) and updates the string pointer
//to the next bit of the roll string
//
// * eg: 
//      numbers: 3 24, 50
//      dice:    D6 D10 etc.
//
// Note: this will also accept really wierd dice, including: D1 D2 D3 D7 D666 etc... :)
int CDiceRoller::ExtractAndRoll( LPCSTR& pszRollString )
{
    //check the pointer
    if( pszRollString == NULL || *pszRollString == '\0' ) return 0;

    //skip over whitespace
    while( *pszRollString == ' ' || *pszRollString == '\t' ) pszRollString++;

    //see if we've got 'nDx' or just 'Dx' - we may even have just 'n'
    int nNumber = 0;
    switch( *pszRollString )
    {
        case 'D':
        case 'd':
            //it's a Dn roll... get the multiplier
            nNumber = ExtractNumber( ++pszRollString );
            if( nNumber >= 0 )
            {
                //roll the dice
                nNumber = RollNSidedDice( nNumber );
            }
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            //it's a number
            int nFirstNumber = ExtractNumber( pszRollString );
            if( nFirstNumber <= 1000 && nFirstNumber >= 0 )
            {
                if( *pszRollString == 'D' || *pszRollString == 'd' )
                {
                    //there's a dice roll straight after - roll n Dice and add the total
                    pszRollString++;
                    int nSides = ExtractNumber( pszRollString );
                    if( nSides >= 0 )
                    {
                        //roll all the dice
                        for( int n = 0; n < nFirstNumber; n++ ) nNumber += RollNSidedDice( nSides );
                    }
                }
                else
                {
                    //it's just a number - return it
                    nNumber = nFirstNumber;
                }
            }
            else
            {
                CString str;
                str.Format( "Roll multiplier %d out of range (0-1000): %d", nFirstNumber );
                AfxMessageBox( str );
                pszRollString = NULL; //break out of outer loop
                return 0;
            }
            break;
        }

        default:
        {
            CString str;
            str.Format( "Malformed Dice Roll Expression: %s\n\"%c\" - Invalid Symbol", pszRollString, *pszRollString );
            AfxMessageBox( str );
            pszRollString = NULL; //break out of outer loop
            return 0;
        }
    }

    //return the result (the pointer has already been updated)
    return nNumber;
}

//this will extract a number from the string - stopping at the first non-numeric value
int CDiceRoller::ExtractNumber( LPCSTR& pszString )
{
    //make sure we've got a valid string
    if( pszString == NULL || *pszString == '\0' ) return 0;

    //start extracting
    int nResult = 0;
    while( isdigit( *pszString ) )
    {
        //extract the character
        char chDigit = *pszString;
        pszString++;

        //convert character to a number between 0-9
        chDigit = chDigit - '0';

        //merge this with the result
        nResult *= 10;
        nResult += chDigit;
    }

    //return the result (the pointer has already been updated)
    return nResult;
}


/* Script Related Stuff */
CScriptObject::MEMBERRESULT CScriptDice::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue  )
{
    if( stricmp( pszName, "Roll" ) == 0 )
    {
        if( nParameters == 1 ) 
        {
            returnValue = m_Dice.Roll( LPCSTR(pParameters[0]) );
            return OK;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    return NOT_FOUND;
}

