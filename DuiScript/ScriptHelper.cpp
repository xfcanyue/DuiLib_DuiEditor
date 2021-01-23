#include "StdAfx.h"
#include "ScriptHelper.h"

#include <assert.h>  // assert()
#include <string.h>  // strstr()
#include <sstream>
using namespace std;

#include "../DuiScript/angelscript/add_on/scriptstdtime/AsTime.h"

namespace DuiLib
{

CScriptHelper::CScriptHelper(void)
{
	_hThreadDebug = NULL;
	m_nCmd = SCRIPT_DEBUG_SETP_OVER;
	m_nCmdLine = 0;
	m_lastCommandAtStackLevel = 0;
	m_lastLine = 0;

	m_hEventDebug = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_hEventDebug);
}

CScriptHelper::~CScriptHelper(void)
{
	CloseHandle(m_hEventDebug);
}

asIScriptEngine *CScriptHelper::GetEngine() const
{
	return engine;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CScriptHelper::TakeCommands()
{
	WaitForSingleObject(m_hEventDebug, INFINITE);
	ResetEvent(m_hEventDebug);
}

void CScriptHelper::AddCommand(int nCmd, int line)
{
	m_nCmd = nCmd;
	m_nCmdLine = line;
	if (m_nCmd==SCRIPT_DEBUG_SETP_OVER ||m_nCmd==SCRIPT_DEBUG_SETP_OUT)
	{
		m_lastCommandAtStackLevel = ctx->GetCallstackSize();
	}
	SetEvent(m_hEventDebug);
}

BOOL CScriptHelper::CheckBreakPoint(int line)
{
	tagScriptMessage msg;
	msg.nType = 1;
	msg.line = line;
	return CallMessageCallback(&msg);
}

BOOL CScriptHelper::GotoLine(int line)
{
	tagScriptMessage msg;
	msg.nType = 0;
	msg.line = line;
	return CallMessageCallback(&msg);
}

void CScriptHelper::PrintContext()
{
	tagScriptMessage msg;
	msg.nType = 2;
	msg.line = 0;
	msg.ctx = ctx;
	CallMessageCallback(&msg);
}

void CScriptHelper::MessageCallback(const asSMessageInfo &msg)
{
	const char *type = "ERR ";
	if( msg.type == asMSGTYPE_WARNING ) 
	{
		type = "WARN";
	}
	else if( msg.type == asMSGTYPE_INFORMATION ) 
	{
		type = "INFO";
	}

	char MsgStr[2048];
	if(msg.row == 0 && msg.col == 0)
	{
		sprintf_s(MsgStr, "%s : %s", type, msg.message);
	}
	else
		sprintf_s(MsgStr, "%s (%d, %d) : %s : %s", msg.section, msg.row, msg.col, type, msg.message);

	tagScriptMessage smsg;
	smsg.nType = 3;
	smsg.line = 0;
	smsg.lpszNotifyText = MsgStr;
	CallMessageCallback(&smsg);
}

void CScriptHelper::ContextLineCallback(asIScriptContext *ctx)
{
	int line = ctx->GetLineNumber();

	if(line == m_lastLine)	return;
	m_lastLine = line;

	if(m_nCmd == SCRIPT_DEBUG_CONTINUE)
	{
		if(!CheckBreakPoint(line-1))
		{		
			return;
		}
	}
	else if(m_nCmd == SCRIPT_DEBUG_SETP_INTO) //进入区块，固定要break;
	{
		//if(!CheckBreakPoint(line-1))
		//	return;
	}
	else if(m_nCmd == SCRIPT_DEBUG_SETP_OVER)
	{
		if( ctx->GetCallstackSize() > m_lastCommandAtStackLevel )
		{
			if(!CheckBreakPoint(line-1))
				return;
		}
	}
	else if(m_nCmd == SCRIPT_DEBUG_SETP_OUT)
	{
		if( ctx->GetCallstackSize() >= m_lastCommandAtStackLevel )
		{
			if(!CheckBreakPoint(line-1))
				return;
		}
	}
	else if(m_nCmd == SCRIPT_DEBUG_SETP_CURSOR)
	{
		if(m_nCmdLine != line-1)
		{
			return;
		}
	}
	else if(m_nCmd == SCRIPT_DEBUG_ABORT)
	{
		ctx->Abort();
		return;
	}

	GotoLine(line-1);
	PrintContext();
	TakeCommands();
}

UINT CScriptHelper::ThreadFunDebug()
{
	asIThreadManager *pThreadManager = asGetThreadManager();
	asPrepareMultithread(pThreadManager);

	tagScriptMessage msg;
	msg.line = 0;
	msg.lpszNotifyText = NULL;

	int r = 0;
	r = ctx->Execute();
	
	if( r == asEXECUTION_FINISHED )
	{
		//脚本执行成功
		msg.nType = 5;
		msg.ctx = ctx;
		CallMessageCallback(&msg);
	}
	else
	{
		//脚本执行异常
		msg.nType = 3;

		char MsgStr[1024];
		// The execution didn't finish as we had planned. Determine why.
		if( r == asEXECUTION_ABORTED )
		{
			msg.lpszNotifyText = "The script was aborted.";
		}
		else if( r == asEXECUTION_EXCEPTION )
		{
			// Write some information about the script exception
			asIScriptFunction *func = ctx->GetExceptionFunction();
			sprintf_s(MsgStr, "func: %s\r\nmodl: %s\r\nsect: %s\r\nline: %d\r\ndesc: %s", 
				func->GetDeclaration(),
				func->GetScriptSectionName(),
				ctx->GetExceptionLineNumber(),
				ctx->GetExceptionString());
			msg.lpszNotifyText = MsgStr;
		}
		else
		{
			sprintf_s(MsgStr, "The script ended for some unforeseen reason: %d", r);
			msg.lpszNotifyText = MsgStr;
		}

		CallMessageCallback(&msg);

		msg.nType = 6; 
		CallMessageCallback(&msg);
	}

	asThreadCleanup();
	if(_hThreadDebug != NULL){ ::CloseHandle(_hThreadDebug); _hThreadDebug = NULL; }
	return 0;
}

bool CScriptHelper::CreateModule(LPCTSTR moduleName)
{
	return CScriptManager::CreateModule(moduleName);
}

void CScriptHelper::DeleteModule()
{
	CScriptManager::DeleteModule();
}

bool CScriptHelper::AddScriptFile(LPCTSTR pstrFileName)
{
	return CScriptManager::AddScriptFile(pstrFileName);
}

bool CScriptHelper::CompileScript()
{
	return CScriptManager::CompileScript();
}

BOOL CScriptHelper::IsRunning()
{
	return ctx->GetState() == asEXECUTION_ACTIVE;
}

void CScriptHelper::DebugRun()
{
	if(_hThreadDebug != NULL) 
	{
		AddCommand(SCRIPT_DEBUG_CONTINUE);
		return;
	}

	m_nCmd = SCRIPT_DEBUG_SETP_OVER;
	m_nCmdLine = 0;
	m_lastCommandAtStackLevel = 0;
	m_lastLine = 0;

	int x = ctx->GetState();
	//ThreadFunDebug();
 	UINT id;
 	_hThreadDebug = (HANDLE)_beginthreadex(NULL, 0, _ThreadFunDebug, this, 0, &id);
}

BOOL CScriptHelper::IsCanDebugRun()
{
	return TRUE;
}

void CScriptHelper::DebugStop()
{
	if(_hThreadDebug != NULL)
	{
		AddCommand(SCRIPT_DEBUG_ABORT);
	}
}

BOOL CScriptHelper::IsCanDebugStop()
{
	return ctx->GetState() == asEXECUTION_ACTIVE;
}

void CScriptHelper::DebugStepInto()
{
	AddCommand(SCRIPT_DEBUG_SETP_INTO);
}

BOOL CScriptHelper::IsCanDebugStepInto()
{
	return ctx->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepOver()
{
	AddCommand(SCRIPT_DEBUG_SETP_OVER);
}

BOOL CScriptHelper::IsCanDebugStepOver()
{
	return ctx->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepReturn()
{
	AddCommand(SCRIPT_DEBUG_SETP_OUT);
}

BOOL CScriptHelper::IsCanDebugStepReturn()
{
	return ctx->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepCursor()
{
	AddCommand(SCRIPT_DEBUG_SETP_CURSOR);
}

BOOL CScriptHelper::IsCanDebugStepCursor()
{
	return ctx->GetState() == asEXECUTION_ACTIVE ;
}

//////////////////////////////////////////////////////////////////////////
bool CScriptHelper::SetMainFun(LPCTSTR lpszMainFun)
{	
	int r = 0;
	LSSTRING_CONVERSION;
	asIScriptFunction *pFun = m_builder.GetModule()->GetFunctionByDecl(LST2UTF8(lpszMainFun));
	if(!pFun) return false;
	return ctx->Prepare(pFun) >= 0;
}

BOOL CScriptHelper::SetArgByte(UINT arg, BYTE value)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgByte(arg, value);
	return r>=0;
}

BOOL CScriptHelper::SetArgWord(UINT arg, WORD value)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgWord(arg, value);
	return r>=0;	
}

BOOL CScriptHelper::SetArgDWord(UINT arg, DWORD value)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgDWord(arg, value);
	return r>=0;	
}

BOOL CScriptHelper::SetArgFloat(UINT arg, float value)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgFloat(arg, value);
	return r>=0;	
}

BOOL CScriptHelper::SetArgDouble(UINT arg, double value)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgDouble(arg, value);
	return r>=0;
}

BOOL CScriptHelper::SetArgAddress(UINT arg, void *addr)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgAddress(arg, addr);
	return r>=0;
}

BOOL CScriptHelper::SetArgObject(UINT arg, void *obj)
{
	if(ctx == NULL)
		return FALSE;

	int r = ctx->SetArgObject(arg, obj);
	return r>=0;
}

void * CScriptHelper::GetAddressOfArg(UINT arg)
{
	if(ctx == NULL)
		return NULL;

	return ctx->GetAddressOfArg(arg);	
}

bool CScriptHelper::Excute()
{
	return CScriptManager::Execute();
}

BYTE CScriptHelper::GetReturnByte()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnByte();
}

WORD CScriptHelper::GetReturnWord()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnWord();	
}

DWORD CScriptHelper::GetReturnDWord()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnDWord();
}

float CScriptHelper::GetReturnFloat()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnFloat();
}

double CScriptHelper::GetReturnDouble()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnDouble();
}

void * CScriptHelper::GetReturnAddress()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnAddress();	
}

void * CScriptHelper::GetReturnObject()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetReturnObject();	
}

void * CScriptHelper::GetAddressOfReturnValue()
{
	if(ctx == NULL)
		return 0;

	return ctx->GetAddressOfReturnValue();	
}

}