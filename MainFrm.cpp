// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <afxadv.h>
#include "Byakhee.h"

#include "MainFrm.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

#define SHEETSTYLE_WIDTH 240

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    ON_COMMAND(IDS_TIPOFTHEDAY, OnTipOfTheDay)
	ON_COMMAND_EX(ID_VIEW_OUTPUT, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateControlBarMenu)
    ON_WM_INITMENU()
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_ACTIVATE_SHEETSTYLE, OnViewSheetStyle)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_HIDEOUTPUT, OnViewHideOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_POINTS,
    ID_INDICATOR_LINENUMBER,
    ID_INDICATOR_NUM,
	ID_INDICATOR_CAPS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//create base
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

	//create child controls
	if (!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) return -1;
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))) return -1;
	m_wndStatusBar.SetPaneText( m_wndStatusBar.CommandToIndex(ID_INDICATOR_LINENUMBER), "" );
	m_wndStatusBar.SetPaneText( m_wndStatusBar.CommandToIndex(ID_INDICATOR_POINTS), "" );

	//turn on tooltips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS );

	//find the button to replace with the combo box
    int iPlaceHolder = 0;
    while( m_wndToolBar.GetItemID(iPlaceHolder) != ID_VIEW_SHEETSTYLE ) iPlaceHolder++;

    //change it to a separator and change it's width
    m_wndToolBar.SetButtonInfo( iPlaceHolder, ID_VIEW_SHEETSTYLE, TBBS_SEPARATOR, SHEETSTYLE_WIDTH );

    //get it's position on the toolbar and expand it so there is room for it to drop down
    RECT rc;
    m_wndToolBar.GetItemRect( iPlaceHolder, &rc );
    rc.bottom += 200;

    //create, change it's font and show it
    if(!m_wndSheetType.Create( WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, rc, &m_wndToolBar, ID_VIEW_SHEETSTYLE)) return -1;
    m_wndSheetType.SendMessage( WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT) );
    m_wndSheetType.ShowWindow(SW_SHOW);
    m_wndSheetType.EnableWindow(FALSE);

	//make toobar flat if requested by user
    if( AfxGetApp()->GetProfileInt( "Display", "FlatToolbar", TRUE ) == TRUE )
        m_wndToolBar.GetToolBarCtrl().ModifyStyle( 0, TBSTYLE_FLAT ); //make it flat

	//Initialize dialog bar m_wndOutput
	if (!m_wndOutput.Create(this, CSize(250,75), ID_VIEW_OUTPUT, CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create output window\n");
		return -1;		// fail to create
	}

    m_wndOutput.SetWindowText( "Output" );
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar(&m_wndOutput, AFX_IDW_DOCKBAR_BOTTOM);
	ShowControlBar(&m_wndOutput, FALSE, FALSE);

	//Load in the docked toolbar positions
	CDockState dockState;
	CString szToolbarPositionsKey;
	szToolbarPositionsKey.LoadString( IDS_TOOLBARKEY );
	dockState.LoadState( szToolbarPositionsKey );
	SetDockState( dockState );
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnViewSheetStyle() 
{
	m_wndSheetType.SetFocus();
    m_wndSheetType.ShowDropDown();
}

void CMainFrame::SetStatusMessage(CString strMessage)
{
    m_wndStatusBar.SetPaneText( 0, strMessage, TRUE );
    m_wndStatusBar.UpdateWindow();
    
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	CMDIFrameWnd::RecalcLayout(bNotify);
	CMDIFrameWnd::RecalcLayout(bNotify); //second one needed to accomodate CSizingControlBar
}

void CMainFrame::OnClose() 
{
	//store the toolbar positions
	CDockState dockState;
	CString szToolbarPositionsKey;
	szToolbarPositionsKey.LoadString( IDS_TOOLBARKEY );
	GetDockState( dockState );
	dockState.SaveState( szToolbarPositionsKey );
	
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnViewHideOutput() 
{
    ShowControlBar(&m_wndOutput, FALSE, FALSE);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
    if( CMDIFrameWnd::OnCreateClient(lpcs, pContext) )
    {
        m_wndMDIClientArea.SubclassWindow(m_hWndMDIClient);
        return TRUE;
    }
    else
	    return FALSE;
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
    CMDIFrameWnd::OnInitMenu(pMenu);

    //find the help submenu
    CMenu* pSubMenu = NULL;
    CString strHelp; strHelp.LoadString(IDS_TIPOFTHEDAYHELP);
    CString strMenu;
    int nMenuCount = pMenu->GetMenuItemCount();
    BOOL bFound = FALSE;
    for (int i=0; i < nMenuCount; i++) 
    {
	    pMenu->GetMenuString(i, strMenu, MF_BYPOSITION);
	    if (strMenu == strHelp)
	    { 
		    pSubMenu = pMenu->GetSubMenu(i);
		    bFound = TRUE;
		    ASSERT(pSubMenu != NULL);
	    }
    }

    CString strTipMenu;
    strTipMenu.LoadString(IDS_TIPOFTHEDAYMENU);
    if(bFound)
    {      
	    //check to see if the menu has already been added.
	    pSubMenu->GetMenuString(0, strMenu, MF_BYPOSITION);

        //only add it if the tips were loaded OK
	    if( strMenu != strTipMenu && m_wndMDIClientArea.m_fTipsLoadedOK )
	    {
		    pSubMenu->InsertMenu(0, MF_BYPOSITION);
		    pSubMenu->InsertMenu(0, MF_STRING|MF_BYPOSITION, IDS_TIPOFTHEDAY, strTipMenu);
	    }
    }
}

void CMainFrame::OnTipOfTheDay()
{
    m_wndMDIClientArea.ShowNextTip();
}
