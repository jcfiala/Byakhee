// Math.cpp: implementation of the CMath class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include <math.h>
#include "Script_Math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMath::CMath()
{
	m_PI = 3.14159265358979323846;
	m_E = 2.7182818284590452354;

	m_PI.SetReadOnly( TRUE );
	m_E.SetReadOnly( TRUE );
}

CMath::~CMath()
{

}

CScriptObject::MEMBERRESULT CMath::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
	if( strcmp( pszName, "abs" ) == 0 )
	{
		if( nParameters == 1 )
		{
			if( pParameters[0].IsNumeric() )
			{
				Variant zero = Variant(0);
				returnValue = pParameters[0] < zero ? zero-pParameters[0] : pParameters[0];
				return OK;
			}
			else
			{
				returnValue = pParameters[0];
				return OK;
			}
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "acos" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = acos(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}
	
	if( strcmp( pszName, "asin" ) == 0 )
	{
		if( nParameters == 1 )
		{
			returnValue = asin(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "atan" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = atan(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "atan2" ) == 0 )
	{
		if( nParameters == 2 )
		{
    		returnValue = atan2(double(pParameters[0]), double(pParameters[1]));
			return OK;
		}
		else
			return nParameters < 2 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "ceil" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = ceil(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "cos" ) == 0 )
	{
		if( nParameters == 1 )
		{
		    returnValue = cos(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}
	
	if( strcmp( pszName, "exp" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = exp(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "floor" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = floor(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "log" ) == 0 )
	{
		if( nParameters == 1 )
		{
		    returnValue = log(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}
	
	if( strcmp( pszName, "max" ) == 0 )
	{
		if( nParameters == 2 )
		{
    		returnValue = pParameters[0] > pParameters[1] ? pParameters[0] : pParameters[1];
			return OK;
		}
		else
			return nParameters < 2 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "min" ) == 0 )
	{
		if( nParameters == 2 )
		{
    		returnValue = pParameters[0] < pParameters[1] ? pParameters[0] : pParameters[1];
			return OK;
		}
		else
			return nParameters < 2 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "pow" ) == 0 )
	{
		if( nParameters == 2 )
		{
   			returnValue = pow( double(pParameters[0]), double(pParameters[1]) );
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "random" ) == 0 )
	{
		if( nParameters == 0 )
		{
			returnValue = (double)rand() / (double)RAND_MAX;
			return OK;
		}
		else
			return TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "round" ) == 0 )
	{
		if( nParameters == 1 )
		{
   			returnValue = int(pParameters[0]);
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "sin" ) == 0 )
	{
		if( nParameters == 1 )
		{
   			returnValue = sin(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "sqrt" ) == 0 )
	{
		if( nParameters == 1 )
		{
   			returnValue = sqrt(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

	if( strcmp( pszName, "tan" ) == 0 )
	{
		if( nParameters == 1 )
		{
    		returnValue = tan(double(pParameters[0]));
			return OK;
		}
		else
			return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
	}

    return NOT_FOUND;
}

Variant* CMath::GetMemberVariable( char* pszName )
{
	//foreground and background colours
	if( strcmp( pszName, "PI" ) == 0 ) return &m_PI;
	if( strcmp( pszName, "E" ) == 0  ) return &m_E;
    return NULL;
}


