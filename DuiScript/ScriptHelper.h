#pragma once
#include <string>
#include <list>
#include "ScriptManager.h"

namespace DuiLib
{


class CScriptHelper : public CScriptManager, public IScriptHelper
{
public:
	CScriptHelper(void);
	virtual ~CScriptHelper(void);

	virtual void SetScriptMessageCallBack(SCRIPTMESSAGECALLBACK pFun, UINT_PTR userdata)
	{
		m_pFun = pFun;
		m_pUser = userdata;
	}

	void CallMessageCallback(TScriptMessage *msg)
	{
		if(m_pFun == NULL) return;
		return (*m_pFun)(msg, m_pUser);
	}

	void WriteMessage(const char *text)
	{
		TScriptMessage msg;
		msg.type = usMsg_Message;
		msg.message = text;
		CallMessageCallback(&msg);
	}

	virtual asIScriptEngine *GetEngine() const override;
	//////////////////////////////////////////////////////////////////////////
	virtual CDuiString AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pstrModuleName=NULL) override;
	virtual CDuiString AddScriptCode(LPCTSTR pstrCode, LPCTSTR pstrModuleName=NULL) override;
	virtual bool RemoveScript(LPCTSTR pstrModuleName) override;
	virtual void RemoveAllScript() override;

	virtual bool CompileScript() override;

	virtual BOOL IsRunning();
	virtual void DebugRun();
	virtual BOOL IsCanDebugRun();
	virtual void DebugStop();
	virtual BOOL IsCanDebugStop();
	virtual void DebugStepInto();
	virtual BOOL IsCanDebugStepInto();
	virtual void DebugStepOver();
	virtual BOOL IsCanDebugStepOver();
	virtual void DebugStepReturn();
	virtual BOOL IsCanDebugStepReturn();
	virtual void DebugStepCursor();
	virtual BOOL IsCanDebugStepCursor();

public:
	static UINT APIENTRY _ThreadFunDebug(LPVOID pPara) { return ((CScriptHelper *)pPara)->ThreadFunDebug(); }
	UINT ThreadFunDebug();

	virtual void MessageCallback(const asSMessageInfo &msg);
	virtual void ContextLineCallback(asIScriptContext *ctx);
	void TakeCommands();
	void AddCommand(int nCmd, int line=0);
	BOOL CheckBreakPoint(int line);
	void GotoLine(int line);
	void PrintContext();

public:
	SCRIPTMESSAGECALLBACK m_pFun;
	UINT_PTR m_pUser;
private:
	int m_nCmd;
	int m_nCmdLine;
	unsigned int m_lastCommandAtStackLevel;
	int m_lastLine;	//上次运行的行
	HANDLE m_hEventDebug;
	volatile HANDLE _hThreadDebug;
};

}