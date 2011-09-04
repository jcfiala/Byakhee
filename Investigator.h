// Investigator.h: interface for the CInvestigator class.
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

#if !defined(AFX_INVESTIGATOR_H__773E62A0_6090_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_INVESTIGATOR_H__773E62A0_6090_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Skill.h"
#include "SkillManager.h"
#include <afxtempl.h>
#include "ScriptEngine/ScriptObject.h"
#include "ScriptEngine/Variant.h"

//COC file header values
#define SHEET_FILE_HEADER10 "COC10"
#define SHEET_FILE_HEADER20 "COC20"

class CInvestigatorSkill : public CObject 
{

public:
	int GetValue( BOOL f5_5Base );
	void SetValue( BOOL f5_5Base, int nValue );
	BOOL IsCalculatedBase();
	BOOL IsBase(BOOL f5_5Mode);
	signed char Compare( const CInvestigatorSkill* pInvestigatorSkill );
	CInvestigatorSkill* GetEmptyCopy();
	CInvestigatorSkill* GetCopy();
    CInvestigatorSkill( CSkill* pSkill, int nCurrent, int nMinimum = -1 );
    CInvestigatorSkill( CSkill* pSkill ) { m_pSkill = pSkill; m_nCurrent = m_nMinimum = 0; m_fOccupational = FALSE; m_fChecked = FALSE; }
    virtual ~CInvestigatorSkill();


    CSkill* m_pSkill;
    int m_nCurrent;
    int m_nMinimum;
    BOOL m_fOccupational;
    BOOL m_fChecked;
};


class CInvestigator : public CObject  
{
public:
	void CalculateSkillPoints();
	void RunIncomeScript();
	void RecalculateMinimumSkillValues();
	void RollNewStats();
	CInvestigatorSkill* FindSubskill( CInvestigatorSkill* pInvestigatorSpecialSkill, int nSubSkillIndex );
	void SortSkillList();
	void CalculateCoreStatistics();
    //construction/destruction
	CInvestigator();
	CInvestigator( CString strRuleFile );
    CInvestigator( const CInvestigator& src );
	virtual ~CInvestigator();
    BOOL SetRuleFile( CString strRuleFile );

    //skill management
	CInvestigatorSkill* AddSkill( CSkill* pSkill, BOOL fOccupational = FALSE );
	void AddOccupationalSkill( CString strSkill );
	BOOL RemoveSkill( CInvestigatorSkill* pInvestigatorSkill );
	BOOL RemoveSkill( CString strSkillName );

    CInvestigatorSkill* FindSkill( CString strName );

	BOOL SetOccupation( CString strOccupation );

    //member access
	int GetAttributeByName( CString strName );

    //display information
	CString GetDBString();
	static CString GetDBString( int nSTR, int nSIZ );

    //skills
	CArray<CInvestigatorSkill*, CInvestigatorSkill*> m_AllSkillList;  //all skills
	CArray<CInvestigatorSkill*, CInvestigatorSkill*> m_SkillList;	 //normal skills
	CArray<CInvestigatorSkill*, CInvestigatorSkill*> m_FirearmList;	 //firearms
	CArray<CInvestigatorSkill*, CInvestigatorSkill*> m_MeleeList;	 //melee weapons

    //attributes
	int m_nSkillPoints;
	int m_nInterestPoints;

	CString m_strName;
	CString m_strOccupation;
	CString m_strBirthplace, m_strColleges, m_strMentalDisorders, m_strSex;
	int m_nAge;
	Era m_Era;

    CString m_strPortrait;

	int m_nAPP;
	int m_nCON;
	int m_nDEX;
	int m_nEDU;
	int m_nINT;
	int m_nPOW;
	int m_nSIZ;
	int m_nSTR;

	CString m_strPlayerName;

    //NEW FOR VERSION 1.1:
    int m_nSAN;
    int m_nHitPoints;
    int m_nMagic;
    CString m_strAffiliation;
    CString m_strCodeName;
    CString m_strCurrentDate;
    CString m_strInsanities;

    //NEW FOR VERSION 2.0
    enum EXTRASCHOOLING { EXTRASCHOOLING_NOTAPPLICABLE, EXTRASCHOOLING_FAILED, EXTRASCHOOLING_SUCCESS };
	EXTRASCHOOLING m_ExtraSchooling;
    CString m_strIncome;
    CString m_strSpells;
	CString m_strRuleFile;

    CString m_strResidence;
    CString m_strPersonalDescription;
    CString m_strContacts;
    CString m_strWounds;
    CString m_strScars;
    CString m_strHistory;
    CString m_strCashOnHand;
    CString m_strSavings;
    CString m_strPersonalProperty;
    CString m_strRealEstate;
    CString m_strPossessions;
    CString m_strTomesRead;
    CString m_strArtifacts;
    CString m_strEntitiesEncountered;
	
    //assignment operator
    const CInvestigator& operator=(const CInvestigator& src );

    //skill serialisation, for export to old versions
    void WriteSkill( CArchive& ar, CSkill* pSkill );
    int ReadSkill( CArchive& ar, BOOL fOccupational, int nCurrent, int nMinimumValue );

protected:
	void DuplicateInvestigator( CInvestigator* destination ) const;

private:
	void SortSkillList_Partition( CArray<CInvestigatorSkill*, CInvestigatorSkill*>& SkillArraySort, const int i, const int j );

};

class CScriptInvestigator : public CScriptObject , public CObject
{
public:
    CScriptInvestigator( CInvestigator* pInvestigator );
    virtual ~CScriptInvestigator();

	virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );
	virtual Variant* GetMemberVariable( char* pszName );

private:
    void ReadStatsFromInvestigator();
    void WriteStatsToInvestigator();

    //pointer to the investigator we're messing around with
    CInvestigator* m_pInvestigator;

    //investigator properties
	Variant m_SkillPoints;
	Variant m_InterestPoints;
	Variant m_Name;
	Variant m_Occupation;
	Variant m_Birthplace;
    Variant m_Colleges;
    Variant m_MentalDisorders;
    Variant m_Sex;
	Variant m_Age;
	Variant m_APP;
	Variant m_CON;
	Variant m_DEX;
	Variant m_EDU;
	Variant m_INT;
	Variant m_POW;
	Variant m_SIZ;
	Variant m_STR;
    Variant m_SAN;
    Variant m_HitPoints;
    Variant m_Magic;
    Variant m_Affiliation;
    Variant m_CodeName;
    Variant m_CurrentDate;
    Variant m_Insanities;
    Variant m_Income;
    Variant m_Spells;

    Variant m_Residence;          
    Variant m_PersonalDescription;
    Variant m_Contacts;           
    Variant m_Wounds;             
    Variant m_Scars;              
    Variant m_History;            
    Variant m_CashOnHand;         
    Variant m_Savings;            
    Variant m_PersonalProperty;   
    Variant m_RealEstate;         
    Variant m_Possessions;        
    Variant m_TomesRead;          
    Variant m_Artifacts;
    Variant m_EntitiesEncountered;
};

#endif // !defined(AFX_INVESTIGATOR_H__773E62A0_6090_11D2_9FF9_00001C192944__INCLUDED_)

