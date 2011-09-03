// ScriptObject.cpp: implementation of the CScriptObject class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include <math.h>
#include "ScriptObject.h"
#include "Variant.h"

extern HWND g_hWndOwner;

CScriptObject::CScriptObject()
{
    m_iReferenceCount = 0;
}

CScriptObject::~CScriptObject()
{
}

void CScriptObject::AddRef()
{
	//increment the reference counter
	m_iReferenceCount++;
}

void CScriptObject::RemoveRef()
{
	//decrement the reference counter
	m_iReferenceCount--;

	//if there are no references, delete this object
	if( m_iReferenceCount <= 0 )
	{
		delete this;
	}
}

CScriptObject::MEMBERRESULT CScriptObject::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
	return NOT_FOUND;
}

Variant* CScriptObject::GetMemberVariable( char* pszName )
{
	return NULL;
}

CScriptObject::MEMBERRESULT CScriptObject::Constructor( Variant* pParameters, int nParameters, Variant& returnValue )
{
    MessageBox( g_hWndOwner, "Cannot create objects of this type", "Script Error", MB_ICONEXCLAMATION|MB_TASKMODAL );
	returnValue = 0;
    return UNKNOWN_ERROR;
}

//evaluation operators
BOOL CScriptObject::operator==(const Variant& varSrc) const
{
    if( varSrc.m_Type == Variant::POBJECT_TYPE )
        return this == varSrc.m_pObject;
	else
		return FALSE;
}

BOOL CScriptObject::operator!=(const Variant& varSrc) const
{
    if( varSrc.m_Type == Variant::POBJECT_TYPE )
        return this != varSrc.m_pObject;
	else
		return FALSE;
}

BOOL CScriptObject::operator<(const Variant& varSrc) const
{
    return FALSE;
}

BOOL CScriptObject::operator<=(const Variant& varSrc) const
{
    return FALSE;
}

BOOL CScriptObject::operator>=(const Variant& varSrc) const
{
    return FALSE;
}

BOOL CScriptObject::operator>(const Variant& varSrc) const
{
    return FALSE;
}

BOOL CScriptObject::operator&&(const Variant& varSrc) const
{
    return FALSE;
}

BOOL CScriptObject::operator||(const Variant& varSrc) const
{
    return FALSE;
}


//arithmetic operators
Variant CScriptObject::operator+(const Variant& varSrc) const
{
    const char* psz1 = LPCSTR(this);
    const char* psz2 = LPCSTR(varSrc);
    char* pszResult = new char[strlen(psz1)+strlen(psz2)+1];
    strcpy( pszResult, psz1 );
    strcat( pszResult, psz2 );

    Variant result( pszResult );
    delete[] pszResult;

    return result;
}

Variant CScriptObject::operator-(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator*(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator/(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator%(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator&(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator|(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator^(const Variant& varSrc) const
{
    return fmod( 0.0, 0.0 ); //NaN
}


//prefix operators
Variant CScriptObject::operator-()
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator!()
{
    return fmod( 0.0, 0.0 ); //NaN
}

Variant CScriptObject::operator~()
{
    return fmod( 0.0, 0.0 ); //NaN
}


//other operators
Variant g_tmpV;
Variant& CScriptObject::operator++()
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}

Variant& CScriptObject::operator--()
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}

Variant CScriptObject::operator++(int)
{
    throw INVALID_OPERATOR_OBJECT;
    return 0;
}

Variant CScriptObject::operator--(int)
{
    throw INVALID_OPERATOR_OBJECT;
    return 0;
}

Variant& CScriptObject::operator<<(int nBits) const
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}

Variant& CScriptObject::operator>>(int nBits) const
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}

PVARIANT CScriptObject::operator[]( UINT nPos ) const
{
    throw INVALID_OPERATOR_OBJECT;
    return NULL;
}

const Variant& CScriptObject::operator>>=(int nBits)
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}

const Variant& CScriptObject::operator<<=(int nBits)
{
    throw INVALID_OPERATOR_OBJECT;
    return g_tmpV;
}


//type casts
CScriptObject::operator int() const
{
    return 0;
}

CScriptObject::operator unsigned int() const
{
    return 0;
}

CScriptObject::operator double() const
{
    return 0;
}

CScriptObject::operator LPCSTR() const
{
    static char* szBuffer = "[object]";
    return szBuffer;
}
