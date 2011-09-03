// InvestigatorDoc.cpp : implementation of the CInvestigatorDoc class
//

#include "stdafx.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "MultiFileMultiDocTemplate.h"
#include "CImage/CImage.h"
#include "RandomNameGenerator.h"
#include "ParseBuffer.h"
#include "DiceRoller.h"
#include "AddSkillDlg.h"
#include "EditSkillsDlg.h"
#include "EditWeaponsDlg.h"
#include "SpellbookDlg.h"
#include "QuickInfoDlg.h"
#include "InvestigatorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorDoc

IMPLEMENT_DYNCREATE(CInvestigatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CInvestigatorDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	//{{AFX_MSG_MAP(CInvestigatorDoc)
	ON_COMMAND(ID_INVESTIGATOR_ADDSKILL, OnInvestigatorAddSkill)
	ON_COMMAND(ID_INVESTIGATOR_MODIFYSKILL, OnInvestigatorModifySkill)
	ON_COMMAND(ID_INVESTIGATOR_CHOOSEWEAPONS, OnInvestigatorChooseWeapons)
	ON_COMMAND(ID_INVESTIGATOR_INCOME, OnInvestigatorIncome)
	ON_COMMAND(ID_TOOLS_RANDOMNAME, OnToolsRandomName)
	ON_COMMAND(ID_TOOLS_REROLL_INVESTIGATOR, OnToolsRerollInvestigator)
	ON_COMMAND(ID_INVESTIGATOR_SPELLBOOK, OnInvestigatorSpellbook)
	ON_COMMAND(ID_TOOLS_CHECKSHEET, OnToolsCheckSheet)
	ON_COMMAND(ID_TOOLS_ROLLCHECKEDSKILLS, OnToolsRollCheckedSkills)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_FILE_EXPORT_BYAKHEE10, OnFileExportByakhee10)
	ON_COMMAND(ID_FILE_EXPORT_BYAKHEE20, OnFileExportByakhee20)
	ON_COMMAND(ID_FILE_EXPORT_HTML, OnFileExportHtml)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorDoc construction/destruction

CInvestigatorDoc::CInvestigatorDoc()
{
    m_pNamedOptionManager = NULL;
    m_pPortrait = NULL;
}

CInvestigatorDoc::~CInvestigatorDoc()
{
    delete m_pPortrait;
    if( m_pNamedOptionManager ) delete m_pNamedOptionManager;
}

BOOL CInvestigatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    m_pPortrait = NULL;
    m_Investigator.RollNewStats();
    m_Investigator.SetOccupation("");

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CInvestigatorDoc diagnostics

#ifdef _DEBUG
void CInvestigatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInvestigatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInvestigatorDoc commands

void CInvestigatorDoc::SetRuleFile(CString strRuleFile)
{
    m_Investigator.SetRuleFile(strRuleFile);
    if( m_pNamedOptionManager ) delete m_pNamedOptionManager;
    m_pNamedOptionManager = new CNamedOptionManager( SkillManager.GetKeyPairValue( m_Investigator.m_strRuleFile, "Options", "options.lst" ), 8 );
}

BOOL CInvestigatorDoc::ReloadPortrait()
{
    if( m_Investigator.m_strPortrait.IsEmpty() ) return FALSE;

    CWaitCursor Wait;

    //load new portrait
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading image ") + CString(m_Investigator.m_strPortrait) + CString("...") );
    CImage* pNewPortrait = new CImage(m_Investigator.m_strPortrait);
	if( pNewPortrait->GetWidth() <= 0 || pNewPortrait->GetHeight() <= 0 )
	{
		delete pNewPortrait;
		return FALSE;
	}
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );

    //load succeeded - remove the existing one and store this one
    delete m_pPortrait;
    m_pPortrait = pNewPortrait;

	return TRUE;
}

BOOL CInvestigatorDoc::SetPortrait( CString strPortrait )
{
    //store the old file name
    CString strOldPortrait = m_Investigator.m_strPortrait;

    //try and load the new one
    m_Investigator.m_strPortrait = strPortrait;
    if( ReloadPortrait() == FALSE )
    {
        //load failed - put the filename back
        m_Investigator.m_strPortrait = strOldPortrait;
        return FALSE;
    }
    else
        return TRUE;
}

void CInvestigatorDoc::OnInvestigatorAddSkill() 
{
    StoreUndo();

    CAddSkillDlg AddSkillDlg;
    AddSkillDlg.SetInvestigator( &m_Investigator );

    if( AddSkillDlg.DoModal() == IDOK )
    {
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }
}

void CInvestigatorDoc::OnInvestigatorModifySkill() 
{
    StoreUndo();

    CEditSkillsDlg EditSkillsDlg;
    EditSkillsDlg.SetInvestigator( &m_Investigator );
    EditSkillsDlg.DoModal();

    m_Investigator.SortSkillList();
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::OnInvestigatorChooseWeapons() 
{
    StoreUndo();

    CEditWeaponsDlg EditWeaponsDlg;

    EditWeaponsDlg.SetInvestigator( &m_Investigator );
    EditWeaponsDlg.DoModal();

    m_Investigator.SortSkillList();
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::OnToolsRandomName() 
{
    StoreUndo();

    //generate a new name
	CRandomNameGenerator m_RandomNameGenerator;

    //copy it to the investigator
    m_Investigator.m_strName = m_RandomNameGenerator.GetName();
    m_Investigator.m_strSex = m_RandomNameGenerator.GetSex();

    //update all views
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::OnInvestigatorIncome() 
{
    StoreUndo();

    //run the income script
    m_Investigator.RunIncomeScript();

    //display the result
    AfxMessageBox( CString("Your Investigator has an income of ") + m_Investigator.m_strIncome );

    //update all views
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::OnToolsRerollInvestigator() 
{
    StoreUndo();

    m_Investigator.RollNewStats();
    UpdateStatusBar();
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::OnInvestigatorSpellbook() 
{
    CSpellbookDlg Spellbook;
    Spellbook.m_strInvestigatorSpells = m_Investigator.m_strSpells;
    if( Spellbook.DoModal() == IDOK )
    {
        StoreUndo();

        //set investigator spell string
        m_Investigator.m_strSpells = Spellbook.m_strInvestigatorSpells;

        //update all views
        SetModifiedFlag();
        UpdateAllViews(NULL);
    }
}

void CInvestigatorDoc::OnToolsCheckSheet() 
{
    //get system console and clear it
    COutput& wndOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWindow();
    wndOutput.Clear();
	
	//check the investigator for errors
    wndOutput.AddString( "Checking Investigator..." );

    BOOL fCheckSkills = TRUE;
    int nErrors = 0;

    //check strings
    if( m_Investigator.m_strName.IsEmpty() )            { wndOutput.AddString( "The investigator has no name" ); nErrors++; };
    if( m_Investigator.m_strOccupation.IsEmpty() )      { wndOutput.AddString( "The investigator has no occupation" ); nErrors++; };
    if( m_Investigator.m_strSex.IsEmpty() )             { wndOutput.AddString( "The investigator has no sex..." ); nErrors++; };

    //check stats
    if( m_Investigator.m_nAPP <  0 )                    { wndOutput.AddString( "The investigator's APP has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nCON <  0 )                    { wndOutput.AddString( "The investigator's CON has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nDEX <  0 )                    { wndOutput.AddString( "The investigator's DEX has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nEDU <  0 )                    { wndOutput.AddString( "The investigator's EDU has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nINT <  0 )                    { wndOutput.AddString( "The investigator's INT has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nPOW <  0 )                    { wndOutput.AddString( "The investigator's POW has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nSIZ <  0 )                    { wndOutput.AddString( "The investigator's SIZ has not been set" ); fCheckSkills = FALSE; nErrors++; }
    if( m_Investigator.m_nSTR <  0 )                    { wndOutput.AddString( "The investigator's STR has not been set" ); fCheckSkills = FALSE; nErrors++; }

    //check other stats
    int nMaxSan = m_Investigator.m_nPOW*5;
    if( nMaxSan > 0 )
    {
        //find Cthulhu Mythos skill & shrink SAN if needed
        CInvestigatorSkill* pCthulhuMythos = m_Investigator.FindSkill("Cthulhu Mythos");
        if( pCthulhuMythos )
        {
            int n99_CthulhuMythos = (99-pCthulhuMythos->m_nCurrent);
            if( m_Investigator.m_nSAN > n99_CthulhuMythos ) { wndOutput.AddString( "The investigator's SAN is greater than their 99-Cthulhu Mythos value" ); fCheckSkills = FALSE; nErrors++; }
        }
    
        if( m_Investigator.m_nSAN > nMaxSan ) 
        { 
            CString strMsg; strMsg.Format( "The investigator's SAN is greater than their maximum SAN value of %d", nMaxSan );
            wndOutput.AddString( strMsg ); fCheckSkills = FALSE; nErrors++; 
        }
    }


    //check each skill
    if( fCheckSkills )
    {
        int iMax = m_Investigator.m_AllSkillList.GetSize();
        int nPointsSpent = 0;
        for( int i = 0; i < iMax; i++ )
        {
            CInvestigatorSkill* pInvestigatorSkill = m_Investigator.m_AllSkillList[i];

            //calculate minimum value
            int nMinimum = pInvestigatorSkill->m_pSkill->EvaluateBase( &m_Investigator );
            if( pInvestigatorSkill->m_nCurrent < nMinimum )
            {
                CString strName;
                if( pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
                {
                    const char* pszParent = ((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName;
                    while( *pszParent == '*' ) pszParent++;

                    if( pszParent )
                        strName.Format( "%s:%s", pszParent, pInvestigatorSkill->m_pSkill->m_strName );
                    else
                        strName = pInvestigatorSkill->m_pSkill->m_strName;
                }
                else
                {
                    const char* pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                    while( *pszName == '*' ) pszName++;
                    strName = pszName;
                }

                CString strError;
                strError.Format( "\"%s\" has been allocated less than its minimum value (%d%%)", strName, nMinimum );
                wndOutput.AddString( strError );
                nErrors++;
            }
            else
            {
                nPointsSpent += (pInvestigatorSkill->m_nCurrent - pInvestigatorSkill->m_nMinimum);
                if( pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
                {
                    if( ( pInvestigatorSkill->m_nCurrent > pInvestigatorSkill->m_nMinimum || pInvestigatorSkill->IsCalculatedBase() ) && pInvestigatorSkill->m_pSkill->m_strName.IsEmpty() )
                    {
                        const char* pszName = LPCSTR(((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName);
                        while( *pszName == '*' ) pszName++;
                    
                        CString strError;
                        strError.Format( "A sub-item of %s has been allocated points but not named", pszName );
                        wndOutput.AddString( strError );
                        nErrors++;
                    }
                }
            }
        }


        //see whether they've spent too much
        int nPointsAvailable = m_Investigator.m_nSkillPoints + m_Investigator.m_nInterestPoints;
        if( nPointsSpent > nPointsAvailable )
        {
            CString strError;
            strError.Format( "%d too many skill have been points allocated", nPointsSpent-nPointsAvailable );
            wndOutput.AddString( strError );
            nErrors++;
        }
        else
        {
            if( nPointsSpent < nPointsAvailable )
            {
                CString strError;
                strError.Format( "%d too few skill points allocated", nPointsAvailable-nPointsSpent );
                wndOutput.AddString( strError );
                nErrors++;
            }
            else
            {
                //display how many points have been allocated
                CString strText;
                strText.Format( "%d points allocated.", nPointsSpent );
                wndOutput.AddString( strText );
            }
        }
    }

    //check number of errors
    if( nErrors == 0 )
    {
        CString strHappyNews = "The investigator check is complete.";
        wndOutput.AddString( strHappyNews );
        ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(&wndOutput, FALSE, FALSE);
        AfxMessageBox( strHappyNews, MB_ICONINFORMATION );
    }
    else
    {
        CString strIDontKnowHowToTellYouThisBut; strIDontKnowHowToTellYouThisBut.Format( "The investigator check failed: %d errors", nErrors );
        wndOutput.AddString( strIDontKnowHowToTellYouThisBut );
        ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(&wndOutput, TRUE, FALSE);
    }
}

void CInvestigatorDoc::OnToolsRollCheckedSkills() 
{
    if( AfxMessageBox( "Are you sure you want to perform end-of-campain skill check updates?", MB_ICONQUESTION|MB_YESNO ) == IDNO ) return;

    StoreUndo();

    //get system console and clear it
    COutput& wndOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWindow();
    wndOutput.Clear();
	
	//do skill rolling...
    wndOutput.AddString( "Rolling checked skills..." );

    int iMax = m_Investigator.m_AllSkillList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_Investigator.m_AllSkillList[i];

        //calculate minimum value
        CString strMessage;
        if( pInvestigatorSkill->m_fChecked )
        {
            //display message
            CString strName;
            if( pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
            {
                const char* pszParent = ((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName;
                while( *pszParent == '*' ) pszParent++;

                if( pszParent )
                    if( pInvestigatorSkill->m_pSkill->m_strName )
                        strName.Format( "%s:%s", pszParent, pInvestigatorSkill->m_pSkill->m_strName );
                    else
                        strName.Format( "%s unnamed subskill", pszParent );
                else
                    strName = pInvestigatorSkill->m_pSkill->m_strName;
            }
            else
            {
                const char* pszName = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
                strName = pszName;
            }

            
            strMessage.Format( "Rolling D100 for skill %s, which is currently at %d%%...", strName, pInvestigatorSkill->m_nCurrent );
            wndOutput.AddString( strMessage );
            
            //roll the D100
            CDiceRoller Dice;
            int nRoll = Dice.Roll( "D100" );
            
            //display result
            strMessage.Format( "    Rolled: %d", nRoll );
            wndOutput.AddString( strMessage );
            
            //if we failed the roll
            if( nRoll > pInvestigatorSkill->m_nCurrent )
            {
                //increase points
                wndOutput.AddString( "    Rolling 1D10 to determine point increase..." );
                int nIncrease = Dice.Roll( "D10" );
                pInvestigatorSkill->m_nCurrent += nIncrease;

                //display message
                strMessage.Format( "    Skill increased by %d points. Now at %d%%", nIncrease, pInvestigatorSkill->m_nCurrent );
                wndOutput.AddString( strMessage );
            }
            else
                //give them the bad news
                wndOutput.AddString( "    No point increase" );

            //add blank line and uncheck skill
            wndOutput.AddString( "" );
            pInvestigatorSkill->m_fChecked = FALSE;
        }
    }

    ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(&wndOutput, TRUE, FALSE);
    wndOutput.AddString( "...done" );
    SetModifiedFlag();
    UpdateAllViews(NULL);
}

void CInvestigatorDoc::StoreUndo()
{
    //push the current investigator onto the undo stack
    m_UndoList.AddHead( m_Investigator );

    //empty the redo list
    m_RedoList.RemoveAll();

    //keep undo stack under control
    if( m_UndoList.GetCount() > MAX_UNDO_SIZE ) m_UndoList.RemoveTail();
}

void CInvestigatorDoc::OnEditUndo() 
{
    if( m_UndoList.IsEmpty() == FALSE )
    {
        //push the current investigator onto the undo stack
        m_RedoList.AddHead( m_Investigator );

        //get the investigator from the undo stack
	    m_Investigator = m_UndoList.RemoveHead();

        //keep redo stack under control
        if( m_RedoList.GetCount() > MAX_UNDO_SIZE ) m_RedoList.RemoveTail();

        //display changes
        SetModifiedFlag();
        ReloadPortrait();
        UpdateAllViews(NULL);
    }
}

void CInvestigatorDoc::OnEditRedo() 
{
    if( m_RedoList.IsEmpty() == FALSE )
    {
        //add the investigator to the undo list
        m_UndoList.AddHead( m_Investigator );

        //get the investigator from the redo stack
	    m_Investigator = m_RedoList.RemoveHead();

        //keep undo stack under control
        if( m_UndoList.GetCount() > MAX_UNDO_SIZE ) m_UndoList.RemoveTail();

        //display changes
        SetModifiedFlag();
        ReloadPortrait();
        UpdateAllViews(NULL);
    }
}

void CInvestigatorDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_UndoList.IsEmpty() == FALSE );
}

void CInvestigatorDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_RedoList.IsEmpty() == FALSE );
}


void CInvestigatorDoc::OnFileExportByakhee10() 
{
    CString strFileName = GetTitle() + CString("_10.coc" );

    char* szFilter = "Cthulhu Investigators|*.coc|All Files (*.*)|*.*||";
    CFileDialog FileDialog( FALSE, ".coc", strFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST, szFilter, AfxGetMainWnd() );

    if( FileDialog.DoModal() == IDOK )
    {
        CFile File;
        if( File.Open( FileDialog.GetPathName(), CFile::modeCreate ) )
        {
            CArchive ar( &File, CArchive::store );
            ar.WriteString( SHEET_FILE_HEADER10 ); ar << '\n';
            if( !SerialiseInvestigator( ar, 1 ))
                AfxMessageBox( "Could not write Byakhee 1.0 file", MB_ICONERROR );
        }
        else
            AfxMessageBox( "Could not open file for output!", MB_ICONERROR );
    }
}

void CInvestigatorDoc::OnFileExportByakhee20() 
{
    CString strFileName = GetTitle() + CString("_20.coc" );

    char* szFilter = "Cthulhu Investigators|*.coc|All Files (*.*)|*.*||";
    CFileDialog FileDialog( FALSE, ".coc", strFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST, szFilter, AfxGetMainWnd() );

    if( FileDialog.DoModal() == IDOK )
    {
		CWaitCursor wait;
        CFile File;
        if( File.Open( FileDialog.GetPathName(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive ) )
        {
            CArchive ar( &File, CArchive::store|CArchive::bNoFlushOnDelete );
	        ar.m_pDocument = this;
	        ar.m_bForceFlat = FALSE;
            ar.WriteString( SHEET_FILE_HEADER20 ); ar << '\n';
            if( !SerialiseInvestigator( ar, 2 ) )
                AfxMessageBox( "Could not write Byakhee 2.0 file", MB_ICONERROR );
		    ar.Close();
        }
        else
            AfxMessageBox( "Could not open file for output!", MB_ICONERROR );
    }
}

void CInvestigatorDoc::OnFileExportHtml() 
{
    CString strFileName = GetTitle() + CString(".html" );

    char* szFilter = "HTML Files|*.htm;*.html|All Files (*.*)|*.*||";
    CFileDialog FileDialog( FALSE, ".html", strFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST, szFilter, AfxGetMainWnd() );

    BOOL fDrawV5_5Bases = AfxGetApp()->GetProfileInt( "Sheet", "V5.5Base", TRUE );


    if( FileDialog.DoModal() == IDOK )
    {
        CWaitCursor wait;

        CFile File;
        if( File.Open( FileDialog.GetPathName(), CFile::modeCreate|CFile::modeWrite ) )
        {
            CString strIndent = "    ";
            CString strEOL = "\r\n";

            CInvestigatorSkill* pInvestigatorSkill = m_Investigator.FindSkill( "Cthulhu Mythos" );
            int nCthulhuMythos = pInvestigatorSkill ? pInvestigatorSkill->m_nCurrent : 0;

            CString strDamageBonus = m_Investigator.GetDBString();

            CString strHTML;

            /* export the investigator */
            strHTML += CString("<html>") + strEOL;

            strHTML += CString("<head>") + strEOL;
            strHTML += CString("<meta http-equiv=\"Content-Type\" content=\"text/html\">") + strEOL;
            strHTML += CString("<meta name=\"GENERATOR\" content=\"Byakhee HTML Export Version 1.2\">") + strEOL;
            strHTML += CString("<title>") + m_Investigator.m_strName + CString("</title>") + strEOL;
            strHTML += CString("</head>") + strEOL + strEOL;

            strHTML += CString("<body>") + strEOL;

            strHTML += CString("<br><br><hr><br><br>") + strEOL + strEOL;

            //name and characteristics boxes
            strHTML += CString("<table border=\"0\" width=\"100%\" >") + strEOL;

            //name box
            strHTML += CString("<tr>") + strEOL;
            strHTML += strIndent + CString("<td valign=\"top\" width=\"50%\" >") + strEOL;
            strHTML += CString("<b>Investigator Name: </b>") + m_Investigator.m_strName + CString("<br>") + strEOL;
            strHTML += CString("<b>Occupation: </b>")        + m_Investigator.m_strOccupation + CString("<br>") + strEOL;
            strHTML += CString("<b>Colleges, Degrees: </b>") + m_Investigator.m_strColleges + CString("<br>") + strEOL;
            strHTML += CString("<b>Birthplace: </b>")        + m_Investigator.m_strBirthplace + CString("<br>") + strEOL;
            strHTML += CString("<b>Mental Disorders: </b>")   + m_Investigator.m_strMentalDisorders + CString("<br>") + strEOL;
            CString strAge; strAge.Format( "%d", m_Investigator.m_nAge );
            strHTML += CString("<b>Sex: </b>") + m_Investigator.m_strSex + CString("&nbsp<b>Age: </b>") + strAge + CString("<br>") + strEOL;
            strHTML += strIndent + CString("</td>") + strEOL;

            //characteristics box
            strHTML += strIndent + CString("<td valign=\"top\" width=\"50%\">") + strEOL;
            strHTML += strIndent + strIndent + CString("<table border=\"0\" width=\"100%\">") + strEOL;
            strHTML += strIndent + strIndent + CString("<tr>") + strEOL;
            CString strNumber; strNumber.Format( "%d", m_Investigator.m_nSTR );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("STR: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nDEX );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("DEX: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nINT );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("INT: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", (m_Investigator.m_nINT*5) );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("Idea: ") + strNumber + CString("</td>") + strEOL;
            strHTML += strIndent + strIndent + CString("</tr>") + strEOL;
            strHTML += strIndent + strIndent + CString("<tr>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nCON );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("CON: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nAPP );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("APP: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nPOW );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("POW: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", (m_Investigator.m_nPOW*5) );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("Luck: ") + strNumber + CString("</td>") + strEOL;
            strHTML += strIndent + strIndent + CString("</tr>") + strEOL;
            strHTML += strIndent + strIndent + CString("<tr>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nSIZ );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("SIZ: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", (m_Investigator.m_nPOW*5) );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("SAN: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", m_Investigator.m_nEDU );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("EDU: ") + strNumber + CString("</td>") + strEOL;
            strNumber.Format( "%d", (m_Investigator.m_nEDU*5) );
            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"25%\">") + CString("Know: ") + strNumber + CString("</td>") + strEOL;
            strHTML += strIndent + strIndent + CString("</tr>") + strEOL;
            strHTML += strIndent + strIndent + CString("<tr>") + strEOL;
            CString str99Cthulhu; str99Cthulhu.Format( "%d", (99-nCthulhuMythos) );
            strHTML += strIndent + strIndent + strIndent + CString("<td colspan=\"2\" width=\"50%\">99-Cthulhu Mythos: ") + str99Cthulhu + CString("</td>") + strEOL;
            strHTML += strIndent + strIndent + strIndent + CString("<td colspan=\"2\" width=\"50%\">Damage Bonus: ") + strDamageBonus + CString("</td>") + strEOL;
            strHTML += strIndent + strIndent + CString("</tr>") + strEOL;
            strHTML += strIndent + strIndent + CString("</table>") + strEOL;
            strHTML += strIndent + CString("</td>") + strEOL;
            strHTML += CString("</tr>") + strEOL;
            strHTML += CString("</table>") + strEOL;

            strHTML += strEOL + CString("<hr>") + strEOL + strEOL;

            //write out skills
            strHTML += CString("<table border=\"0\" width=\"100%\">") + strEOL;

            int nCount = 2;
            strHTML += strIndent + strIndent + CString("<tr>") + strEOL;

            int iMax, i;
            iMax = m_Investigator.m_SkillList.GetSize();
            for( i = 0; i < iMax; i++ )
            {
                switch( m_Investigator.m_SkillList[i]->m_pSkill->m_SkillType )
                {
                    case SKILL:
                    {
                        //wrap for columns
                        if( ++nCount % 3 == 0 ) strHTML += strIndent + strIndent + CString("</tr>") + strEOL + strIndent + strIndent + CString("<tr>") + strEOL;

                        //get current
                        int nCurrent = m_Investigator.m_SkillList[i]->GetValue(fDrawV5_5Bases);

                        //output leader HTML
                        strHTML += strIndent + strIndent + strIndent + CString("<td width=\"33%\"><font size=\"2\">");
                        if( m_Investigator.m_SkillList[i]->m_nCurrent > m_Investigator.m_SkillList[i]->m_nMinimum ) strHTML += CString("<b>");

                        //output skill
                        const char * pszName = LPCSTR( m_Investigator.m_SkillList[i]->m_pSkill->m_strName);
                        while( *pszName == '*' ) pszName++;
                        CString strCurrent; strCurrent.Format( "%d",  nCurrent );
                        strHTML += CString(pszName) + CString(" ") + strCurrent + CString("%");

                        //output trailing HTML
                        if( m_Investigator.m_SkillList[i]->m_nCurrent > m_Investigator.m_SkillList[i]->m_nMinimum ) strHTML += CString("</b>");
                        strHTML += CString("</font></td>") + strEOL;
                        break;
                    }

                    case SUBSKILL:
                    {
                        const char * pszName = LPCSTR(m_Investigator.m_SkillList[i]->m_pSkill->m_strName);
                        while( *pszName == '*' ) pszName++;
                        if( strlen(pszName) > 0 )
                        {
                            //wrap for columns
                            if( ++nCount % 3 == 0 ) strHTML += strIndent + strIndent + CString("</tr>") + strEOL + strIndent + strIndent + CString("<tr>") + strEOL;

                            //get current
                            int nCurrent = m_Investigator.m_SkillList[i]->GetValue(fDrawV5_5Bases);

                            //output leader HTML
                            strHTML += strIndent + strIndent + strIndent + CString("<td width=\"33%\"><font size=\"2\">");
                            if( m_Investigator.m_SkillList[i]->m_nCurrent > m_Investigator.m_SkillList[i]->m_nMinimum ) strHTML += CString("<b>");

                            //output skill
                            const char * pszParentName = LPCSTR(((CSubSkill*)m_Investigator.m_SkillList[i]->m_pSkill)->m_pSkill->m_strName);
                            while( *pszParentName == '*' ) pszParentName++;
                            strNumber.Format( "%d", nCurrent );
                            strHTML += CString(pszParentName) + CString(": ") + pszName + CString(" ") + strNumber + CString("%");

                            //output trailing HTML
                            if( m_Investigator.m_SkillList[i]->m_nCurrent > m_Investigator.m_SkillList[i]->m_nMinimum ) strHTML += CString("</b>");
                            strHTML += CString("</font></td>") + strEOL;
                        }
                        break;
                    }
                }
            }

            //now do firearms
            iMax = m_Investigator.m_FirearmList.GetSize();            
            for( i = 0; i < iMax; i++ )
            {
                //wrap for columns
                if( ++nCount % 3 == 0 ) strHTML += strIndent + strIndent + CString("</tr>") + strEOL + strIndent + strIndent + CString("<tr>") + strEOL;

                //get current
                int nCurrent = m_Investigator.m_FirearmList[i]->GetValue(fDrawV5_5Bases);

                //output leader HTML
                strHTML += strIndent + strIndent + strIndent + CString("<td width=\"33%\"><font size=\"2\">");
                if( m_Investigator.m_FirearmList[i]->m_nCurrent > m_Investigator.m_FirearmList[i]->m_nMinimum ) strHTML += CString("<b>");

                //output skill
                const char * pszName = LPCSTR( m_Investigator.m_FirearmList[i]->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
                CString strCurrent; strCurrent.Format( "%d", nCurrent ); 
                strHTML += CString(pszName) + CString(" ") + strCurrent + CString("%");

                //output trailing HTML
                if( m_Investigator.m_FirearmList[i]->m_nCurrent > m_Investigator.m_FirearmList[i]->m_nMinimum ) strHTML += CString("</b>");
                strHTML += CString("</font></td>") + strEOL;
            }

            //now do melee weapons
            iMax = m_Investigator.m_MeleeList.GetSize();
            for( i = 0; i < iMax; i++ )
            {
                //wrap for columns
                if( ++nCount % 3 == 0 ) strHTML += strIndent + strIndent + CString("</tr>") + strEOL + strIndent + strIndent + CString("<tr>") + strEOL;

                //get current
                int nCurrent = m_Investigator.m_MeleeList[i]->GetValue(fDrawV5_5Bases);

                //output leader HTML
                strHTML += strIndent + strIndent + strIndent + CString("<td width=\"33%\"><font size=\"2\">");
                if( m_Investigator.m_MeleeList[i]->m_nCurrent > m_Investigator.m_MeleeList[i]->m_nMinimum ) strHTML += CString("<b>");

                //output skill
                const char * pszName = LPCSTR( m_Investigator.m_MeleeList[i]->m_pSkill->m_strName);
                while( *pszName == '*' ) pszName++;
                CString strCurrent; strCurrent.Format( "%d", nCurrent ); 
                strHTML += CString(pszName) + CString(" ") +  strCurrent + CString("%");

                //output trailing HTML
                if( m_Investigator.m_MeleeList[i]->m_nCurrent > m_Investigator.m_MeleeList[i]->m_nMinimum ) strHTML += CString("</b>");
                strHTML += CString("</font></td>") + strEOL;
            }

            strHTML += strIndent + strIndent + CString("</tr>") + strEOL;
            strHTML += strIndent + CString("</table>") + strEOL;

            //end of HTML
            strHTML += CString("<br><br><hr><br><br>") + strEOL + strEOL;
            strHTML += CString("<font size=\"1\">Call of Cthulhu is a registered trademark of Chaosium Inc.</font>") + strEOL;
            strHTML += strEOL + CString("</body>") + strEOL + CString("</html>") + strEOL;

            //write file buffer
            File.Write( LPCSTR(strHTML), strHTML.GetLength() );
        }
        else
            AfxMessageBox( "Could not open file for output!", MB_ICONERROR );
    }
}

BOOL CInvestigatorDoc::SerialiseInvestigator(CArchive & ar, int nVersion)
{
    if( ar.IsStoring() )
    {
        //basic stats
        ar.WriteString( m_Investigator.m_strName ); ar << '\n';
        ar.WriteString( m_Investigator.m_strOccupation ); ar << '\n';
        ar.WriteString( m_Investigator.m_strBirthplace ); ar << '\n';
        ar.WriteString( m_Investigator.m_strColleges ); ar << '\n';
        ar.WriteString( m_Investigator.m_strMentalDisorders ); ar << '\n';
        ar.WriteString( m_Investigator.m_strSex ); ar << '\n';
        ar << m_Investigator.m_nAge;
        ar << int(m_Investigator.m_Era);
        ar.WriteString( m_Investigator.m_strPortrait ); ar << '\n';
        ar << m_Investigator.m_nAPP;
        ar << m_Investigator.m_nCON;
        ar << m_Investigator.m_nDEX;
        ar << m_Investigator.m_nEDU;
        ar << m_Investigator.m_nINT;
        ar << m_Investigator.m_nPOW;
        ar << m_Investigator.m_nSIZ;
        ar << m_Investigator.m_nSTR;

        //write out skill count
        ar << m_Investigator.m_AllSkillList.GetSize();

        //write out skills
        for( int n = 0; n < m_Investigator.m_AllSkillList.GetSize(); n++ )
        {
            CInvestigatorSkill* pInvestigatorSkill = m_Investigator.m_AllSkillList.GetAt(n);

            //write out additional skill info
            ar << pInvestigatorSkill->m_nCurrent;
            ar << pInvestigatorSkill->m_nMinimum;
            ar << pInvestigatorSkill->m_fOccupational;

            //write out skill
            m_Investigator.WriteSkill( ar, pInvestigatorSkill->m_pSkill );
        }

        //write player name
        ar.WriteString( m_Investigator.m_strPlayerName ); ar << '\n';

        if( nVersion >= 2 )
        {
            ar << m_Investigator.m_nSAN;
            ar << m_Investigator.m_nHitPoints;
            ar << m_Investigator.m_nMagic;
            ar.WriteString( m_Investigator.m_strAffiliation ); ar << '\n';
            ar.WriteString( m_Investigator.m_strCodeName ); ar << '\n';
            ar.WriteString( m_Investigator.m_strCurrentDate ); ar << '\n';
            ar.WriteString( m_Investigator.m_strInsanities ); ar << '\n';
        }
    }
    else
    {
        //basic stats
        if( nVersion >= 1  )
        {
            ar.ReadString( m_Investigator.m_strName );
            ar.ReadString( m_Investigator.m_strOccupation );
            ar.ReadString( m_Investigator.m_strBirthplace );
            ar.ReadString( m_Investigator.m_strColleges );
            ar.ReadString( m_Investigator.m_strMentalDisorders );
            ar.ReadString( m_Investigator.m_strSex );
            ar >> m_Investigator.m_nAge;
            int nEra; ar >> nEra; m_Investigator.m_Era = Era(nEra);
            ar.ReadString( m_Investigator.m_strPortrait );
            ar >> m_Investigator.m_nAPP;
            ar >> m_Investigator.m_nCON;
            ar >> m_Investigator.m_nDEX;
            ar >> m_Investigator.m_nEDU;
            ar >> m_Investigator.m_nINT;
            ar >> m_Investigator.m_nPOW;
            ar >> m_Investigator.m_nSIZ;
            ar >> m_Investigator.m_nSTR;

            //read in skill count
            int nSkills = 0;
            ar >> nSkills;

            //read in skills
            for( int i = 0; i < nSkills; i++ )
            {
                int nCurrent, m_nMinimum;
                BOOL fOccupational;

                //read in additional skill info
                ar >> nCurrent;
                ar >> m_nMinimum;
                ar >> fOccupational;

                //read in skill
                if( m_Investigator.ReadSkill( ar, fOccupational, nCurrent, m_nMinimum ) == 0 )
                    AfxThrowArchiveException( CArchiveException::badIndex, "" );
            }

            //read player name
            ar.ReadString( m_Investigator.m_strPlayerName );

            //*** new for version 2.0: ***

            //plug in defaults
            m_Investigator.m_nSAN = m_Investigator.m_nPOW*5;
            m_Investigator.m_nHitPoints = (int)( 0.5 + (((double)m_Investigator.m_nCON + (double)m_Investigator.m_nSIZ ) / 2.0 ) );
            m_Investigator.m_nMagic = m_Investigator.m_nPOW;

            //only load SAN etc if the version number was >= 2.0
            if( nVersion >= 2 )
            {
                ar >> m_Investigator.m_nSAN;
                ar >> m_Investigator.m_nHitPoints;
                ar >> m_Investigator.m_nMagic;
                ar.ReadString( m_Investigator.m_strAffiliation );
                ar.ReadString( m_Investigator.m_strCodeName );
                ar.ReadString( m_Investigator.m_strCurrentDate );
                ar.ReadString( m_Investigator.m_strInsanities );
            } //version 2.0
        } //version 1.0

        //remove all ANYSKILL types as they're useless after the wizard
        while( m_Investigator.RemoveSkill( "*" ) );

        //sort all skills into order
        m_Investigator.SortSkillList();

        //set rule file
        if( m_Investigator.m_strRuleFile.IsEmpty() ) m_Investigator.m_strRuleFile = "default.rul";
        SkillManager.AddRuleFile( m_Investigator.m_strRuleFile );
    }

    return TRUE;
}

BOOL CInvestigatorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    CWaitCursor Wait;

    CString strFile;
    const CString strEOL = "\r\n";
    const CString strIndent = "\t";

    /* build output buffer */

    //header
    strFile = "COC" + strEOL;

    //sheet
    strFile += "Sheet { " + strEOL;
    strFile += BuildKeyPair( "Script", m_strLastSheet );
    strFile += BuildKeyPair( "Class",  "Investigator" ); //future proofing
    strFile += "}" + strEOL + strEOL;

    //core investigator
    strFile += "Investigator { " + strEOL;
    strFile += BuildKeyPair( "Name",                m_Investigator.m_strName );
	strFile += BuildKeyPair( "Occupation",          m_Investigator.m_strOccupation );
	strFile += BuildKeyPair( "Birthplace",          m_Investigator.m_strBirthplace );
    strFile += BuildKeyPair( "Colleges",            m_Investigator.m_strColleges );
    strFile += BuildKeyPair( "MentalDisorders",     m_Investigator.m_strMentalDisorders );
    strFile += BuildKeyPair( "Sex",                 m_Investigator.m_strSex );
	strFile += BuildKeyPair( "Age",                 m_Investigator.m_nAge );
	strFile += BuildKeyPair( "Era",                 m_Investigator.m_Era );
    strFile += BuildKeyPair( "Portrait",            m_Investigator.m_strPortrait );
	strFile += BuildKeyPair( "APP",                 m_Investigator.m_nAPP );
	strFile += BuildKeyPair( "CON",                 m_Investigator.m_nCON );
	strFile += BuildKeyPair( "DEX",                 m_Investigator.m_nDEX );
	strFile += BuildKeyPair( "EDU",                 m_Investigator.m_nEDU );
	strFile += BuildKeyPair( "INT",                 m_Investigator.m_nINT );
	strFile += BuildKeyPair( "POW",                 m_Investigator.m_nPOW );
	strFile += BuildKeyPair( "SIZ",                 m_Investigator.m_nSIZ );
	strFile += BuildKeyPair( "STR",                 m_Investigator.m_nSTR );
	strFile += BuildKeyPair( "Player",              m_Investigator.m_strPlayerName );
    strFile += BuildKeyPair( "SAN",                 m_Investigator.m_nSAN );
    strFile += BuildKeyPair( "HP",                  m_Investigator.m_nHitPoints );
    strFile += BuildKeyPair( "Magic",               m_Investigator.m_nMagic );
    strFile += BuildKeyPair( "Affiliation",         m_Investigator.m_strAffiliation );
    strFile += BuildKeyPair( "CodeName",            m_Investigator.m_strCodeName );
    strFile += BuildKeyPair( "CurrentDate",         m_Investigator.m_strCurrentDate );
    strFile += BuildKeyPair( "Insanities",          m_Investigator.m_strInsanities );
	strFile += BuildKeyPair( "ExtraSchooling",      m_Investigator.m_ExtraSchooling );
    strFile += BuildKeyPair( "Income",              m_Investigator.m_strIncome );
    strFile += BuildKeyPair( "Spells",              m_Investigator.m_strSpells );
	strFile += BuildKeyPair( "RuleFile",            m_Investigator.m_strRuleFile );
    strFile += BuildKeyPair( "Residence",           m_Investigator.m_strResidence );
    strFile += BuildKeyPair( "PersonalDescription", m_Investigator.m_strPersonalDescription );
    strFile += BuildKeyPair( "Contacts",            m_Investigator.m_strContacts );
    strFile += BuildKeyPair( "Wounds",              m_Investigator.m_strWounds );
    strFile += BuildKeyPair( "Scars",               m_Investigator.m_strScars );
    strFile += BuildKeyPair( "History",             m_Investigator.m_strHistory );
    strFile += BuildKeyPair( "CashOnHand",          m_Investigator.m_strCashOnHand );
    strFile += BuildKeyPair( "Savings",             m_Investigator.m_strSavings );
    strFile += BuildKeyPair( "PersonalProperty",    m_Investigator.m_strPersonalProperty );
    strFile += BuildKeyPair( "RealEstate",          m_Investigator.m_strRealEstate );
    strFile += BuildKeyPair( "Posessions",          m_Investigator.m_strPossessions );
    strFile += BuildKeyPair( "TomesRead",           m_Investigator.m_strTomesRead );
    strFile += BuildKeyPair( "Artifacts",           m_Investigator.m_strArtifacts );
    strFile += BuildKeyPair( "EntitiesEncountered", m_Investigator.m_strEntitiesEncountered );
	strFile += BuildKeyPair( "SkillPoints",         m_Investigator.m_nSkillPoints );
	strFile += BuildKeyPair( "InterestPoints",      m_Investigator.m_nInterestPoints );

    strFile += strIndent + "Skills {" + strEOL;

    //skills
    int iMax = m_Investigator.m_AllSkillList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_Investigator.m_AllSkillList[i];

        if( pInvestigatorSkill )
        {
            int j;
            switch( pInvestigatorSkill->m_pSkill->m_SkillType )
            {
                case SKILL:
                    strFile += strIndent + strIndent + "Skill { " + strEOL;
                    strFile += strIndent + strIndent + BuildKeyPair( "Name", pInvestigatorSkill->m_pSkill->m_strName );
                    strFile += strIndent + strIndent + BuildKeyPair( "Base", pInvestigatorSkill->m_pSkill->m_strBase );
                    break;

                case SPECIALSKILL:
                    strFile += strIndent + strIndent + "SpecialSkill { " + strEOL;
                    strFile += strIndent + strIndent + BuildKeyPair( "Name", pInvestigatorSkill->m_pSkill->m_strName );
                    strFile += strIndent + strIndent + BuildKeyPair( "Base", pInvestigatorSkill->m_pSkill->m_strBase );
                    strFile += strIndent + strIndent + BuildKeyPair( "SubSkills", ((CSpecialSkill*)pInvestigatorSkill->m_pSkill)->m_nSkills );

                    //add all subskills now
                    for( j = 0; j < iMax; j++ )
                    {
                        CInvestigatorSkill* pInvestigatorSkill2 = m_Investigator.m_AllSkillList[j];
                        if( pInvestigatorSkill2 && pInvestigatorSkill2->m_pSkill->m_SkillType == SUBSKILL )
                        {
                            if( ((CSubSkill*)pInvestigatorSkill2->m_pSkill)->m_pSkill == pInvestigatorSkill->m_pSkill )
                            {
                                strFile += strIndent + strIndent + strIndent + "SubSkill { " + strEOL;
                                strFile += strIndent + strIndent + strIndent + strIndent + BuildKeyPair( "Name", pInvestigatorSkill2->m_pSkill->m_strName );
                                strFile += strIndent + strIndent + strIndent + strIndent + BuildKeyPair( "Base", pInvestigatorSkill2->m_pSkill->m_strBase );
                                strFile += strIndent + strIndent + strIndent + strIndent + BuildKeyPair( "Current", pInvestigatorSkill2->m_nCurrent );
                                strFile += strIndent + strIndent + strIndent + strIndent + BuildKeyPair( "Occupational", pInvestigatorSkill2->m_fOccupational );
                                strFile += strIndent + strIndent + strIndent + strIndent + BuildKeyPair( "Checked", pInvestigatorSkill2->m_fChecked );
                                strFile += strIndent + strIndent + strIndent + "}" + strEOL;
                            }
                        }
                    }
                    break;

                case SUBSKILL:
                    //added by SPECIALSKILL case
                    continue;

                case FIREARM:
                    strFile += strIndent + strIndent + "Firearm { " + strEOL;
                    strFile += strIndent + strIndent + BuildKeyPair( "Name", pInvestigatorSkill->m_pSkill->m_strName );
                    strFile += strIndent + strIndent + BuildKeyPair( "Base", pInvestigatorSkill->m_pSkill->m_strBase );
                    strFile += strIndent + strIndent + BuildKeyPair( "Bullets", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strBullets );
                    strFile += strIndent + strIndent + BuildKeyPair( "Dmg", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg );
                    strFile += strIndent + strIndent + BuildKeyPair( "HP", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strHP );
                    strFile += strIndent + strIndent + BuildKeyPair( "MAL", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strMAL );
                    strFile += strIndent + strIndent + BuildKeyPair( "Range", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strRange );
                    strFile += strIndent + strIndent + BuildKeyPair( "Shots", ((CFirearmSkill*)pInvestigatorSkill->m_pSkill)->m_strShots );
                    break;

                case MELEE:
                    strFile += strIndent + strIndent + "Melee { " + strEOL;
                    strFile += strIndent + strIndent + BuildKeyPair( "Name", pInvestigatorSkill->m_pSkill->m_strName );
                    strFile += strIndent + strIndent + BuildKeyPair( "Base", pInvestigatorSkill->m_pSkill->m_strBase );
                    strFile += strIndent + strIndent + BuildKeyPair( "Attacks", ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strAttacks );
                    strFile += strIndent + strIndent + BuildKeyPair( "Dmg", ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strDmg );
                    strFile += strIndent + strIndent + BuildKeyPair( "HP", ((CMeleeSkill*)pInvestigatorSkill->m_pSkill)->m_strHP );
                    break;

                default:
                    continue;
            }

            strFile += strIndent + strIndent + BuildKeyPair( "Current", pInvestigatorSkill->m_nCurrent );
            strFile += strIndent + strIndent + BuildKeyPair( "Occupational", pInvestigatorSkill->m_fOccupational );
            strFile += strIndent + strIndent + BuildKeyPair( "Checked", pInvestigatorSkill->m_fChecked );
            strFile += strIndent + strIndent + "}" + strEOL + strEOL;
        }
    }

    strFile += strIndent + "}" + strEOL;
    strFile += "}" + strEOL + strEOL;

    //create output file
    CFile File;
    File.Open( lpszPathName, CFile::modeWrite|CFile::modeCreate );

    //write file buffer
    File.Write( LPCSTR(strFile), strFile.GetLength() );
	
    //reset modified flag and exit
    SetModifiedFlag(FALSE);
	return TRUE;
}

CString CInvestigatorDoc::BuildKeyPair(CString strName, CString strValue)
{
    //search and replace all " with '
    const char* psz = LPCSTR(strValue);
    while( psz && *psz != '\0' ) 
    {
        if( *psz == '\"' ) *(char*)psz = '\'';
        psz++;
    }

    //continue to build keypair
    return CString( "\t\"" ) + strName + CString( "\" \"" ) + strValue + CString( "\"\r\n" );
}

CString CInvestigatorDoc::BuildKeyPair(CString strName, int nValue)
{
    CString strValue;
    strValue.Format( "%d", nValue );
    return CString( "\t\"" ) + strName + CString( "\" \"" ) + strValue + CString( "\"\r\n" );
}


BOOL CInvestigatorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    /* open file */
    CFile File;
    if( File.Open( lpszPathName, CFile::modeRead ) == FALSE )
    {
        AfxMessageBox( "Cannot open investigator file", MB_ICONERROR );
        return FALSE;
    }

    
    /* load the file into a buffer */

	//determine file size
    char* pszBuffer = NULL;
	DWORD dwLength = File.GetLength();
	if( dwLength == 0 ) 
	{
		AfxMessageBox( "File is empty", MB_ICONERROR );
		return FALSE;
	}

	//allocate a buffer
	pszBuffer = new char[dwLength+1];
	if( pszBuffer == NULL )
	{
		AfxMessageBox( "Cannot allocate buffer", MB_ICONERROR );
		return FALSE;
	}

	//load the whole file
	if( File.Read( pszBuffer, dwLength ) < dwLength ) 
	{ 
		delete[] pszBuffer;
		AfxMessageBox( "Unexpected end of file", MB_ICONERROR );
		return FALSE;
	}

	//add a terminator at the end of the string
	pszBuffer[dwLength] = '\0';


    //empty the skill points
    m_Investigator.m_nSkillPoints = 0;
    m_Investigator.m_nInterestPoints = 0;

    /* parse the file */
    CParseBuffer Buffer(pszBuffer);
    delete[] pszBuffer;

    //read first line
    CString strHeader = Buffer.ExtractUnquotedString();

    //check for olde versions
    if( strHeader.Compare( SHEET_FILE_HEADER10 ) == 0 )
    {
        File.Seek( 0, CFile::begin );
        CArchive ar( &File, CArchive::load );
        ar.ReadString( strHeader );
        if( SerialiseInvestigator( ar, 1 ) )
        {
            if( m_Investigator.m_nSkillPoints <= 0 || m_Investigator.m_nInterestPoints <= 0 ) m_Investigator.CalculateSkillPoints();
            ReloadPortrait();
            return TRUE;
        }
        else return FALSE;
    }
    else
        if( strHeader.Compare( SHEET_FILE_HEADER20 ) == 0 )
        {
            File.Seek( 0, CFile::begin );
            CArchive ar( &File, CArchive::load );
            ar.ReadString( strHeader );
            if( SerialiseInvestigator( ar, 2 ) )
            {
                if( m_Investigator.m_nSkillPoints <= 0 || m_Investigator.m_nInterestPoints <= 0 ) m_Investigator.CalculateSkillPoints();
                ReloadPortrait();
                return TRUE;
            }
            else return FALSE;
        }

    //make sure it's a v3+ friendly header
    if( strHeader.CompareNoCase( "COC" ) != 0 )
    {
        AfxMessageBox( "Unknown file format: unknown file header", MB_ICONERROR );
        return FALSE;
    }

    //read in the rest of the file
    Buffer.IgnoreLineFeeds();
    while( Buffer.IsEmpty() == FALSE )
    {
        CString strSectionType = Buffer.ExtractUnquotedString();
        CString strSection = Buffer.ExtractSurroundedString( '{', '}' );
        if( Buffer.IsUnderflow() == FALSE && Buffer.IsParseError() == FALSE )
        {
            if( strSectionType.CompareNoCase( "Investigator" ) == 0 ) ParseInvestigatorSection(strSection); else
            if( strSectionType.CompareNoCase( "Sheet" ) == 0 ) ParseSheetSection(strSection); else
                ((CMainFrame*)AfxGetMainWnd())->GetOutputWindow().AddString( CString("Skipping Unknown section type in investigator file: ") + strSectionType );
        }
        else
        {
            AfxMessageBox( "Invalid file format", MB_ICONERROR );
            return FALSE;
        }
    }

    //finish loading
    if( m_Investigator.m_nSkillPoints <= 0 || m_Investigator.m_nInterestPoints <= 0 ) m_Investigator.CalculateSkillPoints();
    ReloadPortrait();
	return TRUE;
}

BOOL CInvestigatorDoc::ParseInvestigatorSection(CString& strSection)
{
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        if( Section.PeekNextChar() == '\"' )
        {
            CString strKey = Section.ExtractQuotedString();
            CString strValue = Section.ExtractQuotedString();
            if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
            {
                if( strKey.CompareNoCase( "Name" ) == 0 )                   m_Investigator.m_strName = strValue; else
                if( strKey.CompareNoCase( "Occupation" ) == 0 )             m_Investigator.m_strOccupation = strValue; else
                if( strKey.CompareNoCase( "Birthplace" ) == 0 )             m_Investigator.m_strBirthplace = strValue; else
                if( strKey.CompareNoCase( "Colleges" ) == 0 )               m_Investigator.m_strColleges = strValue; else
                if( strKey.CompareNoCase( "MentalDisorders" ) == 0 )        m_Investigator.m_strMentalDisorders = strValue; else
                if( strKey.CompareNoCase( "Sex" ) == 0 )                    m_Investigator.m_strSex = strValue; else
                if( strKey.CompareNoCase( "Age" ) == 0 )                    m_Investigator.m_nAge = atoi(strValue); else
                if( strKey.CompareNoCase( "Era" ) == 0 )                    m_Investigator.m_Era = Era(atoi(strValue)); else
                if( strKey.CompareNoCase( "Portrait" ) == 0 )               m_Investigator.m_strPortrait = strValue; else
                if( strKey.CompareNoCase( "APP" ) == 0 )                    m_Investigator.m_nAPP = atoi(strValue); else
                if( strKey.CompareNoCase( "CON" ) == 0 )                    m_Investigator.m_nCON = atoi(strValue); else
                if( strKey.CompareNoCase( "DEX" ) == 0 )                    m_Investigator.m_nDEX = atoi(strValue); else
                if( strKey.CompareNoCase( "EDU" ) == 0 )                    m_Investigator.m_nEDU = atoi(strValue); else
                if( strKey.CompareNoCase( "INT" ) == 0 )                    m_Investigator.m_nINT = atoi(strValue); else
                if( strKey.CompareNoCase( "POW" ) == 0 )                    m_Investigator.m_nPOW = atoi(strValue); else
                if( strKey.CompareNoCase( "SIZ" ) == 0 )                    m_Investigator.m_nSIZ = atoi(strValue); else
                if( strKey.CompareNoCase( "STR" ) == 0 )                    m_Investigator.m_nSTR = atoi(strValue); else
                if( strKey.CompareNoCase( "Player" ) == 0 )                 m_Investigator.m_strPlayerName = strValue; else
                if( strKey.CompareNoCase( "SAN" ) == 0 )                    m_Investigator.m_nSAN = atoi(strValue); else
                if( strKey.CompareNoCase( "HP" ) == 0 )                     m_Investigator.m_nHitPoints = atoi(strValue); else
                if( strKey.CompareNoCase( "Magic" ) == 0 )                  m_Investigator.m_nMagic = atoi(strValue); else
                if( strKey.CompareNoCase( "Affiliation" ) == 0 )            m_Investigator.m_strAffiliation = strValue; else
                if( strKey.CompareNoCase( "CodeName" ) == 0 )               m_Investigator.m_strCodeName = strValue; else
                if( strKey.CompareNoCase( "CurrentDate" ) == 0 )            m_Investigator.m_strCurrentDate = strValue; else
                if( strKey.CompareNoCase( "Insanities" ) == 0 )             m_Investigator.m_strInsanities = strValue; else
                if( strKey.CompareNoCase( "ExtraSchooling" ) == 0 )         m_Investigator.m_ExtraSchooling = CInvestigator::EXTRASCHOOLING(atoi(strValue)); else
                if( strKey.CompareNoCase( "Income" ) == 0 )                 m_Investigator.m_strIncome = strValue; else
                if( strKey.CompareNoCase( "Spells" ) == 0 )                 m_Investigator.m_strSpells = strValue; else
                if( strKey.CompareNoCase( "RuleFile" ) == 0 )               m_Investigator.m_strRuleFile = strValue; else
                if( strKey.CompareNoCase( "Residence" ) == 0 )              m_Investigator.m_strResidence = strValue; else
                if( strKey.CompareNoCase( "PersonalDescription" ) == 0 )    m_Investigator.m_strPersonalDescription = strValue; else
                if( strKey.CompareNoCase( "Contacts" ) == 0 )               m_Investigator.m_strContacts = strValue; else
                if( strKey.CompareNoCase( "Wounds" ) == 0 )                 m_Investigator.m_strWounds = strValue; else
                if( strKey.CompareNoCase( "Scars" ) == 0 )                  m_Investigator.m_strScars = strValue; else
                if( strKey.CompareNoCase( "History" ) == 0 )                m_Investigator.m_strHistory = strValue; else
                if( strKey.CompareNoCase( "CashOnHand" ) == 0 )             m_Investigator.m_strCashOnHand = strValue; else
                if( strKey.CompareNoCase( "Savings" ) == 0 )                m_Investigator.m_strSavings = strValue; else
                if( strKey.CompareNoCase( "PersonalProperty" ) == 0 )       m_Investigator.m_strPersonalProperty = strValue; else
                if( strKey.CompareNoCase( "RealEstate" ) == 0 )             m_Investigator.m_strRealEstate = strValue; else
                if( strKey.CompareNoCase( "Posessions" ) == 0 )             m_Investigator.m_strPossessions = strValue; else
                if( strKey.CompareNoCase( "TomesRead" ) == 0 )              m_Investigator.m_strTomesRead = strValue; else
                if( strKey.CompareNoCase( "Artifacts" ) == 0 )              m_Investigator.m_strArtifacts = strValue; else
                if( strKey.CompareNoCase( "EntitiesEncountered" ) == 0 )    m_Investigator.m_strEntitiesEncountered = strValue; else
                if( strKey.CompareNoCase( "SkillPoints" ) == 0 )            m_Investigator.m_nSkillPoints = atoi(strValue); else
                if( strKey.CompareNoCase( "InterestPoints" ) == 0 )         m_Investigator.m_nInterestPoints = atoi(strValue);
            }
            else
                return FALSE;
        }
        else
        {
            CString strKey = Section.ExtractUnquotedString();
            CString strSection = Section.ExtractSurroundedString( '{', '}' );
            if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
            {
                if( strKey.CompareNoCase( "Skills" ) == 0 )
                {
                    CParseBuffer Section( strSection );
                    Section.IgnoreLineFeeds();
                    while( Section.IsEmpty() == FALSE )
                    {
                        CString strSectionType = Section.ExtractUnquotedString();
                        CString strSubSection = Section.ExtractSurroundedString( '{', '}' );
                        if( strSectionType.CompareNoCase( "Skill" ) == 0 )        ParseSkillSection(strSubSection); else
                        if( strSectionType.CompareNoCase( "SpecialSkill" ) == 0 ) ParseSpecialSkillSection(strSubSection); else
                        if( strSectionType.CompareNoCase( "Firearm" ) == 0 )      ParseFirearmSection(strSubSection); else
                        if( strSectionType.CompareNoCase( "Melee" ) == 0 )        ParseMeleeSection(strSubSection);
                    }
                }
            }
            else
                return FALSE;
        }
    }

    return TRUE;
}

BOOL CInvestigatorDoc::ParseSkillSection(CString& strSection)
{
    //create new skill item and add it
    CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSkill(ERA_ISALL) );
    m_Investigator.m_AllSkillList.Add( pNew );
    m_Investigator.m_SkillList.Add( pNew );
    pNew->m_pSkill->m_SkillType = SKILL;
    
    //read each keypair
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        CString strKey = Section.ExtractQuotedString();
        CString strValue = Section.ExtractQuotedString();
        if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
        {
            if( strKey.CompareNoCase( "Name" ) == 0 )           pNew->m_pSkill->m_strName = strValue; else
            if( strKey.CompareNoCase( "Base" ) == 0 )           pNew->m_pSkill->m_strBase = strValue; else
            if( strKey.CompareNoCase( "Current" ) == 0 )        pNew->m_nCurrent = atoi(strValue); else
            if( strKey.CompareNoCase( "Occupational" ) == 0 )   pNew->m_fOccupational = atoi(strValue); else
            if( strKey.CompareNoCase( "Checked" ) == 0 )        pNew->m_fChecked = atoi(strValue);
        }
        else
            return FALSE;
    }

    //calculate minimum skill value
    pNew->m_nMinimum = pNew->m_pSkill->EvaluateBase( &m_Investigator );
    return TRUE;
}

BOOL CInvestigatorDoc::ParseSpecialSkillSection(CString& strSection)
{
    //find the subskills key
    int nSubsets = 0;
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        if( Section.PeekNextChar() == '\"' )
        {
            CString strKey = Section.ExtractQuotedString();
            CString strValue = Section.ExtractQuotedString();
            if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
            {
                if( strKey.CompareNoCase( "SubSkills" ) == 0 )
                {
                    nSubsets = atoi(strValue);
                    break;
                }
            }
            else
                return FALSE;
        }
        else
        {
            Section.ExtractUnquotedString();
            Section.ExtractSurroundedString( '{', '}' );
        }
    }
    if( nSubsets == 0 )
    {
        AfxMessageBox( "Warning: Subskill section has no subskills!" );
        return TRUE;
    }

    //create new skill item and add it
    CInvestigatorSkill* pNew = new CInvestigatorSkill( new CSpecialSkill("", "", ERA_ISALL, nSubsets, FALSE) );
    m_Investigator.m_AllSkillList.Add( pNew );
    m_Investigator.m_SkillList.Add( pNew );
    
    //read each keypair
    Section.Reset(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        if( Section.PeekNextChar() == '\"' )
        {
            CString strKey = Section.ExtractQuotedString();
            CString strValue = Section.ExtractQuotedString();
            if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
            {
                if( strKey.CompareNoCase( "Name" ) == 0 )           pNew->m_pSkill->m_strName = strValue; else
                if( strKey.CompareNoCase( "Base" ) == 0 )           pNew->m_pSkill->m_strBase = strValue; else
                if( strKey.CompareNoCase( "Current" ) == 0 )        pNew->m_nCurrent = atoi(strValue); else
                if( strKey.CompareNoCase( "Occupational" ) == 0 )   pNew->m_fOccupational = atoi(strValue); else
                if( strKey.CompareNoCase( "Checked" ) == 0 )        pNew->m_fChecked = atoi(strValue);
            }
            else
                return FALSE;
        }
        else
        {
            //skip over subskill definition
            Section.ExtractUnquotedString();
            Section.ExtractSurroundedString( '{', '}' );
        }
    }

    //calculate minimum skill values
    pNew->m_nMinimum = pNew->m_pSkill->EvaluateBase( &m_Investigator );

    //now read each subskill section
    Section.Reset(strSection);
    Section.IgnoreLineFeeds();
    int iSubSkill = 0;
    while( Section.IsEmpty() == FALSE )
    {
        if( Section.PeekNextChar() != '\"' )
        {
            CString strSubSkillName = Section.ExtractUnquotedString();
            CString strSubSkill = Section.ExtractSurroundedString( '{', '}' );
            if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
            {
                if( strSubSkillName.CompareNoCase( "SubSkill" ) == 0 )
                {
                    //make sure there's a free slot on the specialskill
                    if( iSubSkill >= nSubsets )
                    {
                        AfxMessageBox( "Warning: Too many subskills for special skill" );
                        break;
                    }

                    //create a new subksill
                    CInvestigatorSkill* pNewSub = new CInvestigatorSkill( new CSubSkill( (CSpecialSkill*)pNew->m_pSkill, "", ERA_ISALL ) );
                    m_Investigator.m_AllSkillList.Add( pNewSub );
                    m_Investigator.m_SkillList.Add( pNewSub );

                    //link it to parent
                    ((CSpecialSkill*)pNew->m_pSkill)->m_pSubSkills[iSubSkill++] = (CSubSkill*)pNewSub->m_pSkill;

                    //set default values
                    pNewSub->m_nCurrent = pNewSub->m_nMinimum = pNew->m_nMinimum;
                    pNewSub->m_fOccupational = pNew->m_fOccupational;
                    pNewSub->m_pSkill->m_strBase = pNew->m_pSkill->m_strBase;

                    //extract all values for this subskill
                    CParseBuffer SubSection(strSubSkill);
                    SubSection.IgnoreLineFeeds();
                    while( SubSection.IsEmpty() == FALSE )
                    {
                        CString strKey = SubSection.ExtractQuotedString();
                        CString strValue = SubSection.ExtractQuotedString();
                        if( SubSection.IsUnderflow() == FALSE && SubSection.IsParseError() == FALSE )
                        {
                            if( strKey.CompareNoCase( "Name" ) == 0 )           pNewSub->m_pSkill->m_strName = strValue; else
                            if( strKey.CompareNoCase( "Base" ) == 0 )           pNewSub->m_pSkill->m_strBase = strValue; else
                            if( strKey.CompareNoCase( "Current" ) == 0 )        pNewSub->m_nCurrent = atoi(strValue); else
                            if( strKey.CompareNoCase( "Occupational" ) == 0 )   pNewSub->m_fOccupational = atoi(strValue); else
                            if( strKey.CompareNoCase( "Checked" ) == 0 )        pNewSub->m_fChecked = atoi(strValue);
                        }
                        else
                            return FALSE;
                    }
                }
            }
            else
                return FALSE;
        }
        else
        {
            //skip over attribute definition
            Section.ExtractQuotedString();
            Section.ExtractQuotedString();
        }
    }

    //fill in uninitialised subskills
    if( iSubSkill < nSubsets )
    {
        AfxMessageBox( "Warning: Too few subskills defined!" );

        while( iSubSkill < nSubsets )
        {
            //create a new subksill
            CInvestigatorSkill* pNewSub = new CInvestigatorSkill( new CSubSkill( (CSpecialSkill*)pNew->m_pSkill, "", ERA_ISALL ) );
            m_Investigator.m_AllSkillList.Add( pNewSub );
            m_Investigator.m_SkillList.Add( pNewSub );

            //link it to parent
            ((CSpecialSkill*)pNew->m_pSkill)->m_pSubSkills[iSubSkill++] = (CSubSkill*)pNewSub->m_pSkill;

            //set default values
            pNewSub->m_nCurrent = pNewSub->m_nMinimum = pNew->m_nMinimum;
            pNewSub->m_fOccupational = pNew->m_fOccupational;
            pNewSub->m_pSkill->m_strBase = pNew->m_pSkill->m_strBase;
        }
    }

    return TRUE;
}

BOOL CInvestigatorDoc::ParseFirearmSection(CString& strSection)
{
    //create new skill item and add it
    CInvestigatorSkill* pNew = new CInvestigatorSkill( new CFirearmSkill(ERA_ISALL) );
    m_Investigator.m_AllSkillList.Add( pNew );
    m_Investigator.m_FirearmList.Add( pNew );
    
    //read each keypair
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        CString strKey = Section.ExtractQuotedString();
        CString strValue = Section.ExtractQuotedString();
        if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
        {
            if( strKey.CompareNoCase( "Name" ) == 0 )           pNew->m_pSkill->m_strName = strValue; else
            if( strKey.CompareNoCase( "Base" ) == 0 )           pNew->m_pSkill->m_strBase = strValue; else

            if( strKey.CompareNoCase( "Bullets" ) == 0 )        ((CFirearmSkill*)pNew->m_pSkill)->m_strBullets = strValue; else
            if( strKey.CompareNoCase( "Dmg" ) == 0 )            ((CFirearmSkill*)pNew->m_pSkill)->m_strDmg = strValue; else
            if( strKey.CompareNoCase( "HP" ) == 0 )             ((CFirearmSkill*)pNew->m_pSkill)->m_strHP = strValue; else
            if( strKey.CompareNoCase( "MAL" ) == 0 )            ((CFirearmSkill*)pNew->m_pSkill)->m_strMAL = strValue; else
            if( strKey.CompareNoCase( "Range" ) == 0 )          ((CFirearmSkill*)pNew->m_pSkill)->m_strRange = strValue; else
            if( strKey.CompareNoCase( "Shots" ) == 0 )          ((CFirearmSkill*)pNew->m_pSkill)->m_strShots = strValue; else
                
            if( strKey.CompareNoCase( "Current" ) == 0 )        pNew->m_nCurrent = atoi(strValue); else
            if( strKey.CompareNoCase( "Occupational" ) == 0 )   pNew->m_fOccupational = atoi(strValue); else
            if( strKey.CompareNoCase( "Checked" ) == 0 )        pNew->m_fChecked = atoi(strValue);
        }
        else
            return FALSE;
    }

    //calculate minimum skill value
    pNew->m_nMinimum = pNew->m_pSkill->EvaluateBase( &m_Investigator );

    return TRUE;
}

BOOL CInvestigatorDoc::ParseMeleeSection(CString& strSection)
{
    //create new skill item and add it
    CInvestigatorSkill* pNew = new CInvestigatorSkill( new CMeleeSkill(ERA_ISALL) );
    m_Investigator.m_AllSkillList.Add( pNew );
    m_Investigator.m_MeleeList.Add( pNew );
    
    //read each keypair
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        CString strKey = Section.ExtractQuotedString();
        CString strValue = Section.ExtractQuotedString();
        if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
        {
            if( strKey.CompareNoCase( "Name" ) == 0 )           pNew->m_pSkill->m_strName = strValue; else
            if( strKey.CompareNoCase( "Base" ) == 0 )           pNew->m_pSkill->m_strBase = strValue; else

            if( strKey.CompareNoCase( "Attacks" ) == 0 )        ((CMeleeSkill*)pNew->m_pSkill)->m_strAttacks = strValue; else
            if( strKey.CompareNoCase( "Dmg" ) == 0 )            ((CMeleeSkill*)pNew->m_pSkill)->m_strDmg = strValue; else
            if( strKey.CompareNoCase( "HP" ) == 0 )             ((CMeleeSkill*)pNew->m_pSkill)->m_strHP = strValue; else
                
            if( strKey.CompareNoCase( "Current" ) == 0 )        pNew->m_nCurrent = atoi(strValue); else
            if( strKey.CompareNoCase( "Occupational" ) == 0 )   pNew->m_fOccupational = atoi(strValue); else
            if( strKey.CompareNoCase( "Checked" ) == 0 )        pNew->m_fChecked = atoi(strValue);
        }
        else
            return FALSE;
    }

    //calculate minimum skill value
    pNew->m_nMinimum = pNew->m_pSkill->EvaluateBase( &m_Investigator );

    return TRUE;
}

BOOL CInvestigatorDoc::ParseSheetSection(CString &strSection)
{
    CParseBuffer Section(strSection);
    Section.IgnoreLineFeeds();
    while( Section.IsEmpty() == FALSE )
    {
        CString strKey = Section.ExtractQuotedString();
        CString strValue = Section.ExtractQuotedString();
        if( Section.IsUnderflow() == FALSE && Section.IsParseError() == FALSE )
        {
            if( strKey.CompareNoCase( "Script" ) == 0 )  m_strLastSheet = strValue;
            if( strKey.CompareNoCase( "Class" ) == 0 && !strValue.IsEmpty() && strValue.CompareNoCase( "Investigator" ) != 0 )
            {
                AfxMessageBox( CString( "This file contains a class of " ) + strValue + CString( ".\nThis version of byakhee can only deal with Investigator classes"), MB_ICONEXCLAMATION );
            }
        }
        else
            return FALSE;
    }

    return TRUE;
}

void CInvestigatorDoc::UpdateStatusBar()
{
    int nPointsAvailable = m_Investigator.m_nSkillPoints + m_Investigator.m_nInterestPoints;
    CString strAvailable, strPoints;

    if( nPointsAvailable != 0 )
    {
        int iMax = m_Investigator.m_AllSkillList.GetSize();

        //calculate the total number of points spent
        int nPointsSpent = 0;
        for( int i = 0; i < iMax; i++ )
        {
            CInvestigatorSkill* pInvestigatorSkill = m_Investigator.m_AllSkillList[i];
            nPointsSpent += (pInvestigatorSkill->m_nCurrent - pInvestigatorSkill->m_nMinimum);
        }

        //display the number of available points
        int nPointsRemaining = nPointsAvailable - nPointsSpent;
        if( nPointsRemaining >= 0 ) strAvailable.Format( "%d Points", nPointsRemaining ); else strAvailable.Format( "%d Over", -nPointsRemaining );

        //display the number of skill and interest points
        strPoints.Format( "Skill: %d Hobby: %d", m_Investigator.m_nSkillPoints, m_Investigator.m_nInterestPoints );
    }

    CMainFrame* wndMainFrame = (CMainFrame*)AfxGetMainWnd();
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_POINTS), strPoints );
    wndMainFrame->GetStatusBar().SetPaneText( wndMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_LINENUMBER), strAvailable );
}