#include "stdafx.h"
#include "ScriptEngine.h"
#include "ScriptRegister.h"

#include <mmsystem.h>
#pragma comment( lib,"winmm.lib" )

static void ScriptLineCallback(asIScriptContext *ctx, DWORD *timeOut)
{
	// If the time out is reached we abort the script
	if( *timeOut < timeGetTime() )
		ctx->Abort();
}

//////////////////////////////////////////////////////////////////////////
//CScriptEngine
CScriptEngine::CScriptEngine(void) : m_nModuleCount(0)
{
	ctx = NULL;

	engine = asCreateScriptEngine();

	int r = 0;
	r = engine->SetMessageCallback(asMETHOD(CScriptEngine, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

	//脚本代码的字符编码  0 - ASCII, 1 - UTF8. Default: 1 (UTF8). 
	r = engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 1); assert( r >= 0 );

	//脚本内部字符串的字符编码 0 - UTF8/ASCII, 1 - UTF16. Default: 0 (UTF8) 
#ifdef _UNICODE
	r = engine->SetEngineProperty(asEP_STRING_ENCODING, 1); assert( r >= 0 );
#endif

	CScriptRegister reg(engine);
	reg.RegisterAll();
}


CScriptEngine::~CScriptEngine(void)
{
	if(ctx)
	{
		ctx->Release();
		ctx = NULL;
	}
}

void CScriptEngine::MessageCallback(const asSMessageInfo &msg)
{
	if( msg.type == asMSGTYPE_ERROR )
	{
		ATL::CStringA temp;
		temp.Format("row = %d\r\ncol = %d\r\nsection=%s \r\nmessage = %s\r\n", 
			msg.row, msg.col, msg.section, msg.message);
		MessageBoxA(NULL, temp, "Duilib script complie error", MB_OK);
	}
}

bool CScriptEngine::AddScriptCode(const char *pUtf8ScriptCode)
{
	m_nModuleCount++;
	
	char module[255];
	sprintf(module, "module%d", m_nModuleCount);

	int r = 0;
	CScriptBuilder builder;
	r = builder.StartNewModule(engine, module);
	if( r < 0 )
		return false;

	builder.AddSectionFromMemory("section1", pUtf8ScriptCode);

	r = builder.BuildModule();
	if( r < 0 ) return false;

	//保存脚本函数地址
	asIScriptModule *pModule = builder.GetModule();
	int funCount = pModule->GetFunctionCount();
	for (int i=0; i<funCount; i++)
	{
		asIScriptFunction *pFun = pModule->GetFunctionByIndex(i);
		USES_CONVERSION;
		m_mapContent.Set( A2T((LPSTR)pFun->GetName()),  pFun);
	}

	return true;
}

bool CScriptEngine::AddScriptFile(LPCTSTR pstrFileName)
{
	LPBYTE pData = NULL;
	DWORD dwSize = CRenderEngine::LoadImage2Memory(STRINGorID(pstrFileName), 0, pData);
	if(dwSize == 0U || !pData)
		return false;
	bool rbool = false;
	rbool = AddScriptCode((const char *)pData);
	CRenderEngine::FreeMemory(pData); 

	return rbool;
}

bool CScriptEngine::ExecuteScript(LPCTSTR funName, CControlUI *pControl)
{
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(funName));
	if(!pFun) return false;

	if(!ctx) ctx = engine->CreateContext();

	DWORD dwTime = timeGetTime() + 5000; //设置脚本运行超时时间

	int r = ctx->SetLineCallback(asFUNCTION(ScriptLineCallback), &dwTime, asCALL_CDECL); if( r < 0 ) return false;

	r = ctx->Prepare(pFun);	if( r < 0 ) return false;

	//传入入口参数
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;

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

	return true;
}

bool CScriptEngine::ExecuteScript(LPCTSTR funName, CControlUI *pControl, TEventUI *ev)
{
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(funName));
	if(!pFun) return false;

	if(!ctx) ctx = engine->CreateContext();

	DWORD dwTime = timeGetTime() + 5000; //设置脚本运行超时时间

	int r = ctx->SetLineCallback(asFUNCTION(ScriptLineCallback), &dwTime, asCALL_CDECL); if( r < 0 ) return false;

	r = ctx->Prepare(pFun);	if( r < 0 ) return false;

	//传入入口参数
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgObject(1, ev); if( r < 0 ) return false;

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

	return true;
}

bool CScriptEngine::ExecuteScript(LPCTSTR funName, CControlUI *pControl, TNotifyUI *pMsg)
{
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(funName));
	if(!pFun) return false;

	if(!ctx) ctx = engine->CreateContext();

	DWORD dwTime = timeGetTime() + 5000; //设置脚本运行超时时间

	int r = ctx->SetLineCallback(asFUNCTION(ScriptLineCallback), &dwTime, asCALL_CDECL); if( r < 0 ) return false;

	r = ctx->Prepare(pFun);	if( r < 0 ) return false;

	//传入入口参数
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgObject(1, pMsg); if( r < 0 ) return false;

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

	return true;
}