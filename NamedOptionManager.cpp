// NamedOptionManager.cpp: implementation of the CNamedOptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "NamedOptionManager.h"
#include "SectionFile.h"
#include "SkillManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNamedOptionManager::CNamedOptionManager( CString strFilename, int nCacheSize )
{
    //allocate the cache
    if( nCacheSize <= 0 ) nCacheSize = 1;
    m_nCacheSize = nCacheSize;
    m_Cache = new NamedOptionManagerCacheElement[m_nCacheSize];

    //empty the cache
    for( int i = 0; i < m_nCacheSize; i++ ) ResetCacheElement(i);

    //get option file name
    m_strOptionFile = CByakheeApp::GetDefaultDirectory() + strFilename;
    m_strDefaultOptionFile = CByakheeApp::GetDefaultDirectory() + CString("options.lst");
}

CNamedOptionManager::~CNamedOptionManager()
{
    delete[] m_Cache;
}

void CNamedOptionManager::FillComboWithOptions(CComboBox * pComboBox, CString strOption)
{
    //empty the combo box
    ASSERT(pComboBox);
    pComboBox->ResetContent();

    //load the options
    CStringArray* pStringArray = GetOptions( strOption );
    if( pStringArray )
    {
        for( int n = 0; n < pStringArray->GetSize(); n++ ) 
            pComboBox->AddString( pStringArray->GetAt(n) );
    }
}

void CNamedOptionManager::FillListBoxWithOptions(CListBox * pListBox, CString strOption)
{
    //empty the combo box
    ASSERT(pListBox);
    pListBox->ResetContent();

    //load the options
    CStringArray* pStringArray = GetOptions( strOption );
    if( pStringArray )
    {
        for( int n = 0; n < pStringArray->GetSize(); n++ ) 
            pListBox->AddString( pStringArray->GetAt(n) );
    }
}

CStringArray* CNamedOptionManager::GetOptions(CString strOption )
{

    //translate the option name
    strOption = GetOptionName(strOption);

    //see if it's already been loaded
    NamedOptionManagerCacheElement* pItem = GetCacheItem( strOption );
    if( pItem == NULL )
    {
        //turn on wait cursor
        CWaitCursor WaitCursor;

        //load the buffer
        CSectionFile SectionFile;
        CString strSection;
        if( SectionFile.Open( m_strOptionFile ) ) strSection = SectionFile.GetSection( strOption );
        if( strSection.IsEmpty() ) 
        {
            CSectionFile DefaultSectionFile;
            if( DefaultSectionFile.Open( m_strDefaultOptionFile ) ) strSection = DefaultSectionFile.GetSection( strOption );
            if( strSection.IsEmpty() ) return NULL;
        }

        //create an entry in the cache for this item
        pItem = MakeFreeCacheItem();
        pItem->strName = strOption;
    
        //get a pointer to the string
        char* pszRead = strSection.LockBuffer();

        //skip over leading blank lines
        while( *pszRead == '\r' || *pszRead == '\n' ) pszRead++;
        char* pszBuffer = pszRead;

        //read each line in
        while( *pszBuffer != '\0' )
        {
            //find the end of this line and terminate the string at that point
            char chRead = '\0';
            while( *pszRead != '\r' && *pszRead != '\n' && *pszRead != '\0' ) chRead = *(++pszRead);
            *pszRead = '\0';

            if( chRead != '\0' )
            {
                //copy the string into the next array element and un-terminate the string
                pItem->Options.Add( pszBuffer );
                *pszRead = chRead;

                //skip over blank lines
                while( *pszRead == '\r' || *pszRead == '\n' ) pszRead++;
            }

            //move to next line
            pszBuffer = pszRead;
        }

        //unlock the string
        strSection.UnlockBuffer();

        //turn off wait cursor
        WaitCursor.Restore();
    }

    //return the array
    return &pItem->Options;
}

CString CNamedOptionManager::GetOptionName(CString strOption)
{
    //remove leading and trailing whitespace
    strOption.TrimLeft();
    strOption.TrimRight();

    //convert spaces to underscores
    int nLength = strOption.GetLength();
    for( int n = 0; n < nLength; n++ ) if( strOption[n] == ' ' ) strOption.SetAt( n, '_' );

    //return the string
    return strOption;
}

void CNamedOptionManager::ResetCacheElement(int i)
{
    ASSERT( i >= 0 && i < m_nCacheSize );

    //reset the item - set it's time to now so it's the newest
    m_Cache[i].dwItemTime = GetTickCount();
    m_Cache[i].strName = "";
    m_Cache[i].Options.RemoveAll();
    m_Cache[i].Options.SetSize( 0, 10 );
}

CNamedOptionManager::NamedOptionManagerCacheElement* CNamedOptionManager::MakeFreeCacheItem()
{
    //find the oldest item in the cache item
    int iSmallest = -1;
    for( int i = 0; i < m_nCacheSize; i++ )
        if( iSmallest == -1 || m_Cache[i].dwItemTime < m_Cache[iSmallest].dwItemTime ) iSmallest = i;

    ASSERT(iSmallest != -1);

    //reset the item
    ResetCacheElement(iSmallest);

    //return it
    return &m_Cache[iSmallest];
}

CNamedOptionManager::NamedOptionManagerCacheElement* CNamedOptionManager::GetCacheItem(CString strOption)
{
    //find this item in the cache
    for( int i = 0; i < m_nCacheSize; i++ )
    {
        if( m_Cache[i].strName.CompareNoCase(strOption) == 0 ) return &m_Cache[i];
    }

    //not found
    return NULL;
}
