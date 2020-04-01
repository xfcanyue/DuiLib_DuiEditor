#include "stdafx.h"
#include "ScriptManager.h"

//#include "ScriptBuilder.h"
//#include "ScriptRegsiter.h"


namespace DuiLib
{

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
CScriptManager::CScriptManager(void) 
{
	
}


CScriptManager::~CScriptManager(void)
{
	
}

void CScriptManager::AddScriptContext(LPCTSTR pScriptCode)
{

}

void CScriptManager::AddScriptFile(LPCTSTR pstrFileName)
{

}

bool CScriptManager::ExecuteScript(LPCTSTR funName, void *pControl)
{
	return false;
}

bool CScriptManager::ExecuteScript(LPCTSTR funName, void *pControl, void *ev)
{
	return false;
}

}