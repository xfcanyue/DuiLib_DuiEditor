#pragma once

namespace DuiLib
{
	/*
	typedef struct UILIB_API tagTEventUI
	{
	int Type;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	TCHAR chKey;
	WORD wKeyState;
	WPARAM wParam;
	LPARAM lParam;
	} TEventUI;
	*/
class regTEventUI
{
	static TEventUI *TEventUI_Ref_Factory()
	{
		return new TEventUI();
	}
public:
	static void Register_Prepare(asIScriptEngine *engine)
	{
		int r =0;

		//Registering DuiLib Class
		r = engine->RegisterObjectType("TEventUI", 0, asOBJ_REF|asOBJ_NOCOUNT); 

		// Registering the factory behaviour
		r = engine->RegisterObjectBehaviour("TEventUI", asBEHAVE_FACTORY, "TEventUI@ f()", asFUNCTION(TEventUI_Ref_Factory), asCALL_CDECL);  assert( r >= 0 );
	}

	static void Register_Extra(asIScriptEngine *engine)
	{
		int r =0;

		r = engine->RegisterObjectProperty("TEventUI", "int Type", asOFFSET(TEventUI, Type));					assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "CControlUI &pSender", asOFFSET(TEventUI, pSender));		assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "DWORD dwTimestamp", asOFFSET(TEventUI, dwTimestamp));	assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "POINT ptMouse", asOFFSET(TEventUI, ptMouse));			assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "char chKey", asOFFSET(TEventUI, ptMouse));				assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "WORD wKeyState", asOFFSET(TEventUI, ptMouse));			assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "WPARAM wParam", asOFFSET(TEventUI, wParam));			assert( r >= 0 );
		r = engine->RegisterObjectProperty("TEventUI", "LPARAM lParam", asOFFSET(TEventUI, lParam));			assert( r >= 0 );
	}
};

} //namespace DuiLib

