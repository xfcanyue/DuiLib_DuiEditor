#pragma once

namespace DuiLib
{

class regRECT
{
	static void RECT_Construct(RECT *obj)
	{
		new (obj) RECT;
	}

	static void RECT_Copy(const RECT &other, RECT *thisPointer)
	{
		new (thisPointer) RECT(other);
	}

	static void RECT_Destruct(RECT *thisPointer)
	{
		thisPointer->~RECT();
	}

	static RECT RECT_opAssign(const RECT &other, RECT &dest)
	{
		dest.left = other.left;
		dest.top = other.top;
		dest.right = other.right;
		dest.bottom = other.bottom;
		return dest;
	}

	//////////////////////////////////////////////////////////////////////////
	static void CDuiRect_Construct(CDuiRect *obj)
	{
		new (obj) CDuiRect();
	}

	static void CDuiRect_Copy(const CDuiRect &other, CDuiRect *thisPointer)
	{
		new (thisPointer) CDuiRect(other);
	}

	static void CDuiRect_Destruct(CDuiRect *thisPointer)
	{
		thisPointer->~CDuiRect();
	}

	static void CDuiRect_Construct2(const RECT &other, CDuiRect *obj)
	{
		new (obj) CDuiRect(other);
	}

	static void CDuiRect_Construct3(int iLeft, int iTop, int iRight, int iBottom, CDuiRect *obj)
	{
		new (obj) CDuiRect(iLeft, iTop, iRight, iBottom);
	}

	static void CDuiRect_Construct4(const CDuiString &other, CDuiRect *obj)
	{
		new (obj) CDuiRect(other.GetData());
	}


	static CDuiRect CDuiRect_opAssign(const CDuiRect &other, CDuiRect &dest)
	{
		dest = other;
		return dest;
	}
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r = 0;

		//注册RECT结构体
		r = engine->RegisterObjectType("RECT", sizeof(RECT), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("RECT", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(RECT_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("RECT", asBEHAVE_CONSTRUCT,  "void f(const RECT &in)", asFUNCTION(RECT_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("RECT", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(RECT_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("RECT", "RECT &opAssign(const RECT &in)", asMETHODPR(RECT, operator =, (const RECT&), RECT&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectProperty("RECT", "long left", asOFFSET(RECT, left)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("RECT", "long top", asOFFSET(RECT, top)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("RECT", "long right", asOFFSET(RECT, right)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("RECT", "long bottom", asOFFSET(RECT, bottom)); assert( r >= 0 );

		//注册CDuiRect类
		r = engine->RegisterObjectType("CRect", sizeof(POINT), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(CDuiRect_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_CONSTRUCT,  "void f(const CRect &in)", asFUNCTION(CDuiRect_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_CONSTRUCT,  "void f(const POINT &in)", asFUNCTION(CDuiRect_Construct2), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_CONSTRUCT,  "void f(int, int, int, int)", asFUNCTION(CDuiRect_Construct3), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(CDuiRect_Construct4), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CRect", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(CDuiRect_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CRect", "CRect &opAssign(const CRect &in)", asMETHODPR(CDuiRect, operator =, (const CDuiRect&), CDuiRect&), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CRect", "bool FromString(const string &in)", asMETHOD(CDuiRect, FromString), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "string ToString()", asMETHOD(CDuiRect, ToString), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectMethod("CRect", "int GetWidth() const", asMETHOD(CDuiRect, GetWidth), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "int GetHeight() const", asMETHOD(CDuiRect, GetHeight), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Empty()", asMETHOD(CDuiRect, Empty), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "bool IsNull() const", asMETHOD(CDuiRect, IsNull), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Join(RECT rc)", asMETHOD(CDuiRect, Join), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void ResetOffset()", asMETHOD(CDuiRect, ResetOffset), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Normalize()", asMETHOD(CDuiRect, Normalize), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Offset(int cx, int cy)", asMETHOD(CDuiRect, Offset), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Inflate(int cx, int cy)", asMETHOD(CDuiRect, Inflate), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Deflate(int cx, int cy)", asMETHOD(CDuiRect, Deflate), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CRect", "void Union(CRect rc)", asMETHOD(CDuiRect, Union), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectProperty("CRect", "long left", asOFFSET(CDuiRect,left)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("CRect", "long top", asOFFSET(CDuiRect, top)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("CRect", "long right", asOFFSET(CDuiRect, right)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("CRect", "long bottom", asOFFSET(CDuiRect, bottom)); assert( r >= 0 );
		return r >= 0;
	}
};

} //namespace DuiLib

