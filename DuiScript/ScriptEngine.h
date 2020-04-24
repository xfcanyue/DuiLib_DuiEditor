#pragma once

#include "../3rd/DuiAngelScript/DuiAngelScript/angelscript/add_on/scriptbuilder/scriptbuilder.h"

class CScriptEngine : public IScriptEngine
{
public:
	CScriptEngine(void);
	virtual ~CScriptEngine(void);

	virtual bool AddScriptCode(LPCTSTR pScriptCode);
	virtual bool AddScriptFile(LPCTSTR pstrFileName);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl, TEventUI *ev);

protected:
	void MessageCallback(const asSMessageInfo &msg);
private:
	asIScriptEngine  *engine;
	asIScriptContext *ctx;

	int m_nModuleCount;

	CStdStringPtrMap m_mapContent;
};
