// FieldEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "FieldEdit.h"
#include "QuickInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldEdit

CFieldEdit::CFieldEdit()
{
    m_pNamedOptionManager = NULL;
    m_pOwner = NULL;
    m_pwndListBox = NULL;
    m_pwndPopup = NULL;
    m_pFieldSheetCommand = NULL;
    m_crText = 0x000000;
    m_dwAdditionalStyle = 0;
    m_pFont = NULL;
}

CFieldEdit::~CFieldEdit()
{
    if( m_pwndListBox ) delete m_pwndListBox;
    if( m_pwndPopup )
    {
        m_pwndPopup->DestroyWindow();
        delete m_pwndPopup;
    }
    if( m_pFont ) delete m_pFont;
}


BEGIN_MESSAGE_MAP(CFieldEdit, CEdit)
	//{{AFX_MSG_MAP(CFieldEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_GETDLGCODE()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldEdit message handlers


void CFieldEdit::EndEditing(BOOL fSaveChanges/*TRUE*/ )
{
    //make sure we're editing
    if( m_pFieldSheetCommand == NULL ) return;
    CFieldSheetCommand* pFieldSheetCommandEdited = m_pFieldSheetCommand;
    m_pFieldSheetCommand = NULL;

    //save changes?
    BOOL fChanges = GetModify();
    if( fSaveChanges )
    {
        if( fChanges )
        {
            //store the undo
            m_pOwner->StoreUndo();

            //get the new value and write it back to the field
            CString strWindowText; GetWindowText( strWindowText );
            pFieldSheetCommandEdited->m_pField->SetValue( LPCSTR(strWindowText), pFieldSheetCommandEdited );
        }
    }

    //destroy the editing windows
    ShowWindow(SW_HIDE);
    if( m_pwndListBox ) delete m_pwndListBox; m_pwndListBox = NULL;
    if( m_pwndPopup ) 
    {
        m_pwndPopup->DestroyWindow();
        delete m_pwndPopup; 
        m_pwndPopup = NULL;
    }

    //redraw everything if a property was changed
    if( fChanges && fSaveChanges )
        //notify the owner that we've changed some field data
        m_pOwner->RedrawFields( pFieldSheetCommandEdited );
    else
        //just repaint the owner
        m_pOwner->GetFieldManagerWindow()->RedrawWindow(NULL,NULL,RDW_INTERNALPAINT);
}

HBRUSH CFieldEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor( m_crText );
	return (HBRUSH)GetStockObject( WHITE_BRUSH );
}

void CFieldEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
            if( m_dwAdditionalStyle & ES_MULTILINE && GetAsyncKeyState(VK_SHIFT) & 0x8000 ) CEdit::OnChar(nChar, nRepCnt, nFlags);
			break;

		case VK_ESCAPE:
			break;

        case '*':
            MessageBeep(0xFFFFFFFF);
            break;

        default:
	        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

void CFieldEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
            if( m_dwAdditionalStyle & ES_MULTILINE && GetAsyncKeyState(VK_SHIFT) & 0x8000 ) CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			break;

		case VK_ESCAPE:
			break;

        case '*':
            MessageBeep(0xFFFFFFFF);
            break;

        case VK_UP:
            if( m_pwndListBox->IsWindowVisible() )
            {
                int n = m_pwndListBox->GetCurSel();
                if( n <= 0 ) n = 0; else n--;
                if( n >= 0 && n < m_pwndListBox->GetCount() ) m_pwndListBox->SetCurSel(n);
            }
            else
	            CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
            break;

        case VK_DOWN:
            if( m_pwndListBox->IsWindowVisible() )
            {
                int n = m_pwndListBox->GetCurSel();
                if( n == LB_ERR ) n = 0; else n++;
                if( n >= 0 && n < m_pwndListBox->GetCount() ) m_pwndListBox->SetCurSel(n);
            }
            else
	            CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
            break;

        default:
	        CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
            break;
	}
}

void CFieldEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
            if( m_dwAdditionalStyle & ES_MULTILINE && GetAsyncKeyState(VK_SHIFT) & 0x8000 ) 
                CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
            else
                if( m_pwndListBox->IsWindowVisible() )
                {
                    int i = m_pwndListBox->GetCurSel();
                    if( i != LB_ERR )
                    {
                        CString str, strCurrent;
                        m_pwndListBox->GetText( i, str );
                        GetWindowText( strCurrent );
                        SetWindowText(str);
                        SetModify();
                        SetSel(0,-1);
                        if( str.CompareNoCase(strCurrent) == 0 ) EndEditing(TRUE);
                    }
                    else
    			        EndEditing( TRUE );
                }
                else
    			    EndEditing( TRUE );
			break;

		case VK_ESCAPE:
			EndEditing( FALSE );
			break;

        case '*':
            MessageBeep(0xFFFFFFFF);
            break;

        default:
        	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
    }
}

UINT CFieldEdit::OnGetDlgCode() 
{
	if( m_pFieldSheetCommand )
		return DLGC_WANTALLKEYS|DLGC_WANTARROWS|DLGC_WANTCHARS;
	else
		return CEdit::OnGetDlgCode();
}

void CFieldEdit::OnKillFocus(CWnd* pNewWnd) 
{
    if( pNewWnd != m_pwndListBox && pNewWnd != m_pwndPopup ) EndEditing( TRUE );
	CEdit::OnKillFocus(pNewWnd);
}

void CFieldEdit::OnEditUndo() 
{
    SendMessage( WM_UNDO );
}

int CFieldEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_pwndPopup = new CFieldEditPopup;
    m_pwndListBox = new CFieldListBox;


    if( CFieldEditPopup::m_strClassName.IsEmpty() ) CFieldEditPopup::m_strClassName = AfxRegisterWndClass( CS_OWNDC, AfxGetApp()->LoadStandardCursor(IDC_ARROW), GetSysColorBrush(COLOR_BTNFACE), NULL );
    m_pwndPopup->CreateEx( WS_EX_DLGMODALFRAME, CFieldEditPopup::m_strClassName, "", WS_POPUP, CRect(0,0,0,0), m_pOwner->GetFieldManagerWindow(), 0 );
    m_pwndPopup->SetFieldEditBox( this );

    m_pwndListBox->Create( WS_VSCROLL|WS_CHILD|LBS_DISABLENOSCROLL|LBS_SORT|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT, CRect(2,2,2,2), m_pwndPopup, IDC_FIELDEDITLISTBOX );
    m_pwndListBox->SetFieldEditBox( this );
    m_pwndListBox->SendMessage( WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT) );

    //reset shift key reading
    GetAsyncKeyState(VK_SHIFT);
    return 0;
}

void CFieldEdit::OnChange() 
{
    if( m_pwndListBox->IsWindowVisible() )
    {
        CString str;
        GetWindowText(str);
        int i = m_pwndListBox->FindString(-1, str );
        m_pwndListBox->SetCurSel( ( i != LB_ERR ) ? i : -1 );
    }
}

void CFieldEdit::SetNamedOptionManager(CNamedOptionManager * pNamedOptionManager)
{
    m_pNamedOptionManager = pNamedOptionManager;
}


/////////////////////////////////////////////////////////////////////////////
// CFieldListBox


BEGIN_MESSAGE_MAP(CFieldListBox, CListBox)
	//{{AFX_MSG_MAP(CFieldListBox)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblClk)
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

void CFieldListBox::OnSelChange()
{
    ASSERT( m_pEdit );

    int i = GetCurSel();
    if( i != LB_ERR )
    {
        CString str;
        GetText( i, str );
        m_pEdit->SetWindowText(str);
        m_pEdit->SetModify();
        m_pEdit->SetSel(0,-1); //select all
    }
    m_pEdit->SetFocus();
}

void CFieldListBox::OnDblClk()
{
    //update text
    OnSelChange();

    //tell edit control to close
    m_pEdit->EndEditing(TRUE);
}

void CFieldListBox::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
    CListBox::OnVScroll(nSBCode, nPos, pScrollBar );
    m_pEdit->SetFocus();
}

void CFieldEdit::CreateControl()
{
    //destroy current control
    DestroyWindow(); m_hWnd = NULL;

    //build a new one
    Create(ES_NOHIDESEL|WS_CHILD|m_dwAdditionalStyle, CRect(0,0,0,0), m_pOwner->GetFieldManagerWindow(), IDC_FIELDEDIT );

    //change font
    SetFont( m_pFont, FALSE );
}

void CFieldEdit::SetOwner(CFieldManager* pOwner)
{
    m_pOwner = pOwner;
}

BOOL CFieldEdit::IsEditing()
{
    return m_pFieldSheetCommand != NULL;
}

void CFieldEdit::BeginEditing(CFieldSheetCommand *pFieldSheetCommand )
{
    //load user text colour parameters
	m_crUserText = AfxGetApp()->GetProfileInt( "Sheet", "UserTextColour", RGB(0,0,128) );
	m_strUserTextFace = AfxGetApp()->GetProfileString( "Sheet", "UserTextFace", "Comic Sans MS" );
    m_fUserTextItalic = AfxGetApp()->GetProfileInt( "Sheet", "UserTextItalic", FALSE );
    m_nUserTextWeight = AfxGetApp()->GetProfileInt( "Sheet", "UserTextWeight", 0 );

    //stop the current edit
    EndEditing();

    //get new field rectangle
    BOOL fRotated = (pFieldSheetCommand->m_pLastUsedFont && pFieldSheetCommand->m_pLastUsedFont->m_nAngle != 0) || (pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_USERTEXT && pFieldSheetCommand->m_nCurrentUserTextAngle != 0 );

    CRect rc = pFieldSheetCommand->m_rc;
    m_pOwner->FieldToWindow( rc );
    if( fRotated && rc.top < 0 )
    {
        rc.bottom = rc.Height() + 16;
        rc.top = 16;
    }

    //change formatting
    m_dwAdditionalStyle = 0;
    m_crText = pFieldSheetCommand->m_crFontColour;

    //create new font
    int nTextHeight = rc.Height();
    if( m_pFont ) delete m_pFont;
    m_pFont = new CFont();

    if( pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_USERTEXT )
    {
        //prepare the logfont
	    LOGFONT lf;
	    memset(&lf, 0, sizeof(LOGFONT));
	    lf.lfCharSet = DEFAULT_CHARSET;
	    lf.lfHeight = pFieldSheetCommand->m_nCurrentUserTextSize;
        lf.lfWeight = m_nUserTextWeight;
        lf.lfItalic = m_fUserTextItalic;
	    strncpy( lf.lfFaceName, m_strUserTextFace, LF_FACESIZE-1 );
        m_crText = m_crUserText;

        //create the font
        m_pFont->CreatePointFontIndirect( &lf );
    }
    else
    {
        if( pFieldSheetCommand->m_pLastUsedFont )
        {
            //initialise the logfont
	        LOGFONT lf;
	        memset(&lf, 0, sizeof(LOGFONT));
	        lf.lfCharSet = DEFAULT_CHARSET;
	        lf.lfHeight = pFieldSheetCommand->m_pLastUsedFont->m_nPointSize;
            if( pFieldSheetCommand->m_pLastUsedFont->m_nTextAttributes & FONTSTYLE_BOLD )          lf.lfWeight = FW_BOLD; else lf.lfWeight = FW_NORMAL;
            if( pFieldSheetCommand->m_pLastUsedFont->m_nTextAttributes & FONTSTYLE_ITALIC )        lf.lfItalic = TRUE;
            if( pFieldSheetCommand->m_pLastUsedFont->m_nTextAttributes & FONTSTYLE_UNDERLINE )     lf.lfUnderline = TRUE;
            if( pFieldSheetCommand->m_pLastUsedFont->m_nTextAttributes & FONTSTYLE_STRIKETHROUGH ) lf.lfStrikeOut = TRUE;
	        strncpy( lf.lfFaceName, LPCSTR(pFieldSheetCommand->m_pLastUsedFont->m_strFace), LF_FACESIZE-1 );

            //create the font
	        m_pFont->CreatePointFontIndirect(&lf);
        }
        else
        {
            //create default font
            m_pFont->CreatePointFont( 100, "Times New Roman" );
        }
    }

    //change edit box style
    if( pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_ALIGNRIGHT ) m_dwAdditionalStyle |= ES_RIGHT;
    else if( pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_ALIGNCENTER ) m_dwAdditionalStyle |= ES_CENTER;
        else m_dwAdditionalStyle |= ES_LEFT;
    if( pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_MULTILINE )
    {
        CQuickInfoDlg QuickInfo;
        QuickInfo.Show( "MultilineEdit", "You can press Shift+Enter to insert a line feed into this text box" );
        m_dwAdditionalStyle |= ES_WANTRETURN|ES_MULTILINE|ES_AUTOVSCROLL; 
    }
    else m_dwAdditionalStyle |= ES_AUTOHSCROLL;

    //get the current value of the field
    Variant currentValue = pFieldSheetCommand->m_pField->GetValue(pFieldSheetCommand);
    if( currentValue.IsNumeric() ) m_dwAdditionalStyle |= ES_NUMBER;


    //create the control
    CreateControl();

    //calculate text height for unclipped editing
    if( !(pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_CLIPPED) || fRotated && ((pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_MULTILINE) == 0) )
    {
        CDC* pDC = GetDC();
        CFont* pOldFont = pDC->SelectObject(m_pFont);
        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);
        pDC->SelectObject( pOldFont );
        ReleaseDC(pDC);
        nTextHeight = tm.tmHeight;
    }

    //set field size
    if( fRotated )
    {
        ModifyStyle( 0, WS_DLGFRAME );
        rc.right = rc.left + 240;
        nTextHeight += ( GetSystemMetrics(SM_CYDLGFRAME) * 2 );
    }
    if( !(pFieldSheetCommand->m_dwFieldStyle & TEXTSTYLE_MULTILINE) ) rc.bottom = rc.top + nTextHeight;
    MoveWindow( rc, FALSE );

    //prepare this one
    if( currentValue.IsNumeric() )
    {
        //initialise the control for integer editing
        LimitText(3);
        CString str;
        str.Format( "%d", int(currentValue) );
        SetWindowText( str );
    }
    else
    {
        //initialise the control for string editing
        SetWindowText( LPCSTR(currentValue) );
        LimitText();

        const char* pszOptionName = pFieldSheetCommand->m_strOptionName;
        if( pszOptionName && m_pNamedOptionManager )
        {
            //initialise the list box
            m_pNamedOptionManager->FillListBoxWithOptions( m_pwndListBox, pszOptionName );
            if( m_pwndListBox->GetCount() > 0 )
            {
                //find the current value in it and select it
                int i = m_pwndListBox->FindStringExact( -1, LPCSTR(currentValue) );
                if( i != LB_ERR ) m_pwndListBox->SetCurSel(i);

                //display the list box
                CRect rcPopup(rc.left+5, rc.bottom, rc.left+200, rc.bottom+120);
                m_pOwner->GetFieldManagerWindow()->ClientToScreen( rcPopup );
                m_pwndPopup->MoveWindow( rcPopup, FALSE );
                m_pwndListBox->MoveWindow( CRect(1, 1, 188, 113), FALSE );
                m_pwndPopup->ShowWindow( SW_SHOWNA );
                m_pwndListBox->ShowWindow( SW_SHOWNA );
            }
        }
        else
            m_pwndListBox->ResetContent();
    }

    //start the edit
    SetMargins( 0, 0 );
    EmptyUndoBuffer();
    SetModify(FALSE);
    SetSel( 0, -1 );

    //store the field we're going to edit
    m_pFieldSheetCommand = pFieldSheetCommand;

    //display cheezy rotation animation if it's rotated
    if( fRotated && AfxGetApp()->GetProfileInt( "Sheet", "FieldAnimation", TRUE ) )
    {
        //prepare screen drawing
        HDC hDC = ::GetDC(NULL);

        //set source and destination rectangles
        CRect rcFrom = pFieldSheetCommand->m_rc;
        m_pOwner->FieldToWindow( rcFrom );
        GetParent()->ClientToScreen( rcFrom );
        CRect rcTo = rc;
        GetParent()->ClientToScreen( rcTo );

        //set frame counter
        const int nFrames = 15;

        //calculate animation deltas etc
        double x1 = rcFrom.left;
        double y1 = rcFrom.top;
        double x2 = rcFrom.right;
        double y2 = rcFrom.bottom;
        double dx1 = double( rcTo.left - rcFrom.left ) / double(nFrames);
        double dy1 = double( rcTo.top - rcFrom.top ) / double(nFrames);
        double dx2 = double( rcTo.right - rcFrom.right ) / double(nFrames);
        double dy2 = double( rcTo.bottom - rcFrom.bottom ) / double(nFrames);

        for( int iFrame = 0; iFrame < nFrames; iFrame++ )
        {
            //draw rectangle
            ::DrawFocusRect( hDC, CRect(x1,y1,x2,y2) );
            Sleep(20);

            //erase rectangle
            ::DrawFocusRect( hDC, CRect(x1,y1,x2,y2) );

            //adjust rectangle
            x1 += dx1;
            y1 += dy1;
            x2 += dx2;
            y2 += dy2;
        }

	    //clear up
        ::ReleaseDC( NULL, hDC );
    }


    //display the control
    ShowWindow( SW_SHOW );
    SetFocus();
}
/////////////////////////////////////////////////////////////////////////////
// CFieldEditPopup

CString CFieldEditPopup::m_strClassName;

CFieldEditPopup::CFieldEditPopup()
{
    m_pEdit = NULL;
}

CFieldEditPopup::~CFieldEditPopup()
{
}


BEGIN_MESSAGE_MAP(CFieldEditPopup, CWnd)
	//{{AFX_MSG_MAP(CFieldEditPopup)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFieldEditPopup::OnSetFocus(CWnd* pOldWnd) 
{
    //if we get the focus, give it back to the edit window immediately
    CWnd::OnSetFocus(pOldWnd);
    if( m_pEdit ) m_pEdit->m_pwndListBox->SetFocus();
}
