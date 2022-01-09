#pragma once

namespace DuiLib
{


class regUIFont
{
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

class regUIPen
{
	static UIPen *UIPen_Ref_Factory() { return UIGlobal::CreatePen(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIPen";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIPen);


	}	
};

class regUIBitmap
{
	static UIBitmap *UIBitmap_Ref_Factory() { return UIGlobal::CreateBitmap(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIBitmap";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIBitmap);
	}	
};

class regUIImage
{
	static UIImage *UIImage_Ref_Factory() { return UIGlobal::CreateImage(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIImage";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIImage);
	}
};

class regUIBrush
{
	static UIBrush *UIBrush_Ref_Factory() { return UIGlobal::CreateBrush(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIBrush";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIBrush);


	}	
};
} //namespace DuiLib

