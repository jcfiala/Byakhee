// RandomNameGenerator.h: interface for the CRandomNameGenerator class.
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
