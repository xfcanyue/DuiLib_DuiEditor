#include "StdAfx.h"
#include "RegCControlUI.h"


CRegCControlUI::CRegCControlUI(void)
{
}


CRegCControlUI::~CRegCControlUI(void)
{
}

//////////////////////////////////////////////////////////////////////////
CControlUI *CControlUI_Ref_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new CControlUI();
}

static void CControlUI_SetText(const CDuiString &str, CControlUI &obj)
{
	obj.SetText(str);
}

bool CRegCControlUI::Register(asIScriptEngine *engine)
{
	int r =0;

	//Registering DuiLib Class
	r = engine->RegisterObjectType("CControlUI", 0, asOBJ_REF|asOBJ_NOCOUNT); 

	// Registering the factory behaviour
	r = engine->RegisterObjectBehaviour("CControlUI", asBEHAVE_FACTORY, "CControlUI@ f()", asFUNCTION(CControlUI_Ref_Factory), asCALL_CDECL);  assert( r >= 0 );

	// Register class method functions
	r = engine->RegisterObjectMethod("CControlUI", "void SetBkColor(int dwBackColor)", asMETHOD(CControlUI, SetBkColor), asCALL_THISCALL);  assert( r >= 0 );

	r = engine->RegisterObjectMethod("CControlUI", "void SetText(const string &in)", asFUNCTIONPR(CControlUI_SetText, (const CDuiString &, CControlUI &), void), asCALL_CDECL_OBJLAST);  assert( r >= 0 );

	return r >= 0;
}