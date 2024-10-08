#pragma once

namespace DuiLib
{

class regCDuiString
{
protected:
	static CDuiString StringFactory(asUINT length, LPCTSTR s)
	{
#ifdef _UNICODE
		return CDuiString(s, length/2);
#else
		return CDuiString(s, length);
#endif
	}

	static void String_Construct_DuiString(string *thisPointer)
	{
		new(thisPointer) CDuiString();
	}

	static void String_CopyConstruct(const CDuiString &other, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(other);
	}

	static void String_Construct_LPCTSTR(LPCTSTR str, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(str);
	}

	static void String_Construct_bool(bool val, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(val);
	}

	static void String_Construct_int(int val, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(val);
	}

	static void String_Construct_int64(DuiLib::Int64 val, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(val);
	}

	static void String_Construct_double(double val, int base, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(val, base);
	}

	static void String_Construct_float(float val, int base, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(val, base);
	}

	static void String_Destruct(CDuiString *thisPointer)
	{
		thisPointer->~CDuiString();
	}

	static CDuiString &SetAt(int n, CDuiString ch, CDuiString &dest)
	{
		if(!ch.IsEmpty())
		{
			dest.SetAt(n, ch.GetAt(0));
		}
		return dest;
	}

	static CDuiString GetAt(int n, CDuiString &dest)
	{
		return dest.GetAt(n);
	}

	static CDuiString opAddString(const CDuiString &str, const CDuiString &dest)
	{
		return str + dest;
	}

	//////////////////////////////////////////////////////////////////////////
	static CDuiString opAddString(bool val, const CDuiString &str)
	{
		return str + val;
	}

	//////////////////////////////////////////////////////////////////////////
	static CDuiString opAddString(int val, const CDuiString &str)
	{
		return str + val;
	}

	//////////////////////////////////////////////////////////////////////////
	static CDuiString opAddString(DuiLib::Int64 val, const CDuiString &str)
	{
		return str + val;
	}

	//////////////////////////////////////////////////////////////////////////
	static CDuiString &opAssign(double val, CDuiString &dest)
	{
		dest = CDuiString(val,2);
		return dest;
	}

	static CDuiString &opAddAssign(double val, CDuiString &dest)
	{
		dest += CDuiString(val,2);
		return dest;
	}

	static CDuiString opAddString(double val, const CDuiString &str)
	{
		return str + val;
	}

	//////////////////////////////////////////////////////////////////////////
	static CDuiString &opAssign(float val, CDuiString &dest)
	{
		dest = CDuiString(val,2);
		return dest;
	}

	static CDuiString &opAddAssign(float val, CDuiString &dest)
	{
		dest += CDuiString(val,2);
		return dest;
	}

	static CDuiString opAddString(float val, const CDuiString &str)
	{
		return str + val;
	}

	static LPCTSTR StringToLPCTSTR(const CDuiString &b)
	{
		//return b;
		return b.GetData();
	}
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r =0;

		//注册字符串类
		r = engine->RegisterObjectType("string", sizeof(CDuiString), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

		//注册字符串类工厂
		r = engine->RegisterStringFactory("string", asFUNCTION(StringFactory), asCALL_CDECL); assert( r >= 0 );

		//默认构造函数 CDuiString();
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(String_Construct_DuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//拷贝构造函数 CDuiString(const CDuiString& src);
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(String_CopyConstruct), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//构造函数 CDuiString(const LPCTSTR src);
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(LPCTSTR &in)", asFUNCTION(String_Construct_LPCTSTR), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(bool)", asFUNCTION(String_Construct_bool), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(int)", asFUNCTION(String_Construct_int), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(int64)", asFUNCTION(String_Construct_int64), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(double, int)", asFUNCTION(String_Construct_double), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(float, int)", asFUNCTION(String_Construct_float), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//析构函数 ~CDuiString();
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(String_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		REG_METHOD_FUNPR2("string", CDuiString, void, SetBufferLength, (int iLength)); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, int, GetBufferLength, ()); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, void, Empty, ()); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, int, GetLength, () const); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, bool, IsEmpty, () const); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string &SetAt(int, string)", asFUNCTION(SetAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "const string opIndex(int) const", asFUNCTION(GetAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string GetAt(int)", asFUNCTION(GetAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asMETHODPR(CDuiString, Assign, (const CDuiString &), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asMETHODPR(CDuiString, Append, (const CDuiString &), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asFUNCTIONPR(opAddString, (const CDuiString &, const CDuiString &), CDuiString), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		
		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(bool)", asMETHODPR(CDuiString, Assign, (bool), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(bool)", asMETHODPR(CDuiString, Append, (bool), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(bool) const", asFUNCTIONPR(opAddString, (bool, const CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(int)", asMETHODPR(CDuiString, Assign, (int), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(int)", asMETHODPR(CDuiString, Append, (int), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(int) const", asFUNCTIONPR(opAddString, (int, const CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(int64)", asMETHODPR(CDuiString, Assign, (DuiLib::Int64), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(int64)", asMETHODPR(CDuiString, Append, (DuiLib::Int64), CDuiString&), asCALL_THISCALL); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(int64) const", asFUNCTIONPR(opAddString, (DuiLib::Int64, const CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(double)", asFUNCTIONPR(opAssign, (double, CDuiString&), CDuiString&), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(double)", asFUNCTIONPR(opAddAssign, (double, CDuiString&), CDuiString&), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(double) const", asFUNCTIONPR(opAddString, (double, const CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(float)", asFUNCTIONPR(opAssign, (float, CDuiString&), CDuiString&), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(float)", asFUNCTIONPR(opAddAssign, (float, CDuiString&), CDuiString&), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(float) const", asFUNCTIONPR(opAddString, (float, const CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		
		//为了兼容
		r = engine->RegisterObjectMethod("string", "void setFloat(float,int)", asMETHODPR(CDuiString, Assign, (float,int), CDuiString&), asCALL_THISCALL); assert( r >= 0 );

		//////////////////////////////////////////////////////////////////////////
		REG_METHOD_FUNPR2("string", CDuiString, bool, toBool, (bool def) const); assert( r >= 0 );					
		REG_METHOD_FUNPR2("string", CDuiString, int, toInt, (int def) const); assert( r >= 0 );	
		r = engine->RegisterObjectMethod("string", "int64 toInt64(int64 def) const", asMETHODPR(CDuiString, toInt64, (DuiLib::Int64 def) const, DuiLib::Int64), asCALL_THISCALL); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, double, toDouble, (double def) const); assert( r >= 0 );
		REG_METHOD_FUNPR2("string", CDuiString, float, toFloat, (float def) const); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "int64 HexToInt64()", asMETHOD(CDuiString, HexToInt64), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string toHex()", asMETHOD(CDuiString, toHex), asCALL_THISCALL); assert( r >= 0 );

		//字符串是否相等
		//bool operator == (LPCTSTR str) const;
		//bool operator != (LPCTSTR str) const;
		r = engine->RegisterObjectMethod("string", "bool opEquals(string) const", asMETHOD(CDuiString, IsEquals), asCALL_THISCALL); assert( r >= 0 );

		//字符串比较大小 
		// 	bool operator <= (LPCTSTR str) const;
		// 	bool operator <  (LPCTSTR str) const;
		// 	bool operator >= (LPCTSTR str) const;
		// 	bool operator >  (LPCTSTR str) const;
		r = engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asMETHOD(CDuiString, Compare), asCALL_THISCALL); assert( r >= 0 );




		REG_METHOD_FUNPR2("string", CDuiString, void, MakeUpper, ()); 

		REG_METHOD_FUNPR2("string", CDuiString, void, MakeLower, ()); 

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString, Left, (int iLength) const); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string Mid(int iPos, int iLength=-1) const", asMETHOD(CDuiString, Mid), asCALL_THISCALL); assert( r >= 0 );

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString, Right, (int iLength) const); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string Section(string, int start, int end=-1) const", asMETHOD(CDuiString, Section), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string ReverseSection(string, int start, int end=-1) const", asMETHOD(CDuiString, ReverseSection), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "int Find(string, int iPos=0) const", asMETHODPR(CDuiString, Find, (LPCTSTR, int) const, int), asCALL_THISCALL); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "int ReverseFind(string, int iPos=0) const", asMETHODPR(CDuiString, ReverseFind, (LPCTSTR, int) const, int), asCALL_THISCALL); assert( r >= 0 );

		REG_METHOD_FUNPR2("string", CDuiString, int, Replace, (LPCTSTR pstrFrom, LPCTSTR pstrTo));

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, TrimLeft, ());

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, TrimRight, ());

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, Trim, ());

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, Remove, (LPCTSTR pstr));

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, Delete, (int nIndex, int nCount));

		REG_METHOD_FUNPR2("string", CDuiString, CDuiString &, ReverseDelete, (int nIndex, int nCount));








		//类型转换  CDuiString转为LPCTSTR
		r = engine->RegisterObjectMethod("string", "LPCTSTR opImplConv() const", asFUNCTION(StringToLPCTSTR), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "LPCTSTR opConv() const", asFUNCTION(StringToLPCTSTR), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		return r >= 0;	
	}
};

} //namespace DuiLib

