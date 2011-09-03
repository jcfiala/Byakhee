// SheetCommand.cpp: implementation of the CSheetCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "byakhee.h"
#include "SheetCommand.h"
#include "FieldEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawSheetCommand::CDrawSheetCommand( SheetCommandType Type, CRect rc, CString strText, DWORD dwFlags /*0*/ ) : CSheetCommand( Type )
{
    m_rc = rc;
    m_strText = strText;
    m_dwFlags = dwFlags;
    m_nValue = 0;
}

CDrawSheetCommand::CDrawSheetCommand( SheetCommandType Type, CRect rc, int nValue /*0x000000*/ ) : CSheetCommand( Type )
{
    m_rc = rc;
    m_nValue = nValue;
    m_dwFlags = 0;
}

CDrawSheetCommand::CDrawSheetCommand( SheetCommandType Type, int nValue, int nValue2 /*0*/  ) : CSheetCommand( Type )
{
    m_nValue = nValue;
    m_nValue2 = nValue2;
    m_dwFlags = 0;
}

CFontSheetCommand::CFontSheetCommand( SheetCommandType Type, CString strFace, int nPointSize, int nTextAttributes, int nAngle /*0*/ ) : CSheetCommand( Type )
{
    m_strFace = strFace;
    m_nPointSize = nPointSize;
    m_nTextAttributes = nTextAttributes;
    m_nAngle = nAngle;
}

CFieldSheetCommand::CFieldSheetCommand( CRect rc, CField* pField ) : CSheetCommand( CommandType_Field )
{
    m_fActive = TRUE;
    m_dwParam1 = m_dwParam2 = 0;
    m_rc = rc;
    m_pField = pField;
    m_strOptionName = pField->m_pszOptionName;

    m_pLastUsedFont = NULL;
    m_crFontColour = 0x000000;
    m_dwFieldStyle = 0;
    m_nCurrentUserTextSize = m_nCurrentUserTextAngle = 0;
}

CTableFieldSheetCommand::CTableFieldSheetCommand( CRect rc, CField* pField, Variant value ) : CFieldSheetCommand( rc, pField )
{
    m_Type = CommandType_TableEntry;
    m_value = value;
}

void CFieldSheetCommand::SetCursor()
{
    if( m_fActive == FALSE || m_pField->m_dwFlags & FIELD_READONLY ) ::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
        else if( m_pField->m_dwFlags & (FIELD_BOOLEAN|FIELD_PICTURE) ) ::SetCursor( AfxGetApp()->LoadCursor(IDC_HAND) );
            else ::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_IBEAM) );
}

void CTableFieldSheetCommand::SetCursor()
{
    if( m_fActive == FALSE || m_pField->m_dwFlags & FIELD_READONLY ) ::SetCursor( AfxGetApp()->LoadStandardCursor(IDC_ARROW) );
        else ::SetCursor( AfxGetApp()->LoadCursor(IDC_HAND) );
}

void CDrawSheetCommand::Draw( CSheetDrawInfo* pDrawInfo )
{
    switch( m_Type )
    {
        case CommandType_DrawLine:          
            pDrawInfo->m_pDC->MoveTo( m_rc.TopLeft() );
            pDrawInfo->m_pDC->LineTo( m_rc.BottomRight() );
            break;

        case CommandType_DrawPicture: 
            pDrawInfo->DrawPicture( m_strText, m_rc );
            break;

        case CommandType_StretchPicture:
            pDrawInfo->StretchPicture( m_strText, m_rc );
            break;

        case CommandType_DrawRectangle:     
        {
            CBrush* pOldBrush = pDrawInfo->m_pDC->SelectObject( CBrush::FromHandle( (HBRUSH)::GetStockObject(NULL_BRUSH)) );
            pDrawInfo->m_pDC->Rectangle(&m_rc);
            pDrawInfo->m_pDC->SelectObject( pOldBrush );
            break;
        }

        case CommandType_FillRectangle:     
        {
            CBrush Brush( m_nValue );
            CPen Pen( PS_SOLID, 0, m_nValue );
            CBrush* pOldBrush = pDrawInfo->m_pDC->SelectObject( &Brush );
            CPen* pOldPen = pDrawInfo->m_pDC->SelectObject( &Pen );
            pDrawInfo->m_pDC->Rectangle(&m_rc);
            pDrawInfo->m_pDC->SelectObject( pOldPen );
            pDrawInfo->m_pDC->SelectObject( pOldBrush );
            break;
        }

        case CommandType_DrawText:          
            pDrawInfo->DrawText( m_strText, m_rc.TopLeft(), m_dwFlags );
            break;

        case CommandType_DrawTextBox:
            pDrawInfo->DrawText( m_strText, m_rc, m_dwFlags );
            break;

        case CommandType_SetLineColour: 
            pDrawInfo->ChangePen( m_nValue ); 
            break;

        case CommandType_SetTextColour: 
            pDrawInfo->m_crText = m_nValue; 
            pDrawInfo->m_pDC->SetTextColor(m_nValue); 
            break;

        case CommandType_SetFieldTextStyle: 
            pDrawInfo->m_dwFieldFormatting = m_nValue; 
            break;

        case CommandType_SetUserTextSize:
            pDrawInfo->ChangeUserText( m_nValue );
            break;

        case CommandType_SetUserTextSizeRotate:
            pDrawInfo->ChangeUserText( m_nValue, m_nValue2 );
            break;

        case CommandType_SetSheetParameter:
            //nothing to draw
            break;
    }
}

void CFontSheetCommand::Draw( CSheetDrawInfo* pDrawInfo )
{
    switch( m_Type )
    {
        case CommandType_SetFont:
        case CommandType_SetFontRotated:
            pDrawInfo->ChangeFont( m_strFace, m_nPointSize, m_nTextAttributes, m_nAngle );
            pDrawInfo->m_pLastUsedFont = this;
            break;
    }
}

void CFieldSheetCommand::Draw( CSheetDrawInfo* pDrawInfo )
{
    m_pLastUsedFont = pDrawInfo->m_pLastUsedFont;
    m_crFontColour = pDrawInfo->m_crText;
    m_dwFieldStyle = pDrawInfo->m_dwFieldFormatting;
    m_nCurrentUserTextSize = pDrawInfo->m_nCurrentUserTextSize;
    m_nCurrentUserTextAngle = pDrawInfo->m_nCurrentUserTextAngle;
    
    //check for special field properties
    if( m_pField->m_dwFlags & FIELD_PICTURE )       pDrawInfo->DrawPicture( LPCSTR(m_pField->GetValue(this)), m_rc );
    else if( m_pField->m_dwFlags & FIELD_BOOLEAN )  pDrawInfo->DrawCheckBox( m_rc, m_pField->GetValue(this) );
    else                                            
        pDrawInfo->DrawText( LPCSTR(m_pField->GetValue(this)), m_rc, pDrawInfo->m_dwFieldFormatting );
}

void CTableFieldSheetCommand::Draw( CSheetDrawInfo* pDrawInfo )
{
    m_pLastUsedFont = pDrawInfo->m_pLastUsedFont;
    m_crFontColour = pDrawInfo->m_crText;
    m_dwFieldStyle = pDrawInfo->m_dwFieldFormatting;
    m_nCurrentUserTextSize = pDrawInfo->m_nCurrentUserTextSize;
    m_nCurrentUserTextAngle = pDrawInfo->m_nCurrentUserTextAngle;

    //see if this table value is activated
    if( m_pField->GetValue(this) == m_value )
    {
        //draw a circle around it
        CBrush* pOldBrush = pDrawInfo->m_pDC->SelectObject( CBrush::FromHandle( (HBRUSH)::GetStockObject(NULL_BRUSH)) );
        pDrawInfo->SelectUserPen();
        pDrawInfo->m_pDC->Ellipse( &m_rc );
        pDrawInfo->DeselectUserPen();
        pDrawInfo->m_pDC->SelectObject( pOldBrush );
    }
}

void CFieldSheetCommand::BeginEditing(CFieldEdit *pFieldEdit)
{
    if( m_fActive && !(m_pField->m_dwFlags & FIELD_READONLY) )
    {
        //edit boolean
        if( m_pField->m_dwFlags & FIELD_BOOLEAN ) 
        {
            pFieldEdit->m_pOwner->StoreUndo();
            m_pField->SetValue( !int(m_pField->GetValue(this)), this );
            pFieldEdit->m_pOwner->RedrawFields(this);
        }
        else
            //edit picture
            if( m_pField->m_dwFlags & FIELD_PICTURE )
            {
                CString strFileName = LPCSTR(m_pField->GetValue(this));
                if( CByakheeApp::BrowseForPicture( strFileName ) )
                {
                    pFieldEdit->m_pOwner->StoreUndo();
                    m_pField->SetValue( LPCSTR(strFileName), this );
                    pFieldEdit->m_pOwner->RedrawFields(this);
                }
            }
            else
                //edit text/number
                pFieldEdit->BeginEditing(this);
    }
}

void CTableFieldSheetCommand::BeginEditing(CFieldEdit *pFieldEdit)
{
    //set the field value to our value
    if( m_fActive && !(m_pField->m_dwFlags & FIELD_READONLY) )
    {
        pFieldEdit->m_pOwner->StoreUndo();
        m_pField->SetValue( m_value, this );
        pFieldEdit->m_pOwner->RedrawFields( this );
    }
}
