#pragma once

#include "ScriptContext.h"
#include "angelscript/add_on/scriptbuilder/scriptbuilder.h"

namespace DuiLib
{
class UISCRIPT_API CScriptManager : public IScriptManager
{
public:
	CScriptManager(void);
	virtual ~CScriptManager(void);

	virtual CDuiString AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pstrModuleName=NULL) override;
	virtual CDuiString AddScriptCode(LPCTSTR pstrCode, LPCTSTR pstrModuleName=NULL) override;
	virtual bool RemoveScript(LPCTSTR pstrModuleName) override;
	virtual void RemoveAllScript() override;

	virtual bool CompileScript() override;

	virtual asIScriptEngine *GetEngine() const { return engine; }
	virtual IScriptContext *CreateContext() override;
	virtual void ReleaseContext(IScriptContext *ctx) override;

	virtual void SetTimeOut(int dwTimeOut) override;
	int GetTimeOut() const { return m_dwTimeOut; }

	virtual void SetMessageCallback(SCRIPT_MESSAGE_CALLBACK pfnCallback) override;

	virtual void SetContextLineCallback(SCRIPT_CONTEXT_LINE_CALLBACK pfnCallback) override;
	SCRIPT_CONTEXT_LINE_CALLBACK GetContextLineCallback() const { return m_fnContextLineCallback; }

	asIScriptFunction *GetFunByName(LPCTSTR lpszFunName);
	asIScriptFunction *GetFunByDecl(LPCTSTR lpszFunDecl);
protected:
	void MessageCallback(const asSMessageInfo &msg);

protected:
	asIScriptEngine  *engine;
	int m_dwTimeOut;
	SCRIPT_MESSAGE_CALLBACK m_fnMessageCallback;
	SCRIPT_CONTEXT_LINE_CALLBACK m_fnContextLineCallback;
	CScriptContext *m_ctx;
	DWORD m_dwTime;
};


} //namespace DuiLib