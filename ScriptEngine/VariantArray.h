// VariantArray.h: interface for the VariantArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIANTARRAY_H__9C341843_9B4A_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_VARIANTARRAY_H__9C341843_9B4A_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptObject.h"
#include "Variant.h"

class VariantArray : public CScriptObject
{
public:
	VariantArray();
    VariantArray( UINT uSize );
    VariantArray( Variant* pValues, int nValues );
	virtual ~VariantArray();

	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );
	static MEMBERRESULT Constructor( Variant* pParameters, int nParameters, Variant& returnValue );

    virtual PVARIANT operator[]( UINT uPos ) const;
	virtual inline operator LPCSTR() const;

    int GetSize() { return int(m_Size); }

private:
	void SetSize();
    BOOL m_fInLPCSTR;
    Variant* m_Array;
    Variant m_Size;
    int m_nSize;
};

#endif // !defined(AFX_VARIANTARRAY_H__9C341843_9B4A_11D1_89C0_00001C192944__INCLUDED_)
