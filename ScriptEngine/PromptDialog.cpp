// PromptDialog.cpp: implementation of the CPromptDialog class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "PromptDialog.h"




HWINDOW g_hWndOwner = NULL;

LPCSTR g_pszPrompt = "";

#define MAX_PROMPT_TEXT 256

#define IDC_PROMPTTEXT 100
#define IDC_EDITTEXT 101

BOOL CALLBACK DialogProc_Prompt( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    static Variant* s_pVariant = NULL;
    switch( uMsg )
    {
        case WM_INITDIALOG:
        {
            RECT rc;
            GetClientRect( hDlg, &rc );

            //store the parameter
            s_pVariant = (Variant*)lParam;

            //change the font
            SendMessage( hDlg, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );

            //create the static control
            HWND hWndStatic = CreateWindow( "STATIC", g_pszPrompt, WS_VISIBLE|WS_CHILD|SS_LEFT|SS_NOPREFIX, 8, 8, rc.right-16, 14, hDlg, (HMENU)IDC_PROMPTTEXT, (HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE), NULL );
            SendMessage( hWndStatic, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );

            //create the edit control
            HWND hWndEdit = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", "", 0x4/*3d look*/|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|ES_AUTOHSCROLL, 8, 26, rc.right-16, 23, hDlg, (HMENU)IDC_EDITTEXT, (HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE), NULL );
            SendMessage( hWndEdit, EM_LIMITTEXT, MAX_PROMPT_TEXT-1, 0 );
            SendMessage( hWndEdit, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );
            SetFocus( hWndEdit );

            //create the buttons
            HWND hWndOK = CreateWindow( "BUTTON", "OK", WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, rc.right-(75+75+16), rc.bottom-(23+8), 75, 23, hDlg, (HMENU)IDOK, (HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE), NULL );
            SendMessage( hWndOK, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );

            HWND hWndCancel = CreateWindow( "BUTTON", "Cancel", WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON, rc.right-(75+8), rc.bottom-(23+8), 75, 23, hDlg, (HMENU)IDCANCEL, (HINSTANCE)GetWindowLong(hDlg,GWL_HINSTANCE), NULL );
            SendMessage( hWndCancel, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), 0 );
            return FALSE;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                {
                    //extract edit control text
                    char szPromptText[MAX_PROMPT_TEXT];
                    strcpy(szPromptText, "" );
                    GetDlgItemText( hDlg, IDC_EDITTEXT, szPromptText, MAX_PROMPT_TEXT-1 );

                    //store it in the return value
                    *s_pVariant = szPromptText;

                    //close the dialog
                    EndDialog( hDlg, IDOK );
                    break;
                }

                case IDCANCEL:
                    EndDialog( hDlg, IDCANCEL );
                    break;
            }
            break;

        case WM_CLOSE:
            EndDialog( hDlg, IDCANCEL );
            break;

        default:
            return FALSE;
    }

    return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPromptDialog::CPromptDialog( LPCSTR pszPrompt )
{
    m_pszPrompt = pszPrompt;
}

CPromptDialog::~CPromptDialog()
{

}

Variant CPromptDialog::Prompt()
{
    Variant returnValue;

    //allocate and prepare the main dialog
    LPDLGTEMPLATE lpdlgt = (LPDLGTEMPLATE)GlobalAlloc(GPTR, 2048);
    lpdlgt->style = WS_BORDER|WS_CAPTION|DS_CENTER|DS_3DLOOK|DS_MODALFRAME|WS_POPUP;
    lpdlgt->dwExtendedStyle = 0;
    lpdlgt->cdit = 0;
    lpdlgt->x = lpdlgt->y = 0;
    lpdlgt->cx = 128;
    lpdlgt->cy = 48;

    //set menu and dialog defaults
    LPWORD lpw = (LPWORD)(lpdlgt + 1);
    *lpw++ = 0; //no menu
    *lpw++ = 0; //predefined dialog box class (by default)

    //set caption
    LPWSTR lpwsz = (LPWSTR)lpw;
    int nchar = 1+MultiByteToWideChar(CP_ACP, 0, "Prompt", -1, lpwsz, 50 );
    lpw += nchar;

    //show the dialog
    g_pszPrompt = m_pszPrompt;
    if( DialogBoxIndirectParam( (HINSTANCE)GetWindowLong((HWND)g_hWndOwner,GWL_HINSTANCE), lpdlgt, (HWND)g_hWndOwner, (DLGPROC)DialogProc_Prompt, (LPARAM)&returnValue ) == IDCANCEL )
        returnValue = Variant::GetNULL();

    //free up resources
    GlobalFree( (HGLOBAL)lpdlgt );

    return returnValue;
}
