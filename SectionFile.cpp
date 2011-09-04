// SectionFile.cpp: implementation of the CSectionFile class.
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

#include "stdafx.h"
#include "Byakhee.h"
#include "SectionFile.h"
#include "ParseBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSectionFile::CSectionFile()
{
	m_pszBuffer = NULL;
}

CSectionFile::~CSectionFile()
{
	GlobalFree( (HANDLE)m_pszBuffer );
}

BOOL CSectionFile::Open(CString strFileName)
{
	//store this file name
	m_strFileName = strFileName;

	//try and open the file
    CFile File;
	if( !File.Open( strFileName, CFile::modeRead) ) 
    {
		SectionFileError( "Cannot open file" );
        return FALSE;
    }

	//determine file size
	DWORD dwLength = File.GetLength();
	if( dwLength == 0 ) 
	{
		SectionFileError( "File is empty" );
		return FALSE;
	}

	//allocate a buffer
	m_pszBuffer = (char*)GlobalAlloc( GPTR, dwLength+1 );
	if( m_pszBuffer == NULL )
	{
		SectionFileError( "Cannot allocate buffer" );
		return FALSE;
	}

	//load the whole file
	if( File.Read( m_pszBuffer, dwLength ) < dwLength ) 
	{ 
		GlobalFree( (HANDLE) m_pszBuffer ); m_pszBuffer = NULL;
		SectionFileError( "Unexpected end of file" );
		return FALSE;
	}

	//add a terminator at the end of the string
	m_pszBuffer[dwLength] = '\0';

	return TRUE;
}

CString CSectionFile::GetSection(CString strSectionName)
{
	//make sure a buffer is loaded
	if( m_pszBuffer == NULL )
	{
		SectionFileError( "File not loaded" );
		return "";
	}

    //copy section name and translate spaces into underscores
    CString strValidSectionName = strSectionName;
    for( int i = 0; i < strValidSectionName.GetLength(); i++ ) if( strValidSectionName.GetAt(i) == ' ' ) strValidSectionName.SetAt(i, '_');

    //get a pointer to the start of the section
    char* pszStart = AdvanceTo( CString("_BEGIN ") + strValidSectionName, m_pszBuffer, TRUE );
    if( pszStart == NULL ) return "";

    //get a pointer to the end of the section
    char* pszEnd = AdvanceTo( CString("_END ") + strValidSectionName, pszStart, FALSE );
    if( pszEnd == NULL )
	{
		SectionFileError( CString("Could not find matching _END for section _BEGIN ") + CString(strValidSectionName) );
		return "";
	}

	//terminate buffer at current point, storing old character
    *pszEnd = '\0';

    //create a CString for the requested section
	CString strSection = pszStart;

	//restore character at the end of the current point and return it
	*pszEnd = '_'; //we know it's an _ because _END xxx marks the end of the section
	return strSection;
}

void CSectionFile::SectionFileError(CString strMessage)
{
	//display a message box
	if( m_strFileName.GetLength() > 0 )
		AfxMessageBox( m_strFileName + CString(" - ") + strMessage, MB_ICONERROR );
	else
		AfxMessageBox( strMessage, MB_ICONERROR );
}

CString CSectionFile::GetKeyPairValue(CString strKeyPairSection, CString strName, CString strDefault)
{
    //parse it for key value pairs
    CParseBuffer Buffer(strKeyPairSection);

    //loop until the buffer is empty
	while( Buffer.IsEmpty() == FALSE )
	{
		//parse values out of buffer
		CString strKey = Buffer.ExtractQuotedString();
		CString strValue = Buffer.ExtractQuotedString();

        //check for parse error
        if( Buffer.IsParseError() == FALSE )
        {
            //was there enough data to create this?
            if( Buffer.IsUnderflow() == FALSE )
            {
                if( strKey.CompareNoCase(strName) == 0 ) return strValue;
            }
            else
            {
                //buffer underflow
                AfxMessageBox( "Unexpected end of keypair section" );
                break;
            }
        }
        else
        {
            //parse error
            break;
        }

        //skip to the next line
        Buffer.SkipToNextLine();
	}

    return strDefault;

}

CString CSectionFile::GetKeyPairValueFromSection(CString strSection, CString strName, CString strDefault)
{
    return GetKeyPairValue( GetSection(strSection), strName, strDefault );
}

char* CSectionFile::AdvanceTo(CString strWhat, char *pszRead, BOOL fSkipOver )
{
    char chFirst = *LPCSTR(strWhat);

    while( *pszRead != '\0' )
    {
        //find an underscore
        while( *pszRead != chFirst && *pszRead != '\0' ) pszRead++;

        //see if it's a "begin"
        if( *pszRead == chFirst && strnicmp( pszRead, strWhat, strWhat.GetLength() ) == 0 )
        {
            //skip over this line if required
            if( fSkipOver ) 
            {
                pszRead = strchr(pszRead, '\n' );
                if( pszRead ) pszRead++;
            }

            //return what's left
            return pszRead;
        }
        else
            pszRead++;
    }

    return NULL;

}
