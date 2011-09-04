// MultiFileMultiDocTemplate.cpp: implementation of the CMultiFileMultiDocTemplate class.
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
#include "byakhee.h"
#include "MultiFileMultiDocTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMultiFileMultiDocTemplate,CMultiDocTemplate)

CMultiFileMultiDocTemplate::CMultiFileMultiDocTemplate( UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, UINT nIDOther ) :
        m_bLoaded(FALSE),
        m_nIDOther(nIDOther),
        CMultiDocTemplate( nIDResource, pDocClass, pFrameClass, pViewClass )
{
    if(m_nIDOther) LoadOtherTemplate();
}

void CMultiFileMultiDocTemplate::LoadTemplate()
{
    CMultiDocTemplate::LoadTemplate();
    if(m_nIDOther) LoadOtherTemplate();
}

void CMultiFileMultiDocTemplate::LoadOtherTemplate()
{
    if(m_bLoaded) return;
    ASSERT(m_nIDOther);

    LPCTSTR pOther = MAKEINTRESOURCE(m_nIDOther);
    HINSTANCE hInst = AfxFindResourceHandle( pOther, RT_MENU );
    HMENU hMenuShared = ::LoadMenu( hInst, pOther );
    HACCEL hAccelTable = ::LoadAccelerators( hInst, pOther );
    if( hMenuShared )
    {
        if( m_hMenuShared ) ::DestroyMenu( m_hMenuShared );
        m_hMenuShared = hMenuShared;
    }

    if( hAccelTable )
    {
        if( m_hAccelTable ) ::FreeResource( (HGLOBAL)m_hAccelTable );
        m_hAccelTable = hAccelTable;
    }

#ifdef _DEBUG
    if( !m_hMenuShared )
        TRACE1("Warning: No menu for template #%d\n",m_nIDOther);

    if( !m_hAccelTable )
        TRACE1("Warning: No accelerator for template #%d\n",m_nIDOther);
#endif
}

void CMultiFileMultiDocTemplate::ChangeTemplateType(UINT uIDNewResource)
{
    //destroy shared components
	if (m_hMenuShared != NULL) ::DestroyMenu(m_hMenuShared);
    m_hMenuShared = NULL;
	if (m_hAccelTable != NULL) ::FreeResource((HGLOBAL)m_hAccelTable);
    m_hAccelTable = NULL;

    //store new resource value
    m_nIDOther = m_nIDResource = uIDNewResource;
    m_bLoaded = FALSE;
    LoadOtherTemplate();
}
