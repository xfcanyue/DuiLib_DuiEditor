#pragma once

namespace DuiLib
{


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


} //namespace DuiLib

