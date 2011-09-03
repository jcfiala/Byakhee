// SheetDrawInfo.cpp: implementation of the CSheetDrawInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "byakhee.h"
#include "SheetDrawInfo.h"
#include "MainFrm.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSheetDrawInfo::CSheetDrawInfo( CFieldManager* pFieldManager, CDC* pDC, CSize sizeSheet )
{
    //erase the background
    m_sizeSheet = sizeSheet;
    pDC->FillRect( CRect(0,0,sizeSheet.cx,-sizeSheet.cy), CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH)) );

    //store the field manager
    m_pFieldManager = pFieldManager;

    //set the default values
    m_crLine = 0x000000;
    m_crText = 0x000000;
    m_nCurrentTextAngle = 0;
    m_dwFieldFormatting = TEXTSTYLE_NORMAL;
    m_pLastUsedFont = NULL;
    m_nCurrentUserTextSize = 100;
    m_nCurrentUserTextAngle = 0;
    m_nShadowOffset = 0;

    //set the default text display styles
    pDC->SetBkMode( TRANSPARENT );
    pDC->SetTextAlign(TA_TOP|TA_LEFT);
    pDC->SetTextColor( m_crText );

    //set the default font
	m_pFont = new CFont;
    m_pFont->CreatePointFont( 100, "Times New Roman", pDC );
	m_pOldFont = pDC->SelectObject( m_pFont );

    //set the default pen
    m_pPen = new CPen(PS_SOLID, 0, m_crLine );
    m_pOldPen = pDC->SelectObject( m_pPen );

    //store the device context
    m_pDC = pDC;

    //load user text colour parameters
	m_crUserText = AfxGetApp()->GetProfileInt( "Sheet", "UserTextColour", RGB(0,0,128) );
	m_strUserTextFace = AfxGetApp()->GetProfileString( "Sheet", "UserTextFace", "Comic Sans MS" );
    m_fUserTextItalic = AfxGetApp()->GetProfileInt( "Sheet", "UserTextItalic", FALSE );
    m_nUserTextWeight = AfxGetApp()->GetProfileInt( "Sheet", "UserTextWeight", 0 );

    //create user pen and initialise user text placeholder
    m_pUserFont = NULL;
    m_nUserTextAngle = 0;
    m_pUserPen = new CPen( PS_SOLID, 0, m_crUserText );
    m_fUserTextSelected = FALSE;
    ChangeUserText( m_nCurrentUserTextSize );
}

CSheetDrawInfo::~CSheetDrawInfo()
{
    //reset the font
	m_pDC->SelectObject( m_pOldFont );
    delete m_pFont;
    m_pFont = NULL;

    //reset the pen
    m_pDC->SelectObject( m_pOldPen );
    delete m_pPen;
    m_pPen = NULL;

    delete m_pUserPen;
    delete m_pUserFont;
}

void CSheetDrawInfo::ChangePen( COLORREF crColour )
{
    //create the new pen
    CPen* pPen = new CPen( PS_SOLID, 0, crColour );
    m_crLine = crColour;

    //use the new pen
    m_pDC->SelectObject( pPen );
    delete m_pPen;
    m_pPen = pPen;
}

void CSheetDrawInfo::ChangeFont( CString strFaceName, int nPointSize, int nFontStyle /*0*/, int nRotation /*0*/, int nWeight /*0*/ )
{
    //create the font object
    CFont* pFont = new CFont();

    //initialise the logfont
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = nPointSize;
    if( nFontStyle & FONTSTYLE_BOLD ) lf.lfWeight = FW_BOLD; else lf.lfWeight = FW_NORMAL;
    if( nWeight != 0 ) lf.lfWeight = nWeight;
    if( nFontStyle & FONTSTYLE_ITALIC ) lf.lfItalic = TRUE;
    if( nFontStyle & FONTSTYLE_UNDERLINE ) lf.lfUnderline = TRUE;
    if( nFontStyle & FONTSTYLE_STRIKETHROUGH ) lf.lfStrikeOut = TRUE;
    if( nRotation != 0 )
    {
        lf.lfEscapement = lf.lfOrientation = nRotation;
        lf.lfClipPrecision = CLIP_LH_ANGLES; //this nice little flag fixes font rotation problems experienced in Byakhee 1 & 2
    }
    m_nCurrentTextAngle = nRotation / 10;
    while( m_nCurrentTextAngle < 0 ) m_nCurrentTextAngle += 360; while( m_nCurrentTextAngle > 360 ) m_nCurrentTextAngle -= 360;
	strncpy( lf.lfFaceName, LPCSTR(strFaceName), LF_FACESIZE-1 );

    //create the font
	pFont->CreatePointFontIndirect(&lf, m_pDC );

    //use the new font
    m_pDC->SelectObject( pFont );
    delete m_pFont;
    m_pFont = pFont;
}

void CSheetDrawInfo::DrawPicture( CString strFileName, CRect rc )
{
    //make sure the path is the Byakhee directory for files without path names
	SetCurrentDirectory( CByakheeApp::GetDefaultDirectory() );

    //determine file extension
    char* pszExtension = strrchr(LPCSTR(strFileName), '.' );
    if( pszExtension ) pszExtension++; else return; //no extension

    //enhanced metafile
    if( stricmp(pszExtension, "emf") == 0 )
    {
        CWaitCursor Wait;

        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading metafile ") + strFileName + CString("...") );
        HENHMETAFILE hEnhMetaFile = GetEnhMetaFile( strFileName );
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        if( hEnhMetaFile )
        {
            //get metafile information
            ENHMETAHEADER emh;
            GetEnhMetaFileHeader( hEnhMetaFile, sizeof(emh), &emh );

            //do uniform scale
            int pw = emh.rclFrame.right-emh.rclFrame.left, ph = emh.rclFrame.bottom-emh.rclFrame.top;
            int fw = rc.right-rc.left, fh = -(rc.bottom-rc.top);
            int dw = fw, dh = fh;
            int ox = 0, oy = 0;
            double AP = double(pw)/double(ph), AF = double(fw)/double(fh);

            //perform scaling
            if( AP >= AF )
                dh = int(double(fh)/(AP/AF));
            else
                dw = int(double(fw)/(AF/AP));

            //set offsets so it's centered in the field
            ox = (fw-dw) / 2;
            oy = (fh-dh) / 2;

            //plug this into the rectangle
            RECT rcDraw;
            rcDraw.left = rc.left+ox; rcDraw.top = rc.top-oy;
            rcDraw.right = rc.left+dw+ox; rcDraw.bottom = rc.top-(dh+oy);

            //draw the meta file
            PlayEnhMetaFile( m_pDC->m_hDC, hEnhMetaFile, &rcDraw );

            //destroy it
            DeleteEnhMetaFile(hEnhMetaFile); 
        }
        return;
    }

/*
    //windows metafile
    if( stricmp(pszExtension, "wmf") == 0 )
    {
        ... fixme!
    }
*/

    //pass it to the CImage object
    CWaitCursor Wait;

    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading image ") + strFileName + CString("...") );
    CImage* pImage = new CImage(strFileName);
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
    if( pImage )
    {
        DrawImage( pImage, rc );
	    delete pImage;
    }
}

void CSheetDrawInfo::StretchPicture( CString strFileName, CRect rc )
{
    //make sure the path is the Byakhee directory for files without path names
	SetCurrentDirectory( CByakheeApp::GetDefaultDirectory() );

    //determine file extension
    char* pszExtension = strrchr(LPCSTR(strFileName), '.' );
    if( pszExtension ) pszExtension++; else return; //no extension

    //enhanced metafile
    if( stricmp(pszExtension, "emf") == 0 )
    {
        CWaitCursor Wait;

        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading metafile ") + strFileName + CString("...") );
        HENHMETAFILE hEnhMetaFile = GetEnhMetaFile( strFileName );
        ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
        if( hEnhMetaFile )
        {
            //get metafile information
            ENHMETAHEADER emh;
            GetEnhMetaFileHeader( hEnhMetaFile, sizeof(emh), &emh );

            //do uniform scale
            int pw = emh.rclFrame.right-emh.rclFrame.left, ph = emh.rclFrame.bottom-emh.rclFrame.top;
            int fw = rc.right-rc.left, fh = -(rc.bottom-rc.top);
            int dw = fw, dh = fh;
            int ox = 0, oy = 0;
            double AP = double(pw)/double(ph), AF = double(fw)/double(fh);

            //perform scaling
            if( AP >= AF )
                dh = int(double(fh)/(AP/AF));
            else
                dw = int(double(fw)/(AF/AP));

            //set offsets so it's centered in the field
            ox = (fw-dw) / 2;
            oy = (fh-dh) / 2;

            //plug this into the rectangle
            RECT rcDraw;
            rcDraw.left = rc.left+ox; rcDraw.top = rc.top-oy;
            rcDraw.right = rc.left+dw+ox; rcDraw.bottom = rc.top-(dh+oy);

            //draw the meta file
            PlayEnhMetaFile( m_pDC->m_hDC, hEnhMetaFile, &rc );

            //destroy it
            DeleteEnhMetaFile(hEnhMetaFile); 
        }
        return;
    }

/*
    //windows metafile
    if( stricmp(pszExtension, "wmf") == 0 )
    {
        ... fixme!
    }
*/

    //pass it to the CImage object
    CWaitCursor Wait;

    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( CString("Loading image ") + strFileName + CString("...") );
    CImage* pImage = new CImage(strFileName);
    ((CMainFrame*)AfxGetMainWnd())->SetStatusMessage( "" );
    if( pImage )
    {
	    if( pImage->GetWidth() > 0 && pImage->GetHeight() > 0 )
	    {
            //set palette
            CPalette* pOldPalette = NULL;
            if( pImage->GetPalette() )
            {
		        pOldPalette = m_pDC->SelectPalette( pImage->GetPalette(), TRUE );
		        m_pDC->RealizePalette();
            }

            pImage->Stretch( m_pDC, rc.left, rc.top, rc.right, rc.bottom );

            //restore palette
            m_pDC->SelectPalette(pOldPalette, FALSE);
        }

        delete pImage;
    }
}


void CSheetDrawInfo::DrawImage( CImage* pImage, CRect rc )
{
    if( pImage == NULL ) return;

	if( pImage->GetWidth() > 0 && pImage->GetHeight() > 0 )
	{
        //set palette
        CPalette* pOldPalette = NULL;
        if( pImage->GetPalette() )
        {
		    pOldPalette = m_pDC->SelectPalette( pImage->GetPalette(), TRUE );
		    m_pDC->RealizePalette();
        }

        //do uniform scale
        int pw = pImage->GetWidth(), ph = pImage->GetHeight();
        int fw = rc.Width(), fh = rc.Height();
        int dw = fw, dh = fh;
        int ox = 0, oy = 0;
        double AP = double(pw)/double(ph), AF = double(fw)/double(fh);

        //perform scaling
        if( AP >= AF )
            dh = int(double(fh)/(AP/AF));
        else
            dw = int(double(fw)/(AF/AP));

        //set offsets so it's centered in the field
        ox = (fw-dw) / 2;
        oy = (fh-dh) / 2;
        
        pImage->Stretch( m_pDC, rc.left+ox, rc.bottom-oy, dw, -dh );

        //restore palette
        m_pDC->SelectPalette(pOldPalette, FALSE);
    }
}


void CSheetDrawInfo::DrawText( CString strText, CRect rc, DWORD dwDrawFlags )
{
    //build formatting
    UINT nFormat = DT_NOPREFIX;

    //turn on user text
    if( dwDrawFlags & TEXTSTYLE_USERTEXT ) SelectUserText();

    //text alignment
    if( dwDrawFlags & TEXTSTYLE_ALIGNCENTER ) nFormat |= DT_CENTER;
        else if( dwDrawFlags & TEXTSTYLE_ALIGNRIGHT ) nFormat |= DT_RIGHT;
            else dwDrawFlags |= DT_LEFT;

    //multiline
    if( dwDrawFlags & TEXTSTYLE_MULTILINE ) 
    {
        //break words if we're in a box rather than at a point
        if( rc.right != rc.left ) nFormat |= DT_WORDBREAK;
    }
    else nFormat |= DT_SINGLELINE;

    //get correct rotation value
    int nAngle = 0;
    if( m_fUserTextSelected ) nAngle = m_nUserTextAngle; else nAngle = m_nCurrentTextAngle;

    //clipping
    if( dwDrawFlags & TEXTSTYLE_CLIPPED && nAngle == 0 ) nFormat |= DT_END_ELLIPSIS; else nFormat |= DT_NOCLIP;

    //deal with rotation
    if( nAngle == 0 )
    {
        //draw underline
        if( dwDrawFlags & TEXTSTYLE_FIELDUNDERLINE )
        {
            CPen* pOldPen = m_pDC->SelectObject( CPen::FromHandle( (HPEN)::GetStockObject(BLACK_PEN)));
            TEXTMETRIC tm; m_pDC->GetOutputTextMetrics( &tm );
            BOOL fDrawnOne = FALSE;
            for( int y = -rc.top+tm.tmHeight-tm.tmDescent; y < -rc.bottom || fDrawnOne == FALSE; y += tm.tmHeight )
            {
                m_pDC->MoveTo( rc.left, -y ); 
                m_pDC->LineTo( rc.right, -y );
                fDrawnOne = TRUE;
            }
            m_pDC->SelectObject( pOldPen );
        }

        //display the text
        m_pDC->DrawText( strText, &rc, nFormat );
    }
    else
    {
        //prepare rotated field rectangle
        CPoint ptField;
        int nLength;
        switch( ((nAngle+45) % 360) / 90 )
        {
            default:
            case 0: ptField.x = rc.left;  ptField.y = -rc.top;    nLength = rc.Width();  break;
            case 1: ptField.x = rc.right; ptField.y = -rc.top;    nLength = -rc.Height(); break;
            case 2: ptField.x = rc.right; ptField.y = -rc.bottom; nLength = rc.Width();  break;
            case 3: ptField.x = rc.left;  ptField.y = -rc.bottom; nLength = -rc.Height(); break;
        }

        //field underline
        if( dwDrawFlags & TEXTSTYLE_FIELDUNDERLINE )
        {
            //FIXME: draw all field underlines on rotated multiline
            CPen* pOldPen = m_pDC->SelectObject( CPen::FromHandle( (HPEN)::GetStockObject(BLACK_PEN)));
            TEXTMETRIC tm; m_pDC->GetOutputTextMetrics( &tm );

            double dAngleTurn = (nAngle*3.142) / 180.0;
	        int x = ptField.x + nLength * cos(dAngleTurn), y = ptField.y + nLength * sin(dAngleTurn);
	        int fx = -(tm.tmHeight-tm.tmDescent) * -sin(dAngleTurn), fy = -(tm.tmHeight-tm.tmDescent) * cos(dAngleTurn);

            m_pDC->MoveTo( ptField.x-fx, -(ptField.y-fy) );
            m_pDC->LineTo( x-fx, -(y-fy) );

            m_pDC->SelectObject( pOldPen );
        }

        //build drawing rectangle
        CRect rc2;
        rc2.left = rc2.right = ptField.x;
        rc2.top = rc2.bottom = -ptField.y;

        //display the text
        m_pDC->DrawText( strText, &rc2, nFormat );
    }

    //turn off user text
    if( dwDrawFlags & TEXTSTYLE_USERTEXT ) DeselectUserText();
}

void CSheetDrawInfo::DrawText( CString strText, CPoint pt, DWORD dwDrawFlags )
{
    //build drawing rectangle
    CRect rc;
    rc.left = rc.right = pt.x;
    rc.top = rc.bottom = pt.y;

    //remove clipping parameter if it's set
    dwDrawFlags &= ~DWORD(TEXTSTYLE_CLIPPED);

    //pass drawing request onto rect version of this function
    DrawText( strText, rc, dwDrawFlags );
}


void CSheetDrawInfo::SelectUserPen()
{
    m_pDC->SelectObject( m_pUserPen );
}

void CSheetDrawInfo::DeselectUserPen()
{
    m_pDC->SelectObject( m_pPen );
}

void CSheetDrawInfo::ChangeUserText( int nPointSize, int nRotation /*0*/ )
{
    //prepare the logfont
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = nPointSize;
    lf.lfWeight = m_nUserTextWeight;
    lf.lfItalic = m_fUserTextItalic;
	strncpy( lf.lfFaceName, m_strUserTextFace, LF_FACESIZE-1 );
    if( nRotation != 0 )
    {
        lf.lfEscapement = lf.lfOrientation = nRotation;
        lf.lfClipPrecision = CLIP_LH_ANGLES; //this nice little flag fixes font rotation problems experienced in Byakhee 1 & 2
    }
    m_nUserTextAngle = nRotation / 10;
    while( m_nUserTextAngle < 0 ) m_nUserTextAngle += 360; while( m_nUserTextAngle > 360 ) m_nUserTextAngle -= 360;

    //create the font
    if( m_pUserFont ) delete m_pUserFont;
    m_pUserFont = new CFont();
    m_pUserFont->CreatePointFontIndirect( &lf, m_pDC );

    //store current values
    m_nCurrentUserTextSize = nPointSize;
    m_nCurrentUserTextAngle = nRotation;
}

void CSheetDrawInfo::SelectUserText()
{
    m_pDC->SetTextColor(m_crUserText);
    m_pDC->SelectObject( m_pUserFont );
    m_fUserTextSelected = TRUE;
}

void CSheetDrawInfo::DeselectUserText()
{
    m_pDC->SetTextColor( m_crText );
    m_pDC->SelectObject( m_pFont );
    m_fUserTextSelected = FALSE;
}

void CSheetDrawInfo::DrawCheckBox(CRect rc, BOOL fChecked )
{
    if( m_nShadowOffset )
    {
        //set shadow offset
        CRect rcShadow = rc;
        rcShadow.left += m_nShadowOffset;
        rcShadow.bottom -= m_nShadowOffset;
        rcShadow.right += m_nShadowOffset;
        rcShadow.top -= m_nShadowOffset;

        //draw checkbox shadow
        CBrush* pOldBrush = m_pDC->SelectObject( CBrush::FromHandle( (HBRUSH)::GetStockObject(BLACK_BRUSH)) );
        m_pDC->Rectangle( &rcShadow );
        m_pDC->SelectObject( pOldBrush );
    }

    //draw checkbox
    CBrush* pOldBrush = m_pDC->SelectObject( CBrush::FromHandle( (HBRUSH)::GetStockObject(WHITE_BRUSH)) );
    m_pDC->Rectangle( &rc );
    m_pDC->SelectObject( pOldBrush );

    //draw check mark if it's needed
    if( fChecked )
    {
        SelectUserPen();
        m_pDC->MoveTo( rc.left,  rc.top );
        m_pDC->LineTo( rc.right, rc.bottom );
        m_pDC->MoveTo( rc.left,  rc.bottom );
        m_pDC->LineTo( rc.right, rc.top );
        DeselectUserPen();
    }
}

