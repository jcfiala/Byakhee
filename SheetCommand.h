// SheetCommand.h: interface for the CSheetCommand class.
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

#if !defined(AFX_SHEETCOMMAND_H__CA4F7941_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_SHEETCOMMAND_H__CA4F7941_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SheetDrawInfo.h"
#include "Field.h"

enum SheetCommandType {
    CommandType_None,
    CommandType_SheetSize, 
    CommandType_DrawLine,
    CommandType_DrawPicture,
    CommandType_StretchPicture,
    CommandType_DrawRectangle,
    CommandType_DrawText,
    CommandType_DrawTextBox,
    CommandType_FillRectangle,
    CommandType_SetLineColour,
    CommandType_SetTextColour,
    CommandType_SetFieldTextStyle,
    CommandType_SetFont,
    CommandType_SetFontRotated,
    CommandType_SetUserTextSize,
    CommandType_SetUserTextSizeRotate,
    CommandType_Field,
    CommandType_TableEntry,
    CommandType_SetSheetParameter,
};

class CSheetDrawInfo;

class CSheetCommand : public CObject  
{
public:
    CSheetCommand( SheetCommandType Type ) { m_Type = Type; };
	virtual void Draw( CSheetDrawInfo* pDrawInfo ) = 0;

    SheetCommandType m_Type;
};

class CDrawSheetCommand : public CSheetCommand
{
public:
    CDrawSheetCommand( SheetCommandType Type, CRect rc, CString strText, DWORD dwFlags = 0 );
    CDrawSheetCommand( SheetCommandType Type, CRect rc, int nValue = 0x000000 );
    CDrawSheetCommand( SheetCommandType Type, int nValue, int nValue2 = 0 );
	virtual void Draw( CSheetDrawInfo* pDrawInfo );

    int m_nValue;
    int m_nValue2;

protected:
    CRect m_rc;
    CString m_strText;
    DWORD m_dwFlags;
};

class CFontSheetCommand : public CSheetCommand
{
public:
    CFontSheetCommand( SheetCommandType Type, CString strFace, int nPointSize, int nTextAttributes, int nAngle = 0 );
	virtual void Draw( CSheetDrawInfo* pDrawInfo );

    CString m_strFace;
    int m_nPointSize;
    int m_nTextAttributes;
    int m_nAngle;
};

class CField;
class CFieldEdit;

class CFieldSheetCommand : public CSheetCommand
{
public:
	virtual void BeginEditing( CFieldEdit* pFieldEdit );
    CFieldSheetCommand( CRect rc, CField* pField );
	virtual void Draw( CSheetDrawInfo* pDrawInfo );
	virtual void SetCursor();

    CFontSheetCommand* m_pLastUsedFont;
    COLORREF m_crFontColour;
    DWORD m_dwFieldStyle;
    int m_nCurrentUserTextSize, m_nCurrentUserTextAngle;

    CString m_strOptionName;

    BOOL m_fActive;
    DWORD m_dwParam1, m_dwParam2;
    CField* m_pField;
    CRect m_rc;
};

class CTableFieldSheetCommand : public CFieldSheetCommand
{
public:
	virtual void BeginEditing( CFieldEdit* pFieldEdit );
    CTableFieldSheetCommand( CRect rc, CField* pField, Variant value );
	virtual void Draw( CSheetDrawInfo* pDrawInfo );
	virtual void SetCursor();

protected:
    Variant m_value;
};

#endif // !defined(AFX_SHEETCOMMAND_H__CA4F7941_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
