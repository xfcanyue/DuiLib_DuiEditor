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
class regUIFont
{
	//DECL_FACTORY(UIFont);
	static UIFont *UIFont_Ref_Factory() { return UIGlobal::CreateFont(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIFont";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIFont);
	}
};


} //namespace DuiLib

