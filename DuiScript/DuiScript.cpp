// DuiScript.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "Include/DuiScriptInterface.h"
#include "ScriptManager.h"
#include "ScriptHelper.h"

//////////////////////////////////////////////////////////////////////////
/*
extern "C" __declspec(dllexport) IScriptManager* __stdcall CreateScriptEngine()
{
	return new CScriptManager;
}

extern "C" __declspec(dllexport) void __stdcall DeleteScriptEngine(IScriptManager *pEngine)
{
	if(pEngine)
	{
		delete (CScriptManager *)pEngine;
		pEngine = NULL;
	}
}

extern "C" __declspec(dllexport) IScriptHelper* __stdcall CreateScriptHelper()
{
	return new CScriptHelper;
}

extern "C" __declspec(dllexport) void __stdcall DeleteScriptHelper(IScriptHelper *pHelper)
{
	if(pHelper)
	{
		delete (CScriptHelper *)pHelper;
		pHelper = NULL;
	}
}
*/

UISCRIPT_API DuiLib::IScriptManager* __stdcall CreateScriptEngine()
{
	return new CScriptManager;
}

UISCRIPT_API void __stdcall DeleteScriptEngine(DuiLib::IScriptManager *pEngine)
{
	if(pEngine)
	{
		delete (CScriptManager *)pEngine;
		pEngine = NULL;
	}
}

UISCRIPT_API void __stdcall DuiScriptRegister()
{
	CPaintManagerUI::LoadScriptPlugin(CreateScriptEngine, DeleteScriptEngine);
}

UISCRIPT_API IScriptHelper* __stdcall CreateScriptHelper()
{
	return new CScriptHelper;
}

UISCRIPT_API void __stdcall DeleteScriptHelper(IScriptHelper *pHelper)
{
	if(pHelper)
	{
		delete (CScriptHelper *)pHelper;
		pHelper = NULL;
	}
}