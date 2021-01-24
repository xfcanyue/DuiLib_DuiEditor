#pragma once

namespace DuiLib
{
/*
	typedef struct UILIB_API tagTImageInfo
	{
		HBITMAP hBitmap;
		LPBYTE pBits;
		LPBYTE pSrcBits;
		int nX;
		int nY;
		bool bAlpha;
		bool bUseHSL;
		CDuiString sResType;
		DWORD dwMask;
	} TImageInfo;
*/
class regTImageInfo
{
	DECL_FACTORY(TImageInfo);
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "TImageInfo";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(TImageInfo);
	
		REG_OBJECT_PROPERTY(TImageInfo, HBITMAP, hBitmap);
		//REG_OBJECT_PROPERTY(TImageInfo, LPBYTE, pBits);
		//REG_OBJECT_PROPERTY(TImageInfo, LPBYTE, pSrcBits);
		REG_OBJECT_PROPERTY(TImageInfo, int, nX);
		REG_OBJECT_PROPERTY(TImageInfo, int, nY);
		REG_OBJECT_PROPERTY(TImageInfo, bool, bAlpha);
		REG_OBJECT_PROPERTY(TImageInfo, bool, bUseHSL);
		REG_OBJECT_PROPERTY(TImageInfo, CDuiString, sResType);
		REG_OBJECT_PROPERTY(TImageInfo, DWORD, dwMask);
	}
};


} //namespace DuiLib

