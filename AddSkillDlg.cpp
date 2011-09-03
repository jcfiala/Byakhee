// AddSkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "AddSkillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSkillDlg dialog


CAddSkillDlg::CAddSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddSkillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddSkillDlg)
	m_strBase = _T("");
	m_nNumberOfSubSkills = 2;
	m_strSkillName = _T("");
	m_nSkillType = -1;
	m_fListWithMainSkills = TRUE;
	//}}AFX_DATA_INIT
    
    m_pInvestigator = NULL;
}


void CAddSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddSkillDlg)
	DDX_Control(pDX, IDC_LISTWITHMAINSKILLS, m_wndListWithMainSkills);
	DDX_Control(pDX, IDC_ADVANCED, m_wndAdvanced);
	DDX_Control(pDX, IDC_SKILLTYPE2, m_wndSkillType2);
	DDX_Control(pDX, IDC_SKILLTYPE1, m_wndSkillType1);
	DDX_Control(pDX, IDC_NUMBEROFSUBSKILLS, m_wndNumberOfSubskills);
	DDX_Control(pDX, IDC_INSERTSPECIALTEXT, m_wndInsertSpecialText);
	DDX_Control(pDX, IDC_DIVIDER, m_wndDivider);
	DDX_Control(pDX, IDC_NUMBEROFSUBSKILLS_SPIN, m_wndNumberOfSubskillsSpin);
	DDX_Text(pDX, IDC_BASE, m_strBase);
	DDX_Text(pDX, IDC_NUMBEROFSUBSKILLS, m_nNumberOfSubSkills);
	DDV_MinMaxInt(pDX, m_nNumberOfSubSkills, 0, 5);
	DDX_Text(pDX, IDC_SKILLNAME, m_strSkillName);
	DDX_Radio(pDX, IDC_SKILLTYPE1, m_nSkillType);
	DDX_Check(pDX, IDC_LISTWITHMAINSKILLS, m_fListWithMainSkills);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddSkillDlg, CDialog)
	//{{AFX_MSG_MAP(CAddSkillDlg)
	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	ON_BN_CLICKED(IDC_INSERTSPECIALTEXT, OnInsertSpecialText)
	ON_BN_CLICKED(IDC_SKILLTYPE1, OnSkilltype1)
	ON_BN_CLICKED(IDC_SKILLTYPE2, OnSkilltype2)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_BASESTRINGPOPUP_APP, OnBaseStringAPP )
    ON_COMMAND(ID_BASESTRINGPOPUP_CON, OnBaseStringCON )
    ON_COMMAND(ID_BASESTRINGPOPUP_DEX, OnBaseStringDEX )
    ON_COMMAND(ID_BASESTRINGPOPUP_EDU, OnBaseStringEDU )
    ON_COMMAND(ID_BASESTRINGPOPUP_INT, OnBaseStringINT )
    ON_COMMAND(ID_BASESTRINGPOPUP_POW, OnBaseStringPOW )
    ON_COMMAND(ID_BASESTRINGPOPUP_SIZ, OnBaseStringSIZ )
    ON_COMMAND(ID_BASESTRINGPOPUP_STR, OnBaseStringSTR )
    ON_COMMAND(ID_BASESTRINGPOPUP_LUCK, OnBaseStringLuck )
    ON_COMMAND(ID_BASESTRINGPOPUP_KNOW, OnBaseStringKnow )
    ON_COMMAND(ID_BASESTRINGPOPUP_IDEA, OnBaseStringIdea )
    ON_COMMAND(ID_BASESTRINGPOPUP_SAN, OnBaseStringSan )
    ON_COMMAND(ID_BASESTRINGPOPUP_HP, OnBaseStringHitPoints )
    ON_COMMAND(ID_BASESTRINGPOPUP_MAGIC, OnBaseStringMagic )
    ON_COMMAND(ID_BASESTRINGPOPUP_NONE, OnBaseStringNothingSpecial )
    ON_COMMAND(ID_BASESTRINGPOPUP_NOMODIFY, OnBaseStringNoModify )
    ON_COMMAND(ID_BASESTRINGPOPUP_COREFIREARM, OnBaseStringCoreFirearm )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddSkillDlg message handlers

void CAddSkillDlg::OnAdvanced() 
{
    BOOL fExpanded = AfxGetApp()->GetProfileInt( "WindowPos", "AddSkillExpand", FALSE );
    AfxGetApp()->WriteProfileInt( "WindowPos", "AddSkillExpand", !fExpanded );
    ShowHideAdvanced();
}

void CAddSkillDlg::OnInsertSpecialText() 
{
    CMenu Menu; 
    VERIFY(Menu.LoadMenu(IDR_BASESTRINGPOPUP));

    CMenu* pPopup = Menu.GetSubMenu(0);
    ASSERT(pPopup != NULL);

    UpdateData(UD_GETDATA);

    //set options default item
    pPopup->CheckMenuRadioItem(ID_BASESTRINGPOPUP_NONE, ID_BASESTRINGPOPUP_COREFIREARM, ID_BASESTRINGPOPUP_NONE, MF_BYCOMMAND );
    if( m_strBase.IsEmpty() == FALSE )
    {
        switch(m_strBase[0])
        {
            case '*':
                pPopup->CheckMenuRadioItem(ID_BASESTRINGPOPUP_NONE, ID_BASESTRINGPOPUP_COREFIREARM, ID_BASESTRINGPOPUP_NOMODIFY, MF_BYCOMMAND );
                break;
            case 'f':
            case 'F':
                pPopup->CheckMenuRadioItem(ID_BASESTRINGPOPUP_NONE, ID_BASESTRINGPOPUP_COREFIREARM, ID_BASESTRINGPOPUP_COREFIREARM, MF_BYCOMMAND );
                break;
        }
    }

    //set multiplier default item
    //FIXME?

    UpdateData(UD_SETDATA);

    RECT rc;
    m_wndInsertSpecialText.GetWindowRect( &rc );
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rc.right, rc.top, this);
	
}

void CAddSkillDlg::OnSkilltype1() 
{
    m_wndNumberOfSubskillsSpin.EnableWindow(FALSE);
    m_wndNumberOfSubskills.EnableWindow(FALSE);
}

void CAddSkillDlg::OnSkilltype2() 
{
    m_wndNumberOfSubskillsSpin.EnableWindow(TRUE);
    m_wndNumberOfSubskills.EnableWindow(TRUE);
}

void CAddSkillDlg::ShowHideAdvanced()
{
    //get current setting
    BOOL fExpanded = AfxGetApp()->GetProfileInt( "WindowPos", "AddSkillExpand", FALSE );

    //get divider position
    CRect rcDivider;
    m_wndDivider.GetWindowRect( rcDivider );
    ScreenToClient( rcDivider );

    //get dialog size
    CRect rcDialog;
    GetWindowRect( rcDialog );
    ScreenToClient( rcDialog );

    if( fExpanded == FALSE )
    {
        SetWindowPos( NULL, 0, 0, rcDialog.Width(), rcDivider.top + (GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME)), SWP_NOMOVE|SWP_NOZORDER );
        m_wndInsertSpecialText.ShowWindow( SW_HIDE );
        m_wndSkillType1.ShowWindow(SW_HIDE);
        m_wndSkillType2.ShowWindow(SW_HIDE);
        m_wndNumberOfSubskillsSpin.ShowWindow(SW_HIDE);
        m_wndNumberOfSubskills.ShowWindow(SW_HIDE);
        m_wndListWithMainSkills.ShowWindow(SW_HIDE);
        m_wndAdvanced.SetWindowText( "&Advanced >>" );
    }
    else
    {
        SetWindowPos( NULL, 0, 0, rcDialog.Width(), m_nExpandedHeight, SWP_NOMOVE|SWP_NOZORDER );
        m_wndInsertSpecialText.ShowWindow( SW_SHOW );
        m_wndSkillType1.ShowWindow(SW_SHOW);
        m_wndSkillType2.ShowWindow(SW_SHOW);
        m_wndNumberOfSubskillsSpin.ShowWindow(SW_SHOW);
        m_wndNumberOfSubskills.ShowWindow(SW_SHOW);
        m_wndListWithMainSkills.ShowWindow(SW_SHOW);
        m_wndAdvanced.SetWindowText( "&Advanced <<" );
    }
}

BOOL CAddSkillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    ASSERT(m_pInvestigator);
	
    CRect rcDialog;
    GetWindowRect( rcDialog );
    ScreenToClient( rcDialog );
    m_nExpandedHeight = rcDialog.Height();
    m_wndNumberOfSubskillsSpin.SetRange( 0, 5 );
    ShowHideAdvanced();
    m_nSkillType = 0; OnSkilltype1();
    m_strBase = "0";
    UpdateData(UD_SETDATA);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddSkillDlg::OnBaseStringAPP()
{
    AddMultiplier( "APP" );
}

void CAddSkillDlg::OnBaseStringCON()
{
    AddMultiplier( "CON" );
}

void CAddSkillDlg::OnBaseStringDEX()
{
    AddMultiplier( "DEX" );
}

void CAddSkillDlg::OnBaseStringEDU()
{
    AddMultiplier( "EDU" );
}

void CAddSkillDlg::OnBaseStringINT()
{
    AddMultiplier( "INT" );
}

void CAddSkillDlg::OnBaseStringPOW()
{
    AddMultiplier( "POW" );
}

void CAddSkillDlg::OnBaseStringSIZ()
{
    AddMultiplier( "SIZ" );
}

void CAddSkillDlg::OnBaseStringSTR()
{
    AddMultiplier( "STR" );
}

void CAddSkillDlg::OnBaseStringLuck()
{
    AddMultiplier( "LUCK" );
}

void CAddSkillDlg::OnBaseStringKnow()
{
    AddMultiplier( "KNOW" );
}

void CAddSkillDlg::OnBaseStringIdea()
{
    AddMultiplier( "IDEA" );
}

void CAddSkillDlg::OnBaseStringSan()
{
    AddMultiplier( "SAN" );
}

void CAddSkillDlg::OnBaseStringHitPoints()
{
    AddMultiplier( "HP" );
}

void CAddSkillDlg::OnBaseStringMagic()
{
    AddMultiplier( "MAGIC" );
}

void CAddSkillDlg::OnBaseStringNoModify()
{
    SetPrefix("*");
}

void CAddSkillDlg::OnBaseStringCoreFirearm()
{
    SetPrefix("F");
}

void CAddSkillDlg::OnBaseStringNothingSpecial()
{
    SetPrefix("");
}

void CAddSkillDlg::SetInvestigator(CInvestigator * pInvestigator)
{
    m_pInvestigator = pInvestigator;
}

void CAddSkillDlg::AddMultiplier(CString strMultiplier)
{
    UpdateData(UD_GETDATA);
    
    //prepare full multiplier
    strMultiplier = CString("^") + strMultiplier;

    CString strPrefix;
    if( m_strBase.IsEmpty() == FALSE )
    {
        //remove current prefix
        if( m_strBase[0] == 'F' || m_strBase[0] == 'f' || m_strBase[0] == '*' )
        {
            strPrefix.Format( "%c", m_strBase[0] );
            m_strBase = m_strBase.Right(m_strBase.GetLength()-1);
        }

        //remove current multiplier
        if( m_strBase[0] == '^' )
        {
            m_strBase = m_strBase.Right(m_strBase.GetLength()-1);
            while( isalpha(m_strBase[0])) m_strBase = m_strBase.Right(m_strBase.GetLength()-1);
        }
    }


    //bulid new base
    m_strBase = strPrefix + strMultiplier + m_strBase;
    UpdateData(UD_SETDATA);
}

void CAddSkillDlg::SetPrefix(CString strPrefix)
{
    UpdateData(UD_GETDATA);

    //remove current base
    if( m_strBase.IsEmpty() == FALSE )
    {
        if( m_strBase[0] == 'F' || m_strBase[0] == 'f' || m_strBase[0] == '*' )
            m_strBase = m_strBase.Right(m_strBase.GetLength()-1);
    }

    //set new prefix
    m_strBase = strPrefix + m_strBase;
    UpdateData(UD_SETDATA);
}


void CAddSkillDlg::OnOK() 
{
    UpdateData(UD_GETDATA);

    //make sure it's OK
    if( m_strSkillName.IsEmpty() || m_strSkillName.Compare( "*" ) == 0 )
    {
        AfxMessageBox( "This is not a valid skill name", MB_ICONEXCLAMATION );
        return;
    }

    //see if this skill already exists
    CInvestigatorSkill* pInvestigatorSkillFind = m_pInvestigator->FindSkill(m_strSkillName);
    if( pInvestigatorSkillFind != NULL && pInvestigatorSkillFind->m_pSkill->m_SkillType != SUBSKILL )
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

    //append the * if it's to be listed with the main skills
    if( m_fListWithMainSkills ) m_strSkillName = CString("*") + m_strSkillName;

    //create this skill
    CSkill* pSkill = NULL;
    if( m_nSkillType == 0 )
        pSkill = new CSkill( m_strSkillName, m_strBase, ERA_ISALL );
    else
        pSkill = new CSpecialSkill( m_strSkillName, m_strBase, ERA_ISALL, m_nNumberOfSubSkills );

    //evaluate the base to make sure it's OK
    int nMinimum = pSkill->EvaluateBase(m_pInvestigator);
    if( nMinimum == -666 ) { delete pSkill; return; }//malformed

    //add it to the skill lists
    m_pInvestigator->AddSkill(pSkill);
    m_pInvestigator->SortSkillList();

	CDialog::OnOK();
}
