#pragma once

namespace DuiLib
{

	/*
	typedef struct tagTNotifyUI
	{
	CDuiString sType;
	CDuiString sVirtualWnd;
	CControlUI* pSender;
	DWORD dwTimestamp;
	POINT ptMouse;
	WPARAM wParam;
	LPARAM lParam;
	} TNotifyUI;
	*/
class regTNotifyUI
{
protected:
	static TNotifyUI *TNotifyUI_Ref_Factory()
	{
		return new TNotifyUI();
	}
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r =0;

		//Registering DuiLib Class
		r = engine->RegisterObjectType("TNotifyUI", 0, asOBJ_REF|asOBJ_NOCOUNT); 

		// Registering the factory behaviour
		r = engine->RegisterObjectBehaviour("TNotifyUI", asBEHAVE_FACTORY, "TNotifyUI@ f()", asFUNCTION(TNotifyUI_Ref_Factory), asCALL_CDECL);  assert( r >= 0 );

		r = engine->RegisterObjectProperty("TNotifyUI", "string sType", asOFFSET(TNotifyUI, sType));				assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "string sVirtualWnd", asOFFSET(TNotifyUI, sVirtualWnd));	assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "CControlUI &pSender", asOFFSET(TNotifyUI, pSender));		assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "DWORD dwTimestamp", asOFFSET(TNotifyUI, dwTimestamp));		assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "POINT ptMouse", asOFFSET(TNotifyUI, ptMouse));				assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "WPARAM wParam", asOFFSET(TNotifyUI, wParam));				assert( r >= 0 );
		r = engine->RegisterObjectProperty("TNotifyUI", "LPARAM lParam", asOFFSET(TNotifyUI, lParam));				assert( r >= 0 );

		return r >= 0;
	}
};

} //namespace DuiLib

