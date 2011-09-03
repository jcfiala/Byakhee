// MultiFileMultiDocTemplate.cpp: implementation of the CMultiFileMultiDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

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
