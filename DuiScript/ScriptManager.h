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

	virtual bool CreateModule(LPCTSTR moduleName = NULL);
	virtual void DeleteModule();

	virtual bool AddScriptFile(LPCTSTR pstrFileName) override;
	virtual bool AddScriptCode(LPCTSTR pstrCode) override;
	virtual bool CompileScript() override;

	virtual asIScriptEngine *GetEngine() const { return engine; }
	virtual IScriptContext *CreateContext() override;
	virtual void ReleaseContext(IScriptContext *ctx) override;

	CStdStringPtrMap *GetFunctionList() { return &m_mapContent; }
	asIScriptFunction *GetFunByName(LPCTSTR lpszFunName);
	asIScriptFunction *GetFunByDecl(LPCTSTR lpszFunDecl);
protected:
	void MessageCallback(const asSMessageInfo &msg);

protected:
	asIScriptEngine  *engine;
	CScriptBuilder m_builder;
	bool m_bHasBuild;
	CScriptContext *m_ctx;
	DWORD m_dwTime;
	CStdStringPtrMap m_mapContent;

};


} //namespace DuiLib