#pragma once

namespace DuiLib
{

class regCDuiMd5
{
protected:
	static void CDuiMd5_Construct(CDuiMd5 *obj)
	{
		new (obj) CDuiMd5();
	}

	static void CDuiMd5_Destruct(CDuiMd5 *thisPointer)
	{
		thisPointer->~CDuiMd5();
	}

public:
	static bool Register(asIScriptEngine *engine)
	{
		int r = 0;

		//×¢²áÀà
		r = engine->RegisterObjectType("CMd5", sizeof(CDuiMd5), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CMd5", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(CDuiMd5_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CMd5", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(CDuiMd5_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		REG_METHOD_FUNPR2("CMd5", CDuiMd5, CDuiString, MakeMd5, (LPCTSTR));
		//r = engine->RegisterObjectMethod("CMd5", "string MakeMD5(string s)", asMETHODPR(CDuiMd5, MakeMD5, (LPCTSTR), CDuiString), asCALL_THISCALL); assert( r >= 0 );

		return r >= 0;
	}

// protected:
// 	static void CDuiMd5_Construct(CDuiMd5 *obj)
// 	{
// 		new (obj) CDuiMd5();
// 	}
// 
// 	static void CDuiMd5_Destruct(CDuiMd5 *thisPointer)
// 	{
// 		thisPointer->~CDuiMd5();
// 	}
// 
// public:
// 	static bool Register(asIScriptEngine *engine)
// 	{
// 		int r =0;
// 
// 		//×¢²áÀà
// 		r = engine->RegisterObjectType("CMd5", sizeof(CDuiMd5), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );
// 		r = engine->RegisterObjectBehaviour("CMd5", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(CDuiMd5_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
// 		r = engine->RegisterObjectBehaviour("CMd5", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(CDuiMd5_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );
// 		
// 		REG_METHOD_FUNPR2("CMd5", CDuiMd5, CDuiString,  MakeMD5, (const char *strSource, int len));
// 		return r >= 0;	
// 	}
};

} //namespace DuiLib

