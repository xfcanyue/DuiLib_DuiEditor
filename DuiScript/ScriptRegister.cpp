#include "StdAfx.h"
#include "ScriptRegister.h"

#include "RegCDuiString.h"
#include "RegCControlUI.h"

CScriptRegister::CScriptRegister(asIScriptEngine *engine_in) : engine(engine_in)
{
}


CScriptRegister::~CScriptRegister()
{
}


void CScriptRegister::RegisterAll()
{
	CRegCDuiString::Register(engine);
	CRegCControlUI::Register(engine);
}