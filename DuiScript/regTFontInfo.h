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

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(TFontInfo);
		
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

