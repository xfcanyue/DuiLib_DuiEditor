#pragma once
#include "RegCControlUI.h"

namespace DuiLib
{

class regCPaintManagerUI
{
protected:
	DECL_FACTORY(CPaintManagerUI);
public:
	static bool Register_Prepare(asIScriptEngine *engine)
	{
		CStringA classname = "CPaintManagerUI";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(CPaintManagerUI);

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
