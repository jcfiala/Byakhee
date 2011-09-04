// SectionFile.h: interface for the CSectionFile class.
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

#if !defined(AFX_SECTIONFILE_H__27F9ED42_4BFF_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SECTIONFILE_H__27F9ED42_4BFF_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSectionFile : public CObject  
{
public:
	CString GetKeyPairValueFromSection( CString strSection, CString strName, CString strDefault );
	CString GetKeyPairValue( CString strKeyPairSection, CString strName, CString strDefault );
	void SectionFileError( CString strMessage );
	CString GetSection( CString strSectionName );
	BOOL Open( CString strFileName );
	CSectionFile();
	virtual ~CSectionFile();

protected:
	char* AdvanceTo( CString strWhat, char* pszRead, BOOL fSkipOver );
	CString m_strFileName;
	char* m_pszBuffer;
};

#endif // !defined(AFX_SECTIONFILE_H__27F9ED42_4BFF_11D2_9FF9_00001C192944__INCLUDED_)
