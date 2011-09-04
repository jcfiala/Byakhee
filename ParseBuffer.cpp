// ParseBuffer.cpp: implementation of the CParseBuffer class.
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
#include "ParseBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParseBuffer::CParseBuffer( const char* pszBuffer )
{
    //initialise member variables
    m_pszWholeBuffer = NULL;
    Reset(pszBuffer);
}

CParseBuffer::~CParseBuffer()
{
    if( m_pszWholeBuffer ) delete[] m_pszWholeBuffer;
}

const char*  CParseBuffer::ExtractQuotedString()
{
    return ExtractSurroundedString( '\"', '\"' );
}

const char* CParseBuffer::ExtractUnquotedString()
{
    //skip whitespace
    SkipWhitespaceLines();

    //make sure we've got a string left!
    if( IsEmpty() == FALSE )
    {
        //find position of the next bit of whitespace (space or tab)
        char* pszEndOfSection = m_pszBuffer;
        while( *pszEndOfSection != ' ' && *pszEndOfSection != '\t' && *pszEndOfSection != '\0' && *pszEndOfSection != '\r' && *pszEndOfSection != '\n' ) pszEndOfSection++;

        if( *pszEndOfSection != '\0' )
        {
            *pszEndOfSection = '\0';
            char* pszReturn = m_pszBuffer;
            m_pszBuffer = ++pszEndOfSection;
            return pszReturn;
        }
        else
        {
            char* pszReturn = m_pszBuffer;
            m_pszBuffer = pszEndOfSection;
            return pszReturn;
        }
    }
    else
        m_fUnderflow = TRUE;

    return "";
}

const char* CParseBuffer::ExtractSurroundedString(char chStart, char chEnd)
{
    //skip whitespace
    SkipWhitespaceLines();

    //make sure we've got a string left!
    if( IsEmpty() == FALSE )
    {
        //make sure we've got a starting token
        if( *m_pszBuffer == chStart )
        {
            //skip over the first character
            m_pszBuffer++;

            //find position of the closing token
            int nSkip = 1;
            BOOL fStop = FALSE;
            char* pszEndOfSection = m_pszBuffer;
            while( fStop == FALSE )
            {
                if( chStart == chEnd ) //start and end terminators are the same
                {
                    if( m_fIgnoreCRLF )
                        fStop = !(*pszEndOfSection != chEnd && *pszEndOfSection != '\0' );
                    else
                        fStop = !(*pszEndOfSection != chEnd && *pszEndOfSection != '\0' && *pszEndOfSection != '\r' && *pszEndOfSection != '\n');
                }
                else
                {
                    if( m_fIgnoreCRLF )
                        fStop = !(*pszEndOfSection != '\0');
                    else
                        fStop = !(*pszEndOfSection != '\0' && *pszEndOfSection != '\r' && *pszEndOfSection != '\n');
                    if( !fStop )
                    {
                        if( *pszEndOfSection == chStart )
                            nSkip++;
                        else
                            if( *pszEndOfSection == chEnd )
                            {
                                nSkip--;
                                fStop = (nSkip == 0);
                            }
                    }
                }

                if( !fStop ) pszEndOfSection++;
            }

            if( *pszEndOfSection == chEnd )
            {
                *pszEndOfSection = '\0';
                char* pszReturn = m_pszBuffer;
                m_pszBuffer = ++pszEndOfSection;
                return pszReturn;
            }
            else
            {
                CString str;
                str.Format( "No closing %c to match opening %c", chEnd, chStart );
                ParseError( str );
            }
        }
        else
        {
            CString str;
            str.Format( "Expected opening %c", chStart );
            ParseError( str );
        }
    }
    else
        m_fUnderflow = TRUE;

    //return the string
    return "";
}

int CParseBuffer::ExtractInteger()
{
    //get the next token
    const char* pszNextToken = ExtractUnquotedString();

    //convert it to an integer
    return atoi(pszNextToken);
}

void CParseBuffer::SkipWhitespaceLines()
{
    BOOL fStop = FALSE;
    while( IsEmpty() == FALSE && fStop == FALSE )
    {
        //skip blank lines & whitespace
        while( *m_pszBuffer == '\r' || *m_pszBuffer == '\n' || *m_pszBuffer == '\t' || *m_pszBuffer == ' ' ) m_pszBuffer++;

        //skip comments
        if( strncmp( m_pszBuffer, "//", 2 ) == 0 )
        {
            //it's a comment - skip over it
            while( *m_pszBuffer != '\r' && *m_pszBuffer != '\n' && *m_pszBuffer != '\0' ) m_pszBuffer++;
        }
        else
            fStop = TRUE;
    }
}

int CParseBuffer::GetLength()
{
	return strlen(m_pszBuffer);
}

int CParseBuffer::GetTotalLength()
{
    return m_nTotalLength;
}

BOOL CParseBuffer::IsUnderflow()
{
    return m_fUnderflow;
}

BOOL CParseBuffer::IsParseError()
{
    return m_fParseError;
}

void CParseBuffer::ParseError(CString strMessage)
{
    //don't display more than one error
    if( m_fParseError == FALSE )
    {
        //extract the rest of the line
        char* pszEndOfLine = strchr( m_pszBuffer, '\n' );
        if( pszEndOfLine ) *pszEndOfLine = '\0';

        //build error message
        CString strErrorMessage = "Parse Error:\n";
        strErrorMessage += m_pszBuffer;
        strErrorMessage += "\n";
        strErrorMessage += strMessage;

        //repair buffer (just in case)
        if( pszEndOfLine ) *pszEndOfLine = '\n';

        //display error message
        AfxMessageBox( strErrorMessage, MB_ICONEXCLAMATION );

#ifdef _DEBUG
        DebugBreak();
#endif

        //set parse error flag
        m_fParseError = TRUE;
    }
}

char CParseBuffer::PeekNextChar()
{
    char* pszRead = m_pszBuffer;

    //skip over leading whitespace
    while( *pszRead == '\r' || *pszRead == '\n' || *pszRead == '\t' || *pszRead == ' ' || (pszRead[0] == '/' && pszRead[1] == '/') )
    {
        //skip over commented line
        if( pszRead[0] == '/' && pszRead[1] == '/' ) 
            while( *pszRead != '\r' && *pszRead != '\n' && *pszRead != '\0' ) pszRead++;
        else
            pszRead++;
    }

    return *pszRead;
}

const char* CParseBuffer::ExtractLine()
{
    //skip over whitespace lines
    SkipWhitespaceLines();

    //find the end of this line and terminate the string there
    char* pszEndOfLine = strchr( m_pszBuffer, '\n' );
    if( pszEndOfLine ) *pszEndOfLine = '\0';
    
    //get return value and advance the buffer pointer
    char* pszReturn = m_pszBuffer;
    m_pszBuffer = ++pszEndOfLine;

    return pszReturn;
}

BOOL CParseBuffer::IsEmpty()
{
    return ( m_pszBuffer == NULL || PeekNextChar() == '\0' );
}

void CParseBuffer::SkipToNextLine()
{
    while( *m_pszBuffer != '\r' && *m_pszBuffer != '\n' && *m_pszBuffer != '\0' ) m_pszBuffer++;
}

void CParseBuffer::IgnoreLineFeeds(BOOL fIgnore /*TRUE*/ )
{
    m_fIgnoreCRLF = fIgnore;
}

void CParseBuffer::Reset( const char* pszBuffer )
{
    //delete current buffer and use new one
    if( m_pszWholeBuffer ) delete[] m_pszWholeBuffer;

    m_nTotalLength = strlen(pszBuffer);
	m_pszWholeBuffer = new char[m_nTotalLength+1];
	strcpy( m_pszWholeBuffer, pszBuffer );

    //intialise the rest of the members
	m_pszBuffer = m_pszWholeBuffer;
    m_fUnderflow = FALSE;
    m_fParseError = FALSE;
    m_fIgnoreCRLF = FALSE;

    //skip commented and blank lines
    SkipWhitespaceLines();
}
