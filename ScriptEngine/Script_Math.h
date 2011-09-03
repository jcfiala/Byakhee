// Math.h: interface for the CMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATH_H__ABCADC05_BC27_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_MATH_H__ABCADC05_BC27_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptObject.h"
#include "Variant.h"

class CMath : public CScriptObject
{
public:
	CMath();
	virtual ~CMath();

	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );
private:
	Variant m_PI, m_E;
};

#endif // !defined(AFX_MATH_H__ABCADC05_BC27_11D1_89C0_00001C192944__INCLUDED_)
