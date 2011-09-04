// SkillInformationPopupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Byakhee.h"
#include "MainFrm.h"
#include "SkillInformationPopupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillInformationPopupWnd
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

CSkillInformationPopupWnd::CSkillInformationPopupWnd( CString strRuleFile )
{
    m_pSkill = NULL;
    m_strRuleFile = strRuleFile;
    SkillManager.AddRuleFile(m_strRuleFile);
    m_hIcon = (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_INFO), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS );
}

CSkillInformationPopupWnd::~CSkillInformationPopupWnd()
{
}


BEGIN_MESSAGE_MAP(CSkillInformationPopupWnd, CPopupWnd)
	//{{AFX_MSG_MAP(CSkillInformationPopupWnd)
	ON_WM_PAINT()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSkillInformationPopupWnd message handlers

void CSkillInformationPopupWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    //set the text colour to the user's chosen tooltip colour
    dc.SetTextColor( ::GetSysColor(COLOR_INFOTEXT) );

    //change the font to the dialog font
    CFont Font, *pOldFont;
    Font.Attach( ::GetStockObject(ANSI_VAR_FONT) );
    pOldFont = dc.SelectObject( &Font );

    //set up transparent drawing
    dc.SetBkMode(TRANSPARENT);

    //draw icon
    ::DrawIconEx( dc.GetSafeHdc(), m_nWidth-20, m_nHeight-20, m_hIcon, 0, 0, 0, NULL, DI_NORMAL );

    //draw skill info
	if( m_pSkill )
    {
        int x = 5, y = 5;
        //draw skill name and underline it
        const char* pszName = m_pSkill->m_strName;
        while( *pszName == '*' ) pszName++;
        dc.TextOut( x, y, pszName );
        y += m_nCharHeight+(m_nCharHeight/2);
        dc.MoveTo( 0, y ); dc.LineTo( m_nWidth, y );
        y += (m_nCharHeight/2);

        //draw generic stuff
        dc.TextOut( x, y, GetEraDescription( m_pSkill->m_EraMask ) ); y += m_nCharHeight;

        //draw skill specific contents
        CString strText;
        switch( m_pSkill->m_SkillType )
        {
            case SKILL:
                dc.TextOut( x, y, CString("Base: ") + m_pSkill->GetBaseString(m_f5_5Mode) ); y += m_nCharHeight;
                break;

            case FIREARM:
                dc.TextOut( x, y, CString("Base: ") +              m_pSkill->GetBaseString(m_f5_5Mode) ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Shots Per Round: ") +   ((CFirearmSkill*)m_pSkill)->m_strShots ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Damage: ") +            ((CFirearmSkill*)m_pSkill)->m_strDmg ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Hit Points: ") +        ((CFirearmSkill*)m_pSkill)->m_strHP ); y += m_nCharHeight;

                dc.TextOut( x, y, CString("Bullets In Gun: ") +   ((CFirearmSkill*)m_pSkill)->m_strBullets ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Range: ") +            ((CFirearmSkill*)m_pSkill)->m_strRange ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Malfunction Number: ")+((CFirearmSkill*)m_pSkill)->m_strMAL ); y += m_nCharHeight;
                break;

            case MELEE:
                dc.TextOut( x, y, CString("Base: ") +              m_pSkill->GetBaseString(m_f5_5Mode) ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Attacks Per Round: ") + ((CMeleeSkill*)m_pSkill)->m_strAttacks ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Damage: ") +            ((CMeleeSkill*)m_pSkill)->m_strDmg ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("Hit Points: ") +        ((CMeleeSkill*)m_pSkill)->m_strHP ); y += m_nCharHeight;
                break;

            case SPECIALSKILL:
                dc.TextOut( x, y, CString("Base: ") + m_pSkill->GetBaseString(m_f5_5Mode) ); y += m_nCharHeight;
                strText.Format( "This skill has %d subskills", ((CSpecialSkill*)m_pSkill)->m_nSkills );
                dc.TextOut( x, y, strText ); y += m_nCharHeight;
                break;

            case SUBSKILL:
                dc.TextOut( x, y, CString("Base: ") + m_pSkill->GetBaseString(m_f5_5Mode) ); y += m_nCharHeight;
                dc.TextOut( x, y, CString("This skill is a subskill of ") + ((CSubSkill*)m_pSkill)->m_pSkill->m_strName ); y += m_nCharHeight;
                break;

            case ANYSKILL:
                dc.TextOut( x, y, "This represents any skill" ); y += m_nCharHeight;
                break;

            case NO_TYPE:
            default:
                break;
        }
    }
    else
    {
        dc.TextOut( 5, 5, "Nothing selected" );
    }

    //put the old font back
    dc.SelectObject( pOldFont );
    Font.Detach();
}

void CSkillInformationPopupWnd::MoveIntoPosition()
{
    //get the various window rectangles
    CRect rcClientArea;
    ::GetWindowRect( ((CMainFrame*)AfxGetMainWnd())->m_hWndMDIClient, &rcClientArea );

    CRect rcScreen( 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) );

    //calculate the size of the font to be used
    CalcFontSizes();
    m_nWidth = m_nMaxCharWidth * 18;
    m_nHeight = m_nCharHeight * 11;

    //set up window size
    int x, y;
    x = rcClientArea.right - m_nWidth - 16;
    y = rcClientArea.top + 16;

    //load previous position
    x = AfxGetApp()->GetProfileInt( "WindowPos", "SkillInfoX", x );
    y = AfxGetApp()->GetProfileInt( "WindowPos", "SkillInfoY", y );

    //don't let it go off the screen
    if( x < 0 ) x = 0;
    if( y < 0 ) y = 0;
    if( x + m_nWidth > rcScreen.right ) x = rcScreen.right - m_nWidth;
    if( y + m_nHeight > rcScreen.bottom ) y = rcScreen.bottom - m_nHeight;

    //set the new size and position for the window
    CRect rc( 0, 0, m_nWidth, m_nHeight );
    rc.OffsetRect( x, y );

    //move the window
    MoveWindow( rc );
}

void CSkillInformationPopupWnd::SetSkill(CString strSkill, CInvestigator* pInvestigator /*NULL*/)
{
    if( strSkill.IsEmpty() )
    {
        m_pSkill = NULL;
    }
    else
    {
        //find the skill
        m_pSkill = SkillManager.FindSkill( m_strRuleFile, strSkill );

        //see if it's in the investigator instead
        if( m_pSkill == NULL && pInvestigator )
        {
            CInvestigatorSkill* pInvestigatorSkill = pInvestigator->FindSkill( strSkill );
            if( pInvestigatorSkill ) m_pSkill = pInvestigatorSkill->m_pSkill;
        }
    }

    //load 5.5 mode option again
    m_f5_5Mode = AfxGetApp()->GetProfileInt( "Sheet", "V5.5Base", TRUE );

    //redraw the window
    if( IsWindowVisible() ) RedrawWindow();
}

CString CSkillInformationPopupWnd::GetEraDescription( ERA_MASK EraMask )
{
    //special cases
    if( EraMask == ERA_ISALL ) return "Available in every era.";
    if( EraMask == ERA_ISNONE ) return "Not available in any era.";

    CString strText = "Available in the ";

    BOOL fOneOnly = ( ( EraMask == ERA_IS1890 ) || ( EraMask == ERA_IS1920 ) || ( EraMask == ERA_IS1990 ) );

    //find first era
    if( EraMask & ERA_IS1890 ) { strText += "1890s"; EraMask &= ~ERA_IS1890; }
    else if( EraMask & ERA_IS1920 ) { strText += "1920s"; EraMask &= ~ERA_IS1920; }
    else if( EraMask & ERA_IS1990 ) { strText += "1990s"; EraMask &= ~ERA_IS1990; }

    //see if it's only one era
    if( fOneOnly == FALSE )
    {
        //nope, get next era
        strText += " and ";
        if( EraMask & ERA_IS1890 ) strText += "1890s";
        else if( EraMask & ERA_IS1920 ) strText += "1920s";
        else if( EraMask & ERA_IS1990 ) strText += "1990s";
    }

    //return it
    return strText + " only."; //only available in one era
}

void CSkillInformationPopupWnd::OnMove(int x, int y) 
{
	CPopupWnd::OnMove(x, y);
	
    if( IsWindowVisible() )
    {
        CPoint pt(0,0);
        ClientToScreen(&pt);
        AfxGetApp()->WriteProfileInt( "WindowPos", "SkillInfoX", pt.x );
        AfxGetApp()->WriteProfileInt( "WindowPos", "SkillInfoY", pt.y );
    }	
}
