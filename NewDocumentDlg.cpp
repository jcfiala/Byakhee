// NewDocumentDlg.cpp : implementation file
//

#include "stdafx.h"
#include <afxadv.h>
#include "Byakhee.h"
#include "MainFrm.h"
#include "NewDocumentDlg.h"
#include "InvestigatorDoc.h"
#include "ScriptEditDoc.h"
#include "HandoutDoc.h"
#include "SectionFile.h"
#include "Wizard_NewInvestigator.h"
#include "ByakheeScriptExecutor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//default script contents
const char* g_pszDefaultNewScript = "//new byakhee script\r\n\r\noutput.show();\r\noutput.write( \"executing new byakhee script...\" );\r\n";
const char* g_pszDefaultNewSheetScript = "//new byakhee sheet script\r\n\r\nsheet.sheetsize( 188.0, 215.0 ); //normal page size\r\n";

#ifndef TBSTYLE_EX_DRAWDDARROWS
	#define TB_SETEXTENDEDSTYLE		(WM_USER+84)
	#define TB_GETEXTENDEDSTYLE     (WM_USER+85)
	#define TBSTYLE_EX_DRAWDDARROWS (0x00000001)
#endif


/////////////////////////////////////////////////////////////////////////////
// CNewDocumentDlg dialog
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

int CNewDocumentDlg::m_iPage = 0; //initial page

CNewDocumentDlg::CNewDocumentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDocumentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewDocumentDlg)
	m_fFullWizard = FALSE;
	//}}AFX_DATA_INIT

    //build the large icon image list
    m_ImageList.Create( 32, 32, ILC_COLOR4|ILC_MASK, 0, 0 );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_SHEETTYPE) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INVESTIGATOR_WIZARD) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWSCRIPT) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWSHEETSCRIPT) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RUNSCRIPT) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWHANDOUTSCRIPT) ) );
    m_ImageList.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_HANDOUTTYPE) ) );

    //build the small icon image list
    m_ImageListSmall.Create( 16, 16, ILC_COLOR4|ILC_MASK, 0, 0 );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_SHEETTYPE) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INVESTIGATOR_WIZARD) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWSCRIPT) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWSHEETSCRIPT) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RUNSCRIPT) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEWHANDOUTSCRIPT) ) );
    m_ImageListSmall.Add( LoadIcon( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_HANDOUTTYPE) ) );
}

CNewDocumentDlg::~CNewDocumentDlg()
{
    for( int i = 0; i < m_NewInvestigatorInfoArray.GetSize(); i++ ) delete m_NewInvestigatorInfoArray[i];
    m_NewInvestigatorInfoArray.RemoveAll();

    //clear status bar
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage("");
}

void CNewDocumentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDocumentDlg)
	DDX_Control(pDX, IDC_FULLWIZARD, m_wndFullWizard);
	DDX_Control(pDX, IDC_NEWDOCUMENT_TYPES, m_wndNewDocumentTypes);
	DDX_Control(pDX, IDC_AVAILABLETYPES, m_wndAvailableTypes);
	DDX_Check(pDX, IDC_FULLWIZARD, m_fFullWizard);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewDocumentDlg, CDialog)
	//{{AFX_MSG_MAP(CNewDocumentDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_AVAILABLETYPES, OnDblclkAvailableTypes)
	ON_NOTIFY(TCN_SELCHANGE, IDC_NEWDOCUMENT_TYPES, OnSelChangeNewDocumentTypes)
    ON_COMMAND(ID_VIEW_LARGEICON, OnViewLargeIcons)
    ON_COMMAND(ID_VIEW_SMALLICON, OnViewSmallIcons)
    ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_NOTIFY(NM_CLICK, IDC_AVAILABLETYPES, OnClickAvailabletypes)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_AVAILABLETYPES, OnItemchangedAvailabletypes)
	ON_BN_CLICKED(IDC_FULLWIZARD, OnFullWizard)
	ON_COMMAND(ID_VIEW_RULEFILEINFORMATION, OnViewInformation)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY(TBN_DROPDOWN, IDC_TOOLBAR, OnToolbarDropDown)
    ON_COMMAND_RANGE(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnFileMRU )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDocumentDlg message handlers

void CNewDocumentDlg::OnDblclkAvailableTypes(NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnOK();
	*pResult = 0;
}

void CNewDocumentDlg::OnSelChangeNewDocumentTypes(NMHDR* pNMHDR, LRESULT* pResult) 
{
    m_iPage = m_wndNewDocumentTypes.GetCurSel();
    InitialisePage( m_iPage );
	*pResult = 0;
}

void CNewDocumentDlg::OnOK() 
{
    //update status bar
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );

    //get data from dialog
    UpdateData(UD_GETDATA);

    //set the item type we're looking for
    LV_ITEM lvi;
    char szCaption[MAX_PATH+1];
    lvi.pszText = szCaption;
    lvi.cchTextMax = MAX_PATH;
    lvi.mask = LVIF_PARAM|LVIF_STATE|LVIF_TEXT;
    lvi.iSubItem = 0;
    lvi.stateMask = LVIS_SELECTED|LVIS_FOCUSED;

    //scan through all available types and find the index of the selected item
    int nMax = m_wndAvailableTypes.GetItemCount();
    int iSelected = -1;
    for( int i = 0; i < nMax; i++ )
    {
        lvi.iItem = i;
        if( m_wndAvailableTypes.GetItem( &lvi ) && lvi.state & (LVIS_SELECTED|LVIS_FOCUSED) ) 
        {
            iSelected = i;
            break;
        }
    }

    //if we found one, create it
    if( iSelected != -1 )
    {
        //get the type index and the new name
        m_iPage = m_wndNewDocumentTypes.GetCurSel();
        int i = (int)lvi.lParam;

        //create a document
        switch( m_iPage )
        {
            case 0: //blank investigator sheet
                CDialog::OnOK();
                AfxGetMainWnd()->RedrawWindow();
                CreateNewInvestigator( m_NewInvestigatorInfoArray[i], FALSE );
                break;

            case 1: //investigator wizard
                CDialog::OnOK();
                AfxGetMainWnd()->RedrawWindow();
                CreateNewInvestigator( m_NewInvestigatorInfoArray[i], TRUE );
                break;

            case 2: //handout
                CDialog::OnOK();
                AfxGetMainWnd()->RedrawWindow();
                CreateNewHandout( lvi.pszText );
                break;

/*
            case 3: //monsters
                break;
*/
            case 3: //script
                switch(lvi.lParam)
                {
                    case NEWDOC_SCRIPT_NEWSCRIPT:
                        CDialog::OnOK();
                        CreateNewScriptDocument( "Script", g_pszDefaultNewScript, "New Script" );
                        break;

                    case NEWDOC_SCRIPT_NEWSHEETSCRIPT:
                        CDialog::OnOK();
                        CreateNewScriptDocument( "SheetScript", g_pszDefaultNewSheetScript, "New Investigator Sheet" );
                        break;

                    case NEWDOC_SCRIPT_NEWHANDOUTSCRIPT:
                        CDialog::OnOK();
                        CreateNewScriptDocument( "HandoutScript", g_pszDefaultNewSheetScript, "New Handout" );
                        break;

                    case NEWDOC_SCRIPT_RUN:
                        if( strlen(lvi.pszText) > 0 )
                        {
                            CDialog::OnOK();
                            CString strFileName = CByakheeApp::GetDefaultDirectory() + CString(lvi.pszText) + CString(".bys");
                            CByakheeScriptExecutor ScriptExecutor( strFileName );
                            ScriptExecutor.Execute();
                        }
                        else
                            AfxMessageBox( "Error: Script has no name!" );
                        break;
                }
                break;
        }
    }
    else
    {
        //tell the user
        AfxMessageBox( "You must select an item", MB_ICONEXCLAMATION );
    }
}

BOOL CNewDocumentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CWaitCursor WaitCursor;

    //set default full wizard setting
    m_fFullWizard = AfxGetApp()->GetProfileInt( "Wizard", "FullWiz", TRUE );
    UpdateData(UD_SETDATA);

    //prepare the button structures
    TBADDBITMAP tbab = { AfxGetInstanceHandle(), IDB_TOOLBAR_NEWDOCDLG };
    TBBUTTON tbb[] = {
        { 0, ID_FILE_OPEN,                  TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_DROPDOWN, 0, NULL },
        { 0, 0,                             0,               TBSTYLE_SEP,        0, NULL },
        { 1, ID_VIEW_LARGEICON,             TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, NULL },
        { 2, ID_VIEW_SMALLICON,             TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, NULL },
        { 3, ID_VIEW_LIST,                  TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, 0, NULL },
        { 0, 0,                             0,               TBSTYLE_SEP,        0, NULL },
        { 4, ID_VIEW_RULEFILEINFORMATION,   TBSTATE_ENABLED, TBSTYLE_BUTTON,     0, NULL },
    };

    //initialise the toolbar
    CWnd* pToolbar = GetDlgItem(IDC_TOOLBAR);

	//make toolbar flat if requested by user
    if( AfxGetApp()->GetProfileInt( "Display", "FlatToolbar", TRUE ) == TRUE ) pToolbar->ModifyStyle( 0, TBSTYLE_FLAT ); //make it flat
    pToolbar->SendMessage( TB_SETEXTENDEDSTYLE, 0, pToolbar->SendMessage(TB_GETEXTENDEDSTYLE) | TBSTYLE_EX_DRAWDDARROWS );
    pToolbar->SendMessage( TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0 );
    pToolbar->SendMessage( TB_ADDBITMAP, 5, (LPARAM)&tbab );
    pToolbar->SendMessage( TB_ADDBUTTONS, 7, (LPARAM)tbb );
    pToolbar->SendMessage( TB_CHECKBUTTON, ID_VIEW_LARGEICON, MAKELONG(TRUE,0) );
    
    //initialise the image lists
    m_wndAvailableTypes.SetImageList( &m_ImageList, LVSIL_NORMAL );
    m_wndAvailableTypes.SetImageList( &m_ImageListSmall, LVSIL_SMALL );

    //add the tabs
    m_wndNewDocumentTypes.DeleteAllItems();
    TC_ITEM tci;
    tci.mask = TCIF_TEXT;
    tci.pszText = "Investigators";       m_wndNewDocumentTypes.InsertItem( 0, &tci );
    tci.pszText = "Investigator Wizards";m_wndNewDocumentTypes.InsertItem( 1, &tci );
    tci.pszText = "Handouts";            m_wndNewDocumentTypes.InsertItem( 2, &tci );
    //tci.pszText = "Monsters";            m_wndNewDocumentTypes.InsertItem( 3, &tci );
    tci.pszText = "Scripts";             m_wndNewDocumentTypes.InsertItem( 3, &tci );
    
    //add all of the .rul files
    BuildRuleFileList();

    //show the most recently used page
    m_wndNewDocumentTypes.SetCurSel( m_iPage );
    InitialisePage( m_iPage );

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewDocumentDlg::InitialisePage(int iPage)
{
    //remove all items from the list
    m_wndAvailableTypes.DeleteAllItems();

    //add all items relating to this page
    switch( iPage )
    {
        case 0: //blank sheets
        case 1: //wizards
        {
            //prepare the list view item structure
            LV_ITEM lvi;
            lvi.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
            lvi.stateMask = 0;
            lvi.iSubItem = 0;
            lvi.iItem = 0;
            lvi.iImage = iPage == 0 ? 0 : 1; //0 = normal document icon, 1 = investigator wizard

            //add all investigator items
            int iMax = m_NewInvestigatorInfoArray.GetSize();
            for( int i = 0; i < iMax; i++ )
            {
                lvi.state = 0;
                lvi.lParam = (LPARAM)i; 
                lvi.pszText = (char*)LPCSTR(m_NewInvestigatorInfoArray[i]->strName );
                m_wndAvailableTypes.InsertItem( &lvi );
            }

            //enable/disable full wizard button
            m_wndFullWizard.EnableWindow( iPage == 1 );
            break;
        }

        case 2: //handouts
            AddAllHandoutFiles();
            break;
/*
        case 3: //monsters
            m_wndFullWizard.EnableWindow(FALSE);
            break;
*/
        case 3: //scripts
        {
            //add "new script" items
            LV_ITEM lvi;
            lvi.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
            lvi.stateMask = 0;
            lvi.iSubItem = 0;
            lvi.iItem = 0;
            lvi.state = 0;

            lvi.iImage = 3; //new sheet script icon
            lvi.lParam = NEWDOC_SCRIPT_NEWSHEETSCRIPT;
            lvi.pszText = "New Investigator Sheet";
            m_wndAvailableTypes.InsertItem( &lvi );

            lvi.iImage = 5; //new sheet script icon
            lvi.lParam = NEWDOC_SCRIPT_NEWHANDOUTSCRIPT;
            lvi.pszText = "New Handout Template";
            m_wndAvailableTypes.InsertItem( &lvi );

            lvi.iImage = 2; //new script icon
            lvi.lParam = NEWDOC_SCRIPT_NEWSCRIPT;
            lvi.pszText = "New Script";
            m_wndAvailableTypes.InsertItem( &lvi );

            //prepare the list view item structure
            lvi.iImage = 4; //script icon
            lvi.lParam = NEWDOC_SCRIPT_RUN;

            //add all scripts
            CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.bys");
	        WIN32_FIND_DATA wfd;
	        HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	        if( hFind != INVALID_HANDLE_VALUE )
	        {
                do {
                    //trim off path and extension, if there is one
                    char* pszFilename = wfd.cFileName;
                    char* pszTemp = strrchr(pszFilename, '\\');
                    if( pszTemp ) pszFilename = ++pszTemp;
                    pszTemp = strrchr(pszFilename, '.' );
                    if( pszTemp ) *pszTemp = '\0';

                    //add it to the list view
                    lvi.pszText = pszFilename;
                    m_wndAvailableTypes.InsertItem( &lvi );

                    //keep searching
	            } while( FindNextFile( hFind, &wfd ) );
    	        FindClose( hFind );
            }
            m_wndFullWizard.EnableWindow(FALSE);
            break;
        }

        default:
            ASSERT(FALSE);
            break;
    }

    //select the first item
    LV_ITEM lvi;
    lvi.mask = LVIF_STATE;
    lvi.iSubItem = lvi.iItem = 0;
    lvi.stateMask = lvi.state = LVIS_SELECTED|LVIS_FOCUSED;
    m_wndAvailableTypes.SetItem( &lvi );
    m_wndAvailableTypes.SetFocus();
                
    //update the description
    UpdateDescription();
}

void CNewDocumentDlg::BuildRuleFileList()
{
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.rul");

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        do {
            //trim off path, if there is one
            char* pszFilename = wfd.cFileName;
            char* pszPathSep = strrchr(pszFilename, '\\');
            if( pszPathSep ) pszFilename = ++pszPathSep;

            //get the ERAS section from the rule file
            CSectionFile SectionFile;
            SectionFile.Open( CByakheeApp::GetDefaultDirectory() + CString(pszFilename) );
            CString strSection = SectionFile.GetSection( "ERAS" );

            Era pEras[] = { Era_1890, Era_1920, Era_1990 };
            if( strSection.IsEmpty() == FALSE )
            {
                //scan it line by line
                char* pszRead = strSection.LockBuffer();

                //skip over leading blank lines
                while( *pszRead == '\r' || *pszRead == '\n' ) pszRead++;
                char* pszBuffer = pszRead;

                for( int i = 0; i < 3; i++ )
                {
                    //find the end of this line and terminate the string at that point
                    char chRead = '\0';
                    while( *pszRead != '\r' && *pszRead != '\n' && *pszRead != '\0' ) chRead = *(++pszRead);
                    *pszRead = '\0';

                    if( chRead != '\0' )
                    {
                        //add the item
                        if( *pszBuffer != '-' )
                        {
                            NewInvestigatorInfo* pNewInvestigatorInfo = new NewInvestigatorInfo;
                            pNewInvestigatorInfo->strRuleFile = pszFilename;
                            pNewInvestigatorInfo->strName = pszBuffer;
                            pNewInvestigatorInfo->era = pEras[i];
                            m_NewInvestigatorInfoArray.Add( pNewInvestigatorInfo );
                        }

                        //unterminate the string
                        *pszRead = chRead;

                        //skip over blank lines
                        while( *pszRead == '\r' || *pszRead == '\n' ) pszRead++;
                    }

                    //move to next line
                    pszBuffer = pszRead;
                }
                strSection.UnlockBuffer();
            }
            else //old .rul file - add sections based on file title
            {
                //get the file title
                char szTitle[MAX_PATH+1];
                strcpy( szTitle, pszFilename );
                char* pszTmpPtr = strrchr(szTitle,'.');
                if(pszTmpPtr) *pszTmpPtr = '\0';

                //add each
                char* pszEraStrings[] = { " (1890s)", " (1920s)", " (1990s)" };
                for( int i = 0; i < 3; i++ )
                {
                    NewInvestigatorInfo* pNewInvestigatorInfo = new NewInvestigatorInfo;
                    pNewInvestigatorInfo->strRuleFile = pszFilename;
                    pNewInvestigatorInfo->strName = CString(szTitle) + CString(pszEraStrings[i]);
                    pNewInvestigatorInfo->era = pEras[i];
                    m_NewInvestigatorInfoArray.Add( pNewInvestigatorInfo );
                }
            }
	    } while( FindNextFile( hFind, &wfd ) );
    	FindClose( hFind );
    }

	if( m_NewInvestigatorInfoArray.GetSize() == 0 )
	{
		AfxMessageBox( "There are no investigator rule files in the Byakhee directory!\nByakhee cannot continue unless it has at least one .rul file.", MB_ICONEXCLAMATION );
	}
}


void CNewDocumentDlg::AddAllHandoutFiles()
{
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.bhs");

    LV_ITEM lvi;
    char szText[MAX_PATH+1];
    lvi.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM|LVIF_STATE;
    lvi.stateMask = LVIS_FOCUSED;
    lvi.iSubItem = 0;
    lvi.iItem = 0;
    lvi.iImage = 6;
    lvi.state = LVIS_FOCUSED;
    lvi.pszText = szText;
    lvi.cchTextMax = MAX_PATH;

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        do {
            //trim off path and extension, if there is one
            char* pszFilename = wfd.cFileName;
            char* pszTemp = strrchr(pszFilename, '\\');
            if( pszTemp ) pszFilename = ++pszTemp;
            pszTemp = strrchr(pszFilename, '.' );
            if( pszTemp ) *pszTemp = '\0';

            //add it
            strcpy( szText, pszFilename );
            m_wndAvailableTypes.InsertItem( &lvi );
            lvi.state = 0;
	    } while( FindNextFile( hFind, &wfd ) );
    	FindClose( hFind );
    }
}

void CNewDocumentDlg::OnViewLargeIcons()
{
    m_wndAvailableTypes.ModifyStyle( LVS_SMALLICON|LVS_LIST, LVS_ICON );
}

void CNewDocumentDlg::OnViewSmallIcons()
{
    m_wndAvailableTypes.ModifyStyle( LVS_ICON|LVS_LIST, LVS_SMALLICON );
}

void CNewDocumentDlg::OnViewList()
{
    m_wndAvailableTypes.ModifyStyle( LVS_ICON|LVS_SMALLICON, LVS_LIST );
}

void CNewDocumentDlg::OnClickAvailabletypes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CNewDocumentDlg::OnItemchangedAvailabletypes(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateDescription();
    *pResult = 0;
}

void CNewDocumentDlg::UpdateDescription()
{
	UpdateData(UD_GETDATA);

    //set the item type we're looking for
    LV_ITEM lvi;
    char szCaption[MAX_PATH+1];
    lvi.pszText = szCaption;
    lvi.cchTextMax = MAX_PATH;
    lvi.mask = LVIF_PARAM|LVIF_STATE|LVIF_TEXT;
    lvi.iSubItem = 0;
    lvi.stateMask = LVIS_SELECTED|LVIS_FOCUSED;

    //scan through all available types and find the index of the selected item
    int nMax = m_wndAvailableTypes.GetItemCount();
    int iSelected = -1;
    for( int i = 0; i < nMax; i++ )
    {
        lvi.iItem = i;
        if( m_wndAvailableTypes.GetItem( &lvi ) && lvi.state & (LVIS_SELECTED|LVIS_FOCUSED) ) 
        {
            iSelected = i;
            break;
        }
    }

    m_strInformation = "";
    CString strStatusBarText = "";

    //if we found one get it's description
    if( iSelected != -1 )
    {
        //get the type index and the new name
        int iNewDocType = m_wndNewDocumentTypes.GetCurSel();
        switch( iNewDocType )
        {
            case 0: //blank investigator
            case 1: //investigator wizard
            {
                int i = (int)lvi.lParam;

                //get the CREDIT section from the rule file
                CSectionFile SectionFile;
                SectionFile.Open( CByakheeApp::GetDefaultDirectory() + m_NewInvestigatorInfoArray[i]->strRuleFile );
                CString strCredit = SectionFile.GetSection( "CREDIT" );
                CString strKeyPair = SectionFile.GetSection( "KEYVALUEPAIRS" );

                //build the desctiption strings
                strStatusBarText = CString(szCaption) + CString(" ") + SectionFile.GetKeyPairValue( strKeyPair, "InvestigatorName", "Investigator" ) + CString(" ") + SectionFile.GetKeyPairValue( strKeyPair, "ShortInfo", "" );
                if( strCredit.IsEmpty() == TRUE ) strCredit = "Byakhee v1.0 or v2.0 rule file\nNo further information available";
                m_strInformation = strStatusBarText + CString("\r\n\r\n") + strCredit;
                break;
             }

            case 2: //handouts
                strStatusBarText = szCaption;
                m_strInformation = CString("This will create a ") + CString(szCaption) + CString(" handout.");
                break;
/*
            case 3: //monsters
                break;
*/
            case 3: //scripts
                switch( lvi.lParam )
                {
                    case NEWDOC_SCRIPT_NEWSHEETSCRIPT:
                        strStatusBarText = "Creates a new investigator sheet script";
                        m_strInformation = strStatusBarText + CString(".\r\n\r\nThese scripts are used do define the layout of an investigator sheet. Load or create a new investigator and select the file name of the sheet script from the toolbar to display it.");
                        break;

                    case NEWDOC_SCRIPT_NEWSCRIPT:
                        strStatusBarText = "Creates a new Byakhee script";
                        m_strInformation = strStatusBarText + CString(".\r\n\r\nThis is a simple Byakhee script. Once it has been createed, it appears in this list.");
                        break;

                    case NEWDOC_SCRIPT_NEWHANDOUTSCRIPT:
                        strStatusBarText = "Creates a new Handout template";
                        m_strInformation = strStatusBarText + CString(".\r\n\r\nThese scripts are used to define the layout of a handout. Load or create a handout and select the file name of the sheet script from the toolbar to display it.");
                        break;

                    case NEWDOC_SCRIPT_RUN:
                        strStatusBarText = "Run this script";
                        m_strInformation = "Scripts can be used to automate various functions of Byakhee\nEach is a separate .bys file in the Byakhee folder";
                        break;
                }
                break;
        }
    }

    //update dialog
    UpdateData(UD_SETDATA);

    //update status bar
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( strStatusBarText );   
}

BOOL CNewDocumentDlg::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	return pMainWnd->OnToolTipText(id, pNMHDR, pResult);
}

void CNewDocumentDlg::OnFullWizard() 
{
    //store full wizard flag
    UpdateData(UD_GETDATA);
    AfxGetApp()->WriteProfileInt( "Wizard", "FullWiz", m_fFullWizard );
}


void CNewDocumentDlg::OnViewInformation() 
{
    AfxMessageBox( m_strInformation, MB_ICONINFORMATION );
}


CDocTemplate* CNewDocumentDlg::GetDocTemplate(CString strDocName)
{
	POSITION p;
	p = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate;
    while( p )
    {
        if( pTemplate = (CDocTemplate*)AfxGetApp()->GetNextDocTemplate( p ) )
        {
		    CString str;
		    if( pTemplate->GetDocString( str, CDocTemplate::docName ) ) if( str.CompareNoCase( strDocName ) == 0 ) return pTemplate;
        }
    }

	return NULL;
}

BOOL CNewDocumentDlg::CreateNewInvestigator(NewInvestigatorInfo * pNewInvestigatorInfo, BOOL fUseWizard)
{
    //make sure the data is valid
    if( pNewInvestigatorInfo == NULL ) return FALSE;

    //get the document template for the investigator document
    CDocTemplate* pDocTemplate = GetDocTemplate( "Investigator" );
    if( pDocTemplate == NULL ) return FALSE;

    //create the new document
    CInvestigatorDoc* pInvestigatorDoc = (CInvestigatorDoc*)pDocTemplate->CreateNewDocument();
    if( pInvestigatorDoc == NULL ) return FALSE;
    pInvestigatorDoc->SetRuleFile( pNewInvestigatorInfo->strRuleFile );
    pInvestigatorDoc->m_Investigator.m_Era = pNewInvestigatorInfo->era;
    pInvestigatorDoc->OnNewDocument();

    //initialise the investigator
    if( fUseWizard )
    {
        CWizard_NewInvestigator NewInvestigatorWizard( &pInvestigatorDoc->m_Investigator );
        if( NewInvestigatorWizard.DoModal() != ID_WIZFINISH )
        {
            delete pInvestigatorDoc;
            return TRUE;
        }
        else
        {
            //load in the portrait
            pInvestigatorDoc->ReloadPortrait();
        }
    }

    //create a frame window for this investigator
	CFrameWnd* pFrameWnd = pDocTemplate->CreateNewFrame( pInvestigatorDoc, NULL );
    if( pFrameWnd == NULL ) { delete pInvestigatorDoc; return FALSE; }

    //update the frame and set it's title
    pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	pDocTemplate->InitialUpdateFrame( pFrameWnd, pInvestigatorDoc, TRUE );
	if( fUseWizard ) pInvestigatorDoc->SetModifiedFlag();
    pInvestigatorDoc->SetTitle( "New Investigator" );

    return TRUE;
}

BOOL CNewDocumentDlg::CreateNewScriptDocument(CString strTemplateName, const char *pszDefaultText, CString strTitle)
{
    //get the document template for the investigator document
    CDocTemplate* pDocTemplate = GetDocTemplate( strTemplateName );
    if( pDocTemplate == NULL ) return FALSE;

    //create the new document
    CScriptEditDoc* pScriptEditDoc = (CScriptEditDoc*)pDocTemplate->CreateNewDocument();
    if( pScriptEditDoc == NULL ) return FALSE;
    pScriptEditDoc->OnNewDocument();

    //create a frame window for this document
	CFrameWnd* pFrameWnd = pDocTemplate->CreateNewFrame( pScriptEditDoc, NULL );
    if( pFrameWnd == NULL ) { delete pScriptEditDoc; return FALSE; }

    //update the frame and set it's title
    pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	pDocTemplate->InitialUpdateFrame( pFrameWnd, pScriptEditDoc, TRUE );
    pScriptEditDoc->SetTitle( strTitle );

    //set default document contents
    POSITION p = pScriptEditDoc->GetFirstViewPosition();
    CView* pView = pScriptEditDoc->GetNextView(p);
    if( pView ) pView->SetWindowText(pszDefaultText);

    return TRUE;
}

BOOL CNewDocumentDlg::CreateNewHandout(CString strHandoutName)
{
    //get the document template for the investigator document
    CDocTemplate* pDocTemplate = GetDocTemplate( "Handout" );
    if( pDocTemplate == NULL ) return FALSE;

    //create the new document
    CHandoutDoc* pHandoutDoc = (CHandoutDoc*)pDocTemplate->CreateNewDocument();
    if( pHandoutDoc == NULL ) return FALSE;
    pHandoutDoc->OnNewDocument();

    //set default document contents
    pHandoutDoc->m_strSheet = strHandoutName;

    //create a frame window for this document
	CFrameWnd* pFrameWnd = pDocTemplate->CreateNewFrame( pHandoutDoc, NULL );
    if( pFrameWnd == NULL ) { delete pHandoutDoc; return FALSE; }

    //update the frame and set it's title
    pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
	pDocTemplate->InitialUpdateFrame( pFrameWnd, pHandoutDoc, TRUE );
    pHandoutDoc->SetTitle( CString("New ") + strHandoutName );
    return TRUE;
}

void CNewDocumentDlg::OnFileOpen() 
{
    //close the dialog
    CDialog::OnOK();
    
    //display the file open dialog
    ((CByakheeApp*)AfxGetApp())->OnFileOpen();
}

void CNewDocumentDlg::OnToolbarDropDown( NMHDR* lpnmt, LRESULT* pResult )
{
    CToolBarCtrl* pToolbar = (CToolBarCtrl*)GetDlgItem(IDC_TOOLBAR);
	NMTOOLBAR* pTemp = (NMTOOLBAR*) lpnmt;

    //get the coordinates of the button
    RECT rc;
    pToolbar->SendMessage( TB_GETRECT, pTemp->iItem, (LPARAM)&rc );
    pToolbar->MapWindowPoints( NULL, (LPPOINT)&rc, 2 );
    
    //create a pop-up menu
    CMenu PopupMenu;
    PopupMenu.CreatePopupMenu();

    //load the recent file list in
    CRecentFileList RecentFileList(0, "Recent File List", "File%d", 8 );
    RecentFileList.ReadList();

    //get current directory
    char szDirectory[MAX_PATH+1];
    GetCurrentDirectory( MAX_PATH, szDirectory );

    //add all recent file list items to the popup menu
    int iMax = RecentFileList.GetSize();
    for( int i = 0; i < iMax; i++ ) 
    {
        CString strItem = RecentFileList[i];
        if(strItem.IsEmpty() == FALSE )
        {
            RecentFileList.GetDisplayName(strItem, i, szDirectory, MAX_PATH, TRUE );
            PopupMenu.AppendMenu( MF_STRING, ID_FILE_MRU_FIRST + i, strItem );
        }
    }

    //display the menu
    PopupMenu.TrackPopupMenu( TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL, rc.left, rc.bottom, this, &rc );

    //destroy the menu
    PopupMenu.DestroyMenu();
}

void CNewDocumentDlg::OnFileMRU( UINT uID )
{
    //close the dialog
    CDialog::OnOK();

    //send the request on to the main window
    ((CByakheeApp*)AfxGetApp())->OnOpenRecentFile( uID );
}

