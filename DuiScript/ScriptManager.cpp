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

	int r = 0;

	engine = g_ScriptEngine.GetEngine();
	r = engine->SetMessageCallback(asMETHOD(CScriptManager, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

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

void CScriptManager::ContextLineCallback(asIScriptContext *ctx)
{
	if( m_dwTime < timeGetTime() )
		ctx->Abort();
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

bool CScriptManager::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl)
{
	int r = 0;
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszFunName));
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

bool CScriptManager::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TEventUI *ev)
{
	int r = 0;
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszFunName));
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

bool CScriptManager::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TNotifyUI *pMsg)
{
	int r = 0;
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszFunName));
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

bool CScriptManager::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
{
	int r = 0;
	asIScriptFunction *pFun = static_cast<asIScriptFunction *>(m_mapContent.Find(lpszFunName));
	if(!pFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pFun) < 0) return false;

// 	asIScriptFunction *pFun1 =(asIScriptFunction *)pFun;
// 	for (asUINT i=0; i<pFun1->GetParamCount(); i++)
// 	{
// 		int typeId;
// 		DWORD flag;
// 		const char *name;
// 		pFun1->GetParam(i, &typeId, &flag, &name);
// 		continue;
// 	}

	//传入入口参数
	int n = sizeof(HDC);
	r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
	r = ctx->SetArgAddress(1, pRender); if( r < 0 ) return false;
	r = ctx->SetArgObject(2, (void *)&rcPaint); if( r < 0 ) return false;
	r = ctx->SetArgObject(3, pStopControl); if( r < 0 ) return false;

	if(Execute())
	{
		return ctx->GetReturnByte() == 1;
	}
	return false;
}

bool CScriptManager::ExecuteScript(IScriptFunction *pFun)
{
	int r = 0;
	asIScriptFunction *pAsFun = static_cast<asIScriptFunction *>(m_mapContent.Find(pFun->m_sFunName));
	if(!pAsFun) return false;
	if(ctx->Prepare((asIScriptFunction *)pAsFun) < 0) return false;

	asIScriptFunction *pFun1 =(asIScriptFunction *)pAsFun;
	for (asUINT i=0; i<pFun1->GetParamCount(); i++)
	{
		int typeId;
		DWORD flag;
		const char *name;
		pFun1->GetParam(i, &typeId, &flag, &name);
		continue;
	}

	//传入入口参数
	for (int i=0; i<pFun->m_arrArgs.GetSize(); i++)
	{
		IScriptFunction::TArgItem *pItem = (IScriptFunction::TArgItem *)pFun->m_arrArgs.GetAt(i);
		switch (pItem->_type)
		{
		case UIArg_void:
			return false;
			break;
		case UIArg_BYTE:
			{
				r = ctx->SetArgByte(i, pItem->_byte); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_WORD:
			{
				r = ctx->SetArgWord(i, pItem->_word); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_DWORD:
			{
				r = ctx->SetArgDWord(i, pItem->_dword); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_float:
			{
				r = ctx->SetArgFloat(i, pItem->_float); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_double:
			{
				r = ctx->SetArgDouble(i, pItem->_double); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_address:
			{
				r = ctx->SetArgAddress(i, pItem->_addr); 
				if( r < 0 ) return false;
			}
			break;
		case UIArg_object:
			{
				r = ctx->SetArgObject(i, pItem->_obj); 
				if( r < 0 ) return false;
			}
			break;
		}
	}

	if(Execute())
	{
		switch (pFun->m_result._type)
		{
		case UIArg_void:
			break;
		case UIArg_BYTE:
			{
				pFun->m_result._byte = ctx->GetReturnByte();
			}
			break;
		case UIArg_WORD:
			{
				pFun->m_result._word = ctx->GetReturnWord();
			}
			break;
		case UIArg_DWORD:
			{
				pFun->m_result._dword = ctx->GetReturnDWord();
			}
			break;
		case UIArg_float:
			{
				pFun->m_result._float = ctx->GetReturnFloat();
			}
			break;
		case UIArg_double:
			{
				pFun->m_result._double = ctx->GetReturnDouble();
			}
			break;
		case UIArg_address:
			{
				pFun->m_result._addr = ctx->GetReturnAddress();
			}
			break;
		case UIArg_object:
			{
				pFun->m_result._obj = ctx->GetReturnObject();
			}
			break;
		}

		return true;
	}

	return false;
}

} //namespace DuiLib