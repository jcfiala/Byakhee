// SkillManager.h: interface for the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLMANAGER_H__452540C0_4B4D_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_SKILLMANAGER_H__452540C0_4B4D_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Skill.h"
#include <afxtempl.h>


class COccupation : public CObject 
{

public:
	CString GetName();
    COccupation( CString strName, ERA_MASK EraMask = ERA_ISALL, CString strScript = "" );
    virtual ~COccupation();

	CStringArray m_SkillArray;
    CStringArray m_OptionalSkillArray;
    int m_nOptional;
    CString m_strName;
    CString m_strScript;
    ERA_MASK m_Era;
};


class CRuleFile : public CObject 
{

public:
	CString GetKeyPairValue( CString strName, CString strDefault );
    CRuleFile( CString strRuleFile );
    virtual ~CRuleFile();

	CArray<CSkill*, CSkill*> m_AllSkillArray;  //all skills
	CArray<CSkill*, CSkill*> m_SkillArray;	 //normal skills
	CArray<CSkill*, CSkill*> m_FirearmArray;	 //firearms
	CArray<CSkill*, CSkill*> m_MeleeArray;	 //melee weapons
    CArray<COccupation*, COccupation*> m_OccupationArray; //all occupations

	BOOL AddSkillsToInvestigator( class CInvestigator* pInvestigator );
	COccupation* FindOccupation( CString strOccupation );
	BOOL IsOccupationScript( CString strOccupation );
	int GetOccupationCount();
	CString GetOccupationName( int n );
    CSkill* FindSkill( CString strName );

    BOOL ParseRuleFile();

protected:
    ERA_MASK CreateEraMask( CString strEraField );
	BOOL BuildMeleeList( CString strSection );
	BOOL BuildFirearmsList( CString strSection );
	BOOL BuildSkillsList( CString strSection );
	BOOL BuildOccupationsList( CString strSection );
	BOOL m_fError;

	CString m_strFileName;
};


#define MAX_RULEFILE_CACHE 10 //limit to 10 rule files in memory at once

class CSkillManager : public CObject  
{
public:
	CString GetKeyPairValue( CString strRuleFile, CString strName, CString strDefault );
	const CRuleFile* GetTempRuleFile( CString strRuleFile );
	CSkillManager();
	virtual ~CSkillManager();

	BOOL AddSkillsToInvestigator( CString strRuleFile, class CInvestigator* pInvestigator );
	COccupation* FindOccupation( CString strRuleFile, CString strOccupation );
	BOOL IsOccupationScript( CString strRuleFile, CString strOccupation );
	int GetOccupationCount(CString strRuleFile );
	CString GetOccupationName( CString strRuleFile, int n );
    CSkill* FindSkill( CString strRuleFile, CString strName );

	BOOL AddRuleFile( CString strRuleFile );
    BOOL RemoveRuleFile( CString strRuleFile );

protected:
    int FindRuleFile( CString strRuleFile );

private:
    struct RuleFileCache {
        CRuleFile* pRuleFile;
        CString strRuleFile;
        DWORD dwTickCount;
        int nReferenceCount;
    };
    RuleFileCache m_RuleFileCache[MAX_RULEFILE_CACHE];
};

extern CSkillManager SkillManager;

#endif // !defined(AFX_SKILLMANAGER_H__452540C0_4B4D_11D2_9FF9_00001C192944__INCLUDED_)
