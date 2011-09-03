// Variant.cpp: implementation of the Variant class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include <math.h>
#include <stdio.h>
#include "Variant.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Variant::Variant()
{
	m_Type = INT_TYPE;
	m_nValue = 0;
    m_fReadOnly = FALSE;
}

Variant::Variant(const Variant& varSrc)
{
    Become(varSrc);
}

Variant::Variant( Variant* pVariant )
{
	m_Type = PVARIANT_TYPE;
	m_pVariant = pVariant;
	if( m_pVariant == NULL )
	{
		m_Type = NULL_TYPE;
		m_nValue = 0;
	}
	else
		if( m_pVariant->m_Type == POBJECT_TYPE ) m_pVariant->m_pObject->AddRef();

    m_fReadOnly = FALSE;
}

Variant::Variant(LPCSTR lpszSrc)
{
	m_Type = PSTR_TYPE;
	m_pszValue = strdup((char*)lpszSrc);
    m_fReadOnly = FALSE;
}

Variant::Variant(int nSrc)
{
	m_Type = INT_TYPE;
	m_nValue = nSrc;
    m_fReadOnly = FALSE;
}

Variant::Variant(unsigned int uSrc)
{
	m_Type = UINT_TYPE;
	m_uValue = uSrc;
    m_fReadOnly = FALSE;
}

Variant::Variant(double dblSrc)
{
	m_Type = DOUBLE_TYPE;
	m_dValue = dblSrc;
    m_fReadOnly = FALSE;
}

Variant::Variant( LPCSCRIPTOBJECT pObject )
{
    if( pObject )
    {
	    m_Type = POBJECT_TYPE;
	    m_pObject = pObject;
	    m_pObject->AddRef();
    }
    else
    {
        m_Type = NULL_TYPE;
        m_nValue = 0;
    }

    m_fReadOnly = FALSE;
}

//private concatention constructor - used by Variant::operator +(...)
Variant::Variant(LPCSTR lpszSrc, LPCSTR lpszSrc2)
{
    //concatenate the strings
	int nLength = strlen( lpszSrc ) + strlen( lpszSrc2 );

	m_Type = PSTR_TYPE;
	m_pszValue = new char[nLength+1];
	strcpy( m_pszValue, lpszSrc );
	strcat( m_pszValue, lpszSrc2 );
    m_fReadOnly = FALSE;
}


Variant::~Variant()
{
	DeleteValue();
}

void Variant::SetReadOnly( BOOL fReadOnly )
{
    m_fReadOnly = fReadOnly;
}

BOOL Variant::IsNULL() const
{
	if( m_Type == NULL_TYPE )
		return TRUE;
	else
		if( m_Type == PVARIANT_TYPE )
			return m_pVariant->IsNULL();
		else
			return FALSE;
}

BOOL Variant::IsNumeric() const
{
	if( m_Type == INT_TYPE || m_Type == DOUBLE_TYPE || m_Type == UINT_TYPE )
		return TRUE;
	else
		if( m_Type == PVARIANT_TYPE ) 
			return m_pVariant->IsNumeric();
		else
			return FALSE;
}

BOOL Variant::IsObject() const
{
	if( m_Type == POBJECT_TYPE )
		return TRUE;
	else
		if( m_Type == PVARIANT_TYPE )
			return m_pVariant->IsObject();
		else
			return FALSE;
}

BOOL Variant::IsString() const
{
	if( m_Type == PSTR_TYPE )
		return TRUE;
	else
		if( m_Type == PVARIANT_TYPE ) 
			return m_pVariant->IsString();
		else
			return FALSE;
}
	
//evaluation operators
BOOL Variant::operator==(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return varSrc.IsNULL();
		case PSTR_TYPE:		return strcmp( m_pszValue, LPCSTR(varSrc) ) == 0;
		case INT_TYPE:		return m_nValue == int(varSrc);
		case UINT_TYPE:		return m_uValue == unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue == double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return TRUE; else return (*m_pVariant == varSrc);
		case POBJECT_TYPE:  return (*m_pObject == varSrc);
		default:			return FALSE;
	}
}

BOOL Variant::operator!=(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return !varSrc.IsNULL();
		case PSTR_TYPE:		return strcmp(m_pszValue, LPCSTR(varSrc) ) != 0;
		case INT_TYPE:		return m_nValue != int(varSrc);
		case UINT_TYPE:		return m_uValue != unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue != double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant != varSrc);
		case POBJECT_TYPE:  return (*m_pObject != varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator<(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return FALSE;
		case PSTR_TYPE:		return strcmp( m_pszValue, LPCSTR(varSrc) ) < 0;
		case INT_TYPE:		return m_nValue < int(varSrc);
		case UINT_TYPE:		return m_uValue < unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue < double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant < varSrc);
		case POBJECT_TYPE:  return (*m_pObject < varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator<=(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return varSrc.IsNULL();
		case PSTR_TYPE:		return strcmp( m_pszValue, LPCSTR(varSrc) ) <= 0;
		case INT_TYPE:		return m_nValue <= int(varSrc);
		case UINT_TYPE:		return m_uValue <= unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue <= double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant <= varSrc);
		case POBJECT_TYPE:  return (*m_pObject <= varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator>=(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return varSrc.IsNULL();
		case PSTR_TYPE:		return strcmp( m_pszValue, LPCSTR(varSrc) ) >= 0;
		case INT_TYPE:		return m_nValue >= int(varSrc);
		case UINT_TYPE:		return m_uValue >= unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue >= double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant >= varSrc);
		case POBJECT_TYPE:  return (*m_pObject >= varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator>(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return FALSE;
		case PSTR_TYPE:		return strcmp( m_pszValue, LPCSTR(varSrc) ) > 0;
		case INT_TYPE:		return m_nValue > int(varSrc);
		case UINT_TYPE:		return m_uValue > unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue > double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant > varSrc);
		case POBJECT_TYPE:  return (*m_pObject > varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator&&(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return FALSE;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return m_nValue && int(varSrc);
		case UINT_TYPE:		return m_uValue && unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue && double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant && varSrc);
		case POBJECT_TYPE:  return (*m_pObject && varSrc );
		default:			return FALSE;
	}
}

BOOL Variant::operator||(const Variant& varSrc) const
{
	//convert the other one to this type & compare
	switch( m_Type )
	{
		case NULL_TYPE:     return int(varSrc);
		case PSTR_TYPE:		return INVALID_OPERATOR_STRING;
		case INT_TYPE:		return m_nValue || int(varSrc);
		case UINT_TYPE:		return m_uValue || unsigned int(varSrc);
		case DOUBLE_TYPE:	return m_dValue || double(varSrc);
		case PVARIANT_TYPE: if( this == m_pVariant ) return FALSE; else return (*m_pVariant || varSrc);
		case POBJECT_TYPE:  return (*m_pObject || varSrc );
		default:			return FALSE;
	}
}


//arithmetic operators
Variant Variant::operator+(const Variant& varSrc) const
{
   //always concatenate strings
    if( ( varSrc.m_Type == PSTR_TYPE || m_Type == PSTR_TYPE ) ||
        ( varSrc.m_Type == PVARIANT_TYPE && varSrc.m_pVariant->m_Type == PSTR_TYPE ) ||
        ( m_Type == PVARIANT_TYPE && m_pVariant->m_Type == PSTR_TYPE ) 
      ) return Variant( LPCSTR(*this), LPCSTR(varSrc) );

	//convert the other one to this type & substract
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //treat as integer
		case INT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_nValue + double(varSrc) : m_nValue + int(varSrc);
		case UINT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_uValue + double(varSrc) : int(m_uValue) + int(varSrc);
		case DOUBLE_TYPE:	return m_dValue + double(varSrc);
		case PVARIANT_TYPE: return (*m_pVariant + varSrc);
		case POBJECT_TYPE:  return (*m_pObject + varSrc);
		default:			return FALSE;
	}


}

Variant Variant::operator-(const Variant& varSrc) const
{
    //convert the other one to this type & substract
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //treat as integer
		case INT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_nValue - double(varSrc) : m_nValue - int(varSrc);
		case UINT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_uValue - double(varSrc) : int(m_uValue) - int(varSrc);
		case DOUBLE_TYPE:	return m_dValue - double(varSrc);
		case PVARIANT_TYPE: return (*m_pVariant - varSrc);
		case POBJECT_TYPE:  return (*m_pObject - varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator*(const Variant& varSrc) const
{
	//convert the other one to this type & multiply
	switch( m_Type )
	{
		case NULL_TYPE:     return 0;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_nValue * double(varSrc) : m_nValue * int(varSrc);
		case UINT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_uValue * double(varSrc) : int(m_uValue) * int(varSrc);
		case DOUBLE_TYPE:	return m_dValue * double(varSrc);
		case PVARIANT_TYPE: return (*m_pVariant * varSrc);
		case POBJECT_TYPE:  return (*m_pObject * varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator/(const Variant& varSrc) const
{
	if( double(varSrc) == 0.0 ) throw DIVIDE_BY_ZERO;

	//convert the other one to this type & divide
	switch( m_Type )
	{
		case NULL_TYPE:     return 0;
		case INT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_nValue / double(varSrc) : m_nValue / int(varSrc);
		case UINT_TYPE:		return varSrc.GetActualType() == DOUBLE_TYPE ? (double)m_uValue / double(varSrc) : int(m_uValue) / int(varSrc);
		case DOUBLE_TYPE:	return m_dValue / double(varSrc);
		case PVARIANT_TYPE: return (*m_pVariant / varSrc);
		case POBJECT_TYPE:  return (*m_pObject / varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator%(const Variant& varSrc) const
{
	if( double(varSrc) == 0.0 ) throw DIVIDE_BY_ZERO;

	//convert the other one to this type & modulo
	switch( m_Type )
	{
		case NULL_TYPE:     return 0;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return Variant(m_nValue % int(varSrc));
		case UINT_TYPE:		return Variant(m_uValue % int(varSrc));
		case DOUBLE_TYPE:	return Variant( fmod( m_dValue, double(varSrc) ));
		case PVARIANT_TYPE: return (*m_pVariant % varSrc);
		case POBJECT_TYPE:  return (*m_pObject % varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator&(const Variant& varSrc) const
{
	//convert the other one to this type & AND
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //treat as integer
		case INT_TYPE:		return Variant(m_nValue & int(varSrc));
		case UINT_TYPE:		return Variant(m_uValue & unsigned int(varSrc));
		case DOUBLE_TYPE:	return Variant(int(m_dValue) & int(varSrc));
		case PVARIANT_TYPE: return (*m_pVariant & varSrc);
		case POBJECT_TYPE:  return (*m_pObject & varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator|(const Variant& varSrc) const
{
	//convert the other one to this type & OR
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //treat as integer
		case INT_TYPE:		return Variant(m_nValue | int(varSrc));
		case UINT_TYPE:		return Variant(m_uValue | unsigned int(varSrc));
		case DOUBLE_TYPE:	return Variant(int(m_dValue) | int(varSrc));
		case PVARIANT_TYPE: return (*m_pVariant | varSrc);
		case POBJECT_TYPE:  return (*m_pObject | varSrc );
		default:			return FALSE;
	}
}


Variant Variant::operator^(const Variant& varSrc) const
{
	//convert the other one to this type & XOR
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //treat as integer
		case INT_TYPE:		return Variant(m_nValue ^ int(varSrc));
		case UINT_TYPE:		return Variant(m_uValue ^ unsigned int(varSrc));
		case DOUBLE_TYPE:	return Variant(int(m_dValue) ^ int(varSrc));
		case PVARIANT_TYPE: return (*m_pVariant ^ varSrc);
		case POBJECT_TYPE:  return (*m_pObject ^ varSrc );
		default:			return FALSE;
	}
}

//prefix operators
Variant Variant::operator-()
{
	switch( m_Type )
	{
		case NULL_TYPE:     return 0;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return -m_nValue;
		case UINT_TYPE:		return -(signed int)m_uValue;
		case DOUBLE_TYPE:	return -m_dValue;
		case PVARIANT_TYPE: return -(*m_pVariant);
		case POBJECT_TYPE:  return -(*m_pObject);
		default:			return FALSE;
	}
}

Variant Variant::operator!()
{
	switch( m_Type )
	{
		case NULL_TYPE:     return 1;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return !m_nValue;
		case UINT_TYPE:		return !m_uValue;
		case DOUBLE_TYPE:	return !(unsigned int)m_dValue;
		case PVARIANT_TYPE: return !(*m_pVariant);
		case POBJECT_TYPE:  return !(*m_pObject);
		default:			return FALSE;
	}
}

Variant Variant::operator~()
{
	switch( m_Type )
	{
		case NULL_TYPE:     return -1;
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return ~m_nValue;
		case UINT_TYPE:		return ~m_uValue;
		case DOUBLE_TYPE:	return ~(unsigned int)m_dValue;
		case PVARIANT_TYPE: return ~(*m_pVariant);
		case POBJECT_TYPE:  return ~(*m_pObject);
		default:			return FALSE;
	}
}


//other operators
Variant& Variant::operator++()
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     m_Type = INT_TYPE; //drop through to int
		case INT_TYPE:		{ m_nValue++; return *this; };
		case UINT_TYPE:		{ m_uValue++; return *this; };
		case DOUBLE_TYPE:	{ m_dValue++; return *this; };
		case PVARIANT_TYPE: { (*m_pVariant)++; return *this; };
        case POBJECT_TYPE:  { (*m_pObject)++; return *this; };
		default:			return *this;
	}
}


Variant& Variant::operator--()
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     m_Type = INT_TYPE; //drop through to int
		case INT_TYPE:		{ m_nValue--; return *this; };
		case UINT_TYPE:		{ m_uValue--; return *this; };
		case DOUBLE_TYPE:	{ m_dValue--; return *this; };
		case PVARIANT_TYPE: { (*m_pVariant)--; return *this; };
        case POBJECT_TYPE:  { (*m_pObject)--; return *this; };
		default:			return *this;
	}
}

Variant Variant::operator++(int)
{
	Variant temp = *this;
	++*this;
	return temp;
}


Variant Variant::operator--(int)
{
	Variant temp = *this;
	--*this;
	return temp;
}


Variant& Variant::operator<<(int nBits) const
{
	switch( m_Type )
	{
		case NULL_TYPE:     return Variant(0);
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return Variant( m_nValue << nBits );
		case UINT_TYPE:		return Variant( m_uValue << nBits );
		case DOUBLE_TYPE:	return Variant( int(m_dValue) << nBits );
		case PVARIANT_TYPE: return (*m_pVariant) << nBits;
		case POBJECT_TYPE:  return (*m_pObject) << nBits;
		default:			return Variant(0);
	}
}

Variant& Variant::operator>>(int nBits) const
{
	switch( m_Type )
	{
		case NULL_TYPE:     return Variant(0);
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING;
		case INT_TYPE:		return Variant( m_nValue >> nBits );
		case UINT_TYPE:		return Variant( m_uValue >> nBits );
		case DOUBLE_TYPE:	return Variant( int(m_dValue) >> nBits );
		case PVARIANT_TYPE: return (*m_pVariant) >> nBits;
		case POBJECT_TYPE:  return (*m_pObject) >> nBits;
		default:			return Variant(0);
	}

	//shouldn't reach here!
	return Variant();
}

PVARIANT Variant::operator[]( UINT nPos ) const
{
    switch( m_Type )
    {
        case PSTR_TYPE:     throw INVALID_OPERATOR_STRING;
		case NULL_TYPE:     //drop through
        case INT_TYPE:      throw INVALID_OPERATOR_INT;
        case UINT_TYPE:     throw INVALID_OPERATOR_UINT;
        case DOUBLE_TYPE:   throw INVALID_OPERATOR_FLOAT;
        case PVARIANT_TYPE: return (*m_pVariant)[nPos];
        case POBJECT_TYPE:  return (*m_pObject)[nPos];
		default:			return 0;
    }

	//shouldn't reach here!
	return 0;
}


//arithmetic assignment operators
const Variant& Variant::operator+=(const Variant& varSrc)
{
	*this = *this + varSrc;
	return *this;
}

const Variant& Variant::operator-=(const Variant& varSrc)
{
	*this = *this - varSrc;
	return *this;
}

const Variant& Variant::operator*=(const Variant& varSrc)
{
	*this = *this * varSrc;
	return *this;
}

const Variant& Variant::operator/=(const Variant& varSrc)
{
	*this = *this / varSrc;
	return *this;
}

const Variant& Variant::operator%=(const Variant& varSrc)
{
	*this = *this % varSrc;
	return *this;
}

const Variant& Variant::operator&=(const Variant& varSrc)
{
	*this = *this & varSrc;
	return *this;
}

const Variant& Variant::operator|=(const Variant& varSrc)
{
	*this = *this | varSrc;
	return *this;
}

const Variant& Variant::operator^=(const Variant& varSrc)
{
	*this = *this ^ varSrc;
	return *this;

}

const Variant& Variant::operator>>=(int nBits)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING; break;
		case NULL_TYPE:		m_Type = INT_TYPE; //drop through
		case INT_TYPE:		m_nValue >>= nBits; break;
		case UINT_TYPE:		m_uValue >>= nBits; break;
		case DOUBLE_TYPE:	*this = int(*this) >> nBits; break;
		case PVARIANT_TYPE: *m_pVariant = *m_pVariant >> nBits; break;
		case POBJECT_TYPE:  *m_pObject >>= nBits; break;
	}
	return *this;
}

const Variant& Variant::operator<<=(int nBits)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	switch( m_Type )
	{
		case PSTR_TYPE:		throw INVALID_OPERATOR_STRING; break;
		case NULL_TYPE:		m_Type = INT_TYPE; //drop through
		case INT_TYPE:		m_nValue <<= nBits; break;
		case UINT_TYPE:		m_uValue <<= nBits; break;
		case DOUBLE_TYPE:	*this = int(*this) << nBits; break;
		case PVARIANT_TYPE: *m_pVariant = *m_pVariant << nBits; break;
		case POBJECT_TYPE:  *m_pObject <<= nBits; break;
	}
	return *this;
}



//assignment operators
const Variant& Variant::operator=(const Variant& varSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	switch( m_Type )
	{
		case PVARIANT_TYPE:
			*m_pVariant = varSrc;
			return *this;

		default:
			switch( varSrc.m_Type )
			{
				case NULL_TYPE:		DeleteValue(); m_Type = varSrc.m_Type; m_nValue = 0;  break;
				case PSTR_TYPE:		DeleteValue(); m_Type = varSrc.m_Type; m_pszValue = strdup(varSrc.m_pszValue); break;
				case INT_TYPE:		DeleteValue(); m_Type = varSrc.m_Type; m_nValue = varSrc.m_nValue; break;
				case UINT_TYPE:		DeleteValue(); m_Type = varSrc.m_Type; m_uValue = varSrc.m_uValue; break;
				case DOUBLE_TYPE:	DeleteValue(); m_Type = varSrc.m_Type; m_dValue = varSrc.m_dValue; break;
				case POBJECT_TYPE:  DeleteValue(); 
									m_Type = varSrc.m_Type; 
									m_pObject = varSrc.m_pObject;
									m_pObject->AddRef();
									m_fReadOnly = varSrc.m_fReadOnly;
									break;
				case PVARIANT_TYPE: {
										//can't assign a Variant to itself
										if( (LPVOID)varSrc.m_pVariant == (LPVOID)this ) break;

										DeleteValue(); 
										m_Type = varSrc.m_Type; 

                                        //get a pointer to the actual variant
                                        Variant* pVar = varSrc.m_pVariant;
                                        while( pVar && pVar->m_Type == PVARIANT_TYPE ) pVar = pVar->m_pVariant;

										if( pVar == NULL )
										{
											m_Type = INT_TYPE;
											m_nValue = 0;
											m_fReadOnly = FALSE;
										}
										else
										{
											//store the variant
											switch( pVar->m_Type )
											{
												//store the value in the pVariant, not the pointer
												case NULL_TYPE:		m_nValue = 0;  break;
												case PSTR_TYPE:     m_pszValue = strdup(pVar->m_pszValue); break;
												case INT_TYPE:		m_nValue = pVar->m_nValue; break;
												case UINT_TYPE:		m_uValue = pVar->m_uValue; break;
												case DOUBLE_TYPE:	m_dValue = pVar->m_dValue; break;
												case POBJECT_TYPE:  m_pObject = pVar->m_pObject;
																	m_pObject->AddRef();
																	break;
											}
											m_Type = pVar->m_Type;
											m_fReadOnly = FALSE;
										}
                                        break;
                                    }
			}
			break;
	}
	return *this;
}

const Variant& Variant::operator=(const LPCSTR lpszSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
	DeleteValue();
	m_pszValue = strdup(lpszSrc); 
    
	m_Type = PSTR_TYPE;
	return *this;
}

const Variant& Variant::operator=(int nSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
    DeleteValue();
	m_Type = INT_TYPE;
	m_nValue = nSrc;

	return *this;
}

const Variant& Variant::operator=(unsigned int uSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
    DeleteValue();
	m_Type = UINT_TYPE;
	m_uValue = uSrc;

	return *this;
}

const Variant& Variant::operator=(double dblSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
    DeleteValue();
	m_Type = DOUBLE_TYPE;
	m_dValue = dblSrc;

	return *this;
}

const Variant& Variant::operator=(const Variant* varSrc)
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
    DeleteValue();

	//get a pointer to the actual variant, not the variant pointer
	Variant* pVar = varSrc->m_pVariant;
	while( pVar && pVar->m_Type == PVARIANT_TYPE ) pVar = pVar->m_pVariant;

	if( pVar == NULL )
	{
		m_nValue = 0;
		m_Type == NULL_TYPE;
		m_fReadOnly = FALSE;
	}
	else
	{
		//copy the data, not the pointer
		m_fReadOnly = pVar->m_fReadOnly;
		m_Type = pVar->m_Type;
		switch( m_Type )
		{
			case NULL_TYPE:		m_nValue = 0;  break;
			case PSTR_TYPE:		m_pszValue = pVar->m_pszValue; break;
			case INT_TYPE:		m_nValue = pVar->m_nValue; break;
			case UINT_TYPE:		m_uValue = pVar->m_uValue; break;
			case DOUBLE_TYPE:	m_dValue = pVar->m_dValue; break;
			case PVARIANT_TYPE: return FALSE;
			case POBJECT_TYPE:  m_pObject = pVar->m_pObject;
								m_pObject->AddRef();
								break;
			default:			return FALSE;
		}
	}

	return *this;
}

const Variant& Variant::operator=(const LPCSCRIPTOBJECT pObject )
{
    if( m_fReadOnly ) throw VALUE_IS_READ_ONLY;
    DeleteValue();

    m_Type = POBJECT_TYPE;
	m_pObject = (CScriptObject*)pObject;
	m_pObject->AddRef();

	return *this;
}

/* type casts */
inline Variant::operator int() const
{
	switch( m_Type )
	{
		case NULL_TYPE:		return 0;
		case PSTR_TYPE:		return atoi(m_pszValue);
		case INT_TYPE:		return m_nValue;
		case UINT_TYPE:		return m_uValue;
		case DOUBLE_TYPE:	return (int)m_dValue;
		case PVARIANT_TYPE: return int(*m_pVariant);
        case POBJECT_TYPE:  return int(*m_pObject);
		default: return 0;
	}
}

inline Variant::operator unsigned int() const
{
	switch( m_Type )
	{
		case NULL_TYPE:		return 0;
		case PSTR_TYPE:		return atoi(m_pszValue);
		case INT_TYPE:		return abs(m_nValue);
		case UINT_TYPE:		return m_uValue;
		case DOUBLE_TYPE:	return (unsigned int)m_dValue;
		case PVARIANT_TYPE: return unsigned int(*m_pVariant);
        case POBJECT_TYPE:  return unsigned int(*m_pObject);
		default: return 0;
	}
}

inline Variant::operator double() const
{
	switch( m_Type )
	{
		case NULL_TYPE:		return 0.0;
		case PSTR_TYPE:		return atof(m_pszValue);
		case INT_TYPE:		return (double)m_nValue;
		case UINT_TYPE:		return (double)m_uValue;
		case DOUBLE_TYPE:	return m_dValue;
		case PVARIANT_TYPE: return double(*m_pVariant);
        case POBJECT_TYPE:  return double(*m_pObject);
		default: return 0;
	}
}

inline Variant::operator LPCSTR() const
{
	static char pszBuffer[64]; //temporary buffer for numeric cast

	switch( m_Type )
	{
		case NULL_TYPE:	return "null";
		case PSTR_TYPE:	return m_pszValue;
		case INT_TYPE:	{ sprintf( pszBuffer, "%d", m_nValue ); return pszBuffer; }
		case UINT_TYPE:	{ sprintf( pszBuffer, "%ud", m_uValue ); return pszBuffer; }
		case DOUBLE_TYPE: { 
                              sprintf( pszBuffer, "%f", (float)m_dValue ); 

                              //strip trailing zeros, and the . if needed
                              if( strlen( pszBuffer ) > 0 )
                              {
                                  char* pszZeros = &pszBuffer[ strlen(pszBuffer)-1 ];
                                  while( pszZeros && *pszZeros == '0' && pszZeros > pszBuffer )
                                  {
                                      *pszZeros = '\0';
                                      pszZeros--;
                                  }
                                  if( pszZeros && *pszZeros == '.' ) *pszZeros = '\0';
                              }
                              return pszBuffer;
                          }

		case PVARIANT_TYPE: return LPCSTR(*m_pVariant);
        case POBJECT_TYPE:  return LPCSTR(*m_pObject);
		default: return 0;
	}
}

Variant::operator LPCSCRIPTOBJECT() const
{
	switch( m_Type )
	{
		case NULL_TYPE:		return NULL;
		case PSTR_TYPE:		return NULL;
		case INT_TYPE:		return NULL;
		case UINT_TYPE:		return NULL;
		case DOUBLE_TYPE:	return NULL;
		case PVARIANT_TYPE: return LPCSCRIPTOBJECT(*m_pVariant);
		case POBJECT_TYPE:  return m_pObject;
		default: return NULL;
	}
}

inline void Variant::DeleteValue()
{
	//free anything allocated
	switch( m_Type )
	{
		case PSTR_TYPE:
			if( m_pszValue ) delete[] m_pszValue; 
			break;

		case POBJECT_TYPE:
			if( m_pObject ) m_pObject->RemoveRef();
			break;

        case PVARIANT_TYPE:
            if( m_pVariant->m_Type == POBJECT_TYPE ) m_pVariant->m_pObject->RemoveRef();
            break;
	}

    m_pValue = NULL;
    m_fReadOnly = FALSE;
}

Variant Variant::GetNULL()
{
	Variant v;
	v.m_Type = NULL_TYPE;
	v.m_nValue = 0;

	return v;
}

const Variant::ValueType Variant::GetActualType() const
{
    switch( m_Type )
    {
        case NULL_TYPE:
        case PSTR_TYPE:
        case INT_TYPE:
        case UINT_TYPE:
        case DOUBLE_TYPE:
        case POBJECT_TYPE:
            return m_Type;

        case PVARIANT_TYPE:
            return m_pVariant->GetActualType();

        default:
            return NULL_TYPE;
    }
}

void Variant::Become( const Variant& varSrc )
{
	m_Type = varSrc.m_Type;
	m_fReadOnly = FALSE;

	switch( m_Type )
	{
		case NULL_TYPE:     m_nValue = 0; break;
		case PSTR_TYPE:		m_pszValue = strdup((char*)varSrc.m_pszValue); break;
		case INT_TYPE:		m_nValue = varSrc.m_nValue; break;
		case UINT_TYPE:		m_uValue = varSrc.m_uValue; break;
		case DOUBLE_TYPE:	m_dValue = varSrc.m_dValue; break;
		case PVARIANT_TYPE: m_pVariant = varSrc.m_pVariant; 
 							m_fReadOnly = varSrc.m_fReadOnly;
							if( m_pVariant == NULL )
							{
								m_Type = NULL_TYPE;
								m_nValue = 0;
								m_fReadOnly = FALSE;
							}
							else
								if( m_pVariant->m_Type == POBJECT_TYPE ) m_pVariant->m_pObject->AddRef();
                            break;
		case POBJECT_TYPE:  m_pObject = varSrc.m_pObject; 
                            m_pObject->AddRef();
 							m_fReadOnly = varSrc.m_fReadOnly;
                            break;
		default:			break;
	}
}

