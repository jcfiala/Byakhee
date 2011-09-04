// Skill.h: interface for the CSkill class hierarchy.
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

#if !defined(AFX_SKILL_H__452540C1_4B4D_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SKILL_H__452540C1_4B4D_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//era type
typedef int ERA_MASK;

//era bit masks
#define ERA_ISNONE 0
#define ERA_IS1890 1
#define ERA_IS1920 2
#define ERA_IS1990 4
#define ERA_ISALL  (ERA_IS1890|ERA_IS1920|ERA_IS1990)

enum Era { Era_1890, Era_1920, Era_1990 };

enum SkillType { NO_TYPE, SKILL, FIREARM, MELEE, ANYSKILL, SPECIALSKILL, SUBSKILL };


class CSkill : public CObject  
{
public:
	CString GetBaseString( BOOL f5_5Mode, BOOL fShowPercent = TRUE );
    CSkill( ERA_MASK EraMask );
	CSkill( CString strName, CString strBase, ERA_MASK EraMask );
	virtual ~CSkill();
	virtual CSkill* GetCopy();

	BOOL IsEra( Era era );
	int EvaluateBase( class CInvestigator* pInvestigator );

    CString m_strName;
    CString m_strBase;
	ERA_MASK m_EraMask;
    SkillType m_SkillType;

protected:
    CSkill();
};


class CFirearmSkill : public CSkill 
{
public:
	CFirearmSkill( CString strName, CString strBase, ERA_MASK EraMask, CString strDmg, CString strRange, CString strShots, CString strBullets, CString strHP, CString strMAL );
    CFirearmSkill( ERA_MASK EraMask );
	virtual ~CFirearmSkill();
	virtual CSkill* GetCopy();

	CString m_strDmg;
	CString m_strRange;
	CString m_strShots;
	CString m_strBullets;
	CString m_strHP;
	CString m_strMAL;
};



class CMeleeSkill : public CSkill 
{
public:
	CMeleeSkill( CString strName, CString strBase, ERA_MASK EraMask, CString strDmg, CString strAttacks, CString strHP );
    CMeleeSkill( ERA_MASK EraMask );
	virtual ~CMeleeSkill();
	virtual CSkill* GetCopy();

	CString m_strDmg;
	CString m_strAttacks;
	CString m_strHP;
};



class CSpecialSkill : public CSkill 
{
public:
	CSpecialSkill( CString strName, CString strBase, ERA_MASK EraMask, int nSubSkills, BOOL fCreateSubskills = TRUE );
	virtual ~CSpecialSkill();
	virtual CSkill* GetCopy();
	CSpecialSkill* GetEmptyCopy();

	int GetSubSkillIndex( const class CSubSkill* pSubSkill );

    class CSubSkill** m_pSubSkills;
    int m_nSkills; //total number of skills in this subset

	BOOL m_fManageSubskills;
};


class CSubSkill : public CSkill  
{
public:
	CSubSkill( CSpecialSkill* pSkill, CString strBase, ERA_MASK EraMask );
	virtual ~CSubSkill();
	virtual CSkill* GetCopy();

    CSpecialSkill* m_pSkill;
};


#endif // !defined(AFX_SKILL_H__452540C1_4B4D_11D2_9FF9_00001C192944__INCLUDED_)
