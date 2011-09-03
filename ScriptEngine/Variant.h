// Variant.h: interface for the Variant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIANT_H__D0AEF643_876C_11D1_89C0_00001C192944__INCLUDED_)
#define AFX_VARIANT_H__D0AEF643_876C_11D1_89C0_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define INVALID_OPERATOR_INT		-1
#define INVALID_OPERATOR_UINT		-2
#define INVALID_OPERATOR_FLOAT		-3
#define INVALID_OPERATOR_STRING		-4
#define INVALID_OPERATOR_OBJECT		-5
#define ARRAY_INDEX_OUT_OF_BOUNDS   -6
#define VALUE_IS_READ_ONLY          -7
#define OUT_OF_MEMORY				-8
#define DIVIDE_BY_ZERO				-9

#include "ScriptObject.h"


class Variant  
{
    friend class CScriptObject;

// Constructors
public:
	Variant();

	Variant(const Variant& varSrc);

	Variant(LPCSTR lpszSrc);
	Variant(int nSrc);
	Variant(unsigned int uSrc );
	Variant(double dblSrc);
	Variant(Variant* pVariant );
	Variant(LPCSCRIPTOBJECT pObject );

	static Variant GetNULL();

    void Become( const Variant& varSrc );

// Operations
public:
	BOOL IsNULL() const;
	BOOL IsNumeric() const;
	BOOL IsObject() const;
	BOOL IsString() const;

	//evaluation operators
	BOOL operator==(const Variant& varSrc) const;
	BOOL operator!=(const Variant& varSrc) const;
	BOOL operator<(const Variant& varSrc) const;
	BOOL operator<=(const Variant& varSrc) const;
	BOOL operator>=(const Variant& varSrc) const;
	BOOL operator>(const Variant& varSrc) const;
	BOOL operator&&(const Variant& varSrc) const;
	BOOL operator||(const Variant& varSrc) const;

	//arithmetic operators
	Variant operator+(const Variant& varSrc) const;
	Variant operator-(const Variant& varSrc) const;
	Variant operator*(const Variant& varSrc) const;
	Variant operator/(const Variant& varSrc) const;
	Variant operator%(const Variant& varSrc) const;
	Variant operator&(const Variant& varSrc) const;
	Variant operator|(const Variant& varSrc) const;
	Variant operator^(const Variant& varSrc) const;

	//prefix operators
	Variant operator-();
	Variant operator!();
	Variant operator~();

	//other operators
	Variant& operator++();
	Variant& operator--();
	Variant operator++(int);
	Variant operator--(int);
	Variant& operator<<(int nBits) const;
	Variant& operator>>(int nBits) const;
    PVARIANT operator[]( UINT nPos ) const;

	//assignment operators
	const Variant& operator+=(const Variant& varSrc);
	const Variant& operator-=(const Variant& varSrc);
	const Variant& operator*=(const Variant& varSrc);
	const Variant& operator/=(const Variant& varSrc);
	const Variant& operator%=(const Variant& varSrc);
	const Variant& operator&=(const Variant& varSrc);
	const Variant& operator|=(const Variant& varSrc);
	const Variant& operator^=(const Variant& varSrc);
	const Variant& operator>>=(int nBits);
	const Variant& operator<<=(int nBits);

	//assignments
	const Variant& operator=(const Variant& varSrc);
	const Variant& operator=(const LPCSTR lpszSrc);
	const Variant& operator=(int nSrc);
	const Variant& operator=(unsigned int uSrc );
	const Variant& operator=(double dblSrc);
	const Variant& operator=(const Variant* varSrc);
	const Variant& operator=(LPCSCRIPTOBJECT pObject );

	//type casts
	inline operator int() const;
	inline operator unsigned int() const;
	inline operator double() const;
	inline operator LPCSTR() const;
	operator LPCSCRIPTOBJECT() const;

// Implementation
public:
	~Variant();
    void SetReadOnly( BOOL fReadOnly );

private:
	void DeleteValue();
	enum ValueType { NULL_TYPE, PSTR_TYPE, INT_TYPE, UINT_TYPE, DOUBLE_TYPE, PVARIANT_TYPE, POBJECT_TYPE };
    
    const ValueType GetActualType() const;


	//private constructor - string concatenate
	Variant(LPCSTR lpszSrc, LPCSTR lpszSrc2);


    BOOL m_fReadOnly;
	ValueType m_Type;

	union {
		char*			m_pszValue;
		int				m_nValue;
		unsigned int	m_uValue;
		double			m_dValue;
		Variant*		m_pVariant;
		CScriptObject*	m_pObject;

		void*			m_pValue;	//generic value pointer
	};
};

#endif // !defined(AFX_VARIANT_H__D0AEF643_876C_11D1_89C0_00001C192944__INCLUDED_)
