// Field.cpp: implementation of the CField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "byakhee.h"
#include "Field.h"
#include "FieldEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/********************************************************************************************************************/

CScriptObject::MEMBERRESULT CFieldScriptObject::ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue )
{
    //field value query/modification functions - only if field properties allow!
    if( stricmp( pszName, "getfieldvalue" ) == 0 )
    {
        if( nParameters == 1 )
        {
            //search the entire field list
            int iMax = m_FieldList.GetSize();
            for( int i = 0; i < iMax; i++ )
            {
                //get the next item and return the value if it's the one we're looking for
                CField* pField = m_FieldList[i];
                if( pField && pField->m_Code == pParameters[0] )
                {
                    if( pField->m_dwFlags & FIELD_SCRIPTREAD )
                    {
                        returnValue = pField->GetValue();
                        return OK;
                    }
                    else
                    {
                        AfxMessageBox( CString("Field is protected: ") + CString(LPCSTR(pParameters[0])) );
                        return UNKNOWN_ERROR;
                    }
                }
            };

            //couldn't find it...
            AfxMessageBox( CString("Field not found: ") + CString(LPCSTR(pParameters[0])) );
            return UNKNOWN_ERROR;
        }
        else
            return nParameters < 1 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    if( stricmp( pszName, "setfieldvalue" ) == 0 )
    {
        if( nParameters == 2 )
        {
            //search the entire field list
            int iMax = m_FieldList.GetSize();
            for( int i = 0; i < iMax; i++ )
            {
                //get the next item and change the value if it's the one we're looking for
                CField* pField = m_FieldList[i];
                if( pField && pField->m_Code == pParameters[0] )
                {
                    if( pField->m_dwFlags & FIELD_SCRIPTWRITE )
                    {
                        pField->SetValue( pParameters[1] );
                        return OK;
                    }
                    else
                    {
                        AfxMessageBox( CString("Field is protected: ") + CString(LPCSTR(pParameters[0])) );
                        return UNKNOWN_ERROR;
                    }
                }
            };

            //couldn't find it...
            AfxMessageBox( CString("Field not found: ") + CString(LPCSTR(pParameters[0])) );
            return UNKNOWN_ERROR;
        }
        else
            return nParameters < 2 ? TOO_FEW_PARAMS : TOO_MANY_PARAMS;
    }

    return NOT_FOUND;
}

Variant* CFieldScriptObject::GetMemberVariable( char* pszName )
{
    //search the entire field list
    int iMax = m_FieldList.GetSize();
    for( int i = 0; i < iMax; i++ )
    {
        //get the next item and return the value if it's the one we're looking for
        CField* pField = m_FieldList[i];
        if( pField && pField->m_strName.CompareNoCase(pszName) == 0 ) return &pField->m_Code;
    };

    return NULL;
}

/********************************************************************************************************************/
CField::CField( CFieldManager* pFieldManager, CString strName, DWORD dwFlags, int nCode, const char* pszOptionName /*NULL*/ )
{
    m_strName = strName;
    m_dwFlags = dwFlags;
    m_pFieldManager = pFieldManager;
    m_pszOptionName = pszOptionName;

    //store code and make it read-only
    m_Code = nCode;
    m_Code.SetReadOnly(TRUE);
}

CStringField::CStringField( CFieldManager* pFieldManager, CString* pstrValue, CString strName, DWORD dwFlags, int nCode, const char* pszOptionName /*NULL*/ ) : CField( pFieldManager, strName, dwFlags, nCode, pszOptionName )
{
    ASSERT( pstrValue != NULL );
    m_pstrValue = pstrValue;
}

CIntField::CIntField( CFieldManager* pFieldManager, int* pnValue, CString strName, DWORD dwFlags, int nCode ) : CField( pFieldManager, strName, dwFlags, nCode )
{
    ASSERT( pnValue != NULL );
    m_pnValue = pnValue;
}


void CField::SetValue(Variant value, CFieldSheetCommand* pFieldSheetCommand /*NULL*/)
{
    m_pFieldManager->SetFieldValue( this, value, pFieldSheetCommand );
}

Variant CField::GetValue( CFieldSheetCommand* pFieldSheetCommand /*NULL*/ )
{
    return m_pFieldManager->GetFieldValue( this, pFieldSheetCommand );
}

void CStringField::SetValue(Variant value, CFieldSheetCommand* )
{
    *m_pstrValue = LPCSTR(value);
}

Variant CStringField::GetValue(CFieldSheetCommand*)
{
    return *m_pstrValue;
}

void CIntField::SetValue(Variant value, CFieldSheetCommand* )
{
    *m_pnValue = int(value);
}

Variant CIntField::GetValue(CFieldSheetCommand*)
{
    return *m_pnValue;
}
