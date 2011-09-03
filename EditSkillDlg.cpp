// EditSkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "EditSkillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditSkillDlg dialog


CEditSkillDlg::CEditSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditSkillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditSkillDlg)
	m_strBase = _T("");
	m_strSkillName = _T("");
	m_fListWithMainSkills = FALSE;
	//}}AFX_DATA_INIT

    m_pInvestigator = NULL;
    m_pInvestigatorSkill = NULL;
}


void CEditSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditSkillDlg)
	DDX_Text(pDX, IDC_BASE, m_strBase);
	DDX_Text(pDX, IDC_SKILLNAME, m_strSkillName);
	DDX_Check(pDX, IDC_LISTWITHMAINSKILLS, m_fListWithMainSkills);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditSkillDlg, CDialog)
	//{{AFX_MSG_MAP(CEditSkillDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditSkillDlg message handlers

BOOL CEditSkillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    ASSERT(m_pInvestigator);
    ASSERT(m_pInvestigatorSkill);
	
    m_strSkillName = m_pInvestigatorSkill->m_pSkill->m_strName;
    m_strBase = m_pInvestigatorSkill->m_pSkill->m_strBase;

    m_fListWithMainSkills = FALSE;
    if( m_strSkillName.IsEmpty() == FALSE && m_strSkillName[0] == '*' )
    {
        m_fListWithMainSkills = TRUE;
        m_strSkillName = m_strSkillName.Right( m_strSkillName.GetLength()-1 );
    }

    UpdateData(UD_SETDATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditSkillDlg::OnOK() 
{
    UpdateData(UD_GETDATA);

    //make sure it's OK
    if( m_strSkillName.IsEmpty() || m_strSkillName.Compare( "*" ) == 0 )
    {
        AfxMessageBox( "This is not a valid skill name", MB_ICONEXCLAMATION );
        return;
    }

    //see if this skill already exists
    CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(m_strSkillName);
    if( pInvestigatorSkill != NULL && pInvestigatorSkill != m_pInvestigatorSkill && pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL )
    {
        AfxMessageBox( CString("This investigator already has a skill called ") + m_strSkillName + CString("\nPlease choose a different name"), MB_ICONEXCLAMATION );
        return;
    }

    //check to see if they're trying to create a 5.5 skill
    if( m_strBase.Compare( "1" ) == 0 )
    {
        switch( AfxMessageBox( "Note: You've chosen to set the base string to \"1\". If you want to display investigator skills so that they're compatible with Call of Cthulhu versions 5.5 upwards, click on View|Cthulhu 5.5 Bases in the main sheet window\nDo you want to set this base back to zero and use this option?", MB_YESNOCANCEL ) )
        {
            case IDYES: m_strBase = "0"; break;
            case IDCANCEL: return;
        }
    }
	

    //change the base
    CString strOldBase = m_pInvestigatorSkill->m_pSkill->m_strBase;
    m_pInvestigatorSkill->m_pSkill->m_strBase = m_strBase;

    //recalculate the skill's minimum value
    int nMinimum = m_pInvestigatorSkill->m_pSkill->EvaluateBase( m_pInvestigator );
    if( nMinimum == -666 )
    {
        //it was malformed - reset it and don't close the dialog
        m_pInvestigatorSkill->m_pSkill->m_strBase = strOldBase;
        return;
    }

    //set minimum value
    m_pInvestigatorSkill->m_nMinimum = nMinimum;
    if( m_pInvestigatorSkill->m_nCurrent < nMinimum ) m_pInvestigatorSkill->m_nCurrent = nMinimum;

    //append the * if it's to be listed with the main skills
    if( m_fListWithMainSkills ) m_strSkillName = CString("*") + m_strSkillName;

    //change name
    m_pInvestigatorSkill->m_pSkill->m_strName = m_strSkillName;

    //close the dialog
	CDialog::OnOK();
}

void CEditSkillDlg::SetInvestigator(CInvestigator * pInvestigator)
{
    m_pInvestigator = pInvestigator;
}

void CEditSkillDlg::SetSkill(CInvestigatorSkill * pInvestigatorSkill)
{
    m_pInvestigatorSkill = pInvestigatorSkill;
}
