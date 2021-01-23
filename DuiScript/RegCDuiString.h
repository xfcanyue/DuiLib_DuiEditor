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

	static void String_Construct(string *thisPointer)
	{
		new(thisPointer) CDuiString();
	}

	static void String_CopyConstruct(const CDuiString &other, CDuiString *thisPointer)
	{
		new(thisPointer) CDuiString(other);
	}

	static void String_Construct2(LPCTSTR str, string *thisPointer)
	{
		new(thisPointer) CDuiString(str);
	}

	static void String_Destruct(CDuiString *thisPointer)
	{
		thisPointer->~CDuiString();
	}
	//////////////////////////////////////////////////////////////////////////
	static CDuiString &opAssignString(const CDuiString &str, CDuiString &dest)
	{
		dest = str;
		return dest;
	}

	static CDuiString &opAddAssignString(const CDuiString &str, CDuiString &dest)
	{
		dest += str;
		return dest;
	}

	static CDuiString opAddString(const CDuiString &str, const CDuiString &dest)
	{
		return str + dest;
	}
	//////////////////////////////////////////////////////////////////////////
	static CDuiString &opAssignInt(int n, CDuiString &dest)
	{
		CDuiString str;
		str.Format(_T("%d"), n);
		dest = str;
		return dest;
	}

	static CDuiString &opAddAssignInt(int n, CDuiString &dest)
	{
		CDuiString str;
		str.Format(_T("%d"), n);
		dest += str;
		return dest;
	}

	static CDuiString opAddInt(int n, const CDuiString &dest)
	{
		CDuiString str;
		str.Format(_T("%d"), n);
		return str + dest;
	}
	//////////////////////////////////////////////////////////////////////////
	static CDuiString &opAssignBool(bool b, CDuiString &dest)
	{
		dest = b ? _T("true") : _T("false");
		return dest;
	}

	static CDuiString &opAddAssignBool(bool b, CDuiString &dest)
	{
		dest += b ? _T("true") : _T("false");
		return dest;
	}

	static CDuiString opAddBool(bool b, const CDuiString &dest)
	{
		return dest + (b ? _T("true") : _T("false"));
	}

	//字符串比较 ==
	static bool StringEquals(const CDuiString& lhs, const CDuiString& rhs)
	{
		return lhs == rhs;
	}

	//字符串比较
	static int StringCmp(const CDuiString &a, const CDuiString &b)
	{
		return a.Compare(b);
	}

	//字符串比较，不区分大小写
	static int StringCmpNoCase(const CDuiString &a, const CDuiString &b)
	{
		return a.CompareNoCase(b);
	}

	//搜索字符串
	static int StringFind(const CDuiString &str, int iPos, const CDuiString &dest)
	{
		return dest.Find(str, iPos);
	}

	//反向搜索
	static int StringReverseFind(const CDuiString &str, const CDuiString &dest)
	{
		return dest.ReverseFind(str.GetAt(0));
	}

	//替换
	static int StringReplace(const CDuiString &From, const CDuiString &To, CDuiString &dest)
	{
		return dest.Replace(From, To);
	}

	static CDuiString GetAt(int n, CDuiString &dest)
	{
		return dest.GetAt(n);
	}

	static LPCTSTR StringToLPCTSTR(const CDuiString &b)
	{
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
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(String_Construct), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//拷贝构造函数 CDuiString(const CDuiString& src);
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(String_CopyConstruct), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//构造函数 CDuiString(const LPCTSTR src);
		//r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(LPCTSTR &in)", asFUNCTION(String_Construct2), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//析构函数 ~CDuiString();
		r = engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(String_Destruct),  asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asFUNCTION(opAssignString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asFUNCTION(opAddAssignString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asFUNCTION(opAddString), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(int)", asFUNCTION(opAssignInt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(int)", asFUNCTION(opAddAssignInt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(int) const", asFUNCTION(opAddInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		//赋值函数  operator =
		r = engine->RegisterObjectMethod("string", "string &opAssign(bool)", asFUNCTION(opAssignBool), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +=
		r = engine->RegisterObjectMethod("string", "string &opAddAssign(bool)", asFUNCTION(opAddAssignBool), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		//字符串连接 operator +
		r = engine->RegisterObjectMethod("string", "string opAdd(bool) const", asFUNCTION(opAddBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		//字符串是否相等
		//bool operator == (LPCTSTR str) const;
		//bool operator != (LPCTSTR str) const;
		r = engine->RegisterObjectMethod("string", "bool opEquals(const string &in) const", asFUNCTIONPR(StringEquals, (const CDuiString &, const CDuiString &), bool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		//字符串比较大小 
		// 	bool operator <= (LPCTSTR str) const;
		// 	bool operator <  (LPCTSTR str) const;
		// 	bool operator >= (LPCTSTR str) const;
		// 	bool operator >  (LPCTSTR str) const;
		r = engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asFUNCTION(StringCmp), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

		//字符串比较 Compare
		r = engine->RegisterObjectMethod("string", "int Compare(const string &in) const", asFUNCTION(StringCmp), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//字符串比较 CompareNoCase
		r = engine->RegisterObjectMethod("string", "int CompareNoCase(const string &in) const", asFUNCTION(StringCmpNoCase), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "void MakeUpper()", asMETHOD(CDuiString, MakeUpper), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "void MakeLower()", asMETHOD(CDuiString, MakeLower), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "int GetLength() const", asMETHOD(CDuiString, GetLength), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "void Empty()", asMETHOD(CDuiString, Empty), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "bool IsEmpty()", asMETHOD(CDuiString, IsEmpty), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string opIndex(int)", asFUNCTION(GetAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "const string opIndex(int) const", asFUNCTION(GetAt), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string GetAt(int)", asFUNCTIONPR(GetAt, (int nIndex, CDuiString &), CDuiString), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "string Left(int nLength)", asMETHOD(CDuiString, Left), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string Mid(int iPos, int nLength)", asMETHOD(CDuiString, Mid), asCALL_THISCALL);  assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "string Right(int nLength)", asMETHOD(CDuiString, Right), asCALL_THISCALL);  assert( r >= 0 );

		r = engine->RegisterObjectMethod("string", "int Find(const string &in, uint iPos=0)", asFUNCTION(StringFind), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "int ReverseFind(const string &in) const", asFUNCTION(StringReverseFind), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "int Replace(const string &in, const string &in)", asFUNCTION(StringReplace), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		//类型转换  CDuiString转为LPCTSTR
		r = engine->RegisterObjectMethod("string", "LPCTSTR opImplConv() const", asFUNCTION(StringToLPCTSTR), asCALL_CDECL_OBJLAST); assert( r >= 0 );
		r = engine->RegisterObjectMethod("string", "LPCTSTR opConv() const", asFUNCTION(StringToLPCTSTR), asCALL_CDECL_OBJLAST); assert( r >= 0 );

		return r >= 0;	
	}
};

} //namespace DuiLib

