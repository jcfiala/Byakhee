// Byakhee.cpp : Defines the class behaviors for the application.

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

#include "stdafx.h"
#include "Byakhee.h"
#include <shlobj.h> //for SHGetSpecialFolderLocation and SHGetPathFromIDList

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MultiFileMultiDocTemplate.h"
#include "InvestigatorDoc.h"
#include "InvestigatorView.h"
#include "ScriptEditDoc.h"
#include "ScriptEditView.h"
#include "HandoutDoc.h"
#include "HandoutView.h"
#include "Splash.h"
#include "SkillManager.h"

#include "AboutDlg.h"
#include "OptionsDlg.h"
#include "Wizard_NewInvestigator.h"
#include "NewDocumentDlg.h"
#include "QuickInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CByakheeApp

BEGIN_MESSAGE_MAP(CByakheeApp, CWinApp)
	//{{AFX_MSG_MAP(CByakheeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CByakheeApp construction

CByakheeApp::CByakheeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CByakheeApp object

CByakheeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CByakheeApp initialization

BOOL CByakheeApp::InitInstance()
{
    //display the splash screen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

    //initialise the random number generator
    srand( GetTickCount() );

    //set the program's registry key
	SetRegistryKey(_T("DHSoft"));

    //load the standard profile
	LoadStdProfileSettings(8);

    //register document templates
    //add main byakhee file types
	AddDocTemplate( new CMultiFileMultiDocTemplate( IDR_SHEETTYPE, RUNTIME_CLASS(CInvestigatorDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CInvestigatorView) ) );
	AddDocTemplate( new CMultiFileMultiDocTemplate( IDR_HANDOUTTYPE, RUNTIME_CLASS(CHandoutDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CHandoutView) ) );
	RegisterShellFileTypes(TRUE);

    //add non-print non-shellnew etc file types (scripts etc)
	AddDocTemplate( new CMultiFileMultiDocTemplate( IDR_SCRIPTEDITTYPE_SHEET, RUNTIME_CLASS(CScriptEditDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CScriptEditView), IDR_SCRIPTEDITTYPE ) );
	AddDocTemplate( new CMultiFileMultiDocTemplate( IDR_SCRIPTEDITTYPE_HANDOUT, RUNTIME_CLASS(CScriptEditDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CScriptEditView), IDR_SCRIPTEDITTYPE ) );
	AddDocTemplate( new CMultiFileMultiDocTemplate( IDR_SCRIPTEDITTYPE, RUNTIME_CLASS(CScriptEditDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CScriptEditView) ) );
	RegisterShellFileTypes(FALSE);

	//create the MDI window
	CMainFrame* pMainFrame = new CMainFrame;
	if( !pMainFrame->LoadFrame(IDR_MAINFRAME) ) return FALSE;
	m_pMainWnd = pMainFrame;

    //show tip of the day if required
    if( cmdInfo.m_bShowSplash ) pMainFrame->m_wndMDIClientArea.ShowTip();

	//enable drag & drop file opening
	m_pMainWnd->DragAcceptFiles();

	//enable DDE file opening
	EnableShellOpen();

	//dispatch commands specified on the command line
    BOOL fCreateNew = FALSE;
    if( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew ) 
    {
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
        fCreateNew = TRUE;
    }
	if( !ProcessShellCommand(cmdInfo) ) return FALSE;

	//the main window has been initialised, so show and update it.
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();

    //create a new document now
    if( fCreateNew ) OnFileNew();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CByakheeApp commands

BOOL CByakheeApp::PreTranslateMessage(MSG* pMsg)
{
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}


void CByakheeApp::OnFileNew() 
{
    //display the new document dialog
    CNewDocumentDlg NewDocumentDlg;
    NewDocumentDlg.DoModal();
}

void CByakheeApp::OnToolsOptions() 
{
	COptionsDlg OptionsDlg;
	if( OptionsDlg.DoModal() == IDOK )
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
        if( pMainFrame )
        {
            //apply all settings
            pMainFrame->GetOutputWindow().LoadDisplaySettings();

            //walk all documents and send them an update message
		    POSITION p;
		    p = GetFirstDocTemplatePosition();
		    CMultiDocTemplate* pTemplate = NULL;
		    while( p )
            {
			    pTemplate = (CMultiDocTemplate*)GetNextDocTemplate( p );
			    if( pTemplate  )
                {
					POSITION pp = pTemplate->GetFirstDocPosition();
					while( pp )
                    {
						CDocument* pDoc = (CDocument*)pTemplate->GetNextDoc( pp );
                        if( pDoc )
                        {
                            POSITION ppp = pDoc->GetFirstViewPosition();
                            while( ppp )
                            {
                                CView* pView = pDoc->GetNextView(ppp);
                                if( pView ) pView->SendMessage( MY_APPSETTINGSCHANGE );
                            }
                        }
                    }
                }
            }
        }
	}   
}

BOOL CByakheeApp::BrowseForPicture(CString & strFilename, CWnd* pWndOwner /*NULL*/)
{
    if( pWndOwner == NULL ) pWndOwner = AfxGetMainWnd();

    //build a file open dialog
    char* szFilter = "Image Files|*.bmp;*.gif;*.jpg;*.jpeg;*.png|Bitmap Files (*.BMP)|*.bmp|CompuServe GIF Files (*.GIF)|*.gif|JPEG Files (*.JPG)|*.jpg;*.jpeg|Portable Network Graphics Files (*.PNG)|*.png|All Files (*.*)|*.*||";
    CFileDialog FileDialog( TRUE, ".bmp", strFilename, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST, szFilter, pWndOwner );
    
    //set default folder to current directory
    char szDocumentsFolder[MAX_PATH+1];
    GetCurrentDirectory( MAX_PATH, szDocumentsFolder );

    //get the path to their documents folder
    LPITEMIDLIST lpItemIDList;
    if( SHGetSpecialFolderLocation( pWndOwner->GetSafeHwnd(), CSIDL_PERSONAL, &lpItemIDList ) == NOERROR ) SHGetPathFromIDList( lpItemIDList, szDocumentsFolder );
    
    //make the file dialog start in their documents folder
    FileDialog.m_ofn.lpstrInitialDir = szDocumentsFolder;

    //show the file dialog
    if( FileDialog.DoModal() == IDOK )
    {
        strFilename = FileDialog.GetPathName();
        return TRUE;
    }
    else
        return FALSE;
}

CString CByakheeApp::GetDefaultDirectory()
{
    //get application's name
    char szBuffer[MAX_PATH+1];
    GetModuleFileName( AfxGetInstanceHandle(), szBuffer, MAX_PATH );

    //find the last path separator
    char* pszEndOfPath = strrchr( szBuffer,'\\' );
    if( pszEndOfPath )
    {
        //terminate the string just after it
        pszEndOfPath++;
        *pszEndOfPath = '\0';
        return CString( szBuffer );
    }
    else
        //there was no path!
        return "";
}

BOOL CByakheeApp::FileExists(CString strFile)
{
    //build full path
    CString strFileName = GetDefaultDirectory() + strFile;

    //try and open it
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strFileName, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        //close the search
    	FindClose( hFind );
        return TRUE;
    }
    else
        return FALSE;
}

void CByakheeApp::ShowScriptExecutionQuickInfo()
{
    CQuickInfoDlg QuickInfo;
    QuickInfo.Show( "RunScript", "Byakhee is about to run an external script file.\nYou can press Ctrl+Break to stop the script at any point." );
}

void CByakheeApp::OnAppAbout()
{
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}
