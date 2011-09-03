// Wizard_NewInvestigatorPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "Wizard_NewInvestigator.h"
#include "Wizard_NewInvestigatorPages.h"
#include "Output.h"
#include "CharacterAgeDlg.h"
#include "SkillManager.h"
#include "SectionFile.h"
#include "ScriptEngine/ScriptingEngine.h"
#include "DiceRoller.h"
#include "QuickInfoDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MAX_AGE 111 //maximum age selectable from the wizard

IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage1, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage2, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage3, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage4, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage5, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage6, CPropertyPage)
IMPLEMENT_DYNCREATE(CWizard_NewInvestigatorPage7, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage1 property page

CWizard_NewInvestigatorPage1::CWizard_NewInvestigatorPage1() : CPropertyPage(CWizard_NewInvestigatorPage1::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage1)
	m_nAGE = 0;
	m_nAPP = 0;
	m_fCanEdit = (BOOL)AfxGetApp()->GetProfileInt( "Wizard", "Can Edit", FALSE );
	m_nCON = 0;
	m_strDB = _T("");
	m_nDEX = 0;
	m_nEDU = 0;
	m_fExtraSchooling = FALSE;
	m_strHP = _T("");
	m_strIdea = _T("");
	m_nINT = 0;
	m_strInterestPoints = _T("");
	m_strKnow = _T("");
	m_strLuck = _T("");
	m_nPOW = 0;
	m_strSAN = _T("");
	m_nSIZ = 0;
	m_strSkillPoints = _T("");
	m_nSTR = 0;
	//}}AFX_DATA_INIT

    m_nBaseEDU = 0;
    m_fDoneCoreGeneration = FALSE;
    m_fShownTipBefore = FALSE;

    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RANDOM), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CWizard_NewInvestigatorPage1::~CWizard_NewInvestigatorPage1()
{
    DestroyIcon( m_hIcon );
}

void CWizard_NewInvestigatorPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage1)
	DDX_Control(pDX, IDC_FRAME2, m_wndFrame2);
	DDX_Control(pDX, IDC_FRAME1, m_wndFrame1);
	DDX_Control(pDX, IDC_REROLL, m_wndReroll);
	DDX_Control(pDX, IDC_EXTRASCHOOLING, m_wndExtraSchooling);
	DDX_Control(pDX, IDC_CANEDIT, m_wndCanEdit);
	DDX_Control(pDX, IDC_AGE, m_wndAge);
	DDX_Control(pDX, IDC_STR, m_wndSTR);
	DDX_Control(pDX, IDC_SIZ, m_wndSIZ);
	DDX_Control(pDX, IDC_POW, m_wndPOW);
	DDX_Control(pDX, IDC_INT, m_wndINT);
	DDX_Control(pDX, IDC_EDU, m_wndEDU);
	DDX_Control(pDX, IDC_DEX, m_wndDEX);
	DDX_Control(pDX, IDC_APP, m_wndAPP);
	DDX_Control(pDX, IDC_CON, m_wndCON);
	DDX_Control(pDX, IDC_AGESPIN, m_wndAgeSpin);
	DDX_Text(pDX, IDC_AGE, m_nAGE);
	DDX_Text(pDX, IDC_APP, m_nAPP);
	DDV_MinMaxInt(pDX, m_nAPP, 0, 99);
	DDX_Check(pDX, IDC_CANEDIT, m_fCanEdit);
	DDX_Text(pDX, IDC_CON, m_nCON);
	DDV_MinMaxInt(pDX, m_nCON, 0, 99);
	DDX_Text(pDX, IDC_DB, m_strDB);
	DDX_Text(pDX, IDC_DEX, m_nDEX);
	DDV_MinMaxInt(pDX, m_nDEX, 0, 99);
	DDX_Text(pDX, IDC_EDU, m_nEDU);
	DDV_MinMaxInt(pDX, m_nEDU, 0, 99);
	DDX_Check(pDX, IDC_EXTRASCHOOLING, m_fExtraSchooling);
	DDX_Text(pDX, IDC_HP, m_strHP);
	DDX_Text(pDX, IDC_IDEA, m_strIdea);
	DDX_Text(pDX, IDC_INT, m_nINT);
	DDV_MinMaxInt(pDX, m_nINT, 0, 99);
	DDX_Text(pDX, IDC_INTERESTPOINTS, m_strInterestPoints);
	DDX_Text(pDX, IDC_KNOW, m_strKnow);
	DDX_Text(pDX, IDC_LUCK, m_strLuck);
	DDX_Text(pDX, IDC_POW, m_nPOW);
	DDV_MinMaxInt(pDX, m_nPOW, 0, 99);
	DDX_Text(pDX, IDC_SAN, m_strSAN);
	DDX_Text(pDX, IDC_SIZ, m_nSIZ);
	DDV_MinMaxInt(pDX, m_nSIZ, 0, 99);
	DDX_Text(pDX, IDC_SKILLPOINTS, m_strSkillPoints);
	DDX_Text(pDX, IDC_STR, m_nSTR);
	DDV_MinMaxInt(pDX, m_nSTR, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_AGESPIN, OnDeltaposAgeSpin)
	ON_EN_KILLFOCUS(IDC_AGE, OnKillFocusAge)
	ON_EN_CHANGE(IDC_APP, OnChangeApp)
	ON_EN_CHANGE(IDC_CON, OnChangeCon)
	ON_EN_CHANGE(IDC_DEX, OnChangeDex)
	ON_EN_CHANGE(IDC_EDU, OnChangeEdu)
	ON_EN_CHANGE(IDC_INT, OnChangeInt)
	ON_EN_CHANGE(IDC_POW, OnChangePow)
	ON_EN_CHANGE(IDC_SIZ, OnChangeSiz)
	ON_EN_CHANGE(IDC_STR, OnChangeStr)
	ON_BN_CLICKED(IDC_REROLL, OnReroll)
	ON_BN_CLICKED(IDC_CANEDIT, OnCanEditToggle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWizard_NewInvestigatorPage1::OnDeltaposAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

    UpdateData(UD_GETDATA);
    int nDifference = ( m_nAGE - (m_nBaseEDU + 6) ) / 10;
	m_nEDU = m_nBaseEDU + nDifference;
    UpdateData(UD_SETDATA);
	
	*pResult = 0;
}

void CWizard_NewInvestigatorPage1::OnKillFocusAge() 
{
    UpdateData(UD_GETDATA);

    int nMinAge = m_nEDU + 6;

    if( m_nAGE < nMinAge ) m_nAGE = nMinAge;
    else if( m_nAGE > MAX_AGE ) m_nAGE = MAX_AGE;

	int nDifference = ( m_nAGE - nMinAge ) / 10;
	m_nEDU = m_nBaseEDU + nDifference;

    UpdateData(UD_SETDATA);
}

void CWizard_NewInvestigatorPage1::OnChangeApp() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndAPP.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangeCon() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndCON.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangeDex() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndDEX.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangeEdu() 
{
    //before we validate it, make sure it's not an empty string
    if( m_wndEDU.GetWindowTextLength() > 0 )
    {
        UpdateData(UD_GETDATA);
        m_nAGE = m_nEDU + 6;
        m_nBaseEDU = m_nEDU;
        m_wndAgeSpin.SetRange( m_nAGE, MAX_AGE );
        UpdateData(UD_SETDATA);
        UpdateInvestigatorStats();
    }
}

void CWizard_NewInvestigatorPage1::OnChangeInt() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndINT.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangePow() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndPOW.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangeSiz() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndSIZ.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnChangeStr() 
{
    //before we update it, make sure it's not an empty string
    if( m_wndSTR.GetWindowTextLength() > 0 ) UpdateInvestigatorStats();
}

void CWizard_NewInvestigatorPage1::OnReroll() 
{
    //new character - enable everything else...
    m_fDoneCoreGeneration = FALSE;
    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.SetInvestigator(NULL);

    //roll a base character
    CDiceRoller Dice;
    m_nSTR = Dice.Roll( "3D6" );
    m_nCON = Dice.Roll( "3D6" );
    m_nPOW = Dice.Roll( "3D6" );
    m_nDEX = Dice.Roll( "3D6" );
    m_nAPP = Dice.Roll( "3D6" );
    m_nSIZ = Dice.Roll( "2D6+6" );
    m_nINT = Dice.Roll( "2D6+6" );
    m_nEDU = Dice.Roll( "3D6+6" );
    m_nBaseEDU = m_nEDU;

    //set stats
    m_nAGE = m_nEDU + 6;
    m_wndAgeSpin.SetRange( m_nAGE, MAX_AGE );

    //write data to dialog
    UpdateData(UD_SETDATA);

    //update the rest of the dialog
    UpdateInvestigatorStats();

    //disable everything if we need to
    m_wndAPP.EnableWindow( !m_fDoneCoreGeneration );
    m_wndCON.EnableWindow( !m_fDoneCoreGeneration );
    m_wndDEX.EnableWindow( !m_fDoneCoreGeneration );
    m_wndEDU.EnableWindow( !m_fDoneCoreGeneration );
    m_wndINT.EnableWindow( !m_fDoneCoreGeneration );
    m_wndPOW.EnableWindow( !m_fDoneCoreGeneration );
    m_wndSIZ.EnableWindow( !m_fDoneCoreGeneration );
    m_wndSTR.EnableWindow( !m_fDoneCoreGeneration );
    m_wndAge.EnableWindow( !m_fDoneCoreGeneration );
    m_wndCanEdit.EnableWindow( !m_fDoneCoreGeneration );
    m_wndAgeSpin.EnableWindow( !m_fDoneCoreGeneration );
    m_wndExtraSchooling.EnableWindow( !m_fDoneCoreGeneration );
}

void CWizard_NewInvestigatorPage1::OnCanEditToggle() 
{
	//get data from dialog
	UpdateData(UD_GETDATA);

    //write it to the registry
    AfxGetApp()->WriteProfileInt( "Wizard", "Can Edit", m_fCanEdit );

    //change read only option
    m_wndAPP.SetReadOnly( !m_fCanEdit );
    m_wndCON.SetReadOnly( !m_fCanEdit );
    m_wndDEX.SetReadOnly( !m_fCanEdit );
    m_wndEDU.SetReadOnly( !m_fCanEdit );
    m_wndINT.SetReadOnly( !m_fCanEdit );
    m_wndPOW.SetReadOnly( !m_fCanEdit );
    m_wndSIZ.SetReadOnly( !m_fCanEdit );
    m_wndSTR.SetReadOnly( !m_fCanEdit );

    //note: we can't change the borders as Windows doesn't appear to support this...

    //switch to the first control and select all if edit has been turned on
    if( m_fCanEdit ) 
    {
        m_wndSTR.SetFocus();
        m_wndSTR.SetSel( 0, -1 );
    }
}

LRESULT CWizard_NewInvestigatorPage1::OnWizardNext()
{
    if( m_fDoneCoreGeneration == FALSE )
    {
        //read values from dialog
        UpdateData(UD_GETDATA);

        //prompt for age degredation stat loss
	    if( m_nAGE + (m_fExtraSchooling ? 1 : 0 ) > 40 )
	    {
		    //FIXME: ok on 50, 60, 70 etc?
		    int nDifference = ( m_nAGE + (m_fExtraSchooling ? 1 : 0 )) / 10;
		    nDifference -= 3; //for every 10 years after 40

		    int nAPPRot = 0;
		    int nDEXRot = 0;
		    int nCONRot = 0;
		    int nSTRRot = 0;

            //make sure this investigator is strong enough
            if( m_nAPP+m_nDEX+m_nCON+m_nSTR <= nDifference )
            {
                AfxMessageBox( "Warning: You have selected a character that is so weak\nthat if the effects of aging were applied, APP, DEX, CON and STR would all be zero or less!\n\nThe effects of aging will not be applied, but you should consider your character seriously!", MB_ICONEXCLAMATION );
            }
            else
            {
		        while( nDifference )
		        {
			        CCharacterAgeDlg CharacterAgeDlg;
			        if( CharacterAgeDlg.DoModal() == IDOK )
			        {
				        switch( CharacterAgeDlg.m_nWhichStatistic )
				        {
					        case CCharacterAgeDlg::ROT_APP: 
                                if(m_nAPP-nAPPRot < 0 ) 
                                    AfxMessageBox( "Invalid Selection: APP is at 0", MB_ICONEXCLAMATION );
                                else
                                {
                                    if(m_nAPP-nAPPRot == 0 ) AfxMessageBox( "Warning: APP is now 0", MB_ICONINFORMATION );
                                    nDifference--;
                                    nAPPRot++; 
                                }
                                break;
					        case CCharacterAgeDlg::ROT_DEX:
                                if(m_nDEX-nDEXRot < 0 ) 
                                    AfxMessageBox( "Invalid Selection: DEX is at 0", MB_ICONEXCLAMATION );
                                else
                                {
                                    if(m_nDEX-nDEXRot == 0 ) AfxMessageBox( "Warning: DEX is now 0", MB_ICONINFORMATION );
                                    nDifference--;
                                    nDEXRot++; 
                                }
                                break;

					        case CCharacterAgeDlg::ROT_CON:
                                if(m_nCON-nCONRot < 0 ) 
                                    AfxMessageBox( "Invalid Selection: CON is at 0", MB_ICONEXCLAMATION );
                                else
                                {
                                    if(m_nCON-nCONRot == 0 ) AfxMessageBox( "Warning: CON is now 0", MB_ICONINFORMATION );
                                    nDifference--;
                                    nCONRot++; 
                                }
                                break;

					        case CCharacterAgeDlg::ROT_STR:
                                if(m_nSTR-nSTRRot < 0 ) 
                                    AfxMessageBox( "Invalid Selection: STR is at 0", MB_ICONEXCLAMATION );
                                else
                                {
                                    if(m_nSTR-nSTRRot == 0 ) AfxMessageBox( "Warning: STR is now 0", MB_ICONINFORMATION );
                                    nDifference--;
                                    nSTRRot++; 
                                }
                                break;

					        default: ASSERT(FALSE);
				        }
			        }
                    else //don't go to next if they click cancel at any point
				        return -1;
		        }

                //apply rotting
		        m_nAPP -= nAPPRot;
		        m_nDEX -= nDEXRot;
		        m_nCON -= nCONRot;
		        m_nSTR -= nSTRRot;
            }
	    }


        //perform stay in school check
        if( m_fExtraSchooling )
        {
            //one year - age regardless
            m_nAGE++;

            //see if we learned anything
            CDiceRoller Dice;
	        if( Dice.Roll( "D100" ) <= (m_nINT*5) )
	        {
                m_nEDU++; //gain one point of EDU
                m_pInvestigator->m_ExtraSchooling = CInvestigator::EXTRASCHOOLING_SUCCESS;
            }
            else 
                m_pInvestigator->m_ExtraSchooling = CInvestigator::EXTRASCHOOLING_FAILED;
        }
        else
            m_pInvestigator->m_ExtraSchooling = CInvestigator::EXTRASCHOOLING_NOTAPPLICABLE;
  
        //write core investigator to investigator sheet
        m_pInvestigator->m_nAge = m_nAGE;

        m_pInvestigator->m_nAPP = m_nAPP;
        m_pInvestigator->m_nCON = m_nCON;
        m_pInvestigator->m_nDEX  = m_nDEX;
        m_pInvestigator->m_nEDU  = m_nEDU;
        m_pInvestigator->m_nINT  = m_nINT;
        m_pInvestigator->m_nPOW = m_nPOW;
        m_pInvestigator->m_nSIZ = m_nSIZ;
        m_pInvestigator->m_nSTR = m_nSTR;

        m_pInvestigator->CalculateSkillPoints();
        m_pInvestigator->CalculateCoreStatistics();

        //store skill points in investigator wizard
        ((CWizard_NewInvestigator*)GetParent())->m_nSkillPoints = m_pInvestigator->m_nSkillPoints;
        ((CWizard_NewInvestigator*)GetParent())->m_nPersonalInterestPoints = m_pInvestigator->m_nInterestPoints;

        //display skill points in status bar
        CString strPoints;
        strPoints.Format( "Skill: %d Hobby: %d", m_pInvestigator->m_nSkillPoints, m_pInvestigator->m_nInterestPoints );
        CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), strPoints );
        wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), "" );

        //run the income script
        m_pInvestigator->RunIncomeScript();

        //put final values onto page
        UpdateData(UD_SETDATA);

        if( m_fChooseSkills == FALSE )
        {
            //add default skills
            if( m_pInvestigator->SetOccupation( "" ) )
                m_pInvestigator->m_strOccupation = "Investigator";
            else
                AfxMessageBox( "Warning: Failed to add skills to investigator!", MB_ICONEXCLAMATION );
        }
    }

    m_fDoneCoreGeneration = TRUE; //all done - disable everything if people click "back" on subsequent wizards

    //show the popup
    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.SetInvestigator(m_pInvestigator);

    //skip to last but one page if they didn't want to allocate skills
    if( m_fChooseSkills == FALSE )
    {
        //jump to last but one page (name input etc.)
        return IDD_WIZARD_NEWINVESTIGATOR_PAGE6;
    }
    else
	    return CPropertyPage::OnWizardNext();
}

void CWizard_NewInvestigatorPage1::UpdateInvestigatorStats()
{
    //get data from dialog
    UpdateData(UD_GETDATA);

	//update calculated fields
	m_strHP.Format( "%d", (int)( 0.5 + (((double)m_nCON + (double)m_nSIZ ) / 2.0 ) ) ); //fraction is rounded up
    m_strDB = CInvestigator::GetDBString( m_nSTR, m_nSIZ );
	m_strLuck.Format( "%d", m_nPOW * 5 );
	m_strKnow.Format( "%d", m_nEDU * 5 );
	m_strIdea.Format( "%d", m_nINT * 5 );
	m_strSAN.Format(  "%d", m_nPOW * 5 );
	m_strInterestPoints.Format( "%d", m_nINT * 10 );
	m_strSkillPoints.Format( "%d", m_nEDU * 20 ); 
    
    //write new data back to the dialog
    UpdateData(UD_SETDATA);
}

BOOL CWizard_NewInvestigatorPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //create the info window
    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.Create(GetParent());
    
    //set the random button's icon
	m_wndReroll.SetIcon( m_hIcon );

    //set the choose skill initial value
    m_fChooseSkills = AfxGetApp()->GetProfileInt( "Wizard", "FullWiz", TRUE );

    //change read only option
    m_wndAPP.SetReadOnly( !m_fCanEdit );
    m_wndCON.SetReadOnly( !m_fCanEdit );
    m_wndDEX.SetReadOnly( !m_fCanEdit );
    m_wndEDU.SetReadOnly( !m_fCanEdit );
    m_wndINT.SetReadOnly( !m_fCanEdit );
    m_wndPOW.SetReadOnly( !m_fCanEdit );
    m_wndSIZ.SetReadOnly( !m_fCanEdit );
    m_wndSTR.SetReadOnly( !m_fCanEdit );

    //create an initial character
    OnReroll();

    //prepare the swap edit control's display rectangles
    CRect rcFrame1; m_wndFrame1.GetWindowRect( rcFrame1 ); ScreenToClient( rcFrame1 );
    CRect rcFrame2; m_wndFrame2.GetWindowRect( rcFrame2 ); ScreenToClient( rcFrame2 );

    m_wndAPP.SetSwapRect( rcFrame1 );
    m_wndCON.SetSwapRect( rcFrame1 );
    m_wndDEX.SetSwapRect( rcFrame1 );
    m_wndPOW.SetSwapRect( rcFrame1 );
    m_wndSTR.SetSwapRect( rcFrame1 );

    m_wndINT.SetSwapRect( rcFrame2 );
    m_wndSIZ.SetSwapRect( rcFrame2 );

    m_wndEDU.SetSwapRect( CRect(-1,-1,-1,-1) ); //no swap rectangle

	return TRUE;
}

BOOL CWizard_NewInvestigatorPage1::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_NEXT );

    //disable everything if we need to
    m_wndAPP.EnableWindow( !m_fDoneCoreGeneration );
    m_wndCON.EnableWindow( !m_fDoneCoreGeneration );
    m_wndDEX.EnableWindow( !m_fDoneCoreGeneration );
    m_wndEDU.EnableWindow( !m_fDoneCoreGeneration );
    m_wndINT.EnableWindow( !m_fDoneCoreGeneration );
    m_wndPOW.EnableWindow( !m_fDoneCoreGeneration );
    m_wndSIZ.EnableWindow( !m_fDoneCoreGeneration );
    m_wndSTR.EnableWindow( !m_fDoneCoreGeneration );
    m_wndAge.EnableWindow( !m_fDoneCoreGeneration );
    m_wndCanEdit.EnableWindow( !m_fDoneCoreGeneration );
    m_wndAgeSpin.EnableWindow( !m_fDoneCoreGeneration );
    m_wndExtraSchooling.EnableWindow( !m_fDoneCoreGeneration );

	BOOL fResult = CPropertyPage::OnSetActive();

    if( m_fShownTipBefore == FALSE )
    {
        //display tip if the dialog is being displayed for the first time ONLY
        m_fShownTipBefore = TRUE;
        CQuickInfoDlg QuickInfo;
        QuickInfo.Show( "WizDragSwap", "You can drag and drop between the text boxes on this page to swap the rolled values over" );
    }

    return fResult;
}








/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage2 property page

CWizard_NewInvestigatorPage2::CWizard_NewInvestigatorPage2() : CPropertyPage(CWizard_NewInvestigatorPage2::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage2)
	m_fUseCharacteristicModifiers = (BOOL)AfxGetApp()->GetProfileInt( "Wizard", "Use Stat Mods", FALSE );
	m_nOccupation = -1;
	//}}AFX_DATA_INIT
}

CWizard_NewInvestigatorPage2::~CWizard_NewInvestigatorPage2()
{
}

void CWizard_NewInvestigatorPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage2)
	DDX_Control(pDX, IDC_OCCUPATION, m_wndOccupation);
	DDX_Control(pDX, IDC_USECHARACTERISTICMODIFIERS, m_wndUseCharacteristicModifiers);
	DDX_Check(pDX, IDC_USECHARACTERISTICMODIFIERS, m_fUseCharacteristicModifiers);
	DDX_CBIndex(pDX, IDC_OCCUPATION, m_nOccupation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage2)
	ON_BN_CLICKED(IDC_USECHARACTERISTICMODIFIERS, OnUseCharacteristicModifiers)
	ON_CBN_SELCHANGE(IDC_OCCUPATION, OnSelChangeOccupation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CWizard_NewInvestigatorPage2::OnWizardNext() 
{
    UpdateData(UD_GETDATA);

    //get the chosen occupation
    CString strOccupation;
    m_wndOccupation.GetLBText( m_nOccupation, strOccupation );

    //assign this occupation to the investigator - don't change pages if it fails
    if( m_pInvestigator->SetOccupation( strOccupation ) == FALSE ) return -1;
    m_pInvestigator->m_strOccupation = strOccupation;

    //run the script associated with this occupation
    if( m_fUseCharacteristicModifiers )
    {
        COccupation* pOccupation = SkillManager.FindOccupation( m_pInvestigator->m_strRuleFile, strOccupation);
        if( pOccupation && pOccupation->m_strScript.GetLength() > 0 )
        {
            //get the global script block
            CSectionFile SectionFile;
            SectionFile.Open( CByakheeApp::GetDefaultDirectory() + m_pInvestigator->m_strRuleFile );
            CString strScript = SectionFile.GetSection("GLOBAL_SCRIPT");

            //add the script to the end of the global script
            strScript = strScript + CString("\r\n\r\n") + pOccupation->m_strScript;

            CScriptingEngine ScriptEngine;
            ScriptEngine.SetOwnerWindow( AfxGetMainWnd()->m_hWnd );
            if( ScriptEngine.SetScript( strScript ) )
            {
                //add the global objects
                ScriptEngine.AddGlobalVariable( "dice", new CScriptDice(), TRUE );
                ScriptEngine.AddGlobalVariable( "output", new CScriptOutput(), TRUE );
                ScriptEngine.AddGlobalVariable( "investigator", new CScriptInvestigator(m_pInvestigator), TRUE );

                //run the script
                CByakheeApp::ShowScriptExecutionQuickInfo();
                if( !ScriptEngine.ExecuteScript() )
                {
                    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.RedrawWindow();
                    if( AfxMessageBox( "Unable to execute the characteristic modifier script - do you want to continue?\n(It's possible that something about the investigator has been modified)", MB_YESNO|MB_ICONQUESTION ) == IDNO ) return -1;
                }
            }
            else
            {
                if( AfxMessageBox( "Unable to use the characteristic modifier script - do you want to continue?", MB_YESNO|MB_ICONQUESTION ) == IDNO ) return -1;
            }
        }
    }

	//FIXME: set a flag to say that we've done this stuff too
    //FIXME: note: there's no point in this at the moment - need to add something to
    //       "reroll" on page 1 so that this bit can be repeated.

    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.RedrawWindow();
	return CPropertyPage::OnWizardNext();
}

BOOL CWizard_NewInvestigatorPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //add all occupations to the list box
    int nOccupations = SkillManager.GetOccupationCount(m_pInvestigator->m_strRuleFile);
    for( int n = 0; n < nOccupations; n++ )
        m_wndOccupation.AddString( SkillManager.GetOccupationName(m_pInvestigator->m_strRuleFile, n) );

    //pick a random skill
    if( nOccupations != 0 )
    {
        m_nOccupation = rand() % nOccupations;

        //disable the use charateristic modifiers check box if there's no information about it
        CString strOccupation;
        m_wndOccupation.GetLBText( m_nOccupation, strOccupation );
        m_wndUseCharacteristicModifiers.EnableWindow( SkillManager.IsOccupationScript(m_pInvestigator->m_strRuleFile, strOccupation) );
    }
    else
    {
        AfxMessageBox( "Rule File Error: No Occupations!", MB_ICONEXCLAMATION );
        m_wndUseCharacteristicModifiers.EnableWindow( FALSE );
    }


    //update the dialog
    UpdateData(UD_SETDATA);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizard_NewInvestigatorPage2::OnUseCharacteristicModifiers() 
{
    //get the current setting and write it to the registry
    UpdateData(UD_GETDATA);
    AfxGetApp()->WriteProfileInt( "Wizard", "Use Stat Mods", m_fUseCharacteristicModifiers );
}

BOOL CWizard_NewInvestigatorPage2::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_BACK|PSWIZB_NEXT );
	return CPropertyPage::OnSetActive();
}

void CWizard_NewInvestigatorPage2::OnSelChangeOccupation() 
{
    UpdateData(UD_GETDATA);

    //disable the use charateristic modifiers check box if there's no information about it
    CString strOccupation;
    m_wndOccupation.GetLBText( m_nOccupation, strOccupation );
    m_wndUseCharacteristicModifiers.EnableWindow( SkillManager.IsOccupationScript(m_pInvestigator->m_strRuleFile, strOccupation) );
}














/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage3 property page

CWizard_NewInvestigatorPage3::CWizard_NewInvestigatorPage3() : CPropertyPage(CWizard_NewInvestigatorPage3::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage3)
	m_fInformation = FALSE;
	//}}AFX_DATA_INIT

    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INFO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
    m_pwndInformationPopup = NULL;
}

CWizard_NewInvestigatorPage3::~CWizard_NewInvestigatorPage3()
{
    delete m_pwndInformationPopup;
    DestroyIcon( m_hIcon );
}

void CWizard_NewInvestigatorPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage3)
	DDX_Control(pDX, IDC_INFORMATION, m_wndInformation);
	DDX_Control(pDX, IDC_INVESTIGATORWEAPONS, m_wndInvestigatorWeapons);
	DDX_Control(pDX, IDC_ALLWEAPONS, m_wndAllWeapons);
	DDX_Check(pDX, IDC_INFORMATION, m_fInformation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage3)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_DBLCLK(IDC_ALLWEAPONS, OnDblclkAllWeapons)
	ON_LBN_DBLCLK(IDC_INVESTIGATORWEAPONS, OnDblclkInvestigatorWeapons)
	ON_BN_CLICKED(IDC_INFORMATION, OnInformation)
	ON_LBN_SELCHANGE(IDC_ALLWEAPONS, OnSelchangeAllweapons)
	ON_LBN_SETFOCUS(IDC_ALLWEAPONS, OnSetfocusAllweapons)
	ON_LBN_SELCHANGE(IDC_INVESTIGATORWEAPONS, OnSelchangeInvestigatorweapons)
	ON_LBN_SETFOCUS(IDC_INVESTIGATORWEAPONS, OnSetfocusInvestigatorweapons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWizard_NewInvestigatorPage3::OnAdd() 
{
    int nSelected = m_wndAllWeapons.GetCurSel();
    if( nSelected == LB_ERR || m_wndAllWeapons.GetCount() <= 0 ) return;
    UpdateData(UD_GETDATA);

    //make sure we've got something to add
    CString strWeaponToAdd;
    m_wndAllWeapons.GetText( nSelected, strWeaponToAdd );
    if( strWeaponToAdd.GetLength() > 0 )
    {
        //remove it from the available weapons list
        int n = m_wndAllWeapons.FindStringExact( -1, strWeaponToAdd );
        if( n != LB_ERR )
            m_wndAllWeapons.DeleteString( n );
        else
            AfxMessageBox( "Error: Unable to locate weapon!", MB_ICONERROR );

        //add it to the investigator
        CSkill* pSkill = SkillManager.FindSkill( m_pInvestigator->m_strRuleFile, strWeaponToAdd );
        if( pSkill )
        {
            //make sure it's a weapon!
            if( pSkill->m_SkillType == FIREARM || pSkill->m_SkillType == MELEE )
            {
                //create a copy of the skill
                CInvestigatorSkill* pInvestigatorSkill = new CInvestigatorSkill( pSkill->GetCopy(), pSkill->EvaluateBase(m_pInvestigator) );

                //add it to the investigator's main skill list
                m_pInvestigator->m_AllSkillList.Add(pInvestigatorSkill);

                //determine which other list to add it to by the skill's type
                switch( pInvestigatorSkill->m_pSkill->m_SkillType )
                {
                    case FIREARM: m_pInvestigator->m_FirearmList.Add(pInvestigatorSkill); break;
                    case MELEE:   m_pInvestigator->m_MeleeList.Add(pInvestigatorSkill);   break;
                    default:      ASSERT(FALSE); return;
                }

                //add it to the investigator weapon list
                m_wndInvestigatorWeapons.AddString( strWeaponToAdd );
            }
        }
    }
}

void CWizard_NewInvestigatorPage3::OnRemove() 
{
    int nSelected = m_wndInvestigatorWeapons.GetCurSel();
    if( nSelected == LB_ERR || m_wndInvestigatorWeapons.GetCount() <= 0 ) return;
    UpdateData(UD_GETDATA);

    //make sure we've got something to remove
    CString strWeaponToRemove;
    m_wndInvestigatorWeapons.GetText( nSelected, strWeaponToRemove );
    if( strWeaponToRemove.GetLength() > 0 )
    {
        //remove it from the investigator weapons list
        int n = m_wndInvestigatorWeapons.FindStringExact( -1, strWeaponToRemove );
        if( n != LB_ERR )
        {
            //find it in the investigator's skill list
            CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->FindSkill(strWeaponToRemove);
            if( pInvestigatorSkill && pInvestigatorSkill->m_fOccupational )
            {
                //warn about removing occupational weapons
                if( IDNO == AfxMessageBox( "Are you sure you want to remove this weapon?\nIt is an occupational weapon. If you add it again after removing it,\nyou will not be able to assign occupational skill points to it.", MB_ICONQUESTION|MB_YESNO ) )
                    return; //don't remove it
            }

            //remove it from the list box
            m_wndInvestigatorWeapons.DeleteString( n );
        }
        else
            AfxMessageBox( "Error: Unable to locate weapon!", MB_ICONERROR );


        //tell the investigator to remove it
        if( m_pInvestigator->RemoveSkill( strWeaponToRemove ) == TRUE )
        {
            //add it to the available weapons list
            m_wndAllWeapons.AddString( strWeaponToRemove );
        }
        else
            AfxMessageBox( "Error: Could not remove skill", MB_ICONERROR );
    }
}

BOOL CWizard_NewInvestigatorPage3::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_NEXT );
	return CPropertyPage::OnSetActive();
}

BOOL CWizard_NewInvestigatorPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //get the investigator's era
    Era era = m_pInvestigator->m_Era;

    //add all melee weapons
    int i;
    const CRuleFile* pTempRuleFile = SkillManager.GetTempRuleFile(m_pInvestigator->m_strRuleFile);
    if( pTempRuleFile != NULL )
    {
        for( i = 0; i < pTempRuleFile->m_MeleeArray.GetSize(); i++ )
        {
            CSkill* pSkill = pTempRuleFile->m_MeleeArray[i];

            //only add melee skills of this era which aren't core melee weapons
            if( pSkill->m_SkillType == MELEE && pSkill->IsEra(era) && pSkill->m_strName[0] != '*' )
            {
                //see which list to add it to
                if( m_pInvestigator->FindSkill( pSkill->m_strName ) )
				    m_wndInvestigatorWeapons.AddString( pSkill->m_strName );
                else
				    m_wndAllWeapons.AddString( pSkill->m_strName );
            }
        }

        //add all firearms
        for( i = 0; i < pTempRuleFile->m_FirearmArray.GetSize(); i++ )
        {
            CSkill* pSkill = pTempRuleFile->m_FirearmArray[i];

            //only add melee skills of this era which aren't core melee weapons
            if( pSkill->m_SkillType == FIREARM && pSkill->IsEra(era) && pSkill->m_strName[0] != '*' )
            {
                //see which list to add it to
                if( m_pInvestigator->FindSkill( pSkill->m_strName ) )
				    m_wndInvestigatorWeapons.AddString( pSkill->m_strName );
                else
				    m_wndAllWeapons.AddString( pSkill->m_strName );
            }
        }
    }

    //initialise the info window
    m_wndInformation.SetIcon( m_hIcon );
    m_fInformation = AfxGetApp()->GetProfileInt( "WindowPos", "SkillInfo", FALSE );

    m_pwndInformationPopup = new CSkillInformationPopupWnd(m_pInvestigator->m_strRuleFile);
    m_pwndInformationPopup->Create( this );
    m_pwndInformationPopup->MoveIntoPosition();
    if( m_fInformation ) m_pwndInformationPopup->ShowWindow( SW_SHOWNA );

    UpdateData(UD_SETDATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizard_NewInvestigatorPage3::OnDblclkAllWeapons() 
{
	OnAdd();
}

void CWizard_NewInvestigatorPage3::OnDblclkInvestigatorWeapons() 
{
    OnRemove();
}

void CWizard_NewInvestigatorPage3::OnInformation() 
{
    UpdateData(UD_GETDATA);
    if( m_fInformation )
        m_pwndInformationPopup->ShowWindow( SW_SHOWNA );
    else
        m_pwndInformationPopup->ShowWindow( SW_HIDE );

    AfxGetApp()->WriteProfileInt( "WindowPos", "SkillInfo", m_fInformation );
}

void CWizard_NewInvestigatorPage3::OnSelchangeAllweapons() 
{
    CString strWeapon;
    int i = m_wndAllWeapons.GetCurSel();
    if( i != LB_ERR && m_wndAllWeapons.GetCount() > 0 ) m_wndAllWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill(strWeapon);
}

void CWizard_NewInvestigatorPage3::OnSetfocusAllweapons() 
{
    CString strWeapon;
    int i = m_wndAllWeapons.GetCurSel();
    if( i != LB_ERR && m_wndAllWeapons.GetCount() > 0 ) m_wndAllWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill(strWeapon);
}

void CWizard_NewInvestigatorPage3::OnSelchangeInvestigatorweapons() 
{
    CString strWeapon;
    int i = m_wndInvestigatorWeapons.GetCurSel();
    if( i != LB_ERR && m_wndInvestigatorWeapons.GetCount() > 0 ) m_wndInvestigatorWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill(strWeapon);
}

void CWizard_NewInvestigatorPage3::OnSetfocusInvestigatorweapons() 
{
    CString strWeapon;
    int i = m_wndInvestigatorWeapons.GetCurSel();
    if( i != LB_ERR && m_wndInvestigatorWeapons.GetCount() > 0 ) m_wndInvestigatorWeapons.GetText(i, strWeapon );
    m_pwndInformationPopup->SetSkill(strWeapon);
}

LRESULT CWizard_NewInvestigatorPage3::OnWizardNext() 
{
    //hide the weapon window before we go...
    m_pwndInformationPopup->ShowWindow( SW_HIDE );
	return CPropertyPage::OnWizardNext();
}










/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage4 property page

CWizard_NewInvestigatorPage4::CWizard_NewInvestigatorPage4() : CPropertyPage(CWizard_NewInvestigatorPage4::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_UNDO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CWizard_NewInvestigatorPage4::~CWizard_NewInvestigatorPage4()
{
    DestroyIcon( m_hIcon );
}

void CWizard_NewInvestigatorPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage4)
	DDX_Control(pDX, IDC_UNDO, m_wndUndo);
	DDX_Control(pDX, IDC_SKILLPOINTS, m_wndAvailablePoints);
	DDX_Control(pDX, IDC_SKILLLIST, m_wndSkillList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage4)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWizard_NewInvestigatorPage4::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_NEXT );

    //get number of points and convert it into a string
    CString str;
    str.Format( "%d", ((CWizard_NewInvestigator*)GetParent())->m_nSkillPoints );

    //store this value in the dialog
    m_wndAvailablePoints.SetWindowText( str );
    
    //back up the existing investigator
    ((CWizard_NewInvestigator*)GetParent())->StoreUndo();

    //redo the list
    m_wndSkillList.Initialise();
	
    //continue initialising
	return CPropertyPage::OnSetActive();
}

BOOL CWizard_NewInvestigatorPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //initialise the skill editing control
    m_wndSkillList.SetPointsDisplayWnd( &m_wndAvailablePoints );
    m_wndSkillList.SetInvestigator( TRUE, m_pInvestigator );

    //set up the undo button
    m_wndUndo.SetIcon( m_hIcon );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWizard_NewInvestigatorPage4::OnWizardNext() 
{
    //get the number of skill points remaining
    CString str;
    m_wndAvailablePoints.GetWindowText( str );
    int nPoints = atoi( str );

    //see if they're sure they want to continue
    if( nPoints > 0 )
    {
        CString strMessage;
        strMessage.Format( "You still have %d skill points unallocated!\nThese points are not transferable and will be lost - okay to continue?", nPoints );
        if( AfxMessageBox( strMessage, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2 ) == IDNO )
            return -1;
    }

    //store this value in the investigator
    ((CWizard_NewInvestigator*)GetParent())->m_nSkillPoints = nPoints;
    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.RedrawWindow();

	return CPropertyPage::OnWizardNext();
}

void CWizard_NewInvestigatorPage4::OnUndo() 
{
    if( IDYES == AfxMessageBox( "Are you sure you want to undo all skill assignments?", MB_ICONQUESTION|MB_YESNO ) )
    {
        //undo
        ((CWizard_NewInvestigator*)GetParent())->Undo();

        //get number of points and convert it into a string
        CString str;
        str.Format( "%d", ((CWizard_NewInvestigator*)GetParent())->m_nSkillPoints );

        //store this value in the dialog
        m_wndAvailablePoints.SetWindowText( str );

        //reset the skill list  
        m_wndSkillList.Initialise();
    }
}

















/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage5 property page

CWizard_NewInvestigatorPage5::CWizard_NewInvestigatorPage5() : CPropertyPage(CWizard_NewInvestigatorPage5::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_UNDO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CWizard_NewInvestigatorPage5::~CWizard_NewInvestigatorPage5()
{
    DestroyIcon( m_hIcon );
}

void CWizard_NewInvestigatorPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage5)
	DDX_Control(pDX, IDC_UNDO, m_wndUndo);
	DDX_Control(pDX, IDC_SKILLLIST, m_wndSkillList);
	DDX_Control(pDX, IDC_SKILLPOINTS, m_wndAvailablePoints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage5, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage5)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWizard_NewInvestigatorPage5::OnUndo() 
{
    if( IDYES == AfxMessageBox( "Are you sure you want to undo all personal interest skill assignments?", MB_ICONQUESTION|MB_YESNO ) )
    {
        //undo
        ((CWizard_NewInvestigator*)GetParent())->Undo();

        //get number of points and convert it into a string
        CString str;
        str.Format( "%d", ((CWizard_NewInvestigator*)GetParent())->m_nPersonalInterestPoints );

        //store this value in the dialog
        m_wndAvailablePoints.SetWindowText( str );

        //reset the skill list  
        m_wndSkillList.Initialise();
    }
}

BOOL CWizard_NewInvestigatorPage5::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_BACK|PSWIZB_NEXT );

    //get number of points and convert it into a string
    CString str;
    str.Format( "%d", ((CWizard_NewInvestigator*)GetParent())->m_nPersonalInterestPoints );

    //store this value in the dialog
    m_wndAvailablePoints.SetWindowText( str );

    //back up the existing investigator
    ((CWizard_NewInvestigator*)GetParent())->StoreUndo();

    //redraw the list
    m_wndSkillList.Initialise();
    
	return CPropertyPage::OnSetActive();
}

LRESULT CWizard_NewInvestigatorPage5::OnWizardNext() 
{
    //get the number of skill points remaining
    CString str;
    m_wndAvailablePoints.GetWindowText( str );
    int nPoints = atoi( str );

    //see if they're sure they want to continue
    if( nPoints > 0 )
    {
        CString strMessage;
        strMessage.Format( "You still have %d occupational points unallocated!\nThese points are not transferable and will be lost - okay to continue?", nPoints );
        if( AfxMessageBox( strMessage, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2 ) == IDNO )
            return -1;
    }

    //store this value in the investigator
    ((CWizard_NewInvestigator*)GetParent())->m_nPersonalInterestPoints = nPoints;
    ((CWizard_NewInvestigator*)GetParent())->m_wndInvestigatorPopup.RedrawWindow();

	return CPropertyPage::OnWizardNext();
}

BOOL CWizard_NewInvestigatorPage5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    //initialise the skill editing control
    m_wndSkillList.SetPointsDisplayWnd( &m_wndAvailablePoints );
    m_wndSkillList.SetInvestigator( FALSE, m_pInvestigator );
    
    //set up the undo button
    m_wndUndo.SetIcon( m_hIcon );
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}








/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage6 property page

CWizard_NewInvestigatorPage6::CWizard_NewInvestigatorPage6() : CPropertyPage(CWizard_NewInvestigatorPage6::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage6)
	m_strMentalDisorders = _T("");
	m_strBirthplace = _T("");
	m_strName = _T("");
	m_strOccupation = _T("");
	m_strPortrait = _T("");
	m_strSex = _T("");
	m_strCollegesDegrees = _T("");
	//}}AFX_DATA_INIT

    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RANDOM), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
    m_pRandomNameGenerator = NULL;
}

CWizard_NewInvestigatorPage6::~CWizard_NewInvestigatorPage6()
{
    DestroyIcon( m_hIcon );
}

void CWizard_NewInvestigatorPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage6)
	DDX_Control(pDX, IDC_COLLEGESDEGREES, m_wndCollegesDegrees);
	DDX_Control(pDX, IDC_RANDOMNAME, m_wndRandom);
	DDX_Control(pDX, IDC_MENTALDISORDERS, m_wndMentalDisorders);
	DDX_Control(pDX, IDC_BIRTHPLACE, m_wndBirthplace);
	DDX_CBString(pDX, IDC_MENTALDISORDERS, m_strMentalDisorders);
	DDX_CBString(pDX, IDC_BIRTHPLACE, m_strBirthplace);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_OCCUPATION, m_strOccupation);
	DDX_Text(pDX, IDC_PORTRAIT, m_strPortrait);
	DDX_Text(pDX, IDC_SEX, m_strSex);
	DDX_CBString(pDX, IDC_COLLEGESDEGREES, m_strCollegesDegrees);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage6, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage6)
	ON_BN_CLICKED(IDC_RANDOMNAME, OnRandomName)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWizard_NewInvestigatorPage6::OnRandomName() 
{
    UpdateData( UD_GETDATA );

    //generate a new name
	m_pRandomNameGenerator->Generate();

    //store it in the dialog
    m_strName = m_pRandomNameGenerator->GetName();
    m_strSex = m_pRandomNameGenerator->GetSex();

    //update the dialog
    UpdateData( UD_SETDATA );
}

LRESULT CWizard_NewInvestigatorPage6::OnWizardNext() 
{
    UpdateData( UD_GETDATA );

	//write the information into the investigator sheet
    m_pInvestigator->m_strOccupation = m_strOccupation;
    m_pInvestigator->m_strBirthplace = m_strBirthplace;
    m_pInvestigator->m_strColleges = m_strCollegesDegrees;
    m_pInvestigator->m_strMentalDisorders = m_strMentalDisorders;
    m_pInvestigator->m_strPortrait = m_strPortrait;
    m_pInvestigator->m_strSex = m_strSex;
    m_pInvestigator->m_strName = m_strName;

    //EASTER EGG: see what they typed for sex - if it's anything like "YES", display the easter egg
    //translators note: you may want to change this letter or check routine for your own language's
    //version of "yes" - although make sure it doesn't clash with your own language's version of
    //"male" or "female". It may be easiest to remove this easter egg entirely.
    if( m_strSex.GetLength() > 0 && toupper(m_strSex[0]) == 'Y' )
    {
        CString strMessage;
        strMessage.Format( "\"%s\" for sex - you couldn't resist it, could you?\n\n:-)", m_strSex );
        AfxMessageBox( strMessage );
    }
	
	return CPropertyPage::OnWizardNext();
}

BOOL CWizard_NewInvestigatorPage6::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_BACK|PSWIZB_NEXT );

    //set the current values from the investigator
    m_strOccupation = m_pInvestigator->m_strOccupation;
    m_strBirthplace = m_pInvestigator->m_strBirthplace;
    m_strCollegesDegrees = m_pInvestigator->m_strColleges;
    m_strMentalDisorders = m_pInvestigator->m_strMentalDisorders;
    m_strPortrait = m_pInvestigator->m_strPortrait;
    m_strSex = m_pInvestigator->m_strSex;
    m_strName = m_pInvestigator->m_strName;
    UpdateData( UD_SETDATA );

	return CPropertyPage::OnSetActive();
}

BOOL CWizard_NewInvestigatorPage6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    //create the random name generator
    if( m_pRandomNameGenerator == NULL ) m_pRandomNameGenerator = new CRandomNameGenerator();

    //set the random button's icon
	m_wndRandom.SetIcon( m_hIcon );

    //set the dropdown options for m_wndBirthplace
    CNamedOptionManager NamedOptionManager( SkillManager.GetKeyPairValue( m_pInvestigator->m_strRuleFile, "Options", "options.lst" ), 1); //don't bother to cache
    NamedOptionManager.FillComboWithOptions( &m_wndBirthplace, "Birthplace" );
    NamedOptionManager.FillComboWithOptions( &m_wndMentalDisorders, "Phobias" );
    NamedOptionManager.FillComboWithOptions( &m_wndCollegesDegrees, "CollegesDegrees" );

    UpdateData( UD_SETDATA );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizard_NewInvestigatorPage6::OnBrowse() 
{
    UpdateData(UD_GETDATA);

    if( CByakheeApp::BrowseForPicture( m_strPortrait, this ) ) UpdateData(UD_SETDATA);
}







/////////////////////////////////////////////////////////////////////////////
// CWizard_NewInvestigatorPage7 property page

CWizard_NewInvestigatorPage7::CWizard_NewInvestigatorPage7() : CPropertyPage(CWizard_NewInvestigatorPage7::IDD)
{
	//{{AFX_DATA_INIT(CWizard_NewInvestigatorPage7)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWizard_NewInvestigatorPage7::~CWizard_NewInvestigatorPage7()
{
}

void CWizard_NewInvestigatorPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizard_NewInvestigatorPage7)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizard_NewInvestigatorPage7, CPropertyPage)
	//{{AFX_MSG_MAP(CWizard_NewInvestigatorPage7)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CWizard_NewInvestigatorPage7::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons( PSWIZB_BACK|PSWIZB_FINISH );
	return CPropertyPage::OnSetActive();
}


BOOL CWizard_NewInvestigatorPage7::OnWizardFinish() 
{
    //remove all ANYSKILL types as they're useless after the wizard
    while( m_pInvestigator->RemoveSkill( "*" ) );

    //sort the skill list before we leave
    m_pInvestigator->SortSkillList();
	return CPropertyPage::OnWizardFinish();
}
