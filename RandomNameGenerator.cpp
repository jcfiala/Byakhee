// RandomNameGenerator.cpp: implementation of the CRandomNameGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Byakhee.h"
#include "RandomNameGenerator.h"
#include "NamedOptionManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandomNameGenerator::CRandomNameGenerator()
{
    //create a named option object to hold the male, female and surname entries
    m_pNamedOptionManager = new CNamedOptionManager( "names.lst", 3);

    //generate a name to start with
    Generate();
}

CRandomNameGenerator::~CRandomNameGenerator()
{
    delete m_pNamedOptionManager;
}

void CRandomNameGenerator::Generate()
{
    //get first name & sex
	if( rand() % 4 == 0 ) //greater male than female weighting (sexist, I know)
	{
        CStringArray* pStringArray = m_pNamedOptionManager->GetOptions( "FemaleNames" );
        if( pStringArray )
        {
            m_strName = pStringArray->GetAt( rand() % pStringArray->GetSize() ); 
		    m_strSex = "F";
        }
	}
	else
	{
        CStringArray* pStringArray = m_pNamedOptionManager->GetOptions( "MaleNames" );
        if( pStringArray )
        {
            m_strName = pStringArray->GetAt( rand() % pStringArray->GetSize() );
		    m_strSex = "M";
        }
	}

    //add a gap between names and get the surnames
    CStringArray* pStringArray = m_pNamedOptionManager->GetOptions( "Surnames" );
    if( pStringArray )
    {
	    m_strName += " ";

        //possibly add something fancy
        if( rand() % 7 == 0 ) //1 in 7 chance of a middle name or double-barrel name
        {
            m_strName += pStringArray->GetAt( rand() % pStringArray->GetSize() );
            if( rand() % 2 == 0 ) m_strName += "-"; else m_strName += " ";
        }
        else
            if( rand() % 50 == 0 ) //1 in 50 chance of being a st-wossname
                m_strName += "St. ";

        //get surname
        m_strName += pStringArray->GetAt( rand() % pStringArray->GetSize() );
    }
    else
        m_strName = "name data not found";
}

CString CRandomNameGenerator::GetName()
{
    return m_strName;
}

CString CRandomNameGenerator::GetSex()
{
    return m_strSex;
}

