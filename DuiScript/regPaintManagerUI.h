#pragma once
#include "RegCControlUI.h"

namespace DuiLib
{

class regPaintManagerUI
{
protected:
	DECL_FACTORY(CPaintManagerUI);
public:
	static bool Register_Prepare(asIScriptEngine *engine)
	{
		CStringA classname = "CPaintManagerUI";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 

		std::string factoryName = classname; factoryName += "@ f()";
		std::string factoryFunc = classname; factoryFunc += "_Ref_Factory";

		r = engine->RegisterObjectBehaviour(classname, asBEHAVE_FACTORY, factoryName.c_str(), asFUNCTION(factoryFunc.c_str()), asCALL_CDECL);  assert( r >= 0 );
//		r = engine->RegisterObjectBehaviour(classname, asBEHAVE_ADDREF, "void f()", asMETHOD(CPaintManagerUI, __AddRef), asCALL_THISCALL); assert( r >= 0 );
//		r = engine->RegisterObjectBehaviour(classname, asBEHAVE_RELEASE, "void f()", asMETHOD(CPaintManagerUI, __ReleaseRef), asCALL_THISCALL); assert( r >= 0 );

		return r>=0;
	}

	static void Register_Extra(asIScriptEngine *engine)
	{
		CStringA classname = "CPaintManagerUI";
		int r = 0;
		REG_METHOD_FUNPR(CPaintManagerUI, CControlUI*, FindControl, (LPCTSTR pstrName) const);
	}
};

} //namespace DuiLib
