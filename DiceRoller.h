// DiceRoller.h: interface for the CDiceRoller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICEROLLER_H__E0272200_5F02_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_DICEROLLER_H__E0272200_5F02_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScriptEngine/ScriptObject.h"
#include "ScriptEngine/Variant.h"

class CDiceRoller : public CObject 
{
public:
	int Roll( CString strRoll );
	CDiceRoller();
	virtual ~CDiceRoller();

protected:
    //dice roller is overridable...
	virtual int RollNSidedDice( int n );

private:
	int ExtractNumber( LPCSTR& pszString );
	int ExtractAndRoll( LPCSTR& pszRollString );
};

class CScriptDice : public CScriptObject , public CObject
{
public:
    virtual MEMBERRESULT ExecuteMemberFunction( char* pszName, Variant* pParameters, int nParameters, Variant& returnValue );

private:
    CDiceRoller m_Dice;
};


#endif // !defined(AFX_DICEROLLER_H__E0272200_5F02_11D2_9FF9_00001C192944__INCLUDED_)
