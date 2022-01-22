#pragma once

#include "angelscript/add_on/scriptbuilder/scriptbuilder.h"

namespace DuiLib
{


class CScriptManager : public IScriptManager
{
public:
	CScriptManager(void);
	virtual ~CScriptManager(void);

	virtual bool CreateModule(LPCTSTR moduleName = NULL);
	virtual void DeleteModule();

	virtual bool AddScriptFile(LPCTSTR pstrFileName) override;
	virtual bool AddScriptCode(LPCTSTR pstrCode) override;
	virtual bool CompileScript() override;
	virtual bool SetMainFun(LPCTSTR lpszMainFun);
	virtual bool Execute();
	virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl) override;
	virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TEventUI *ev) override;
	virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TNotifyUI *pMsg) override;
	virtual bool ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;
	virtual bool ExecuteScript(IScriptFunction *pFun) override;
	asIScriptEngine *GetEngine() const { return engine; }
protected:
	virtual void MessageCallback(const asSMessageInfo &msg);
	virtual void ContextLineCallback(asIScriptContext *ctx);

protected:
	asIScriptEngine  *engine;
	CScriptBuilder m_builder;
	bool m_bHasBuild;
	asIScriptContext *ctx;
	DWORD m_dwTime;
	CStdStringPtrMap m_mapContent;

};


} //namespace DuiLib