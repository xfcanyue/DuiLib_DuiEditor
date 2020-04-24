// DuiScript.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "ScriptEngine.h"

//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) IScriptEngine* __stdcall CreateScriptEngine()
{
	return new CScriptEngine;
}

extern "C" __declspec(dllexport) void __stdcall DeleteScriptEngine(IScriptEngine *pEngine)
{
	if(pEngine)
	{
		delete (CScriptEngine *)pEngine;
		pEngine = NULL;
	}
}