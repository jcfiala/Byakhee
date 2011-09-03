// RandomNameGenerator.h: interface for the CRandomNameGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMNAMEGENERATOR_H__97E65822_6A02_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_RANDOMNAMEGENERATOR_H__97E65822_6A02_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "NamedOptionManager.h"

class CRandomNameGenerator : public CObject  
{
public:
	CString GetSex();
	CString GetName();
	void Generate();
	CRandomNameGenerator();
	virtual ~CRandomNameGenerator();

private:
	CString m_strSex;
	CString m_strName;

protected:
    CNamedOptionManager* m_pNamedOptionManager;
};

#endif // !defined(AFX_RANDOMNAMEGENERATOR_H__97E65822_6A02_11D2_9FF9_00001C192944__INCLUDED_)
