// ParseBuffer.h: interface for the CParseBuffer class.
//
//////////////////////////////////////////////////////////////////////

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
