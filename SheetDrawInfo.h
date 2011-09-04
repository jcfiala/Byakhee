// SheetDrawInfo.h: interface for the CSheetDrawInfo class.
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

#if !defined(AFX_SHEETDRAWINFO_H__CA4F7944_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_SHEETDRAWINFO_H__CA4F7944_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FONTSTYLE_NORMAL        0
#define FONTSTYLE_BOLD          1
#define FONTSTYLE_ITALIC        2
#define FONTSTYLE_UNDERLINE     4
#define FONTSTYLE_STRIKETHROUGH 8

#define TEXTSTYLE_NORMAL        0
#define TEXTSTYLE_ALIGNLEFT     0
#define TEXTSTYLE_ALIGNRIGHT    1
#define TEXTSTYLE_ALIGNCENTER   2
#define TEXTSTYLE_MULTILINE     4
#define TEXTSTYLE_CLIPPED       8
#define TEXTSTYLE_FIELDUNDERLINE 16
#define TEXTSTYLE_USERTEXT      32

#include "Field.h"
#include "SheetCommand.h"
#include <afxstr.h>
#include <atlimage.h>

class CFieldManager;

class CSheetDrawInfo : public CObject  
{
public:
	CSheetDrawInfo( CFieldManager* pFieldManager, CDC* pDC, CSize sizeSheet );
	virtual ~CSheetDrawInfo();

	void DrawCheckBox( CRect rc, BOOL fChecked );
	void DeselectUserPen();
	void DeselectUserText();
    void SelectUserPen();
    void SelectUserText();
    void ChangeUserText( int nPointSize, int nRotation = 0 );

    void ChangePen( COLORREF crColour );
    void ChangeFont( CString strFaceName, int nPointSize, int nFontStyle = 0, int nRotation = 0, int nWeight = 0 );
    void DrawPicture( CString strFileName, CRect rc );
    void StretchPicture( CString strFileName, CRect rc );
	void DrawImage( CImage* pImage, CRect rc );
    void DrawText( CString strText, CRect rc, DWORD dwDrawFlags );
    void DrawText( CString strText, CPoint pt, DWORD dwDrawFlags );

    DWORD m_dwFieldFormatting;
    int m_nCurrentTextAngle;
    int m_nCurrentUserTextSize, m_nCurrentUserTextAngle;
    int m_nShadowOffset;

    COLORREF m_crLine;
    COLORREF m_crText;
    CFont* m_pFont;
    CPen* m_pPen;

    CFont* m_pOldFont;
    CPen* m_pOldPen;

    CFont* m_pUserFont;
    CPen* m_pUserPen;
    int m_nUserTextAngle;
    BOOL m_fUserTextSelected;

    CDC* m_pDC;

    BOOL m_fUserTextItalic;
    int m_nUserTextWeight;
	COLORREF m_crUserText;
	CString m_strUserTextFace;

    CSize m_sizeSheet;

    class CFontSheetCommand* m_pLastUsedFont;
    class CDrawSheetCommand* m_pLastFontColour;
    class CDrawSheetCommand* m_pLastFieldStyle;

    CFieldManager* m_pFieldManager;
};

#endif // !defined(AFX_SHEETDRAWINFO_H__CA4F7944_A4A9_11D2_A50C_00105A4ACC2C__INCLUDED_)
