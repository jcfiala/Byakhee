// ScriptSheetView.cpp : implementation file
//
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
#include "MainFrm.h"
#include "Output.h"
#include "ScriptSheetView.h"
#include "FieldEdit.h"
#include "ScriptEngine/ScriptingEngine.h"
#include "ColourConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView

IMPLEMENT_DYNCREATE(CScriptSheetView, CScrollView)

CScriptSheetView::CScriptSheetView()
{
    //prevent deletion by the scripting language
    AddRef();
    m_Fields.AddRef();

    //set inital sheet size
    m_SheetSize = CSize(10000,10000);
    SetScrollSizes( MM_HIMETRIC, m_SheetSize );

    //set active field index
    m_nFieldID = -1;

    //init all arrays
    ClearSheetCommands();
    m_Fields.m_FieldList.SetSize(0, 10);

    //set other member variables
    m_fLeftButtonClick = FALSE;
    m_pbmSheet = NULL;
    m_iLastEditedFieldSheetCommand = -1;

    //add sheet parameters
    AddSheetParameter( "checkboxshadowoffset", SHEETPARAM_SHADOWOFFSET );
}

CScriptSheetView::~CScriptSheetView()
{
    //wipe the sheet command list
    ClearSheetCommands();

    //wipe the main field list
    int i;
    for( i = 0; i < m_Fields.m_FieldList.GetSize(); i++ ) delete m_Fields.m_FieldList[i];

    //remove sheet parameters
    for( i = 0; i < m_SheetParameters.GetSize(); i++ ) delete m_SheetParameters[i];

    //destroy off screen buffer
    if( m_pbmSheet ) delete m_pbmSheet;
}


BEGIN_MESSAGE_MAP(CScriptSheetView, CScrollView)
	//{{AFX_MSG_MAP(CScriptSheetView)
	ON_COMMAND(ID_VIEW_REFRESH_SHEET, OnViewRefreshSheet)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_EDIT_NEXTFIELD, OnEditNextField)
	ON_COMMAND(ID_EDIT_PREVFIELD, OnEditPrevField)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_VIEW_SHEETSTYLE, OnSheetStyleChange)
    ON_MESSAGE(MY_APPSETTINGSCHANGE, OnAppSettingsChange)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScriptSheetView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScriptSheetView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScriptSheetView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView drawing

void CScriptSheetView::OnDraw(CDC* pDC)
{
    if( pDC->IsPrinting() == FALSE )
    {
        //create a device to draw the background into
        CDC DCSheet;
        DCSheet.CreateCompatibleDC(pDC);
        DCSheet.SetMapMode(MM_HIMETRIC);
        CBitmap* pOldBitmap = NULL;

        //see if we need to draw the sheet again
        if( m_pbmSheet == NULL )
        {
            CWaitCursor Wait;

            //create a bitmap to store the background
            CSize dsz( m_SheetSize.cx, -m_SheetSize.cy );
            DCSheet.LPtoDP( &dsz );

            m_pbmSheet = new CBitmap();
            m_pbmSheet->CreateCompatibleBitmap( pDC, dsz.cx, -dsz.cy );
            pOldBitmap = DCSheet.SelectObject( m_pbmSheet );

            //prepare sheet drawing
            CSheetDrawInfo DrawInfo( this, &DCSheet, m_SheetSize );
            DrawInfo.m_nShadowOffset = GetSheetParameter(SHEETPARAM_SHADOWOFFSET, 0);

            //execute all drawing commands
            int iMax = m_Fields.m_SheetCommands.GetSize();
            for( int i = 0; i < iMax; i++ ) ExecuteSheetCommand( m_Fields.m_SheetCommands[i], &DrawInfo );
        }
        else
            pOldBitmap = DCSheet.SelectObject( m_pbmSheet );

        //copy the background to the display
        pDC->BitBlt( 0, 0, m_SheetSize.cx, -m_SheetSize.cy, &DCSheet, 0, 0, SRCCOPY );

        //remove the bitmap from the device
        DCSheet.SelectObject( pOldBitmap );
    }
    else
    {
        //draw directly to device when it's printing

        //prepare sheet drawing
        CSheetDrawInfo DrawInfo( this, pDC, m_SheetSize );
        DrawInfo.m_nShadowOffset = GetSheetParameter(SHEETPARAM_SHADOWOFFSET, 0);

        //execute all drawing commands
        int iMax = m_Fields.m_SheetCommands.GetSize();
        for( int i = 0; i < iMax; i++ ) ExecuteSheetCommand( m_Fields.m_SheetCommands[i], &DrawInfo );
    }
}

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView diagnostics

#ifdef _DEBUG
void CScriptSheetView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScriptSheetView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView message handlers

BOOL CScriptSheetView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CScriptSheetView::OnActivateView(BOOL bActivate, CScrollView* pActivateView, CScrollView* pDeactiveView) 
{
    if( bActivate )
    {
	    CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
        AddAllSheetTypes();
    }
    else
	    CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CScriptSheetView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
    AddAllSheetTypes();
}

void CScriptSheetView::OnSheetStyleChange()
{
    CString strActive;

    //get the sheet type combo box
    CComboBox* pSheetTypeCombo = &((CMainFrame*)AfxGetMainWnd())->m_wndSheetType;
    int iActive = pSheetTypeCombo->GetCurSel();
    if( iActive != CB_ERR ) pSheetTypeCombo->GetLBText( iActive, strActive );

    if( strActive.IsEmpty() == FALSE && strActive.Compare(m_strSheet) != 0 ) 
    {
        //start tab-stopping at the beginning of the sheet again
        m_iLastEditedFieldSheetCommand = -1;

        //change the sheet
        SetSheet( strActive );
    }
}

void CScriptSheetView::OnInitialUpdate() 
{
    //initialise the editing controls
    m_wndFieldEdit.SetOwner( this );

    //initialise the fields
    AddAllFields();

    //initialise the sheet manager
    m_SheetSize = CSize(10000,10000);
    SetSheet(m_strSheet);

    //initialise the combo box
    AddAllSheetTypes();

    //now do OnUpdate()...
	CScrollView::OnInitialUpdate();
}

void CScriptSheetView::OnViewRefreshSheet() 
{
    //initialise the combo box
    AddAllSheetTypes();

    //reload the sheet and reuse it
    m_SheetSize = CSize(10000,10000);
    ReloadSheet();
    RedrawWindow();
}

BOOL CScriptSheetView::OnEraseBkgnd(CDC* pDC) 
{
    CBrush br( GetSysColor(COLOR_APPWORKSPACE) );
    FillOutsideRect( pDC, &br );
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CScriptSheetView member functions

void CScriptSheetView::SetSheetFileExtension(CString strFileExtension)
{
    ASSERT( strFileExtension.IsEmpty() == FALSE );
    m_strSheetFileExtension = strFileExtension;
}

BOOL CScriptSheetView::SetSheet(CString strSheetFileName /*""*/ )
{
    ASSERT( m_strSheetFileExtension.IsEmpty() == FALSE );

    //don't load a second time if the name hasn't changed, unless it's still empty
    if( strSheetFileName.IsEmpty() )
        strSheetFileName = GetFirstSheetFile();
    else
    {
        //see if it exists
        CString strFileName = m_strSheet + CString(".") + m_strSheetFileExtension;
        if( CByakheeApp::FileExists(strFileName) == FALSE ) strSheetFileName = GetFirstSheetFile();
    }

    //store it
    m_strSheet = strSheetFileName;

    //tell anyone who wants to know
    SheetChanged();

    //reload the sheet
    return ReloadSheet();
}

void CScriptSheetView::AddAllSheetTypes()
{
    //make sure there's a file extension and a combo box
    CComboBox* pComboBox = &((CMainFrame*)AfxGetMainWnd())->m_wndSheetType;
    if( pComboBox == NULL || m_strSheetFileExtension.IsEmpty() ) return;

    //empty the combo box and prepare search parameters
    pComboBox->ResetContent();
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.") + m_strSheetFileExtension;

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        pComboBox->EnableWindow(TRUE);
        do {
            //trim off path and extension
            char* pszFilename = wfd.cFileName;
            char* pszTemp = strrchr(pszFilename, '\\');
            if( pszTemp ) pszFilename = ++pszTemp;
            pszTemp = strrchr( pszFilename, '.' );
            if( pszTemp ) *pszTemp = '\0';

            //add it
            pComboBox->AddString(pszFilename);

	    } while( FindNextFile( hFind, &wfd ) );
    	FindClose( hFind );
    }
    else
    {
        //disable the combo box
        pComboBox->EnableWindow(FALSE);
    }

    //select the current string in the list box
    int iPos = pComboBox->FindStringExact(-1, m_strSheet);
    if( iPos != CB_ERR ) pComboBox->SetCurSel(iPos);
}

CString CScriptSheetView::GetFirstSheetFile()
{
    if( m_strSheetFileExtension.IsEmpty() ) return "";
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.") + m_strSheetFileExtension;
    CString strResult = "";

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        //trim off path and extension
        char* pszFilename = wfd.cFileName;
        char* pszTemp = strrchr(pszFilename, '\\');
        if( pszTemp ) pszFilename = ++pszTemp;
        pszTemp = strrchr( pszFilename, '.' );
        if( pszTemp ) *pszTemp = '\0';

        //store it
        strResult = pszFilename;

        //stop searching
    	FindClose( hFind );
    }

    return strResult;
}

void CScriptSheetView::AddDummySheetCommands( const char* pszMessage /*NULL*/ )
{
    if( pszMessage == NULL ) pszMessage = "An error occurred while trying to process the sheet script";
    
    CRect rc( ScaleToSheet(3), -ScaleToSheet(2), ScaleToSheet(100.0), -ScaleToSheet(25.0) );
    m_SheetSize = CSize( rc.right, -rc.bottom );

    m_Fields.m_SheetCommands.Add( new CDrawSheetCommand( CommandType_SetTextColour, RGB(0,0,0) ) );
    m_Fields.m_SheetCommands.Add( new CFontSheetCommand( CommandType_SetFont, "Arial", 160, FONTSTYLE_ITALIC|FONTSTYLE_BOLD ) );
    m_Fields.m_SheetCommands.Add( new CDrawSheetCommand( CommandType_DrawTextBox, rc, "Sheet Script Error" ) );

    rc.top = ScaleToSheet(-10.0);
    m_Fields.m_SheetCommands.Add( new CFontSheetCommand( CommandType_SetFont, "MS Sans Serif", 80, FONTSTYLE_NORMAL ) );
    m_Fields.m_SheetCommands.Add( new CDrawSheetCommand( CommandType_DrawTextBox, rc, pszMessage ) );
}


BOOL CScriptSheetView::ReloadSheet()
{
    //stop editing
    m_wndFieldEdit.EndEditing();

    m_SheetSize = CSize(10000,10000);
    if( m_pbmSheet ) delete m_pbmSheet;
    m_pbmSheet = NULL;

    //reload the sheet
    BOOL fResult = FALSE;
    if( m_strSheet.IsEmpty() == FALSE )
    {
        //build the file name
        CString strFileName = CByakheeApp::GetDefaultDirectory() + m_strSheet + CString(".") + m_strSheetFileExtension;

        //initialise the scripting engine
        CScriptingEngine ScriptEngine;
        ScriptEngine.SetOwnerWindow( AfxGetMainWnd()->m_hWnd );
        if( ScriptEngine.LoadScript( strFileName ) )
        {
            //reset the sheet command list etc.
            ClearSheetCommands();

            //add the global objects
            ScriptEngine.AddGlobalVariable( "output", new CScriptOutput(), TRUE );
            ScriptEngine.AddGlobalVariable( "sheet", this, TRUE );
            ScriptEngine.AddGlobalVariable( "field", &m_Fields, TRUE );

            //run the script
            if( !ScriptEngine.ExecuteScript() )
                AddDummySheetCommands( "A problem was encountered executing the current sheet layout script" );
            else
                fResult = TRUE;
        }
        else AddDummySheetCommands( "A problem was encountered trying to load the current sheet layout script" );
    }
    else
        AddDummySheetCommands( "There are no sheet files available (*." + m_strSheetFileExtension + CString(")") );

    //adjust the size of the view
    SetScrollSizes( MM_HIMETRIC, m_SheetSize );

    //let subclass tweak it's custom field commands
    InitialiseSheetCommands();

    //invalidate the window
    RedrawWindow();
    return fResult;
}


void CScriptSheetView::WindowToSheet( CPoint& point )
{
	CWindowDC dc(NULL);
	dc.SetMapMode(MM_HIMETRIC);
	dc.DPtoLP( &point );

    CPoint ptScroll = GetScrollPosition();
    point.x += ptScroll.x;
    point.y += ptScroll.y;
}


void CScriptSheetView::SheetToWindow( CPoint& point )
{
    CPoint ptScroll = GetScrollPosition();
    point.x -= ptScroll.x;
    point.y -= ptScroll.y;

    CWindowDC dc(NULL);
	dc.SetMapMode(MM_HIMETRIC);
	dc.LPtoDP( &point );
}

void CScriptSheetView::WindowToSheet( CRect& rect )
{
	CWindowDC dc(NULL);
	dc.SetMapMode(MM_HIMETRIC);
	dc.DPtoLP( &rect );

    CPoint ptScroll = GetScrollPosition();
    rect.left += ptScroll.x;
    rect.right += ptScroll.x;
    rect.top += ptScroll.y;
    rect.bottom += ptScroll.y;
}

void CScriptSheetView::SheetToWindow( CRect& rect )
{
    CPoint ptScroll = GetScrollPosition();
    rect.left -= ptScroll.x;
    rect.right -= ptScroll.x;
    rect.top -= ptScroll.y;
    rect.bottom -= ptScroll.y;

    CWindowDC dc(NULL);
	dc.SetMapMode(MM_HIMETRIC);
	dc.LPtoDP( &rect );
}

void CScriptSheetView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
    //read margin values from the registry
    int nMarginLeft, nMarginTop;
    nMarginTop = AfxGetApp()->GetProfileInt( "Sheet", "TopMargin", 180 );
    nMarginLeft = AfxGetApp()->GetProfileInt( "Sheet", "LeftMargin", 0 );

    //set margins
    pDC->SetViewportOrg( nMarginLeft, nMarginTop );

    //continue printing
	CScrollView::OnPrint(pDC, pInfo);
}

void CScriptSheetView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    SetCapture();
    m_fLeftButtonClick = TRUE;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CScriptSheetView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    ReleaseCapture();

    if( m_fLeftButtonClick )
    {
        m_fLeftButtonClick = FALSE;

        //get the field that was clicked
        WindowToSheet( point );
        int iField = -1;
        CFieldSheetCommand* pFieldSheetCommand = HotFieldFromPoint(point, &iField);

        //edit it
        m_wndFieldEdit.EndEditing();
        if( pFieldSheetCommand )
        {
            if( pFieldSheetCommand->m_pField->m_dwFlags & FIELD_TABSTOP && pFieldSheetCommand->m_Type != CommandType_TableEntry ) m_iLastEditedFieldSheetCommand = iField;
            pFieldSheetCommand->BeginEditing( &m_wndFieldEdit );
        }
    }

	CScrollView::OnLButtonUp(nFlags, point);
}

long CScriptSheetView::OnAppSettingsChange(WPARAM,LPARAM)
{
    //redraw the sheet
    ReloadSheet();
    RedrawWindow();
    return 0;
}

void CScriptSheetView::OnMouseMove(UINT nFlags, CPoint point)
{
    CWnd* pWnd = ChildWindowFromPoint(point, CWP_SKIPINVISIBLE);
    if( pWnd == this || pWnd == NULL )
    {
        //convert the point into view coordinates and get the field at that point
        WindowToSheet( point );
        CFieldSheetCommand* pFieldSheetCommand = HotFieldFromPoint( point );
        if( pFieldSheetCommand ) 
            pFieldSheetCommand->SetCursor();
        else 
            ::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
    }

    CScrollView::OnMouseMove(nFlags, point);
}

void CScriptSheetView::ResetFieldList()
{
    //reset field ID counter and empty the list (start at -1 and count *backwards*)
    m_nFieldID = -1;

    //wipe the sheet command list
    ClearSheetCommands();

    //wipe the main field list
    for( int i = 0; i < m_Fields.m_FieldList.GetSize(); i++ ) delete m_Fields.m_FieldList[i];
    m_Fields.m_FieldList.RemoveAll();
    m_Fields.m_FieldList.SetSize(0, 10);

    //add all fields
    AddAllFields();
}

void CScriptSheetView::ClearSheetCommands()
{
    for( int i = 0; i < m_Fields.m_SheetCommands.GetSize(); i++ ) delete m_Fields.m_SheetCommands[i];
    m_Fields.m_SheetCommands.RemoveAll();
    m_Fields.m_HotFieldList.RemoveAll();
    m_Fields.m_SheetCommands.SetSize(0,25);
    m_Fields.m_HotFieldList.SetSize(0,20);
}


BOOL CScriptSheetView::AddField(CString *pstrValue, CString strName, DWORD dwFlags /*FIELD_NORMAL*/, const char* pszOptionName /*NULL*/ )
{
    m_Fields.m_FieldList.Add( new CStringField( this, pstrValue, strName, dwFlags, m_nFieldID--, pszOptionName ) );
    return TRUE;
}

BOOL CScriptSheetView::AddField(int *pnValue, CString strName, DWORD dwFlags /*FIELD_NORMAL*/ )
{
    m_Fields.m_FieldList.Add( new CIntField( this, pnValue, strName, dwFlags, m_nFieldID-- ) );
    return TRUE;
}

BOOL CScriptSheetView::AddField( int nFieldCode, CString strName, DWORD dwFlags /*FIELD_NORMAL*/, const char* pszOptionName /*NULL*/  )
{
    //field code must be positive (0 is also reserved, just in case...)
    ASSERT(nFieldCode > 0);
    m_Fields.m_FieldList.Add( new CField( this, strName, dwFlags, nFieldCode, pszOptionName ) );
    return TRUE;
}

CScriptObject::MEMBERRESULT CScriptSheetView::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
    //sheet command lookup table
    struct SheetCommandLookupTableType
    {
        const char* pszName;
        SheetCommandType Type;
        int nParameters;
    };

    const SheetCommandLookupTableType SheetCommandLookupTable[] = {
        { "sheetsize",              CommandType_SheetSize,              2 },
        { "drawline",               CommandType_DrawLine,               4 },
        { "drawpicture",            CommandType_DrawPicture,            5 },
        { "stretchpicture",         CommandType_StretchPicture,         5 },
        { "drawrectangle",          CommandType_DrawRectangle,          4 },
        { "drawtext",               CommandType_DrawText,               4 },
        { "drawtextbox",            CommandType_DrawTextBox,            6 },
        { "fillrectangle",          CommandType_FillRectangle,          5 },
        { "setlinecolour",          CommandType_SetLineColour,          1 }, { "setlinecolor",          CommandType_SetLineColour,          1 },
        { "settextcolour",          CommandType_SetTextColour,          1 }, { "settextcolor",          CommandType_SetTextColour,          1 },
        { "setfieldtextstyle",      CommandType_SetFieldTextStyle,      1 },
        { "setfont",                CommandType_SetFont,                3 },
        { "setfontrotated",         CommandType_SetFontRotated,         4 },
        { "addfield",               CommandType_Field,                  5 },
        { "addtableentry",          CommandType_TableEntry,             6 },
        { "setusertextsize",        CommandType_SetUserTextSize,        1 },
        { "setusertextsizerotate",  CommandType_SetUserTextSizeRotate,  2 },
        { "setsheetparameter",      CommandType_SetSheetParameter,      2 },
        { NULL,                     CommandType_None,                   0 },
    };

    //find the sheet command that matches best
    for( int i = 0; SheetCommandLookupTable[i].pszName != NULL; i++ )
    {
        //see if the name matches
        if( stricmp( pszName, SheetCommandLookupTable[i].pszName ) == 0 )
        {
            //see if the number of parameters is correct
            if( SheetCommandLookupTable[i].nParameters == nParameters )
            {
                //build the field rectangle now
                CRect rc;
                if( nParameters >= 2 )
                {
                    rc.left = ScaleToSheet(pParameters[0]);
                    rc.top = -ScaleToSheet(pParameters[1]);
                    if( nParameters >= 4 )
                    {
                        rc.right = ScaleToSheet(pParameters[2]);
                        rc.bottom = -ScaleToSheet(pParameters[3]);
                    }
                }

                //process sheet command
                CSheetCommand* pSheetCommand = NULL;
                SheetCommandType Type = SheetCommandLookupTable[i].Type;
                switch( Type )
                {
                    //non-command functions
                    case CommandType_SheetSize:             
                        m_SheetSize.cx = rc.left; 
                        m_SheetSize.cy = -rc.top; 
                        if( m_SheetSize.cx <= 0 ) m_SheetSize.cx = 100;
                        if( m_SheetSize.cy <= 0 ) m_SheetSize.cy = 100;
                        break;

                    //drawing commands
                    case CommandType_DrawLine:              pSheetCommand = new CDrawSheetCommand( Type, rc ); break;
                    case CommandType_DrawPicture:           pSheetCommand = new CDrawSheetCommand( Type, rc, CString(LPCSTR(pParameters[4])) ); break;
                    case CommandType_StretchPicture:        pSheetCommand = new CDrawSheetCommand( Type, rc, CString(LPCSTR(pParameters[4])) ); break;
                    case CommandType_DrawRectangle:         pSheetCommand = new CDrawSheetCommand( Type, rc ); break;
                    case CommandType_FillRectangle:         pSheetCommand = new CDrawSheetCommand( Type, rc, HEXTOCOLORREF(pParameters[4]) ); break;
                    case CommandType_DrawText:              pSheetCommand = new CDrawSheetCommand( Type, rc, CString(LPCSTR(pParameters[2])), int(pParameters[3]) ); break;
                    case CommandType_DrawTextBox:           pSheetCommand = new CDrawSheetCommand( Type, rc, CString(LPCSTR(pParameters[4])), int(pParameters[5]) ); break;
                    case CommandType_SetLineColour:         pSheetCommand = new CDrawSheetCommand( Type, HEXTOCOLORREF(pParameters[0]) ); break;
                    case CommandType_SetTextColour:         pSheetCommand = new CDrawSheetCommand( Type, HEXTOCOLORREF(pParameters[0]) ); break;
                    case CommandType_SetFieldTextStyle:     pSheetCommand = new CDrawSheetCommand( Type, pParameters[0] ); break;
                    case CommandType_SetUserTextSize:       pSheetCommand = new CDrawSheetCommand( Type, int(double(pParameters[0]) * 10.0 ) ); break;
                    case CommandType_SetUserTextSizeRotate: pSheetCommand = new CDrawSheetCommand( Type, int(double(pParameters[0]) * 10.0 ), int(double(pParameters[1]) * 10.0)  ); break;
                    case CommandType_SetSheetParameter:     
                        switch( int(pParameters[0]) )
                        {
                            case SHEETPARAM_SHADOWOFFSET: //I know this is a nasty hack, but it's either this or I don't go to the pub this evening :)
                                pSheetCommand = new CDrawSheetCommand( Type, pParameters[0], ScaleToSheet(double(pParameters[1])) ); break;
                                break;

                            default:
                                pSheetCommand = new CDrawSheetCommand( Type, pParameters[0], pParameters[1] ); break;
                                break;
                        }
                        break;
                    //font commands
                    case CommandType_SetFont:               pSheetCommand = new CFontSheetCommand( Type, LPCSTR(pParameters[0]), int(double(pParameters[1]) * 10.0), pParameters[2] ); break;
                    case CommandType_SetFontRotated:        pSheetCommand = new CFontSheetCommand( Type, LPCSTR(pParameters[0]), int(double(pParameters[1]) * 10.0), pParameters[2], int(double(pParameters[3]) * 10.0) ); break;
                    //field commands
                    case CommandType_Field:                 pSheetCommand = AddHotArea( int(pParameters[4]), rc ); break;
                    case CommandType_TableEntry:            pSheetCommand = AddTableHotArea( int(pParameters[4]), rc, pParameters[5] ); break;
                }

                //add the sheet command to the command execute buffer
                if( pSheetCommand ) m_Fields.m_SheetCommands.Add( pSheetCommand );

                //all OK...
                return OK;
            }
            else
                return nParameters < SheetCommandLookupTable[i].nParameters ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
        }
    }

    //other commands
    if( strcmpi( pszName, "findvalidfont" ) == 0 )
    {
        if( nParameters == 1 )
        {
            //see if the font exists
            returnValue = FindFont( LPCSTR(pParameters[0] ) );
            return OK;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    //couldn't find anything
    return NOT_FOUND;
}

Variant* CScriptSheetView::GetMemberVariable( char* pszName )
{
    static Variant fontnormal = FONTSTYLE_NORMAL, fontbold = FONTSTYLE_BOLD, fontitalic = FONTSTYLE_ITALIC, fontunderline = FONTSTYLE_UNDERLINE, fontstrikethrough = FONTSTYLE_STRIKETHROUGH;
    static Variant textnormal = TEXTSTYLE_NORMAL, alignleft = TEXTSTYLE_ALIGNLEFT, alignright = TEXTSTYLE_ALIGNRIGHT, aligncenter = TEXTSTYLE_ALIGNCENTER, textmultiline = TEXTSTYLE_MULTILINE, textclipped = TEXTSTYLE_CLIPPED, fieldunderline = TEXTSTYLE_FIELDUNDERLINE, usertext = TEXTSTYLE_USERTEXT;

    struct 
        { char* pszName; int nFieldCode; Variant* pVariant; }
    vt[] = {
        //font style
        { "fontnormal",         FONTSTYLE_NORMAL, &fontnormal },
        { "fontbold",           FONTSTYLE_BOLD, &fontbold },
        { "fontitalic",         FONTSTYLE_ITALIC, &fontitalic },
        { "fontunderline",      FONTSTYLE_UNDERLINE, &fontunderline },
        { "fontstrikethrough",  FONTSTYLE_STRIKETHROUGH, &fontstrikethrough },
        //text styles
        { "textnormal",         TEXTSTYLE_NORMAL, &textnormal },
        { "alignleft",          TEXTSTYLE_ALIGNLEFT, &alignleft },
        { "alignright",         TEXTSTYLE_ALIGNRIGHT, &alignright },
        { "aligncenter",        TEXTSTYLE_ALIGNCENTER, &aligncenter },
        { "textmultiline",      TEXTSTYLE_MULTILINE, &textmultiline },
        { "textclipped",        TEXTSTYLE_CLIPPED, &textclipped },
        { "fieldunderline",     TEXTSTYLE_FIELDUNDERLINE, &fieldunderline },
        { "usertext",           TEXTSTYLE_USERTEXT, &usertext },

        { NULL, 0 }
    };

    //scan through table until we find the variable
    int i;
    for( i = 0; vt[i].pszName != NULL; i++ )
    {
        if( stricmp( vt[i].pszName, pszName ) == 0 ) 
        {
            vt[i].pVariant->SetReadOnly(TRUE);
            return vt[i].pVariant;
        }
    }

    //check the sheet parameter array
    int iMax = m_SheetParameters.GetSize();
    for( i = 0; i < iMax; i++ )
    {
        if( m_SheetParameters[i]->strName.CompareNoCase(pszName) == 0 ) return &m_SheetParameters[i]->Code;
    }

    return NULL;
}

typedef struct {
    CString strFound;
    const char* pszFontFaceName;
} FINDFONTDATA, *PFINDFONTDATA;

int CALLBACK EnumFontProc_FindFont( ENUMLOGFONT FAR* lpelf, NEWTEXTMETRIC FAR* lpntm, int FontType, PFINDFONTDATA pFindFontData )
{
    //compare font names
    if( lpelf && strnicmp( pFindFontData->pszFontFaceName, lpelf->elfLogFont.lfFaceName, strlen(pFindFontData->pszFontFaceName) ) == 0 )
    {
        pFindFontData->strFound = lpelf->elfLogFont.lfFaceName;
        if( stricmp( pFindFontData->pszFontFaceName, lpelf->elfLogFont.lfFaceName ) == 0 ) 
            return 0;
        else
            return 1; //not a perfect match - keep searching
    }
    else
        return 1;
}

CString CScriptSheetView::FindFont(const char * pszFontFaceName)
{
    //prepare the find font structure
    FINDFONTDATA FindFontData;
    FindFontData.strFound = "";
    FindFontData.pszFontFaceName = pszFontFaceName;

    //enumerate all fonts
    CDC* pDC = AfxGetMainWnd()->GetDC();
    EnumFontFamilies( pDC->GetSafeHdc(), NULL, (FONTENUMPROC)EnumFontProc_FindFont, (LPARAM)&FindFontData );
    AfxGetMainWnd()->ReleaseDC(pDC);

    //examine result of search
    return FindFontData.strFound;
}


CSheetCommand* CScriptSheetView::AddHotArea(int nFieldCode, CRect rcField)
{
    //find the field with this code
    CField* pField = FindField(nFieldCode);
    if( pField )
    {
        CFieldSheetCommand* pHotField = new CFieldSheetCommand( rcField, pField );
        m_Fields.m_HotFieldList.Add( pHotField );

        //return it
        return pHotField;
    }

    return NULL;
}

CSheetCommand* CScriptSheetView::AddTableHotArea( int nFieldCode, CRect rcField, Variant value )
{
    //find the field with this code
    CField* pField = FindField(nFieldCode);
    if( pField )
    {
        //make sure this field is not a special case (checkbox or picture etc)
        if( pField->m_dwFlags & FIELD_SPECIALTYPE )
            AfxMessageBox( pField->m_strName + CString( " cannot be placed in a table" ) );
        else
        {
            //nope - add it to the end of the field hotlist
            CTableFieldSheetCommand* pTableHotField = new CTableFieldSheetCommand( rcField, pField, value );
            m_Fields.m_HotFieldList.Add( pTableHotField );
            return pTableHotField;
        }
    }

    return NULL;
}

CFieldSheetCommand* CScriptSheetView::HotFieldFromPoint(CPoint pt, int* pnIndex /*NULL*/)
{
    //search the entire field array
    int iMax = m_Fields.m_HotFieldList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        CRect rc = m_Fields.m_HotFieldList[i]->m_rc;
        if( !(m_Fields.m_HotFieldList[i]->m_pField->m_dwFlags & FIELD_NOTACTIVE) && pt.x > rc.left && pt.x < rc.right && pt.y < rc.top && pt.y > rc.bottom)
        {
            if( pnIndex ) *pnIndex = i;
            return m_Fields.m_HotFieldList[i];
        }
    }

    if( pnIndex ) *pnIndex = -1;
    return NULL;
}

CField* CScriptSheetView::FindField(int nFieldCode)
{
    //search the entire field list
    int iMax = m_Fields.m_FieldList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        //get the next item and return the value if it's the one we're looking for
        CField* pField = m_Fields.m_FieldList[i];
        if( pField && int(pField->m_Code) == nFieldCode ) return pField;
    };

    return NULL;

}

Variant CScriptSheetView::GetFieldValue( CField* pField, CFieldSheetCommand* pFieldSheetCommand /*NULL*/  )
{
    //FIXME: move this into derived classes and keep this one pure?
    return "";
}

void CScriptSheetView::SetFieldValue( CField* pField, Variant value, CFieldSheetCommand* pFieldSheetCommand /*NULL*/  )
{
    //FIXME move this into derived classes and keep this one pure?
}

void CScriptSheetView::ExecuteSheetCommand(CSheetCommand *pSheetCommand, CSheetDrawInfo* pDrawInfo )
{
    ASSERT(pSheetCommand);

    switch( pSheetCommand->m_Type )
    {
        case CommandType_TableEntry:
        case CommandType_Field:
            //only call it's core drawing function if it's a string or integer field
            /*if( int(((CFieldSheetCommand*)pSheetCommand)->m_pField->m_Code) < 0 ) */ pSheetCommand->Draw( pDrawInfo );
            break;

        default:
            pSheetCommand->Draw( pDrawInfo );
            break;
    }
}

int CScriptSheetView::ScaleToSheet(double dValue)
{
    return int(dValue * 100.0);
}

BOOL CScriptSheetView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if( nHitTest == HTCLIENT && (pWnd == this || pWnd == NULL ) ) 
        return TRUE; //cursor changing is done in mouse move in the client area
    else
	    return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CScriptSheetView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    if( m_wndFieldEdit.IsEditing() ) pCmdUI->Enable( m_wndFieldEdit.CanUndo() );
/*    { FIXME
        CHandoutDoc* pDoc = GetDocument();
        if( pDoc ) pDoc->OnUpdateEditUndo( pCmdUI );
    }*/
}

void CScriptSheetView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
    if( m_wndFieldEdit.IsEditing() ) pCmdUI->Enable( FALSE );
    /*{ FIXME
        CHandoutDoc* pDoc = GetDocument();
        if( pDoc ) pDoc->OnUpdateEditRedo( pCmdUI );
    }*/
}

void CScriptSheetView::OnEditUndo() 
{
    if( m_wndFieldEdit.IsEditing() ) m_wndFieldEdit.Undo();
    /*{ FIXME
        CHandoutDoc* pDoc = GetDocument();
        if( pDoc ) pDoc->OnEditUndo();
    }*/
}

void CScriptSheetView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    //stop editing
    m_wndFieldEdit.EndEditing();

    //reload the sheet?
    ReloadSheet();
}

void CScriptSheetView::RedrawFields( CFieldSheetCommand* pFieldChanged /*NULL*/ )
{
    if( m_pbmSheet ) delete m_pbmSheet;
    m_pbmSheet = NULL;

    RedrawWindow();
}

CWnd* CScriptSheetView::GetFieldManagerWindow()
{
    return this;
}

void CScriptSheetView::FieldToWindow( CRect& rc )
{
    SheetToWindow( rc );
}

void CScriptSheetView::InitialiseSheetCommands()
{
    //nothing to do in the base class
}

void CScriptSheetView::AddSheetParameter(CString strName, int nCode )
{
    SheetParameter* pSheetParameter = new SheetParameter;
    pSheetParameter->strName = strName;
    pSheetParameter->Code = nCode;
    m_SheetParameters.Add( pSheetParameter );
}

int CScriptSheetView::GetSheetParameter(int nCode, int nDefaultValue)
{
    //zip through the entire sheet command array looking for sheet parameters
    int iMax = m_Fields.m_SheetCommands.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        if( m_Fields.m_SheetCommands[i]->m_Type == CommandType_SetSheetParameter && ((CDrawSheetCommand*)m_Fields.m_SheetCommands[i])->m_nValue == nCode )
            return ((CDrawSheetCommand*)m_Fields.m_SheetCommands[i])->m_nValue2;
    }

    return nDefaultValue;
}

void CScriptSheetView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_wndFieldEdit.EndEditing();
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CScriptSheetView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_wndFieldEdit.EndEditing();
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CScriptSheetView::OnEditNextField() 
{
    //stop editing
    m_wndFieldEdit.EndEditing();

    //make sure the field index is still valid
    if( m_iLastEditedFieldSheetCommand < 0 && m_iLastEditedFieldSheetCommand > m_Fields.m_HotFieldList.GetUpperBound() ) m_iLastEditedFieldSheetCommand = 0;

    //move to the next field
    int iNext = m_iLastEditedFieldSheetCommand + 1; if( iNext > m_Fields.m_HotFieldList.GetUpperBound() ) iNext = 0;
    while( iNext != m_iLastEditedFieldSheetCommand )
    {
        if( m_Fields.m_HotFieldList[iNext]->m_pField->m_dwFlags & FIELD_TABSTOP && m_Fields.m_HotFieldList[iNext]->m_fActive && m_Fields.m_HotFieldList[iNext]->m_Type != CommandType_TableEntry )
        {
            //scroll the field into view
            CRect rc = m_Fields.m_HotFieldList[iNext]->m_rc;
            EnsureRectVisible( CRect( rc.left, -rc.top, rc.right, -rc.bottom ) );

            //edit the field
            m_iLastEditedFieldSheetCommand = iNext;
            m_Fields.m_HotFieldList[iNext]->BeginEditing( &m_wndFieldEdit );
            return;
        }

        //keep searching. Loop when we get to the end of the array
        iNext++;
        if( iNext > m_Fields.m_HotFieldList.GetUpperBound() ) iNext = 0;
    }

    m_iLastEditedFieldSheetCommand = -1;
}

void CScriptSheetView::OnEditPrevField() 
{
    //stop editing
    m_wndFieldEdit.EndEditing();

    //make sure the field index is still valid
    if( m_iLastEditedFieldSheetCommand < 0 && m_iLastEditedFieldSheetCommand > m_Fields.m_HotFieldList.GetUpperBound() ) m_iLastEditedFieldSheetCommand = 0;

    //move to the next field
    int iNext = m_iLastEditedFieldSheetCommand - 1; if( iNext < 0 ) iNext = m_Fields.m_HotFieldList.GetUpperBound();
    while( iNext != m_iLastEditedFieldSheetCommand )
    {
        if( m_Fields.m_HotFieldList[iNext]->m_pField->m_dwFlags & FIELD_TABSTOP && m_Fields.m_HotFieldList[iNext]->m_fActive && m_Fields.m_HotFieldList[iNext]->m_Type != CommandType_TableEntry )
        {
            //scroll the field into view
            CRect rc = m_Fields.m_HotFieldList[iNext]->m_rc;
            EnsureRectVisible( CRect( rc.left, -rc.top, rc.right, -rc.bottom ) );

            //edit the field
            m_iLastEditedFieldSheetCommand = iNext;
            m_Fields.m_HotFieldList[iNext]->BeginEditing( &m_wndFieldEdit );
            return;
        }

        //keep searching. Loop when we get to the end of the array
        iNext--;
        if( iNext < 0 ) iNext = m_Fields.m_HotFieldList.GetUpperBound();
    }

    m_iLastEditedFieldSheetCommand = -1;
}

void CScriptSheetView::EnsureRectVisible(CRect rc)
{
    //FIXME: scroll to the position of the field in the view, but don't let the view scroll away (can't just use ScrollToPosition as it tries to center it or something. Maybe)
}
