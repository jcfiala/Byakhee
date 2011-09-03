// SpellbookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "byakhee.h"
#include "SectionFile.h"
#include "ParseBuffer.h"
#include "SpellbookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellbookDlg dialog


CSpellbookDlg::CSpellbookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpellbookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpellbookDlg)
	//}}AFX_DATA_INIT

    m_ImageList.Create( 16, 16, ILC_COLOR4|ILC_MASK, 0, 3 );
    m_ImageList.Add( (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CLOSEDBOOK), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS ) ); 
    m_ImageList.Add( (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_OPENBOOK), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS ) ); 
    m_ImageList.Add( (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAGE), IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS ) ); 
}


void CSpellbookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellbookDlg)
	DDX_Control(pDX, IDC_INVESTIGATORSPELLS, m_wndInvestigatorSpells);
	DDX_Control(pDX, IDC_SPELLLIST, m_wndSpellList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpellbookDlg, CDialog)
	//{{AFX_MSG_MAP(CSpellbookDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_SPELLLIST, OnDblclkSpellList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_RETURN, IDC_SPELLLIST, OnReturnSpellList)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_SPELLLIST, OnItemExpandingSpellList)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_DBLCLK(IDC_INVESTIGATORSPELLS, OnDblclkInvestigatorspells)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellbookDlg message handlers

void CSpellbookDlg::OnDblclkSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CSpellbookDlg::OnReturnSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CSpellbookDlg::OnAdd() 
{
    //get the selected item
	HTREEITEM hSelected = m_wndSpellList.GetSelectedItem();
    if( hSelected )
    {
        TV_ITEM tvi;
        char szBuffer[1024];
        ZeroMemory( szBuffer, sizeof(szBuffer) );

        //get the selected item's caption
        tvi.hItem = hSelected;
        tvi.mask = TVIF_TEXT|TVIF_CHILDREN;
        tvi.cChildren = 0;
        tvi.pszText = szBuffer;
        tvi.cchTextMax = sizeof(szBuffer);

        if( m_wndSpellList.GetItem( &tvi ) && *szBuffer != '\0' && tvi.cChildren == 0 )
        {
            UpdateData(UD_GETDATA);
            m_wndInvestigatorSpells.AddString( szBuffer );
            UpdateData(UD_SETDATA);
        }
    }
}

BOOL CSpellbookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //set the tree view's image list
	m_wndSpellList.SetImageList( &m_ImageList, TVSIL_NORMAL );

    //add all spellbooks
    CString strSearchPath = CByakheeApp::GetDefaultDirectory() + CString("*.bsb");
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile( strSearchPath, &wfd );
	if( hFind != INVALID_HANDLE_VALUE )
	{
        do {
            //load it
            AddSpellBook( wfd.cFileName );

            //keep searching
	    } while( FindNextFile( hFind, &wfd ) );
    	FindClose( hFind );
    }
    else
    {
        AfxMessageBox( "Could not find any spell books in the Byakhee folder (*.bsb)", MB_ICONEXCLAMATION );
    }

    //add all current investigator's skills to the list box
    m_wndInvestigatorSpells.ResetContent();
    char* pszBuffer = m_strInvestigatorSpells.GetBuffer();
    while( pszBuffer && *pszBuffer )
    {
        char* pszEnd = strchr(pszBuffer, ',');
        if( pszEnd ) *pszEnd = '\0';

        while( *pszBuffer == ' ' || *pszBuffer == '\t' ) pszBuffer++;
        if( *pszBuffer != '\0' )
            m_wndInvestigatorSpells.AddString(pszBuffer);

        pszBuffer = pszEnd ? ++pszEnd : NULL;
    }

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpellbookDlg::OnRemove() 
{
    int i = m_wndInvestigatorSpells.GetCurSel();
    if( i != LB_ERR ) m_wndInvestigatorSpells.DeleteString(i);
}

void CSpellbookDlg::AddSpellBook(const char * pszFileName)
{
    //open the spell book
    CSectionFile SpellBook;
    SpellBook.Open( CByakheeApp::GetDefaultDirectory() + pszFileName );

    //read the title
    CString strTitle = SpellBook.GetSection( "TITLE" );
	char * psTitle = strTitle.GetBuffer();
    if( strTitle.IsEmpty() )
    {
        //trim off path and extension, if there is one, and use that for the title
		char * psFileName;
		psFileName = (char*)malloc(sizeof(char)*strlen(pszFileName));
		strcpy(psFileName, pszFileName);
        char* pszTemp = strrchr(psFileName, '\\');
        if( pszTemp ) psFileName = ++pszTemp;
        pszTemp = strrchr(psFileName, '.' );
        if( pszTemp ) *pszTemp = '\0';
        strTitle = psFileName;
    }
    else
    {
        //find the end of the first line
        char* pszTemp = strchr( psTitle, '\r' );
        char* pszTemp2 = strchr( psTitle, '\n' );
        if( pszTemp != NULL || pszTemp2 != NULL )
        {
            //find the one nearest to the end of the first line
            if( pszTemp == NULL ) pszTemp = pszTemp2;
                else if( pszTemp2 != NULL && pszTemp2 < pszTemp ) pszTemp = pszTemp2;

            //terminate it at that point
            *pszTemp = '\0';
            CString strTemp = LPCSTR(psTitle);
            *pszTemp = '\n';
            strTitle = strTemp;
        }
    }

    //read the rest of the spellbook (1D10 months, D6 SAN...)
    CString strContents = SpellBook.GetSection( "SPELLS" );
    if( strContents.IsEmpty() ) return; //book was empty

    //add this book to the tree
    HTREEITEM hSpellBook = m_wndSpellList.InsertItem( strTitle, 1, 1 );
    HTREEITEM hParent = hSpellBook;
    ExpandTreeItem( hSpellBook );

    //read each line
    CParseBuffer ParseBuffer(strContents);
    while( ParseBuffer.IsEmpty() == FALSE )
    {
        //add item
        CString strItem;
        switch( ParseBuffer.PeekNextChar() )
        {
            case '*':
                hParent = hSpellBook;
                strItem = ParseBuffer.ExtractSurroundedString( '*', '*' );
                if(strItem.IsEmpty() == FALSE )
                    hParent = m_wndSpellList.InsertItem( strItem, 0, 0, hParent );
                break;

            default: //this default will generate a parse error if it's not a "", which will report an error in the spellbook
            case '\"':
                strItem = ParseBuffer.ExtractQuotedString();
                if(strItem.IsEmpty() == FALSE ) m_wndSpellList.InsertItem( strItem, 2, 2, hParent );
                break;
        }

        //skip over the rest of the line
        ParseBuffer.SkipToNextLine();
    }


}

void CSpellbookDlg::OnItemExpandingSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    //use opened book image for expanded tree items
    if( pNMTreeView->action == TVE_EXPAND )
        pNMTreeView->itemNew.iImage = pNMTreeView->itemNew.iSelectedImage = 1;
    else
        pNMTreeView->itemNew.iImage = pNMTreeView->itemNew.iSelectedImage = 0;

    //change icon
    m_wndSpellList.SetItem( &pNMTreeView->itemNew );

	*pResult = 0;
}

void CSpellbookDlg::ExpandTreeItem(HTREEITEM hTreeItem)
{
    TV_ITEM tvi;

    //expand the given item
    tvi.hItem = hTreeItem;
    tvi.mask = TVIF_STATE;
    tvi.state = TVIS_EXPANDED;
    tvi.stateMask = TVIS_EXPANDED;

    m_wndSpellList.SetItem( &tvi );

}


void CSpellbookDlg::OnOK() 
{
    m_strInvestigatorSpells = "";
    int max = m_wndInvestigatorSpells.GetCount();

    for( int i = 0; i < max; i++ )
    {
        CString strText;
        m_wndInvestigatorSpells.GetText( i, strText );
        if( m_strInvestigatorSpells.IsEmpty() )
            m_strInvestigatorSpells = strText;
        else
            m_strInvestigatorSpells = m_strInvestigatorSpells + CString(", ") + strText;
    }

	CDialog::OnOK();
}

void CSpellbookDlg::OnDblclkInvestigatorspells() 
{
    OnRemove();	
}
