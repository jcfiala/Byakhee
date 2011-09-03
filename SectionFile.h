// SectionFile.h: interface for the CSectionFile class.
//
//////////////////////////////////////////////////////////////////////

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
