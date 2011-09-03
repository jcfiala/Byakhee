// HandoutDoc.cpp : implementation of the CHandoutDoc class
//

#include "stdafx.h"
#include "Byakhee.h"
#include "HandoutDoc.h"
#include "ParseBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandoutDoc

IMPLEMENT_DYNCREATE(CHandoutDoc, CDocument)

BEGIN_MESSAGE_MAP(CHandoutDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	//{{AFX_MSG_MAP(CHandoutDoc)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandoutDoc construction/destruction

CHandoutDoc::CHandoutDoc()
{
	// TODO: add one-time construction code here

}

CHandoutDoc::~CHandoutDoc()
{
}

BOOL CHandoutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHandoutDoc serialization

void CHandoutDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHandoutDoc diagnostics

#ifdef _DEBUG
void CHandoutDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHandoutDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHandoutDoc commands

void CHandoutDoc::StoreUndo()
{
    HandoutUndoRedoData hurd;
    DuplicateStringArray( hurd.m_strUndoStrings, m_strStrings, MAX_HANDOUT_TEXT_FIELDS );
    DuplicateStringArray( hurd.m_strUndoPictures, m_strPictures, MAX_HANDOUT_PICTURE_FIELDS );

    //push the current investigator onto the undo stack
    m_UndoList.AddHead( hurd );

    //empty the redo list
    m_RedoList.RemoveAll();

    //keep undo stack under control
    if( m_UndoList.GetCount() > MAX_UNDO_SIZE ) m_UndoList.RemoveTail();
}

void CHandoutDoc::OnEditUndo() 
{
    if( m_UndoList.IsEmpty() == FALSE )
    {
        //get an item from the undo stack
        HandoutUndoRedoData hurd = m_UndoList.RemoveHead();
    
        //store the strings it will replace
        HandoutUndoRedoData hurdcurrent;
    	DuplicateStringArray( hurdcurrent.m_strUndoStrings, m_strStrings, MAX_HANDOUT_TEXT_FIELDS );
        DuplicateStringArray( hurdcurrent.m_strUndoPictures, m_strPictures, MAX_HANDOUT_PICTURE_FIELDS );
        m_RedoList.AddHead( hurdcurrent );

        //replace current strings
    	DuplicateStringArray( m_strStrings, hurd.m_strUndoStrings, MAX_HANDOUT_TEXT_FIELDS );
        DuplicateStringArray( m_strPictures, hurd.m_strUndoPictures, MAX_HANDOUT_PICTURE_FIELDS );

        //keep redo stack under control
        if( m_RedoList.GetCount() > MAX_UNDO_SIZE ) m_RedoList.RemoveTail();

        //display changes
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }
}

void CHandoutDoc::OnEditRedo() 
{
    if( m_RedoList.IsEmpty() == FALSE )
    {
        //get the item from the redo stack
        HandoutUndoRedoData hurd = m_RedoList.RemoveHead();

        //store the strings it will replace
        HandoutUndoRedoData hurdcurrent;
    	DuplicateStringArray( hurdcurrent.m_strUndoStrings, m_strStrings, MAX_HANDOUT_TEXT_FIELDS );
        DuplicateStringArray( hurdcurrent.m_strUndoPictures, m_strPictures, MAX_HANDOUT_PICTURE_FIELDS );
        m_UndoList.AddHead( hurdcurrent );

        //replace current string
    	DuplicateStringArray( m_strStrings, hurd.m_strUndoStrings, MAX_HANDOUT_TEXT_FIELDS );
        DuplicateStringArray( m_strPictures, hurd.m_strUndoPictures, MAX_HANDOUT_PICTURE_FIELDS );

        //keep undo stack under control
        if( m_UndoList.GetCount() > MAX_UNDO_SIZE ) m_UndoList.RemoveTail();

        //display changes
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }
}

void CHandoutDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_UndoList.IsEmpty() == FALSE );
}

void CHandoutDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_RedoList.IsEmpty() == FALSE );
}

CString CHandoutDoc::BuildKeyPair(CString strName, CString strValue)
{
    //search and replace all " with '
    const char* psz = LPCSTR(strValue);
    while( psz && *psz != '\0' ) 
    {
        if( *psz == '\"' ) *(char*)psz = '\'';
        psz++;
    }

    //continue to build keypair
    return CString( "\t\"" ) + strName + CString( "\" \"" ) + strValue + CString( "\"\r\n" );
}

CString CHandoutDoc::BuildKeyPair(CString strName, int nValue)
{
    CString strValue;
    strValue.Format( "%d", nValue );
    return CString( "\t\"" ) + strName + CString( "\" \"" ) + strValue + CString( "\"\r\n" );
}

BOOL CHandoutDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    CWaitCursor Wait;

    CString strFile;
    const CString strEOL = "\r\n";

    /* build output buffer */
    strFile += "Handout { " + strEOL;
    strFile += BuildKeyPair( "Template", m_strSheet );
    int i;
    for( i = 0; i < MAX_HANDOUT_TEXT_FIELDS; i++ )
    {
        if( m_strStrings[i].IsEmpty() == FALSE ) //only write it out if it's not blank
        {
            CString strKey; strKey.Format( "Text%d", i );
	        strFile += BuildKeyPair( strKey, m_strStrings[i] );
        }
    }
    for( i = 0; i < MAX_HANDOUT_PICTURE_FIELDS; i++ )
    {
        if( m_strPictures[i].IsEmpty() == FALSE ) //only write it out if it's not blank
        {
            CString strKey; strKey.Format( "Picture%d", i );
	        strFile += BuildKeyPair( strKey, m_strPictures[i] );
        }
    }
    strFile += "}" + strEOL + strEOL;

    //create output file
    CFile File;
    File.Open( lpszPathName, CFile::modeWrite|CFile::modeCreate );

    //write file buffer
    File.Write( LPCSTR(strFile), strFile.GetLength() );
	
	return TRUE;
}

BOOL CHandoutDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    /* open file */
    CFile File;
    if( File.Open( lpszPathName, CFile::modeRead ) == FALSE )
    {
        AfxMessageBox( "Cannot open handout file", MB_ICONERROR );
        return FALSE;
    }

    
    /* load the file into a buffer */

	//determine file size
    char* pszBuffer = NULL;
	DWORD dwLength = File.GetLength();
	if( dwLength == 0 ) 
	{
		AfxMessageBox( "File is empty", MB_ICONERROR );
		return FALSE;
	}

	//allocate a buffer
	pszBuffer = new char[dwLength+1];
	if( pszBuffer == NULL )
	{
		AfxMessageBox( "Cannot allocate buffer", MB_ICONERROR );
		return FALSE;
	}

	//load the whole file
	if( File.Read( pszBuffer, dwLength ) < dwLength ) 
	{ 
		delete[] pszBuffer;
		AfxMessageBox( "Unexpected end of file", MB_ICONERROR );
		return FALSE;
	}

	//add a terminator at the end of the string
	pszBuffer[dwLength] = '\0';

    /* parse the file */
    CParseBuffer Buffer(pszBuffer);
    delete[] pszBuffer;

    //read in the rest of the file
    Buffer.IgnoreLineFeeds();
    while( Buffer.IsEmpty() == FALSE )
    {
        CString strSectionType = Buffer.ExtractUnquotedString();
        CString strSection = Buffer.ExtractSurroundedString( '{', '}' );
        if( Buffer.IsUnderflow() == FALSE && Buffer.IsParseError() == FALSE )
        {
            if( strSectionType.CompareNoCase( "Handout" ) == 0 ) ParseHandoutSection(strSection); else
                AfxMessageBox( CString("Warning Unknown section type: ") + strSectionType );
        }
        else
        {
            AfxMessageBox( "Invalid file format", MB_ICONERROR );
            return FALSE;
        }
    }
	
	return TRUE;
}


BOOL CHandoutDoc::ParseHandoutSection(CString &strSection)
{
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        CString strKey = Section.ExtractQuotedString();
        CString strValue = Section.ExtractQuotedString();
        if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
        {
            if( strKey.CompareNoCase( "Template" ) == 0 ) m_strSheet = strValue; else
            if( strnicmp( strKey, "TEXT", 4 ) == 0 )
            {
                int i = atoi( LPCSTR(strKey)+4 );
                if( i >= 0 && i < MAX_HANDOUT_TEXT_FIELDS ) m_strStrings[i] = strValue;
            }
            else
            if( strnicmp( strKey, "PICTURE", 7 ) == 0 )
            {
                int i = atoi( LPCSTR(strKey)+7 );
                if( i >= 0 && i < MAX_HANDOUT_PICTURE_FIELDS ) m_strPictures[i] = strValue;
            }
        }
        else
            return FALSE;
    }

    return TRUE;

}

void CHandoutDoc::DuplicateStringArray(CString *pDestination, CString *pSource, int nItems)
{
    for( int i = 0; i < nItems; i++ ) pDestination[i] = pSource[i];
}
