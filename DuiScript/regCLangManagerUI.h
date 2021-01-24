#pragma once

namespace DuiLib
{

class regCLangManagerUI
{
protected:
	DECL_FACTORY(CLangManagerUI);
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "CLangManagerUI";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(CLangManagerUI);

		REG_METHOD_FUNPR(CLangManagerUI, void, ReloadLanguage, ());
		r = engine->RegisterGlobalFunction("void SetLanguage(LPCTSTR lpstrPath, LPCTSTR lpstrname)", asFUNCTION(CLangManagerUI::SetLanguage), asCALL_CDECL); 		assert( r >= 0 ); 
		r = engine->RegisterGlobalFunction("LPCTSTR GetString(int id, LPCTSTR defaultstring)", asFUNCTION(CLangManagerUI::GetString), asCALL_CDECL); 		assert( r >= 0 ); 
		r = engine->RegisterGlobalFunction("LPCTSTR GetStringExtra(int id, LPCTSTR defaultstring)", asFUNCTION(CLangManagerUI::GetStringExtra), asCALL_CDECL); 		assert( r >= 0 ); 
	}
};


} //namespace DuiLib