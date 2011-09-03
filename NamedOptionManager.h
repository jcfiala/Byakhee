// NamedOptionManager.h: interface for the CNamedOptionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDOPTIONMANAGER_H__90CDA700_6C3C_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_NAMEDOPTIONMANAGER_H__90CDA700_6C3C_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define DEFAULT_NAMEDOPTIONMANAGER_CACHE_SIZE 5

class CNamedOptionManager : public CObject  
{
public:
	CNamedOptionManager( CString strFilename, int nCacheSize = DEFAULT_NAMEDOPTIONMANAGER_CACHE_SIZE );
	virtual ~CNamedOptionManager();

    CStringArray* GetOptions( CString strOption );
	void FillComboWithOptions( CComboBox* pComboBox, CString strOption );
	void FillListBoxWithOptions( CListBox* pListBox, CString strOption );

private:
	CString GetOptionName( CString strOption );
	CString m_strOptionFile;
    CString m_strDefaultOptionFile;

    struct NamedOptionManagerCacheElement {
        CString strName;
        CStringArray Options;
        DWORD dwItemTime;
    };

    NamedOptionManagerCacheElement* m_Cache;
    int m_nCacheSize;

protected:
	void ResetCacheElement( int i );
	NamedOptionManagerCacheElement* GetCacheItem( CString strOption );
	NamedOptionManagerCacheElement* MakeFreeCacheItem();
};

#endif // !defined(AFX_NAMEDOPTIONMANAGER_H__90CDA700_6C3C_11D2_9FF9_00001C192944__INCLUDED_)
