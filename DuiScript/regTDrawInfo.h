#pragma once

namespace DuiLib
{


/*
typedef struct UILIB_API tagTDrawInfo
{
	tagTDrawInfo();
	void Parse(LPCTSTR pStrImage, LPCTSTR pStrModify, CPaintManagerUI *paintManager);
	void Clear();

	CDuiString sDrawString;
	CDuiString sDrawModify;
	CDuiString sImageName;
	CDuiString sResType;
	RECT rcDest;
	RECT rcSource;
	RECT rcCorner;
	DWORD dwMask;
	BYTE uFade;
	bool bHole;
	bool bTiledX;
	bool bTiledY;
	bool bHSL;
	int width;
	int height;
} TDrawInfo;
*/
class regTDrawInfo
{
	DECL_FACTORY(TDrawInfo);
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "TDrawInfo";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(TDrawInfo);
		
		REG_METHOD_FUNPR(TDrawInfo, void, Parse, (LPCTSTR pStrImage, LPCTSTR pStrModify, CPaintManagerUI *paintManager) );
		REG_METHOD_FUNPR(TDrawInfo, void, Clear, ());

		REG_OBJECT_PROPERTY(TDrawInfo, CDuiString, sDrawString);
		REG_OBJECT_PROPERTY(TDrawInfo, CDuiString, sDrawModify);
		REG_OBJECT_PROPERTY(TDrawInfo, CDuiString, sImageName);
		REG_OBJECT_PROPERTY(TDrawInfo, CDuiString, sResType);
		REG_OBJECT_PROPERTY(TDrawInfo, RECT, rcDest);
		REG_OBJECT_PROPERTY(TDrawInfo, RECT, rcSource);
		REG_OBJECT_PROPERTY(TDrawInfo, RECT, rcCorner);
		REG_OBJECT_PROPERTY(TDrawInfo, DWORD, dwMask);
		REG_OBJECT_PROPERTY(TDrawInfo, BYTE, uFade);
		REG_OBJECT_PROPERTY(TDrawInfo, bool, bHole);
		REG_OBJECT_PROPERTY(TDrawInfo, bool, bTiledX);
		REG_OBJECT_PROPERTY(TDrawInfo, bool, bTiledY);
		REG_OBJECT_PROPERTY(TDrawInfo, bool, bHSL);
		REG_OBJECT_PROPERTY(TDrawInfo, int, width);
		REG_OBJECT_PROPERTY(TDrawInfo, int, height);
	}
};


} //namespace DuiLib

