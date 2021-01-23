#pragma once
#include <string>
#include <list>
#include "Include/DuiScriptInterface.h"
#include "ScriptManager.h"

namespace DuiLib
{


class CScriptHelper : public CScriptManager, public IScriptHelper
{
public:
	CScriptHelper(void);
	~CScriptHelper(void);

	virtual void SetScriptMessageCallBack(SCRIPTMESSAGECALLBACK pFun, UINT_PTR userdata)
	{
		m_pFun = pFun;
		m_pUser = userdata;
	}
	BOOL CallMessageCallback(TScriptMessage *msg)
	{
		if(m_pFun == NULL) return FALSE;
		return (*m_pFun)(msg, m_pUser);
	}

	void WriteMessage(const char *text)
	{
		TScriptMessage msg;
		msg.nType = 3;
		msg.lpszNotifyText = text;
		CallMessageCallback(&msg);
	}

	virtual asIScriptEngine *GetEngine() const;
	//////////////////////////////////////////////////////////////////////////
	virtual bool CreateModule(LPCTSTR moduleName);
	virtual void DeleteModule();
	virtual bool AddScriptFile(LPCTSTR pstrFileName);
	virtual bool CompileScript();

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

	virtual bool SetMainFun(LPCTSTR lpszMainFun);

	virtual bool Excute();

	virtual BOOL	SetArgByte(UINT arg, BYTE value);
	virtual BOOL	SetArgWord(UINT arg, WORD value);
	virtual BOOL	SetArgDWord(UINT arg, DWORD value);
	virtual BOOL	SetArgFloat(UINT arg, float value);
	virtual BOOL	SetArgDouble(UINT arg, double value);
	virtual BOOL	SetArgAddress(UINT arg, void *addr);
	virtual BOOL	SetArgObject(UINT arg, void *obj);
	virtual void *  GetAddressOfArg(UINT arg);

	virtual BYTE	GetReturnByte();
	virtual WORD	GetReturnWord();
	virtual DWORD	GetReturnDWord();
	virtual float	GetReturnFloat();
	virtual double	GetReturnDouble();
	virtual void *	GetReturnAddress();
	virtual void *	GetReturnObject();
	virtual void *	GetAddressOfReturnValue();

public:
	static UINT APIENTRY _ThreadFunDebug(LPVOID pPara) { return ((CScriptHelper *)pPara)->ThreadFunDebug(); }
	UINT ThreadFunDebug();

	virtual void MessageCallback(const asSMessageInfo &msg);
	virtual void ContextLineCallback(asIScriptContext *ctx);
	void TakeCommands();
	void AddCommand(int nCmd, int line=0);
	BOOL CheckBreakPoint(int line);
	BOOL GotoLine(int line);
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