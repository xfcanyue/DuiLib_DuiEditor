#pragma once

#include "angelscript/add_on/scriptbuilder/scriptbuilder.h"

class CScriptEngine : public IScriptEngine
{
public:
	CScriptEngine(void);
	virtual ~CScriptEngine(void);

	virtual bool AddScriptCode(const char *pUtf8ScriptCode);
	virtual bool AddScriptFile(LPCTSTR pstrFileName);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl, TEventUI *ev);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl, TNotifyUI *pMsg);

protected:
	void MessageCallback(const asSMessageInfo &msg);
private:
	asIScriptEngine  *engine;
	asIScriptContext *ctx;

	int m_nModuleCount;

	CStdStringPtrMap m_mapContent;
};
