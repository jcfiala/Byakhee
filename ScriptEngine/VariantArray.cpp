// VariantArray.cpp: implementation of the VariantArray class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _AFXDLL
#include "../stdafx.h"
#endif
#include "VariantArray.h"


#define MAX_ARRAY_SIZE 0xFFFF //64K array buffer

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VariantArray::VariantArray()
	:CScriptObject()
{
    m_Array = NULL;
    m_Size = m_nSize = 0;
    m_fInLPCSTR = FALSE;
}

VariantArray::VariantArray( UINT uSize )
	:CScriptObject()
{
    if( uSize > MAX_ARRAY_SIZE ) throw OUT_OF_MEMORY;
    m_Array = new Variant[uSize];
	if( m_Array == NULL ) throw OUT_OF_MEMORY;
    m_Size = m_nSize = uSize;
    m_fInLPCSTR = FALSE;
}

VariantArray::VariantArray( Variant* pValues, int nValues )
	:CScriptObject()
{
    if( nValues > MAX_ARRAY_SIZE ) throw OUT_OF_MEMORY;
    m_Array = new Variant[nValues];
	if( m_Array == NULL ) throw OUT_OF_MEMORY;
    m_Size = m_nSize = nValues;

    for( int i = 0; i < nValues; i++ )
    {
        m_Array[i] = pValues[i];
    }
    m_fInLPCSTR = FALSE;
}

VariantArray::~VariantArray()
{
    if( m_Array ) 
    {
        delete[] m_Array;
    }
}

CScriptObject::MEMBERRESULT VariantArray::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
    if( strcmp( pszName, "sort" ) == 0 )
    {
        if( nParameters == 0 )
        {
            //sort the array
			int nSize = m_Size;
			Variant v;
			SetSize();

			while( nSize > 1 )
			{
				for( int nPos = 0; nPos < (nSize-1); nPos++ )
				{
					if( m_Array[nPos] > m_Array[nPos+1] )
					{
						memcpy( &v, &m_Array[nPos], sizeof(Variant) );
						memcpy( &m_Array[nPos], &m_Array[nPos+1], sizeof(Variant) );
						memcpy( &m_Array[nPos+1], &v, sizeof(Variant) );
					}
				}
				nSize--;
			}
			return OK;
        }
        else return TOO_MANY_PARAMS;
    }

    if( strcmp( pszName, "reverse" ) == 0 )
    {
        if( nParameters == 0 )
        {
			int nSize = m_Size;
			Variant v;
			SetSize();

			for( int nPos = 0; nPos < nSize/2; nPos++ )
			{
				memcpy( &v, &m_Array[nPos], sizeof(Variant) );
				memcpy( &m_Array[nPos], &m_Array[nSize-nPos-1], sizeof(Variant) );
				memcpy( &m_Array[nSize-nPos-1], &v, sizeof(Variant) );
			}
			return OK;
        }
        else
            return TOO_MANY_PARAMS;
    }

    return NOT_FOUND;
}

Variant* VariantArray::GetMemberVariable( char* pszName )
{
    if( strcmp( pszName, "length" ) == 0 )
        return &m_Size;
    else
        return NULL;
}

CScriptObject::MEMBERRESULT VariantArray::Constructor( Variant* pParameters, int nParameters, Variant& returnValue )
{
    switch( nParameters )
    {
        case 0:
        {
            //new, empty array
            VariantArray* pArray = new VariantArray();
            returnValue = pArray;
            return OK;
            break;
        }

        case 1:
        {
            //parameter 1 contains either the start size, or an element
            if( pParameters[0].IsNumeric() )
            {
                //start size
                VariantArray* pArray = new VariantArray( unsigned int(pParameters[0]) );
                returnValue = pArray;
                return OK;
            }
            //else drop through to default - parameters are elements
        }

        default:
        {
            //parameters contain the default elements
            VariantArray* pArray = new VariantArray( pParameters, nParameters );
            returnValue = pArray;
            return OK;
        }
    }

    //shouldn't reach here!
    return NOT_FOUND;
}

PVARIANT VariantArray::operator[]( UINT uPos ) const
{
	//the pThis hack gets around the fact that a [] operator *must* be const
	//but the VariantArray needs to resize on the fly
    VariantArray* pThis = (VariantArray*)this;

	if( uPos >= unsigned int(m_Size) ) pThis->m_Size = (uPos+1);
	pThis->SetSize();

    if( m_Array )
        return &m_Array[uPos];
    else
        return NULL; //something bad has happened!
}

inline VariantArray::operator LPCSTR() const
{
    return "Array";
    /* FIXME: can't be arsed to convert this from MFC...
    if( m_fInLPCSTR )
        return "";
    else
    {
        static CString s_strReturn;
        CString strValue = "";
        int nItem, nSize;

        ((VariantArray*)this)->m_fInLPCSTR = TRUE;

        nSize = m_Size;
        if( nSize )
        {
            for( nItem = 0; nItem < nSize; nItem++ )
            {
                strValue = strValue + LPCSTR(m_Array[nItem]) + ",";
            }

            //remove last ',' during copy
            s_strReturn = strValue.Left( strValue.GetLength()-1 );
        }
        else
            s_strReturn = "";

        ((VariantArray*)this)->m_fInLPCSTR = FALSE;

        return LPCSTR(s_strReturn);
    }
    */
}


void VariantArray::SetSize()
{
	if( m_Array )
	{
		if( unsigned int(m_Size) != m_nSize )
		{
			int nNewSize = unsigned int(m_Size)+1;

			//allocate a new array
            if( nNewSize > MAX_ARRAY_SIZE ) throw OUT_OF_MEMORY;
			Variant* pNewArray = new Variant[ nNewSize ];
			if( pNewArray == NULL ) throw OUT_OF_MEMORY;

			//copy the elements over
			int sz;
			if( unsigned int(m_Size) > m_nSize ) sz = m_nSize; else sz = m_Size;
			for( int n = 0; n < sz; n++ )
			{
				pNewArray[n] = m_Array[n];
			}
			for( n = sz; n < nNewSize; n++ )
			{
				pNewArray[n] = 0;
			}

			//remove existing array and store new one
			delete[] m_Array;
			m_Array = pNewArray;

			//store new size
			m_Size = m_nSize = nNewSize;
		}
	}
	else
	{
		//create a new array
		int nNewSize = unsigned int(m_Size)+1;

        if( nNewSize > MAX_ARRAY_SIZE ) throw OUT_OF_MEMORY;
		m_Array = new Variant[ nNewSize ];
		if( m_Array == NULL ) throw OUT_OF_MEMORY;
		for( int n = 0; n < nNewSize; n++ ) m_Array[n] = 0;
		m_nSize = nNewSize;
	}
}
