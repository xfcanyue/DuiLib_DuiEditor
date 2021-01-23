#pragma once

namespace DuiLib
{

class regSIZE
{

	static void SIZE_Construct(SIZE *obj)
	{
		new (obj) SIZE;
	}

	static void SIZE_Copy(const SIZE &other, SIZE *thisPointer)
	{
		new (thisPointer) SIZE(other);
	}

	static void SIZE_Destruct(SIZE *thisPointer)
	{
		thisPointer->~SIZE();
	}

	static SIZE SIZE_opAssign(const SIZE &other, SIZE &dest)
	{
		dest.cx = other.cx;
		dest.cy = other.cy;
		return dest;
	}

	//////////////////////////////////////////////////////////////////////////
	static void CDuiSize_Construct(CDuiSize *obj)
	{
		new (obj) CDuiSize();
	}

	static void CDuiSize_Copy(const CDuiSize &other, CDuiSize *thisPointer)
	{
		new (thisPointer) CDuiSize(other);
	}

	static void CDuiSize_Destruct(CDuiSize *thisPointer)
	{
		thisPointer->~CDuiSize();
	}

	static void CDuiSize_Construct2(const SIZE &other, CDuiSize *obj)
	{
		new (obj) CDuiSize(other);
	}

	static void CDuiSize_Construct3(int cx, int cy, CDuiSize *obj)
	{
		new (obj) CDuiSize(cx, cy);
	}

	static void CDuiSize_Construct4(const CDuiString &other, CDuiSize *obj)
	{
		new (obj) CDuiSize(other.GetData());
	}

	static void CDuiSize_Construct5(const CDuiRect &other, CDuiSize *obj)
	{
		new (obj) CDuiSize(other);
	}

	static CDuiSize CDuiSize_opAssign(const CDuiSize &other, CDuiSize &dest)
	{
		dest = other;
		return dest;
	}
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r = 0;

		//注册SIZE结构体
		r = engine->RegisterObjectType("SIZE", sizeof(SIZE), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("SIZE", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(SIZE_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("SIZE", asBEHAVE_CONSTRUCT,  "void f(const SIZE &in)", asFUNCTION(SIZE_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("SIZE", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(SIZE_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("SIZE", "SIZE &opAssign(const SIZE &in)", asMETHODPR(SIZE, operator =, (const SIZE&), SIZE&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectProperty("SIZE", "long cx", asOFFSET(SIZE, cx)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("SIZE", "long cy", asOFFSET(SIZE, cy)); assert( r >= 0 );

		//注册CDuiSize类
		r = engine->RegisterObjectType("CSize", sizeof(POINT), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(CDuiSize_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f(const CSize &in)", asFUNCTION(CDuiSize_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f(const SIZE &in)", asFUNCTION(CDuiSize_Construct2), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f(int x, int y)", asFUNCTION(CDuiSize_Construct3), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(CDuiSize_Construct4), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_CONSTRUCT,  "void f(const CRect &in)", asFUNCTION(CDuiSize_Construct5), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CSize", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(CDuiSize_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CSize", "CSize &opAssign(const CSize &in)", asMETHODPR(CDuiSize, operator =, (const CDuiSize&), CDuiSize&), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CSize", "bool FromString(const string &in)", asMETHOD(CDuiSize, FromString), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CSize", "string ToString()", asMETHOD(CDuiSize, ToString), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectProperty("CSize", "long cx", asOFFSET(CDuiSize, cx)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("CSize", "long cy", asOFFSET(CDuiSize, cy)); assert( r >= 0 );
		return r >= 0;
	}
};

} //namespace DuiLib

