// MultiFileMultiDocTemplate.h: interface for the CMultiFileMultiDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIFILEMULTIDOCTEMPLATE_H__281B3003_A266_11D2_A50C_00105A4ACC2C__INCLUDED_)
#define AFX_MULTIFILEMULTIDOCTEMPLATE_H__281B3003_A266_11D2_A50C_00105A4ACC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMultiFileMultiDocTemplate : public CMultiDocTemplate  
{
    DECLARE_DYNAMIC(CMultiFileMultiDocTemplate)

public:
	void ChangeTemplateType( UINT uIDNewResource );
    CMultiFileMultiDocTemplate( UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass, UINT nIDOther = 0 );    
    virtual void LoadTemplate();
    
protected:
    void LoadOtherTemplate();
    UINT m_nIDOther;
    BOOL m_bLoaded;
};

#endif // !defined(AFX_MULTIFILEMULTIDOCTEMPLATE_H__281B3003_A266_11D2_A50C_00105A4ACC2C__INCLUDED_)
