// Field.h: interface for the CField class.
//
//////////////////////////////////////////////////////////////////////
/**
 * Copyright (C) 1998  David Harvey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if !defined(AFX_FIELD_H__CA4F7940_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_FIELD_H__CA4F7940_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ScriptEngine/ScriptObject.h"
#include "ScriptEngine/Variant.h"
#include "SheetCommand.h"
#include "SheetDrawInfo.h"

//field property bit-fields
#define FIELD_NORMAL        0
#define FIELD_READONLY      1
#define FIELD_SCRIPTREAD    2
#define FIELD_SCRIPTWRITE   4
#define FIELD_TABSTOP       8
#define FIELD_NOTACTIVE     16
#define FIELD_CALCRELIANT   32
#define FIELD_BOOLEAN       64
#define FIELD_PICTURE       128
#define FIELD_SPECIALTYPE   (FIELD_BOOLEAN|FIELD_PICTURE)


class CField;
class CSheetCommand;
class CFieldSheetCommand;
class CSheetDrawInfo;

class CFieldManager
{
public:
    virtual Variant GetFieldValue( CField* pField, CFieldSheetCommand* pFieldSheetCommand = NULL ) = 0;
    virtual void SetFieldValue( CField* pField, Variant value, CFieldSheetCommand* pFieldSheetCommand = NULL ) = 0;
    virtual void RedrawFields( CFieldSheetCommand* pFieldChanged = NULL ) = 0;
    virtual CWnd* GetFieldManagerWindow() = 0;
    virtual void FieldToWindow( CRect& rc ) = 0;
    virtual void StoreUndo() {};
};


class CFieldScriptObject : public CScriptObject , public CObject
{
public:
	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );

    CArray<CField*, CField*> m_FieldList;
    CArray<CSheetCommand*, CSheetCommand*> m_SheetCommands;
    CArray<CFieldSheetCommand*, CFieldSheetCommand*> m_HotFieldList;
};



class CField : public CObject  
{
public:
	CField( CFieldManager* pFieldManager, CString strName, DWORD dwFlags, int nCode, const char* pszOptionName = NULL );
	virtual Variant GetValue( CFieldSheetCommand* pFieldSheetCommand = NULL );
	virtual void SetValue( Variant value, CFieldSheetCommand* pFieldSheetCommand = NULL );

    const char* m_pszOptionName;

    CString m_strName;
    DWORD m_dwFlags;
    Variant m_Code;
    CFieldManager* m_pFieldManager;
};


class CStringField : public CField
{
public:
    CStringField( CFieldManager* pFieldManager, CString* pstrValue, CString strName, DWORD dwFlags, int nCode, const char* pszOptionName = NULL );
	virtual Variant GetValue( CFieldSheetCommand* pFieldSheetCommand = NULL );
	virtual void SetValue( Variant value, CFieldSheetCommand* pFieldSheetCommand = NULL );
    
    CString* m_pstrValue;
};

class CIntField : public CField
{
public:
    CIntField( CFieldManager* pFieldManager, int* pnValue, CString strName, DWORD dwFlags, int nCode );
	virtual Variant GetValue( CFieldSheetCommand* pFieldSheetCommand = NULL );
	virtual void SetValue( Variant value, CFieldSheetCommand* pFieldSheetCommand = NULL );

    int* m_pnValue;
};

#endif // !defined(AFX_FIELD_H__CA4F7940_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
