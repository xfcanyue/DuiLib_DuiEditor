#pragma once


namespace DuiLib
{

class CScriptEngine
{
public:
	CScriptEngine(void);
	virtual ~CScriptEngine(void);

	asIScriptEngine *GetEngine();
protected:
	void Init();
	void reg_GlobalProperty(); //注册全局属性
	void reg_ControlHierarchies(); //统一注册类的层次关系

	//void MessageCallback(const asSMessageInfo &msg);
	asIScriptEngine  *engine;
};

extern CScriptEngine g_ScriptEngine;

} //namespace DuiLib