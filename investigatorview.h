// InvestigatorView.h : interface of the CInvestigatorView class
//
/////////////////////////////////////////////////////////////////////////////
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

#if !defined(AFX_INVESTIGATORVIEW_H__A9FB21CE_4B45_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_INVESTIGATORVIEW_H__A9FB21CE_4B45_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptSheetView.h"

/* Field Type definitions */

//skill types
#define FIELDTYPE_SKILL                     100
#define FIELDTYPE_SKILL_CHECK               101
#define FIELDTYPE_SKILL_NAME                102
#define FIELDTYPE_SKILL_PERCENT             103

#define FIELDTYPE_FIREARM                   200
#define FIELDTYPE_FIREARM_CHECK             201
#define FIELDTYPE_FIREARM_NAME              202
#define FIELDTYPE_FIREARM_PERCENT           203
#define FIELDTYPE_FIREARM_DAMAGE            204
#define FIELDTYPE_FIREARM_RANGE             205
#define FIELDTYPE_FIREARM_SHOTS             206
#define FIELDTYPE_FIREARM_BULLETS           207
#define FIELDTYPE_FIREARM_HITPOINTS         208
#define FIELDTYPE_FIREARM_MAL               209

#define FIELDTYPE_MELEE                     300
#define FIELDTYPE_MELEE_CHECK               301
#define FIELDTYPE_MELEE_NAME                302
#define FIELDTYPE_MELEE_PERCENT             303
#define FIELDTYPE_MELEE_DAMAGE              304
#define FIELDTYPE_MELEE_ATTACKS             305
#define FIELDTYPE_MELEE_HITPOINTS           306

#define FIELDTYPE_WEAPON                    400
#define FIELDTYPE_WEAPON_CHECK              401
#define FIELDTYPE_WEAPON_NAME               402
#define FIELDTYPE_WEAPON_PERCENT            403
#define FIELDTYPE_WEAPON_DAMAGE             404
#define FIELDTYPE_WEAPON_HITPONTS           405
#define FIELDTYPE_WEAPON_BULLETS            406
#define FIELDTYPE_WEAPON_RANGE              407
#define FIELDTYPE_WEAPON_ATTACKS            408
#define FIELDTYPE_WEAPON_MAL                409

//others
#define FIELDTYPE_MAXSAN                    500
#define FIELDTYPE_CTHULHUMYTHOS             501
#define FIELDTYPE_STARTINGSAN               502
#define FIELDTYPE_LUCK                      503
#define FIELDTYPE_KNOW                      504
#define FIELDTYPE_IDEA                      505
#define FIELDTYPE_DAMAGEBONUS               506
#define FIELDTYPE_ERA                       507

#define FIELDTYPE_PORTRAIT                  600 //the investigator document looks after the CImage object, so ask us for it

//sheet parameters
#define SHEETPARAM_SHORT                    700
#define SHEETPARAM_SHOWCOREFIREARMS         701
#define SHEETPARAM_SHOWBASEPERCENT          702


class CInvestigatorView : public CScriptSheetView
{
protected: // create from serialization only
	CInvestigatorView();
	DECLARE_DYNCREATE(CInvestigatorView)

// Attributes
public:
	CInvestigatorDoc* GetDocument();

    virtual Variant GetFieldValue( CField* pField, CFieldSheetCommand* pFieldSheetCommand /*NULL*/ );
    virtual void SetFieldValue( CField* pField, Variant value, CFieldSheetCommand* pFieldSheetCommand /*NULL*/ );
    virtual void ExecuteSheetCommand( CSheetCommand* pSheetCommand, CSheetDrawInfo* pDrawInfo );

    void RedrawFields( CFieldSheetCommand* pFieldChanged );
    virtual void StoreUndo();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestigatorView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnActivateView(BOOL bActivate, CScrollView* pActivateView, CScrollView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInvestigatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	virtual void InitialiseSheetCommands();
	virtual void AddAllFields();
    virtual void SheetChanged();

    //sheet options
	BOOL m_fShowBasePercent;
    BOOL m_fDrawTruncated;
    BOOL m_fShowFirearms;

    //menu options
    static BOOL m_fCustomSkillsInUserText;
    static BOOL m_fShowAllSkillPercentiles;
	static BOOL m_fDrawV5_5Bases;
    static BOOL m_fTrimToMinimum;

    //abbreviation lookup function
	LPCSTR Abbreviate( const char* pszSkillName );

	//{{AFX_MSG(CInvestigatorView)
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnViewLikeCthulhu55();
	afx_msg void OnUpdateViewLikeCthulhu55(CCmdUI* pCmdUI);
	afx_msg void OnViewCustomskillsInUserText();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnViewAllSkillPercentiles();
	afx_msg void OnUpdateViewAllSkillPercentiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCustomSkillsInUserText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsClipSkillPoints(CCmdUI* pCmdUI);
	afx_msg void OnToolsClipSkillPoints();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    struct Abbreviation {
        CString strOriginal;
        CString strShort;
    };
    CArray<Abbreviation, Abbreviation&> m_Abbreviations;
};

#ifndef _DEBUG  // debug version in InvestigatorView.cpp
inline CInvestigatorDoc* CInvestigatorView::GetDocument()
   { return (CInvestigatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVESTIGATORVIEW_H__A9FB21CE_4B45_11D2_9FF9_00001C192944__INCLUDED_)
