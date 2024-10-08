#include "stdafx.h"
#include "ScriptManager.h"
#include "ScriptEngine.h"
#include "ScriptContext.h"

#include <mmsystem.h>
#pragma comment( lib,"winmm.lib" )


namespace DuiLib
{

	extern CScriptEngine g_ScriptEngine;
//////////////////////////////////////////////////////////////////////////
//CScriptEngine
CScriptManager::CScriptManager(void)
{
	int r = 0;
	m_dwTimeOut = 10000;

	engine = g_ScriptEngine.GetEngine();
	r = engine->SetMessageCallback(asMETHOD(CScriptManager, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

	m_fnMessageCallback = NULL;
	m_fnContextLineCallback = NULL;

	m_ctx = new CScriptContext(this);
}


CScriptManager::~CScriptManager(void)
{
	delete m_ctx;
}

void CScriptManager::MessageCallback(const asSMessageInfo &msg)
{
	if(m_fnMessageCallback)
	{
		(*m_fnMessageCallback)(msg.type, msg.row, msg.col, msg.section, msg.message);
	}
	else
	{
		if( msg.type == asMSGTYPE_ERROR )
		{
			CDuiStringUtf8 temp;
			temp.Format("row = %d\r\ncol = %d\r\nsection=%s \r\nmessage = %s\r\n", 
				msg.row, msg.col, msg.section, msg.message);
			MessageBoxA(NULL, temp, "Duilib script error", MB_OK);
		}
	}
}

CDuiString CScriptManager::AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pstrModuleName)
{
	int r = 0;

	CUIFile f;
	if(!f.LoadFile(pstrFileName))
		return _T("");

	CDuiStringUtf8 sModuleName = CDuiString(pstrModuleName);
	if(sModuleName.IsEmpty())
	{
		sModuleName = CDuiString(pstrFileName);
	}

	asIScriptModule *mod = engine->GetModule(sModuleName, asGM_CREATE_IF_NOT_EXISTS);
	if(mod == NULL) return _T("");

	r = mod->AddScriptSection(sModuleName, (const char *)f.GetData(), f.GetSize()); assert(r>=0);
	if( r >= 0)
	{
		return sModuleName.convert_to_auto();
	}

	return _T("");
}

CDuiString CScriptManager::AddScriptCode(LPCTSTR pstrCode, LPCTSTR pstrModuleName)
{
	int r = 0;

	CDuiStringUtf8 sModuleName = CDuiString(pstrModuleName);
	if(sModuleName.IsEmpty())
	{
		GUID guid;
		::CoCreateGuid(&guid);
		sModuleName.Format("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", 
			guid.Data1, guid.Data2, guid.Data3, 
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
			guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}

	asIScriptModule *mod = engine->GetModule(sModuleName, asGM_CREATE_IF_NOT_EXISTS);
	if(mod == NULL) return _T("");
	
	LSSTRING_CONVERSION;
	const char *pData = LST2UTF8(pstrCode);
	mod->AddScriptSection(sModuleName, pData, strlen(pData)); assert(r>=0);
	if( r >= 0)
	{
		return sModuleName.convert_to_auto();
	}

	return _T("");
}

bool CScriptManager::RemoveScript(LPCTSTR pstrModuleName)
{
	CDuiStringUtf8 sName = CDuiString(pstrModuleName);
	for (asUINT i=0; i<engine->GetModuleCount(); i++)
	{
		asIScriptModule *mod = engine->GetModuleByIndex(i);
		if(sName == mod->GetName())
		{
			engine->DiscardModule(sName);
			return true;
		}
	}
	return false;
}

void CScriptManager::RemoveAllScript()
{
	while (engine->GetModuleCount() > 0)
	{
		asIScriptModule *mod = engine->GetModuleByIndex(0);
		engine->DiscardModule(mod->GetName());
	}
}

bool CScriptManager::CompileScript()
{
	int r=0;
	
	for (asUINT i=0; i<engine->GetModuleCount(); i++)
	{
		asIScriptModule *mod = engine->GetModuleByIndex(i);
		r = mod->Build(); assert(r>=0);
		if( r < 0 ) return false;
	}

	return true;
}

IScriptContext *CScriptManager::CreateContext()
{
	return new CScriptContext(this);
}

void CScriptManager::ReleaseContext(IScriptContext *ctx)
{
	delete ctx;
}

void CScriptManager::SetTimeOut(int dwTimeOut)
{
	m_dwTimeOut = dwTimeOut;
}

void CScriptManager::SetMessageCallback(SCRIPT_MESSAGE_CALLBACK pfnCallback)
{
	m_fnMessageCallback = pfnCallback;
}

void CScriptManager::SetContextLineCallback(SCRIPT_CONTEXT_LINE_CALLBACK pfnCallback)
{
	m_fnContextLineCallback = pfnCallback;
}

asIScriptFunction *CScriptManager::GetFunByName(LPCTSTR lpszFunName)
{
	CDuiStringUtf8 sFunName = CDuiString(lpszFunName);
	for (asUINT i=0; i<engine->GetModuleCount(); i++)
	{
		asIScriptModule *mod = engine->GetModuleByIndex(i);
		asIScriptFunction *fun = mod->GetFunctionByName(sFunName);
		if(fun) return fun;
	}
	return NULL;
}

asIScriptFunction *CScriptManager::GetFunByDecl(LPCTSTR lpszFunDecl)
{
	CDuiStringUtf8 sFunName = CDuiString(lpszFunDecl);
	for (asUINT i=0; i<engine->GetModuleCount(); i++)
	{
		asIScriptModule *mod = engine->GetModuleByIndex(i);
		asIScriptFunction *fun = mod->GetFunctionByDecl(sFunName);
		if(fun) return fun;
	}
	return NULL;
}

} //namespace DuiLib