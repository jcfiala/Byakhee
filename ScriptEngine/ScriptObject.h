// ScriptObject.h: interface for the CScriptObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTOBJECT_H__355C9AC1_97F8_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_SCRIPTOBJECT_H__355C9AC1_97F8_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <typeinfo.h>
#include <windows.h>

class Variant;

typedef Variant* PVARIANT;

/* script-enabled object base class */
class CScriptObject {

public:
	CScriptObject();
    virtual ~CScriptObject();

	enum MEMBERRESULT { OK, 
						UNKNOWN_ERROR,
						NOT_FOUND, 
						TOO_FEW_PARAMS, 
						TOO_MANY_PARAMS };

	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );
	static MEMBERRESULT Constructor( Variant* pParameters, int nParameters, Variant& returnValue );

	void AddRef();
	void RemoveRef();

	//evaluation operators
	virtual BOOL operator==(const Variant& varSrc) const;
	virtual BOOL operator!=(const Variant& varSrc) const;
	virtual BOOL operator<(const Variant& varSrc) const;
	virtual BOOL operator<=(const Variant& varSrc) const;
	virtual BOOL operator>=(const Variant& varSrc) const;
	virtual BOOL operator>(const Variant& varSrc) const;
	virtual BOOL operator&&(const Variant& varSrc) const;
	virtual BOOL operator||(const Variant& varSrc) const;

	//arithmetic operators
	virtual Variant operator+(const Variant& varSrc) const;
	virtual Variant operator-(const Variant& varSrc) const;
	virtual Variant operator*(const Variant& varSrc) const;
	virtual Variant operator/(const Variant& varSrc) const;
	virtual Variant operator%(const Variant& varSrc) const;
	virtual Variant operator&(const Variant& varSrc) const;
	virtual Variant operator|(const Variant& varSrc) const;
	virtual Variant operator^(const Variant& varSrc) const;

	//prefix operators
	virtual Variant operator-();
	virtual Variant operator!();
	virtual Variant operator~();

	//other operators
	virtual Variant& operator++();
	virtual Variant& operator--();
	virtual Variant operator++(int);
	virtual Variant operator--(int);
	virtual Variant& operator<<(int nBits) const;
	virtual Variant& operator>>(int nBits) const;
    virtual PVARIANT operator[]( UINT nPos ) const;

	//assignment operators
	virtual const Variant& operator>>=(int nBits);
	virtual const Variant& operator<<=(int nBits);

	//type casts
	virtual operator int() const;
	virtual operator unsigned int() const;
	virtual operator double() const;
    virtual operator LPCSTR() const;

private:
	int m_iReferenceCount;
};

typedef CScriptObject *LPCSCRIPTOBJECT;
typedef CScriptObject::MEMBERRESULT (*PFNCONSTRUCTOR) (Variant*, int, Variant&) ;

#endif // !defined(AFX_SCRIPTOBJECT_H__355C9AC1_97F8_11D1_89C0_00001C192944__INCLUDED_)
