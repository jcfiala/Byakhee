// InvestigatorDoc.h : interface of the CInvestigatorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVESTIGATORDOC_H__A9FB21CC_4B45_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_INVESTIGATORDOC_H__A9FB21CC_4B45_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Byakhee.h"
#include "Investigator.h"
#include "NamedOptionManager.h"

class CInvestigatorDoc : public CDocument
{
protected: // create from serialization only
	CInvestigatorDoc();
	DECLARE_DYNCREATE(CInvestigatorDoc)

// Attributes
public:
    CInvestigator m_Investigator;

// Operations
public:
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestigatorDoc)
	public:
	virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void StoreUndo();
	BOOL SetPortrait( CString strPortrait );
    class CImage* GetPortrait() { return m_pPortrait; };
	BOOL ReloadPortrait();
	void SetRuleFile( CString strRuleFile );
	virtual ~CInvestigatorDoc();

    void UpdateStatusBar();

    CString m_strLastSheet;

    CNamedOptionManager* GetNamedOptionManager() { return m_pNamedOptionManager; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    class CImage* m_pPortrait;
    CNamedOptionManager* m_pNamedOptionManager;

	CList<CInvestigator, CInvestigator&> m_UndoList;
	CList<CInvestigator, CInvestigator&> m_RedoList;


// Generated message map functions
protected:
	BOOL ParseSheetSection( CString& strSection );
	BOOL SerialiseInvestigator( CArchive& ar, int nVersion );
	//{{AFX_MSG(CInvestigatorDoc)
	afx_msg void OnInvestigatorAddSkill();
	afx_msg void OnInvestigatorModifySkill();
	afx_msg void OnInvestigatorChooseWeapons();
	afx_msg void OnInvestigatorIncome();
	afx_msg void OnToolsRandomName();
	afx_msg void OnToolsRerollInvestigator();
	afx_msg void OnInvestigatorSpellbook();
	afx_msg void OnToolsCheckSheet();
	afx_msg void OnToolsRollCheckedSkills();
	afx_msg void OnFileExportByakhee10();
	afx_msg void OnFileExportByakhee20();
	afx_msg void OnFileExportHtml();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL ParseMeleeSection( CString& strSection );
	BOOL ParseFirearmSection( CString& strSection );
	BOOL ParseSpecialSkillSection( CString& strSection );
	BOOL ParseSkillSection( CString& strSection );
	BOOL ParseInvestigatorSection( CString& strSection );
	CString BuildKeyPair( CString strName, int nValue );
	CString BuildKeyPair( CString strName, CString strValue );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVESTIGATORDOC_H__A9FB21CC_4B45_11D2_9FF9_00001C192944__INCLUDED_)
