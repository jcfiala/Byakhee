// MultiFileMultiDocTemplate.h: interface for the CMultiFileMultiDocTemplate class.
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
