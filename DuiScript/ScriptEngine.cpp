#include "stdafx.h"
#include "ScriptEngine.h"

#include <mmsystem.h>
#pragma comment( lib,"winmm.lib" )

/*
static asIScriptEngine *engine = NULL;
asIScriptEngine *GetEngine()
{
	if(engine == NULL)
	{
		//创建AngelScript脚本引擎
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

		int r = 0;

		//消息回调
		r = engine->SetMessageCallback(asMETHOD(CScriptManager, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

		//脚本代码的字符编码  0 - ASCII, 1 - UTF8. Default: 1 (UTF8). 
		r = engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 0); assert( r >= 0 );

#ifdef _UNICODE
		//脚本内部字符串的字符编码 0 - UTF8/ASCII, 1 - UTF16. Default: 0 (UTF8) 
		r = engine->SetEngineProperty(asEP_STRING_ENCODING, 1); assert( r >= 0 );
#endif
	}

	return engine;
}
*/

static void ScriptLineCallback(asIScriptContext *ctx, DWORD *timeOut)
{
	// If the time out is reached we abort the script
	if( *timeOut < timeGetTime() )
		ctx->Abort();

	// It would also be possible to only suspend the script,
	// instead of aborting it. That would allow the application
	// to resume the execution where it left of at a later 
	// time, by simply calling Execute() again.
}

//////////////////////////////////////////////////////////////////////////
//CScriptMgr
CScriptEngine::CScriptEngine(void) : m_nSectionCount(0)
{
	
}


CScriptEngine::~CScriptEngine(void)
{
	if(ctx)
	{
		ctx->Release();
		ctx = NULL;
	}
}

void CScriptEngine::Init()
{
	if(!m_builder.GetModule())
	{
		m_builder.StartNewModule(engine, "generic");
	}
}

bool CScriptEngine::AddScriptCode(LPCTSTR pScriptCode)
{
	if(!m_builder.GetModule())	Init();
	if(!m_builder.GetModule())	return false;

	m_nSectionCount++;

	char sectionName[255];
	sprintf(sectionName, "section%04d", m_nSectionCount);

	LSSTRING_CONVERSION;
	int r = m_builder.AddSectionFromMemory(LST2A(pScriptCode), sectionName);
	//std::string str = "int main(){}";
	//str += char(EOF);
	//char str[11] = "int x = 0";
	//str[10]= char(EOF);
	//int r = m_builder.AddSectionFromMemory(str.c_str(), sectionName);
	
	if(r<0) return false;
	return true;
}

bool CScriptEngine::AddScriptFile(LPCTSTR pstrFileName)
{
	if(!m_builder.GetModule())	Init();
	if(!m_builder.GetModule())	return false;

	LPBYTE pData = NULL;
	DWORD dwSize = CRenderEngine::LoadImage2Memory(STRINGorID(pstrFileName), 0, pData);
	if(dwSize == 0U || !pData)
		return false;
	bool rbool = AddScriptCode((LPCTSTR)pData);
	delete []pData; pData = NULL;

	return rbool;
}

bool CScriptEngine::CompileScript()
{
	if(!m_builder.GetModule())	return false;
	int r = m_builder.BuildModule();
	if(r<0)	return false;

	//保存脚本函数地址
	asIScriptModule *pModule = m_builder.GetModule();
	int funCount = pModule->GetFunctionCount();
	for (int i=0; i<funCount; i++)
	{
		asIScriptFunction *pFun = pModule->GetFunctionByIndex(i);
		USES_CONVERSION;
		m_mapContent.Set( A2T((LPSTR)pFun->GetName()),  pFun);
	}
	return true;
}

bool CScriptEngine::ExecuteScript(LPCTSTR funName, CControlUI *pControl)
{
	if(!m_builder.GetModule())	return false;

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
		MessageBoxA(NULL, temp, "script error", MB_OK);
		return false;
	}

	return true;
}

bool CScriptEngine::ExecuteScript(LPCTSTR funName, CControlUI *pControl, TEventUI *ev)
{
	if(!m_builder.GetModule())	return false;

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
		MessageBoxA(NULL, temp, "script error", MB_OK);
		return false;
	}

	return true;
}
