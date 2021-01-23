#pragma once

namespace DuiLib
{
	/*
	typedef struct UILIB_API tagTFontInfo
	{
		HFONT hFont;
		CDuiString sFontName;
		int iSize;
		bool bBold;
		bool bUnderline;
		bool bItalic;
		TEXTMETRIC tm;
	} TFontInfo;
	*/
class regTFontInfo
{
	DECL_FACTORY(TFontInfo);
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "TFontInfo";
		std::string factoryName = classname; factoryName += "@ f()";
		std::string factoryFunc = classname; factoryFunc += "_Ref_Factory";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		r = engine->RegisterObjectBehaviour(classname, asBEHAVE_FACTORY, factoryName.c_str(), asFUNCTION(factoryFunc.c_str()), asCALL_CDECL);  assert( r >= 0 );

		REG_OBJECT_PROPERTY(TFontInfo, HFONT, hFont);
		REG_OBJECT_PROPERTY(TFontInfo, CDuiString, sFontName);
		REG_OBJECT_PROPERTY(TFontInfo, int, iSize);
		REG_OBJECT_PROPERTY(TFontInfo, bool, bBold);
		REG_OBJECT_PROPERTY(TFontInfo, bool, bUnderline);
		REG_OBJECT_PROPERTY(TFontInfo, bool, bItalic);
		//REG_OBJECT_PROPERTY(TFontInfo, TEXTMETRIC, tm);
	}
};


} //namespace DuiLib

