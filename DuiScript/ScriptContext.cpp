#include "StdAfx.h"
#include "ScriptContext.h"

#include "ScriptManager.h"

#include <mmsystem.h>
#pragma comment( lib,"winmm.lib" )

namespace DuiLib
{

CScriptContext::CScriptContext(IScriptManager *pScriptManager)
{
	m_pScriptManager = pScriptManager;
	ASSERT(m_pScriptManager);
	m_pEngine = ((CScriptManager *)m_pScriptManager)->GetEngine();
	ASSERT(m_pEngine);
	m_ctx = m_pEngine->CreateContext();
	ASSERT(m_ctx);

	int r = 0;
	r = m_ctx->SetLineCallback(asMETHOD(CScriptContext, ContextLineCallback), this, asCALL_THISCALL); assert( r >= 0 );
	m_dwStartTime = timeGetTime();
}

CScriptContext::~CScriptContext(void)
{
	if(m_ctx)
	{
		m_ctx->Release();
		m_ctx = NULL;
	}
}

int CScriptContext::SetFunByName(LPCTSTR lpszFunName)
{
	int r = 0;
	CScriptManager *pScManager = (CScriptManager *)m_pScriptManager;
	asIScriptFunction *pFun = pScManager->GetFunByName(lpszFunName);
	if(!pFun) return -1;
	return m_ctx->Prepare(pFun);
}

int CScriptContext::SetFunByDecl(LPCTSTR lpszFunDecl)
{
	int r = 0;
	CScriptManager *pScManager = (CScriptManager *)m_pScriptManager;
	asIScriptFunction *pFun = pScManager->GetFunByDecl(lpszFunDecl);
	if(!pFun) return -1;
	return m_ctx->Prepare(pFun);
}

int CScriptContext::SetArgByte(UINT arg, BYTE value)
{
	return m_ctx->SetArgByte(arg, value);
}

int CScriptContext::SetArgWord(UINT arg, WORD value)
{
	return m_ctx->SetArgWord(arg, value);
}

int CScriptContext::SetArgDWord(UINT arg, DWORD value)
{
	return m_ctx->SetArgDWord(arg, value);
}

int CScriptContext::SetArgFloat(UINT arg, float value)
{
	return m_ctx->SetArgFloat(arg, value);
}

int CScriptContext::SetArgDouble(UINT arg, double value)
{
	return m_ctx->SetArgDouble(arg, value);
}

int CScriptContext::SetArgAddress(UINT arg, void *addr)
{
	return m_ctx->SetArgAddress(arg, addr);
}

int CScriptContext::SetArgObject(UINT arg, void *obj)
{
	return m_ctx->SetArgObject(arg, obj);
}

void *CScriptContext::GetAddressOfArg(UINT arg)
{
	return m_ctx->GetAddressOfArg(arg);
}

int CScriptContext::Execute()
{
	int r = 0;

	m_dwStartTime = timeGetTime();

	r = m_ctx->Execute();
	if(r == asEXECUTION_FINISHED)
	{
		
	}
	else if( r == asEXECUTION_EXCEPTION )
	{
		CStringA temp;
		temp.Format("Exception:%s \r\nFunction: %s \r\nLine: %d", 
			m_ctx->GetExceptionString(), 
			m_ctx->GetExceptionFunction()->GetDeclaration(), 
			m_ctx->GetExceptionLineNumber());
		MessageBoxA(NULL, temp, "Duilib script error", MB_OK);
	}

	return r;
}

int CScriptContext::Abort()
{
	return m_ctx->Abort();
}

BYTE CScriptContext::GetReturnByte()
{
	return m_ctx->GetReturnByte();
}

WORD CScriptContext::GetReturnWord()
{
	return m_ctx->GetReturnWord();
}

DWORD CScriptContext::GetReturnDWord()
{
	return m_ctx->GetReturnDWord();
}

float CScriptContext::GetReturnFloat()
{
	return m_ctx->GetReturnFloat();
}

double CScriptContext::GetReturnDouble()
{
	return m_ctx->GetReturnDouble();
}

void *CScriptContext::GetReturnAddress()
{
	return m_ctx->GetReturnAddress();
}

void *CScriptContext::GetReturnObject()
{
	return m_ctx->GetReturnObject();
}

void *CScriptContext::GetAddressOfReturnValue()
{
	return m_ctx->GetAddressOfReturnValue();
}

void CScriptContext::ContextLineCallback(asIScriptContext *ctx)
{
	CScriptManager *pMgr = (CScriptManager *)m_pScriptManager;
	SCRIPT_CONTEXT_LINE_CALLBACK pfn = pMgr->GetContextLineCallback();
	if(pfn)
	{
		(*pfn)(this);
	}

	if(pMgr->GetTimeOut() > 0)
	{
		if( timeGetTime() - m_dwStartTime >= (DWORD)pMgr->GetTimeOut() )
			ctx->Abort();
	}	
}

}