#pragma once

namespace DuiLib
{

class regUIGlobal
{
public:
	static void Register(asIScriptEngine *engine)
	{
		int r = 0;

		REG_GLOBAL_FUNPR(UIRender *,	UIGlobal::CreateRenderTarget,	()	);
		REG_GLOBAL_FUNPR(UIFont *,		UIGlobal::CreateFont,			()	);
		REG_GLOBAL_FUNPR(UIPen *,		UIGlobal::CreatePen,			()	);
		REG_GLOBAL_FUNPR(UIBitmap *,	UIGlobal::CreateBitmap,			()	);
		REG_GLOBAL_FUNPR(UIBrush *,		UIGlobal::CreateBrush,			()	);
		REG_GLOBAL_FUNPR(UIImage *,		UIGlobal::CreateImage,			()	);
	}
};

class regUIRender
{
	static UIRender *UIRender_Ref_Factory() { return UIGlobal::CreateRenderTarget(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIRender";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIRender);

		REG_METHOD_FUNPR(TDrawInfo, void, Clear, ());
	}
};


} //namespace DuiLib