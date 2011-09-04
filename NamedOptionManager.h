// NamedOptionManager.h: interface for the CNamedOptionManager class.
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
