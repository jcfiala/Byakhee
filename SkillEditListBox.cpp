// SkillEditListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "SkillEditListBox.h"
#include "Investigator.h"
#include "ChooseSkillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrlFrame

CSkillEditCtrlFrame::CSkillEditCtrlFrame()
{
}

CSkillEditCtrlFrame::~CSkillEditCtrlFrame()
{
}


BEGIN_MESSAGE_MAP(CSkillEditCtrlFrame, CStatic)
	//{{AFX_MSG_MAP(CSkillEditCtrlFrame)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrlFrame message handlers

void CSkillEditCtrlFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    //don't call base class
}

/////////////////////////////////////////////////////////////////////////////
// CSkillEditListBox

CSkillEditListBox::CSkillEditListBox()
{
    m_pWndStatic = NULL;
    m_pInvestigator = NULL;
    m_fOccupationalOnly = FALSE;
    m_pInvestigatorSkill = NULL;
    m_fPercentageEditing = FALSE;
    m_fSpecialEditing = FALSE;
    m_fCtrlInit = FALSE;
    m_pNamedOptionManager = NULL;
}

CSkillEditListBox::~CSkillEditListBox()
{
    if( m_pNamedOptionManager ) delete m_pNamedOptionManager;
}


BEGIN_MESSAGE_MAP(CSkillEditListBox, CListBox)
	//{{AFX_MSG_MAP(CSkillEditListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChange)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillEditListBox message handlers

void CSkillEditListBox::SetPointsDisplayWnd(CStatic * pWndStatic)
{
    ASSERT(pWndStatic);

    m_pWndStatic = pWndStatic;
}

void CSkillEditListBox::SetInvestigator( BOOL fOccupationalOnly, CInvestigator * pInvestigator)
{
    ASSERT(pInvestigator);

    m_fOccupationalOnly = fOccupationalOnly;
    m_pInvestigator = pInvestigator;
    if( m_pNamedOptionManager ) delete m_pNamedOptionManager;
    m_pNamedOptionManager = new CNamedOptionManager( SkillManager.GetKeyPairValue( pInvestigator->m_strRuleFile, "Options", "options.lst" ), 8);
}

void CSkillEditListBox::OnSelChange() 
{
    if( IsPercentageEditing() ) EndPercentageEditing();
    if( IsSpecialEditing() ) EndSpecialEditing();
}

void CSkillEditListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    //draw selection background colour
    if( lpDrawItemStruct->itemState & ODS_SELECTED )
    {
        FillRect( lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, (HBRUSH)(COLOR_HIGHLIGHT+1) );
        SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHT) );
        SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT) );
    }
    else
    {
        FillRect( lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, (HBRUSH)(COLOR_WINDOW+1) );
        SetBkColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOW) );
        SetTextColor( lpDrawItemStruct->hDC, GetSysColor(COLOR_WINDOWTEXT) );
    }

    //draw the text
    CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)lpDrawItemStruct->itemData;
	if( pInvestigatorSkill )
	{
		//set the font
		SetTextAlign( lpDrawItemStruct->hDC, TA_LEFT|VTA_BASELINE );
        HFONT hFontOld = NULL;
		if( pInvestigatorSkill->m_fOccupational ) //draw occupational skills differently
		{
			//get the current font
			CFont* pFont = GetFont();

			//get the LOGFONT and toggle bold
			LOGFONT lf;
			pFont->GetLogFont( &lf );
			lf.lfWeight = ( lf.lfWeight == 0 || lf.lfWeight == FW_NORMAL || lf.lfWeight == FW_REGULAR ) ? FW_BOLD : FW_NORMAL;

			//create a new font based on it
			CFont NewFont;
			NewFont.CreateFontIndirect( &lf );
			hFontOld = (HFONT)SelectObject( lpDrawItemStruct->hDC, HFONT(NewFont) );
		}

        CString strText;
		if( pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
		{
            CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSkill->m_pSkill;

            strText = pSubSkill->m_pSkill->m_strName;
			strText += ": ";
			strText += pSubSkill->m_strName;

            if( pSubSkill->m_strName.GetLength() == 0 )
            {
				SIZE sz;
				GetTextExtentPoint32( lpDrawItemStruct->hDC, LPCSTR(strText), strText.GetLength(), &sz );
				
				RECT rc;
				rc.top = lpDrawItemStruct->rcItem.top + 5;
				rc.left = lpDrawItemStruct->rcItem.left + 8 + sz.cx;
				rc.right = lpDrawItemStruct->rcItem.right - 48;
				rc.bottom = lpDrawItemStruct->rcItem.bottom - 5;
				DrawFocusRect( lpDrawItemStruct->hDC, &rc );
            }
		}
        else
		    strText = pInvestigatorSkill->m_pSkill->m_strName;

        if( pInvestigatorSkill->m_pSkill->m_SkillType == ANYSKILL )
		{
			RECT rc;
			rc.top = lpDrawItemStruct->rcItem.top + 5;
			rc.left = lpDrawItemStruct->rcItem.left + 4;
			rc.right = lpDrawItemStruct->rcItem.right - 48;
			rc.bottom = lpDrawItemStruct->rcItem.bottom - 5;
			DrawFocusRect( lpDrawItemStruct->hDC, &rc );
			rc.top = lpDrawItemStruct->rcItem.top + 5;
			rc.left = lpDrawItemStruct->rcItem.right - 40;
			rc.right = lpDrawItemStruct->rcItem.right - 4;
			rc.bottom = lpDrawItemStruct->rcItem.bottom - 5;
			DrawFocusRect( lpDrawItemStruct->hDC, &rc );
			strText = ""; 
		}

        if( strText.GetLength() > 0 && strText[0] == '*' )
        {
            const char * pszName = LPCSTR(strText);
            while( *pszName == '*' ) pszName++;
            CString strNewText = pszName;

		    //draw the text, without a leading '*'
		    TextOut( lpDrawItemStruct->hDC, 2+lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top + ((lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) / 2 ) + 3, strNewText, strNewText.GetLength() );
        }
        else
		    //draw the text
		    TextOut( lpDrawItemStruct->hDC, 2+lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top + ((lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) / 2 ) + 3, LPCSTR(strText), strText.GetLength() );

        //put the old font back again
        if( hFontOld ) DeleteObject( SelectObject( lpDrawItemStruct->hDC, hFontOld ) );

		//draw the skill chance
		if( pInvestigatorSkill->m_pSkill->m_SkillType != ANYSKILL )
		{
			strText.Format( "%d%%", pInvestigatorSkill->m_nCurrent );
			SetTextAlign( lpDrawItemStruct->hDC, TA_LEFT|VTA_BASELINE );
			TextOut( lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.right - 36, lpDrawItemStruct->rcItem.top + ((lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) / 2 ) + 3, LPCSTR(strText), strText.GetLength() );
		}
	}

    //draw the focus rectangle
    if( lpDrawItemStruct->itemState & ODS_FOCUS ) DrawFocusRect( lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem );
}

BOOL CSkillEditListBox::Initialise()
{
    //set the height of each list box element
    SetItemHeight( -1, 20 );

    //delete all existing strings
    ResetContent();

    //add all of the investigator's skills (or at least the occupational ones, if m_fOccupationalOnly == TRUE)
    if( m_pInvestigator == NULL )
    {
        ASSERT(FALSE);
        return FALSE;
    }

    for( int i = 0; i < m_pInvestigator->m_AllSkillList.GetSize(); i++ )
    {
        CInvestigatorSkill* pInvestigatorSkill = m_pInvestigator->m_AllSkillList.GetAt(i);
        SkillType Type = pInvestigatorSkill ? pInvestigatorSkill->m_pSkill->m_SkillType : NO_TYPE;
        if( pInvestigatorSkill && ( ( m_fOccupationalOnly == TRUE && pInvestigatorSkill->m_fOccupational ) || ( m_fOccupationalOnly == FALSE ) ) && pInvestigatorSkill->m_pSkill->IsEra(m_pInvestigator->m_Era) && ( ( m_fOccupationalOnly == FALSE && Type != ANYSKILL ) || ( m_fOccupationalOnly == TRUE ) ) && Type != NO_TYPE )
        {
            //prepare the string - trim leading *s or set special name
            CString strSkillName;
            switch( Type )
            {
                case ANYSKILL:
                    strSkillName = "(free choice)"; break;

                case SUBSKILL:
                {
                    //sort on this subskill's parent skill name
                    CSubSkill* pSubSkill = (CSubSkill*)pInvestigatorSkill->m_pSkill;
                    const char *pBuf = LPCSTR(pSubSkill->m_pSkill->m_strName);
                    while( *pBuf == '*' ) pBuf++;
                    strSkillName = pBuf;
                    break;
                }

                default:
                {
                    const char *pBuf = LPCSTR(pInvestigatorSkill->m_pSkill->m_strName);
                    while( *pBuf == '*' ) pBuf++;
                    strSkillName = pBuf;
                    break;
                }
            }
            
            //add the skill
            if( Type != SPECIALSKILL && *LPCSTR(pInvestigatorSkill->m_pSkill->m_strBase) != '*' ) //don't add special skill types
            {
                int nPosition = AddString( strSkillName );
			    if( nPosition != LB_ERR ) SetItemData( nPosition, (DWORD)pInvestigatorSkill );
            }
        }
    }

    //create the child controls if it doesn't exist
    if( !m_fCtrlInit )
    {
        m_fCtrlInit = TRUE;

        CRect rc; rc.SetRectEmpty();

        //create the edit control
        m_wndEditCtrl.Create( ES_NUMBER, rc, this, 0xFFFF );
        m_wndEditCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE );

        //create the combo control
        m_wndComboCtrl.Create( CBS_DROPDOWN|CBS_SORT|WS_VSCROLL|CBS_AUTOHSCROLL, rc, this, 0xFFFF );
        m_wndComboCtrl.m_wndComboEditCtrl.SetOwnerList( this );

        //set the font on the child controls
        m_wndEditCtrl.SetFont( GetFont() );
        m_wndComboCtrl.SetFont( GetFont() );
    }


    return TRUE;
}

BOOL CSkillEditListBox::IsPercentageEditing()
{
    return m_fPercentageEditing;
}

BOOL CSkillEditListBox::IsSpecialEditing()
{
    return m_fSpecialEditing;
}

void CSkillEditListBox::BeginPercentageEditing()
{
    m_fPercentageEditing = TRUE;

    //get the selected item's skill pointer
    int iItem = GetCurSel();
    m_pInvestigatorSkill = (CInvestigatorSkill*)GetItemData(iItem);

    //stop editing if this isn't a normal skill
	if( m_pInvestigatorSkill->m_pSkill->m_SkillType == ANYSKILL || (m_pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL && ((CSubSkill*)m_pInvestigatorSkill->m_pSkill)->m_strName.GetLength() == 0 ) )
	{
		EndPercentageEditing( FALSE );
		return;
	}

    //get the selected item's position in the list box window, and the list box window rect itself
    CRect rcItem, rcListBox;
    GetItemRect( iItem, &rcItem );
    GetClientRect( &rcListBox );

    //prepare the selection rectangle
    rcItem.top += 1;
    rcItem.left = rcListBox.right - 40;
    rcItem.right = rcItem.left + 38;
    rcItem.bottom = rcItem.top + 18;
    m_wndEditCtrlFrame.Create( "", WS_CHILD|WS_VISIBLE, rcItem, this );
    m_wndEditCtrl.SetParent( &m_wndEditCtrlFrame );

    //prepare and set the initial value
	CString strCurrent;
    m_nOldValue = m_pInvestigatorSkill->m_nCurrent;
	strCurrent.Format( "%d", m_nOldValue );
	m_wndEditCtrl.SetWindowText( strCurrent );

	m_wndEditCtrl.SetSel( 0, -1 );
	m_wndEditCtrl.SetWindowPos( &wndTop, 0, 0, rcItem.Width(), rcItem.Height(), SWP_SHOWWINDOW );
    m_wndEditCtrl.SetOwnerList( this );

    //create the spin control
    m_wndEditCtrl.m_wndSpinButtonCtrl.Create( WS_VISIBLE|UDS_SETBUDDYINT|UDS_ALIGNRIGHT|UDS_NOTHOUSANDS, rcItem, &m_wndEditCtrlFrame, 0xFFFF );
    m_wndEditCtrl.m_wndSpinButtonCtrl.SetRange( m_pInvestigatorSkill->m_nMinimum, 999 );
    m_wndEditCtrl.m_wndSpinButtonCtrl.SetPos( m_nOldValue );
    m_wndEditCtrl.m_wndSpinButtonCtrl.SetBuddy( &m_wndEditCtrl );

    //activate the edit control
    m_wndEditCtrl.SetFocus();
}

void CSkillEditListBox::ChangePercentageEditing()
{
    EndPercentageEditing();
    BeginPercentageEditing();
}

void CSkillEditListBox::EndPercentageEditing( BOOL fSaveChanges /*TRUE*/ )
{
	if( m_fPercentageEditing == FALSE ) return;
	
	if( fSaveChanges )
	{
		CString strBuffer;
		m_wndEditCtrl.GetWindowText( strBuffer );
		int nNewValue = atoi(strBuffer);

		BOOL fSomethingFishy = FALSE;

		//don't let it be set below the base value
		if( nNewValue < m_pInvestigatorSkill->m_nMinimum )
		{
			fSomethingFishy = TRUE;
			nNewValue = m_pInvestigatorSkill->m_nMinimum;
		}
		
		//make sure they've got enough skill points left
        m_pWndStatic->GetWindowText( strBuffer );
        int nSkillPoints = atoi(strBuffer);

		if( nSkillPoints - (nNewValue - m_nOldValue) < 0 )
		{
			nNewValue = m_nOldValue + nSkillPoints;
			fSomethingFishy = TRUE;
		}

		//update the value
		nSkillPoints -= (nNewValue - m_nOldValue);
        strBuffer.Format( "%d", nSkillPoints );
        m_pWndStatic->SetWindowText( strBuffer );

		//anything wrong?
		if( fSomethingFishy ) MessageBeep(0xFFFFFFFF);

		//update value in the skill
		m_pInvestigatorSkill->m_nCurrent = nNewValue;
	}

    //stop editing
	m_fPercentageEditing = FALSE;
    m_wndEditCtrl.ShowWindow(SW_HIDE);
    m_wndEditCtrl.SetParent( this );
    m_wndEditCtrl.m_wndSpinButtonCtrl.DestroyWindow();
    m_wndEditCtrlFrame.DestroyWindow();
}


void CSkillEditListBox::BeginSpecialEditing()
{
    m_fSpecialEditing = TRUE;

    //get the selected item's skill pointer
    int iItem = GetCurSel();
    m_pInvestigatorSkill = (CInvestigatorSkill*)GetItemData(iItem);

    //stop editing if this isn't a subset skill
	if( m_pInvestigatorSkill->m_pSkill->m_SkillType != SUBSKILL )
	{
		EndSpecialEditing( FALSE );
		return;
	}
    m_fSpecialEditing = FALSE;

    //get the superskill name
    CString strSuperSkillName = ((CSubSkill*)m_pInvestigatorSkill->m_pSkill)->m_pSkill->m_strName;

    //get the selected item's position in the list box window, and the list box window rect itself
    CRect rcItem, rcListBox;
    GetItemRect( iItem, &rcItem );

    rcItem.bottom -= 1;
    rcItem.right -= 48;

    //show the combo box
    m_wndComboCtrl.SetWindowPos( &wndTop, rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height() + 120, 0 );
    m_wndComboCtrl.SetOwnerList( this );

    //subclass the combo box's edit control
    CWnd* pWnd = m_wndComboCtrl.GetWindow( GW_CHILD );
    if( pWnd ) 
    {
        m_wndComboCtrl.m_wndComboEditCtrl.SubclassWindow( pWnd->m_hWnd );
        m_wndComboCtrl.m_wndComboEditCtrl.m_fSubclassed = TRUE;
    }

    //add all items
    m_wndComboCtrl.ResetContent();
    FillComboWithOptions( strSuperSkillName );

    //prepare and set the initial value
	m_wndComboCtrl.SetWindowText( m_pInvestigatorSkill->m_pSkill->m_strName );
	m_wndComboCtrl.SetEditSel( 0, -1 );

    //activate the combo box
    m_wndComboCtrl.ShowWindow( SW_SHOW );
    m_wndComboCtrl.SetFocus();

    m_fSpecialEditing = TRUE;
}

void CSkillEditListBox::ChangeSpecialEditing()
{
    EndSpecialEditing();
    BeginSpecialEditing();
}

void CSkillEditListBox::EndSpecialEditing( BOOL fSaveChanges /*TRUE*/ )
{
	if( m_fSpecialEditing == FALSE ) return;
	
	if( fSaveChanges )
	{
		CString strBuffer;
		m_wndComboCtrl.GetWindowText( strBuffer );
		m_pInvestigatorSkill->m_pSkill->m_strName = strBuffer;
	}

    //stop editing
	m_fSpecialEditing = FALSE;
    if( m_wndComboCtrl.m_wndComboEditCtrl.m_fSubclassed == TRUE )
    {
        m_wndComboCtrl.m_wndComboEditCtrl.m_fSubclassed = FALSE;
        m_wndComboCtrl.m_wndComboEditCtrl.UnsubclassWindow();
    }
    m_wndComboCtrl.ShowWindow(SW_HIDE);
    SetFocus(); //this line is need, otherwise the focus seems to stay with the hidden m_wndComboCtrl.m_wndComboEditCtrl window
}



/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrl

CSkillEditCtrl::CSkillEditCtrl()
{
    m_pOwnerList = NULL;
}

CSkillEditCtrl::~CSkillEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkillEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CSkillEditCtrl)
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillEditCtrl message handlers

void CSkillEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
			break;

		case VK_ESCAPE:
			break;

        default:
	        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

UINT CSkillEditCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS;
}

void CSkillEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
    m_pOwnerList->EndPercentageEditing();
}

void CSkillEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_UP:
		{	
			int n = m_pOwnerList->GetCurSel();
			if( n > 0 ) 
			{
				m_pOwnerList->SetCurSel( --n );
				m_pOwnerList->ChangePercentageEditing();
			}
			else MessageBeep(0xFFFFFFFF);
			break;
		}

		case VK_DOWN:
		{	
			int n = m_pOwnerList->GetCurSel();
			if( n < m_pOwnerList->GetCount() - 1 ) 
			{
				m_pOwnerList->SetCurSel( ++n );
				m_pOwnerList->ChangePercentageEditing();
			}
			else
				MessageBeep(0xFFFFFFFF);
			break;
		}

		case VK_RETURN:
		case VK_ESCAPE:
            //don't call default handler
			break;

		default:
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CSkillEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
        {
			int n = m_pOwnerList->GetCurSel();
			if( n < m_pOwnerList->GetCount() - 1 ) 
			{
				m_pOwnerList->SetCurSel( ++n );
				m_pOwnerList->ChangePercentageEditing();
			}
			else
			    m_pOwnerList->EndPercentageEditing();
			break;
        }

		case VK_ESCAPE:
			m_pOwnerList->EndPercentageEditing( FALSE );
			break;

		default:
			CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

void CSkillEditCtrl::SetOwnerList( CSkillEditListBox* pOwnerList )
{
    m_pOwnerList = pOwnerList;
}

void CSkillEditListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    //get the selected item's skill pointer
    int iItem = GetCurSel();
    CInvestigatorSkill* pInvestigatorSkill = (CInvestigatorSkill*)GetItemData(iItem);

    //determine how to edit, depending on the item selected
	if( pInvestigatorSkill->m_pSkill->m_SkillType == ANYSKILL )
	{
		CChooseSkillDlg ChooseSkillDlg;
		ChooseSkillDlg.m_strCustomPrompt = "Please select a skill as a personal specialty";
		ChooseSkillDlg.m_pInvestigator = m_pInvestigator;
        ChooseSkillDlg.SetAsOccupational();
 		if( ChooseSkillDlg.DoModal() == IDOK ) 
        {
            //remove the ANYSKILL
            m_pInvestigator->RemoveSkill( pInvestigatorSkill );

            //reset the list
            Initialise();
        }
	}
	else
	{
		if( pInvestigatorSkill->m_pSkill->m_SkillType == SUBSKILL )
		{
            //determine x position of double click
            CRect rcListBox;
            GetClientRect( &rcListBox );

            //prepare the selection rectangle
            if( point.x < rcListBox.right - 40 || ((CSubSkill*)pInvestigatorSkill->m_pSkill)->m_strName.GetLength() == 0 )
                BeginSpecialEditing();
            else
                BeginPercentageEditing();
		}
		else
			BeginPercentageEditing();
	}

    //don't call base class
}
/////////////////////////////////////////////////////////////////////////////
// CSkillComboCtrl

CSkillComboCtrl::CSkillComboCtrl()
{
}

CSkillComboCtrl::~CSkillComboCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkillComboCtrl, CComboBox)
	//{{AFX_MSG_MAP(CSkillComboCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillComboCtrl message handlers

void CSkillEditListBox::FillComboWithOptions(CString strOptionName)
{
    const char* pszOptionName = LPCSTR(strOptionName);
    if( pszOptionName )
    {
        while( *pszOptionName == '*' ) pszOptionName++;
        m_pNamedOptionManager->FillComboWithOptions( &m_wndComboCtrl, pszOptionName );
    }
    else
        m_wndComboCtrl.ResetContent();
}

void CSkillComboCtrl::SetOwnerList( CSkillEditListBox * pOwnerList )
{
    m_pOwnerList = pOwnerList;
}

void CSkillEditListBox::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);

    //stop editing if we get the focus
    if( IsPercentageEditing() ) EndPercentageEditing();
    if( IsSpecialEditing() ) EndSpecialEditing();
}


/////////////////////////////////////////////////////////////////////////////
// CSkillComboEditCtrl

CSkillComboEditCtrl::CSkillComboEditCtrl()
{
    m_pOwnerList = NULL;
    m_fSubclassed = FALSE;
}

CSkillComboEditCtrl::~CSkillComboEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkillComboEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CSkillComboEditCtrl)
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillComboEditCtrl message handlers

void CSkillComboEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
			break;

		case VK_ESCAPE:
			break;

        default:
	        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

UINT CSkillComboEditCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS;
}

void CSkillComboEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_UP:
		{	
			int n = m_pOwnerList->GetCurSel();
			if( n > 0 ) 
			{
				m_pOwnerList->SetCurSel( --n );
				m_pOwnerList->ChangeSpecialEditing();
			}
			else MessageBeep(0xFFFFFFFF);
			break;
		}

		case VK_DOWN:
		{	
			int n = m_pOwnerList->GetCurSel();
			if( n < m_pOwnerList->GetCount() - 1 ) 
			{
				m_pOwnerList->SetCurSel( ++n );
				m_pOwnerList->ChangeSpecialEditing();
			}
			else
				MessageBeep(0xFFFFFFFF);
			break;
		}

		case VK_RETURN:
		case VK_ESCAPE:
            //don't call default handler
			break;

		default:
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CSkillComboEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch( nChar )
	{
		case VK_RETURN:
        {
			int n = m_pOwnerList->GetCurSel();
			if( n < m_pOwnerList->GetCount() - 1 ) 
			{
				m_pOwnerList->SetCurSel( ++n );
				m_pOwnerList->ChangeSpecialEditing();
			}
			else
			    m_pOwnerList->EndSpecialEditing();
			break;
        }

		case VK_ESCAPE:
			m_pOwnerList->EndSpecialEditing( FALSE );
			break;

		default:
			CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

void CSkillComboEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
    m_pOwnerList->EndSpecialEditing();
}

void CSkillComboEditCtrl::SetOwnerList(CSkillEditListBox * pOwnerList)
{
    m_pOwnerList = pOwnerList;
}
