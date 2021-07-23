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
	virtual bool AddScriptFile(LPCTSTR pstrFileName);
	virtual bool AddScriptCode(LPCTSTR pstrCode);
	virtual bool CompileScript();
	virtual void *GetFunAddress(LPCTSTR lpszFunName);
	virtual bool SetMainFun(LPCTSTR lpszMainFun);
	virtual bool Execute();
	virtual bool ExecuteScript(void *pFun, CControlUI *pControl);
	virtual bool ExecuteScript(void *pFun, CControlUI *pControl, TEventUI *ev);
	virtual bool ExecuteScript(void *pFun, CControlUI *pControl, TNotifyUI *pMsg);
	virtual bool ExecuteScript(void *pFun, CControlUI *pControl, HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

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