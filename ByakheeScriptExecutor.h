// ByakheeScriptExecutor.h: interface for the CByakheeScriptExecutor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYAKHEESCRIPTEXECUTOR_H__84DAA162_7816_11D2_9FF9_00001C192944__INCLUDED_)
#define AFX_BYAKHEESCRIPTEXECUTOR_H__84DAA162_7816_11D2_9FF9_00001C192944__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CByakheeScriptExecutor : public CObject  
{
public:
	BOOL Execute();
	CByakheeScriptExecutor( CString strScriptFileName );
	virtual ~CByakheeScriptExecutor();

private:
    CString m_strScriptFileName;
};

#endif // !defined(AFX_BYAKHEESCRIPTEXECUTOR_H__84DAA162_7816_11D2_9FF9_00001C192944__INCLUDED_)
