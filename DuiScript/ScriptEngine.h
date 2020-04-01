#pragma once

#include "../3rd/DuiAngelScript/DuiAngelScript/angelscript/add_on/scriptbuilder/scriptbuilder.h"

class CScriptEngine : public IScriptEngine
{
public:
	CScriptEngine(void);
	virtual ~CScriptEngine(void);

	void SetEngine(asIScriptEngine *pEngine) { engine = pEngine;}
	asIScriptEngine *GetEngine() { return engine; }

	void Init();
	virtual bool AddScriptCode(LPCTSTR pScriptCode);
	virtual bool AddScriptFile(LPCTSTR pstrFileName);
	virtual bool CompileScript();
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl);
	virtual bool ExecuteScript(LPCTSTR funName, CControlUI *pControl, TEventUI *ev);

private:
	asIScriptEngine  *engine;
	asIScriptContext *ctx;
	int m_nSectionCount;
	CScriptBuilder m_builder;

	CStdStringPtrMap m_mapContent;
};
