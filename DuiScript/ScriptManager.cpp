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

	engine = g_ScriptEngine.GetEngine();
	r = engine->SetMessageCallback(asMETHOD(CScriptManager, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

	m_bHasBuild = false;
	CreateModule(NULL);

	m_ctx = new CScriptContext(this);
}


CScriptManager::~CScriptManager(void)
{
	delete m_ctx;
	DeleteModule();
}

void CScriptManager::MessageCallback(const asSMessageInfo &msg)
{
	if( msg.type == asMSGTYPE_ERROR )
	{
		LSSTRING_CONVERSION;
		ATL::CStringA temp;
		temp.Format("row = %d\r\ncol = %d\r\nsection=%s \r\nmessage = %s\r\n", 
			msg.row, msg.col, msg.section, msg.message);
		MessageBoxA(NULL, LSUTF82A(temp), "Duilib script error", MB_OK);
	}
}

bool CScriptManager::CreateModule(LPCTSTR moduleName)
{
	if(moduleName != NULL && *moduleName != '\0')
	{
		LSSTRING_CONVERSION;
		return m_builder.StartNewModule(engine, LST2UTF8(moduleName)) >= 0;
	}

	GUID guid;
	::CoCreateGuid(&guid);
	CStringA mName;
	mName.Format("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", 
		guid.Data1, guid.Data2, guid.Data3, 
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
		guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return m_builder.StartNewModule(engine, mName) >= 0;
}

void CScriptManager::DeleteModule()
{
	m_builder.GetModule()->Discard();
	m_bHasBuild = false;
}

bool CScriptManager::AddScriptFile(LPCTSTR pstrFileName)
{
	int r = 0;

	CUIFile f;
	if(!f.LoadFile(pstrFileName))
		return false;

	LSSTRING_CONVERSION;
	r = m_builder.AddSectionFromMemory(LST2UTF8(pstrFileName), (const char *)f.GetData(), f.GetSize()); assert(r>=0);

	return r >= 0;
}

bool CScriptManager::AddScriptCode(LPCTSTR pstrCode)
{
	int r = 0;

	GUID guid;
	::CoCreateGuid(&guid);
	CStringA sName;
	sName.Format("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", 
		guid.Data1, guid.Data2, guid.Data3, 
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
		guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	LSSTRING_CONVERSION;
	const char *pData = LST2UTF8(pstrCode);
	r = m_builder.AddSectionFromMemory(sName, pData, strlen(pData)); assert(r>=0);
	return r >= 0;
}

bool CScriptManager::CompileScript()
{
	if(m_bHasBuild) return true;

	int r=0;
	
	r = m_builder.BuildModule();
	if( r < 0 ) return false;

	//保存脚本函数地址
	asIScriptModule *pModule = m_builder.GetModule();
	int funCount = pModule->GetFunctionCount();
	for (int i=0; i<funCount; i++)
	{
		asIScriptFunction *pFun = pModule->GetFunctionByIndex(i);
		LSSTRING_CONVERSION;
		m_mapContent.Set(LSUTF82T(pFun->GetName()), pFun);
	}
	m_bHasBuild = true;
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

asIScriptFunction *CScriptManager::GetFunByName(LPCTSTR lpszFunName)
{
	LSSTRING_CONVERSION;
	return m_builder.GetModule()->GetFunctionByName(LST2UTF8(lpszFunName));
}

asIScriptFunction *CScriptManager::GetFunByDecl(LPCTSTR lpszFunDecl)
{
	LSSTRING_CONVERSION;
	return m_builder.GetModule()->GetFunctionByDecl(LST2UTF8(lpszFunDecl));
}

} //namespace DuiLib