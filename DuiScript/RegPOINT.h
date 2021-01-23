#pragma once

namespace DuiLib
{


//注册POINT 和 CDuiPoint
class regPOINT
{
	static void POINT_Construct(POINT *obj)
	{
		new (obj) POINT;
	}

	static void POINT_Copy(const POINT &other, POINT *thisPointer)
	{
		new (thisPointer) POINT(other);
	}

	static void POINT_Destruct(POINT *thisPointer)
	{
		thisPointer->~POINT();
	}

	static POINT POINT_opAssign(const POINT &other, POINT &dest)
	{
		dest.x = other.x;
		dest.y = other.y;
		return dest;
	}

	//////////////////////////////////////////////////////////////////////////
	static void CDuiPoint_Construct(CDuiPoint *obj)
	{
		new (obj) CDuiPoint();
	}

	static void CDuiPoint_Copy(const CDuiPoint &other, CDuiPoint *thisPointer)
	{
		new (thisPointer) CDuiPoint(other);
	}

	static void CDuiPoint_Destruct(CDuiPoint *thisPointer)
	{
		thisPointer->~CDuiPoint();
	}

	static void CDuiPoint_Construct2(const POINT &other, CDuiPoint *obj)
	{
		new (obj) CDuiPoint(other);
	}

	static void CDuiPoint_Construct3(int x, int y, CDuiPoint *obj)
	{
		new (obj) CDuiPoint(x, y);
	}

	static void CDuiPoint_Construct4(LPARAM lParam, CDuiPoint *obj)
	{
		new (obj) CDuiPoint(lParam);
	}

	static void CDuiPoint_Construct5(const CDuiString &other, CDuiPoint *obj)
	{
		new (obj) CDuiPoint(other.GetData());
	}


	static CDuiPoint CDuiPoint_opAssign(const CDuiPoint &other, CDuiPoint &dest)
	{
		dest = other;
		return dest;
	}
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r =0;

		//注册POINT结构体
		r = engine->RegisterObjectType("POINT", sizeof(POINT), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("POINT", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(POINT_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("POINT", asBEHAVE_CONSTRUCT,  "void f(const POINT &in)", asFUNCTION(POINT_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("POINT", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(POINT_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("POINT", "POINT &opAssign(const POINT &in)", asMETHODPR(POINT, operator =, (const POINT&), POINT&), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectProperty("POINT", "long x", asOFFSET(POINT, x)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("POINT", "long y", asOFFSET(POINT, y)); assert( r >= 0 );

		//注册CDuiPoint类
		r = engine->RegisterObjectType("CPoint", sizeof(POINT), asOBJ_VALUE| asOBJ_APP_CLASS_CDAK   ); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(CDuiPoint_Construct), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f(const CPoint &in)", asFUNCTION(CDuiPoint_Copy), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f(const POINT &in)", asFUNCTION(CDuiPoint_Construct2), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f(int x, int y)", asFUNCTION(CDuiPoint_Construct3), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f(LPARAM lParam)", asFUNCTION(CDuiPoint_Construct4), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(CDuiPoint_Construct5), asCALL_CDECL_OBJLAST);  assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("CPoint", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(CDuiPoint_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CPoint", "CPoint &opAssign(const CPoint &in)", asMETHODPR(CDuiPoint, operator =, (const CDuiPoint&), CDuiPoint&), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("CPoint", "bool FromString(const string &in)", asMETHOD(CDuiPoint, FromString), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("CPoint", "string ToString()", asMETHOD(CDuiPoint, ToString), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectProperty("CPoint", "long x", asOFFSET(CDuiPoint, x)); assert( r >= 0 );
		r = engine->RegisterObjectProperty("CPoint", "long y", asOFFSET(CDuiPoint, y)); assert( r >= 0 );
		return r >= 0;
	}
};

} //namespace DuiLib

