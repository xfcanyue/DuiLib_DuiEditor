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

	m_pFun = NULL;
	m_pUser = 0;

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
		m_lastCommandAtStackLevel = m_ctx->CTX()->GetCallstackSize();
	}
	SetEvent(m_hEventDebug);
}

BOOL CScriptHelper::CheckBreakPoint(int line)
{
	tagScriptMessage msg;
	msg.type = usMsg_CheckBreakPoint;
	msg.line = line;
	msg.breakpoint = false;
	CallMessageCallback(&msg);
	return msg.breakpoint;
}

void CScriptHelper::GotoLine(int line)
{
	tagScriptMessage msg;
	msg.type = usMsg_GoToLine;
	msg.line = line;
	CallMessageCallback(&msg);
}

void CScriptHelper::PrintContext()
{
	tagScriptMessage msg;
	msg.type = usMsg_PrintContext;
	msg.line = 0;
	msg.ctx = m_ctx->CTX();
	CallMessageCallback(&msg);
}

void CScriptHelper::MessageCallback(const asSMessageInfo &msg)
{
	const char *type = "\0";
	if( msg.type == asMSGTYPE_ERROR )
		type = "ERR : ";
	else if( msg.type == asMSGTYPE_WARNING ) 
	{
		type = "WARN : ";
	}
// 	else if( msg.type == asMSGTYPE_INFORMATION ) 
// 	{
// 		type = "INFO : ";
// 	}

	char MsgStr[2048];
	if(msg.row == 0 && msg.col == 0)
	{
		sprintf_s(MsgStr, "%s%s", type, msg.message);
	}
	else
		sprintf_s(MsgStr, "%s (%d, %d) : %s%s", msg.section, msg.row, msg.col, type, msg.message);

	tagScriptMessage smsg;
	smsg.type = usMsg_Message;
	smsg.line = 0;
	smsg.message = MsgStr;
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
	msg.message = NULL;

	int r = 0;
	r = m_ctx->Execute();
	
	if( r == asEXECUTION_FINISHED )
	{
		//脚本执行成功
		msg.type = usMsg_RunEnd;
		msg.ctx = m_ctx->CTX();
		CallMessageCallback(&msg);
	}
	else
	{
		//脚本执行异常
		msg.type = usMsg_Message;

		char MsgStr[1024];
		// The execution didn't finish as we had planned. Determine why.
		if( r == asEXECUTION_ABORTED )
		{
			msg.message = "The script was aborted.";
		}
		else if( r == asEXECUTION_EXCEPTION )
		{
			// Write some information about the script exception
			asIScriptFunction *func = m_ctx->CTX()->GetExceptionFunction();
			sprintf_s(MsgStr, "func: %s\r\nmodl: %s\r\nsect: %s\r\nline: %d\r\ndesc: %s", 
				func->GetDeclaration(),
				func->GetModuleName(),
				func->GetScriptSectionName(),
				m_ctx->CTX()->GetExceptionLineNumber(),
				m_ctx->CTX()->GetExceptionString());
			msg.message = MsgStr;
		}
		else
		{
			sprintf_s(MsgStr, "The script ended for some unforeseen reason: %d", r);
			msg.message = MsgStr;
		}

		CallMessageCallback(&msg);

		msg.type = usMsg_RunAbort; 
		CallMessageCallback(&msg);
	}

	asThreadCleanup();
	if(_hThreadDebug != NULL){ ::CloseHandle(_hThreadDebug); _hThreadDebug = NULL; }
	return 0;
}

// IScriptContext *CScriptHelper::CreateContext()
// {
// 	return CScriptManager::CreateContext();
// }
// 
// void CScriptHelper::ReleaseContext(IScriptContext *ctx)
// {
// 	return CScriptManager::ReleaseContext((IScriptContext *)ctx);
// }

CDuiString CScriptHelper::AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pstrModuleName)
{
	return CScriptManager::AddScriptFile(pstrFileName, pstrModuleName);
}

CDuiString CScriptHelper::AddScriptCode(LPCTSTR pstrCode, LPCTSTR pstrModuleName)
{
	return CScriptManager::AddScriptCode(pstrCode, pstrModuleName);
}

bool CScriptHelper::RemoveScript(LPCTSTR pstrModuleName)
{
	return CScriptManager::RemoveScript(pstrModuleName);
}

void CScriptHelper::RemoveAllScript()
{
	return CScriptManager::RemoveAllScript();
}

bool CScriptHelper::CompileScript()
{
	return CScriptManager::CompileScript();
}

BOOL CScriptHelper::IsRunning()
{
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE;
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

	int x = m_ctx->CTX()->GetState();
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
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE;
}

void CScriptHelper::DebugStepInto()
{
	AddCommand(SCRIPT_DEBUG_SETP_INTO);
}

BOOL CScriptHelper::IsCanDebugStepInto()
{
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepOver()
{
	AddCommand(SCRIPT_DEBUG_SETP_OVER);
}

BOOL CScriptHelper::IsCanDebugStepOver()
{
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepReturn()
{
	AddCommand(SCRIPT_DEBUG_SETP_OUT);
}

BOOL CScriptHelper::IsCanDebugStepReturn()
{
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE ;
}

void CScriptHelper::DebugStepCursor()
{
	AddCommand(SCRIPT_DEBUG_SETP_CURSOR);
}

BOOL CScriptHelper::IsCanDebugStepCursor()
{
	return m_ctx->CTX()->GetState() == asEXECUTION_ACTIVE ;
}

//////////////////////////////////////////////////////////////////////////

}