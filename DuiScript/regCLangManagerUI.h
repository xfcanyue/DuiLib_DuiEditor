#pragma once

namespace DuiLib
{

class regCLangManagerUI
{
protected:
	DECL_FACTORY(CLangManagerUI);
	static void regCLangManagerUI::SetLanguage(CDuiString lpstrPath, CDuiString lpstrname)
	{
		CLangManagerUI::SetLanguage(lpstrPath.GetData(), lpstrname.GetData());
	}

	static CDuiString regCLangManagerUI::GetString(int id, CDuiString defaultstring)
	{
		return CLangManagerUI::GetString(id, defaultstring);
	}

	static CDuiString regCLangManagerUI::GetStringExtra(int id, CDuiString defaultstring)
	{
		return CLangManagerUI::GetStringExtra(id, defaultstring);
	}
public:
	static void Register(asIScriptEngine *engine)
	{
		CDuiStringA classname = "CLangManagerUI";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(CLangManagerUI);

		REG_METHOD_FUNPR(CLangManagerUI, void, ReloadLanguage, ());
		r = engine->RegisterGlobalFunction("void SetLanguage(string strPath, string strname)", asFUNCTION(SetLanguage), asCALL_CDECL); 		assert( r >= 0 ); 
		r = engine->RegisterGlobalFunction("string GetString(int id, string defaultstring)", asFUNCTION(GetString), asCALL_CDECL); 		assert( r >= 0 ); 
		r = engine->RegisterGlobalFunction("string GetStringExtra(int id, string defaultstring)", asFUNCTION(GetStringExtra), asCALL_CDECL); 		assert( r >= 0 ); 
	}
};


} //namespace DuiLib