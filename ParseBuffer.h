// ParseBuffer.h: interface for the CParseBuffer class.
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

#if !defined(AFX_PARSEBUFFER_H__121EFEC2_4E7F_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_PARSEBUFFER_H__121EFEC2_4E7F_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CParseBuffer : public CObject  
{
public:
    //initialisation
	CParseBuffer( const char* pszBuffer );
	virtual ~CParseBuffer();
	void Reset( const char* pszBuffer );

    //settings
	void IgnoreLineFeeds( BOOL fIgnore = TRUE );

    //status query
	BOOL IsEmpty();
	BOOL IsParseError();
	BOOL IsUnderflow();

    //general buffer information
	int GetLength();
    int GetTotalLength();
	char PeekNextChar();

    //buffer reading
	void SkipToNextLine();
	const char* ExtractSurroundedString( char chStart, char chEnd );
	const char* ExtractLine();
	const char* ExtractUnquotedString();
	const char* ExtractQuotedString();
	int ExtractInteger();
	
protected:
	BOOL m_fIgnoreCRLF;
	void SkipWhitespaceLines();

private:
	void ParseError( CString strMessage );
	BOOL m_fParseError;
	BOOL m_fUnderflow;
	char* m_pszBuffer;
    char* m_pszWholeBuffer;
    int m_nTotalLength;
};

#endif // !defined(AFX_PARSEBUFFER_H__121EFEC2_4E7F_11D2_9FF9_00001C192944__INCLUDED_)
