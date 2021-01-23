#include "stdafx.h"
#include "ScriptManager.h"
#include "ScriptEngine.h"

#include <mmsystem.h>
#pragma comment( lib,"winmm.lib" )


namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
//CScriptEngine
CScriptManager::CScriptManager(void)
{
	ctx = NULL;

	engine = g_ScriptEngine.GetEngine();

	int r = 0;

	m_bHasBuild = false;
	//CreateModule(_T("duilib"));
	CreateModule(NULL);

	m_dwTime = 0;
	ctx = engine->CreateContext();
	r = ctx->SetLineCallback(asMETHOD(CScriptManager, ContextLineCallback), this, asCALL_THISCALL); assert( r >= 0 );
}


CScriptManager::~CScriptManager(void)
{
	if(ctx)
	{
		ctx->Release();
		ctx = NULL;
	}
	DeleteModule();
}

void CScriptManager::ContextLineCallback(asIScriptContext *ctx)
{
	if( m_dwTime < timeGetTime() )
		ctx->Abort();
}

bool CScriptManager::CreateModule(LPCTSTR moduleName)
{
	if(moduleName != NULL && *moduleName=='\0')
	{
		LSSTRING_CONVERSION;
		return m_builder.StartNewModule(engine, LST2UTF8(moduleName)) >= 0;
	}

	CStringA mName;
	SYSTEMTIME st;
	GetLocalTime(&st);
	srand(timeGetTime());
	mName.Format("%04d%02d%02d %02d.%02d.%02d.%d.%03d", //年月日时分秒+3位随机值
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%1000);
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
	LPBYTE pData = NULL;
	DWORD dwSize = CRenderEngine::LoadImage2Memory(STRINGorID(pstrFileName), 0, pData);
	if(dwSize == 0U || !pData)
		return false;

	LSSTRING_CONVERSION;
	r = m_builder.AddSectionFromMemory(LST2UTF8(pstrFileName), (const char *)pData, dwSize); assert(r>=0);

	CRenderEngine::FreeMemory(pData); 
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

void *CScriptManager::GetFunAddress(LPCTSTR lpszFunName)
{
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszFunName));
	return pFun;
}

bool CScriptManager::SetMainFun(LPCTSTR lpszMainFun)
{
	int r = 0;
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszMainFun));
	if(!pFun) return false;
	return ctx->Prepare(pFun) >= 0;
}

bool CScriptManager::Execute()
{
	int r = 0;
	m_dwTime = timeGetTime() + 5000;

	r = ctx->Execute();
	if(r == asEXECUTION_FINISHED)
	{
		return true;
	}
	else if( r == asEXECUTION_EXCEPTION )
	{
		CStringA temp;
		temp.Format("Exception:%s \r\nFunction: %s \r\nLine: %d", 
			ctx->GetExceptionString(), 
			ctx->GetExceptionFunction()->GetDeclaration(), 
			ctx->GetExceptionLineNumber());
		MessageBoxA(NULL, temp, "Duilib script error", MB_OK);
		return false;
	}

	return false;
}

bool CScriptManager::ExecuteScript(void *pFun, CControlUI *pControl)
{
	int r = 0;
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

bool CScriptManager::ExecuteScript(void *pFun, CControlUI *pControl, TEventUI *ev)
{
	int r = 0;
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

	//传入入口参数
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgObject(1, ev); if( r < 0 ) return false;

	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

bool CScriptManager::ExecuteScript(void *pFun, CControlUI *pControl, TNotifyUI *pMsg)
{
	int r = 0;
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

	//传入入口参数
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgObject(1, pMsg); if( r < 0 ) return false;

	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

bool CScriptManager::ExecuteScript(void *pFun, CControlUI *pControl, HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	int r = 0;
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

	asIScriptFunction *pFun1 =(asIScriptFunction *)pFun;
	for (int i=0; i<pFun1->GetParamCount(); i++)
	{
		int typeId;
		DWORD flag;
		const char *name;
		pFun1->GetParam(i, &typeId, &flag, &name);
		continue;
	}

	//传入入口参数
	int n = sizeof(HDC);
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgAddress(1, &hDC); if( r < 0 ) return false;
	r = ctx->SetArgObject(2, (void *)&rcPaint); if( r < 0 ) return false;
	r = ctx->SetArgObject(3, pStopControl); if( r < 0 ) return false;

	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

} //namespace DuiLib