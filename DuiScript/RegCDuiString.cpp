#include "StdAfx.h"
#include "RegCDuiString.h"

#include <map>      // std::map

namespace DuiLib {

typedef std::map<CDuiString, int> map_t;

class RegCDuiStringFactory : public asIStringFactory
{
public:
	RegCDuiStringFactory() {}
	~RegCDuiStringFactory() 
	{
		// The script engine must release each string 
		// constant that it has requested
		assert(stringCache.size() == 0);
	}

	const void *GetStringConstant(const char *data, asUINT length)
	{
		// The string factory might be modified from multiple 
		// threads, so it is necessary to use a mutex.
		asAcquireExclusiveLock();

#ifdef _UNICODE
		CDuiString str((const wchar_t *)data, length/sizeof(wchar_t));
#else
		CDuiString str((const char *)data, length);
#endif

		map_t::iterator it = stringCache.find(str);
		if (it != stringCache.end())
			it->second++;
		else
			it = stringCache.insert(map_t::value_type(str, 1)).first;

		asReleaseExclusiveLock();

		return reinterpret_cast<const void*>(&it->first);
	}

	int  ReleaseStringConstant(const void *str)
	{
		if (str == 0)
			return asERROR;

		int ret = asSUCCESS;

		// The string factory might be modified from multiple 
		// threads, so it is necessary to use a mutex.
		asAcquireExclusiveLock();

#ifdef _DEBUG
		CDuiString *sTemp = (CDuiString *)str;
#endif

		map_t::iterator it = stringCache.find(*reinterpret_cast<const CDuiString*>(str));
		if (it == stringCache.end())
			ret = asERROR;
		else
		{
			it->second--;
			if (it->second == 0)
				stringCache.erase(it);
		}

		asReleaseExclusiveLock();

		return ret;
	}

	int  GetRawStringData(const void *str, char *data, asUINT *length) const
	{
		if (str == 0)
			return asERROR;

		if (length)
		{
			*length = (asUINT)reinterpret_cast<const CDuiString*>(str)->GetLength();
#ifdef _UNICODE
			*length *= 2;
#endif
		}

		if (data)
		{
#ifdef _DEBUG
			CDuiString *sTemp = (CDuiString *)str;
#endif
#ifdef _UNICODE
			memcpy(data, reinterpret_cast<const CDuiString*>(str)->GetData(), reinterpret_cast<const CDuiString*>(str)->GetLength()*sizeof(wchar_t));
#else
			memcpy(data, reinterpret_cast<const CDuiString*>(str)->GetData(), reinterpret_cast<const CDuiString*>(str)->GetLength());
#endif		
		}

		return asSUCCESS;
	}

	// THe access to the string cache is protected with the common mutex provided by AngelScript
	map_t stringCache;
};

static RegCDuiStringFactory *stringFactory = 0;

// TODO: Make this public so the application can also use the string 
//       factory and share the string constants if so desired, or to
//       monitor the size of the string factory cache.
RegCDuiStringFactory *GetStdStringFactorySingleton()
{
	if( stringFactory == 0 )
	{
		// Make sure no other thread is creating the string factory at the same time
		asAcquireExclusiveLock();
		if (stringFactory == 0)
		{
			// The following instance will be destroyed by the global 
			// CStdStringFactoryCleaner instance upon application shutdown
			stringFactory = new RegCDuiStringFactory();
		}
		asReleaseExclusiveLock();
	}
	return stringFactory;
}

class RegCDuiStringFactoryCleaner
{
public:
	~RegCDuiStringFactoryCleaner()
	{
		if (stringFactory)
		{
			// Only delete the string factory if the stringCache is empty
			// If it is not empty, it means that someone might still attempt
			// to release string constants, so if we delete the string factory
			// the application might crash. Not deleting the cache would
			// lead to a memory leak, but since this is only happens when the
			// application is shutting down anyway, it is not important.
			if (stringFactory->stringCache.empty())
			{
				delete stringFactory;
				stringFactory = 0;
			}
		}
	}
};

static RegCDuiStringFactoryCleaner cleaner;

// CDuiString regCDuiString::StringFactory(asUINT length, LPCTSTR s)
// {
// #ifdef _UNICODE
// 	return CDuiString(s, length/2);
// #else
// 	return CDuiString(s, length);
// #endif
// }

void regCDuiString::String_Construct_DuiString(CDuiString *thisPointer)
{
	new(thisPointer) CDuiString();
}

void regCDuiString::String_CopyConstruct(const CDuiString &other, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(other);
}

void regCDuiString::String_Construct_LPCTSTR(LPCTSTR str, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(str);
}

void regCDuiString::String_Construct_bool(bool val, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(val);
}

void regCDuiString::String_Construct_int(int val, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(val);
}

void regCDuiString::String_Construct_int64(DuiLib::Int64 val, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(val);
}

void regCDuiString::String_Construct_double(double val, int base, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(val, base);
}

void regCDuiString::String_Construct_float(float val, int base, CDuiString *thisPointer)
{
	new(thisPointer) CDuiString(val, base);
}

void regCDuiString::String_Destruct(CDuiString *thisPointer)
{
	thisPointer->~CDuiString();
}

CDuiString &regCDuiString::SetAt(int n, CDuiString ch, CDuiString &dest)
{
	if(!ch.IsEmpty())
	{
		dest.SetAt(n, ch.GetAt(0));
	}
	return dest;
}

CDuiString regCDuiString::GetAt(int n, CDuiString &dest)
{
	return dest.GetAt(n);
}

CDuiString regCDuiString::opAddString(const CDuiString &str, const CDuiString &dest)
{
	return str + dest;
}

//////////////////////////////////////////////////////////////////////////
CDuiString regCDuiString::opAddString(bool val, const CDuiString &str)
{
	return str + val;
}

//////////////////////////////////////////////////////////////////////////
CDuiString regCDuiString::opAddString(int val, const CDuiString &str)
{
	return str + val;
}

//////////////////////////////////////////////////////////////////////////
CDuiString regCDuiString::opAddString(DuiLib::Int64 val, const CDuiString &str)
{
	return str + val;
}

//////////////////////////////////////////////////////////////////////////
CDuiString &regCDuiString::opAssign(double val, CDuiString &dest)
{
	dest = CDuiString(val,2);
	return dest;
}

CDuiString &regCDuiString::opAddAssign(double val, CDuiString &dest)
{
	dest += CDuiString(val,2);
	return dest;
}

CDuiString regCDuiString::opAddString(double val, const CDuiString &str)
{
	return str + val;
}

//////////////////////////////////////////////////////////////////////////
CDuiString &regCDuiString::opAssign(float val, CDuiString &dest)
{
	dest = CDuiString(val,2);
	return dest;
}

CDuiString &regCDuiString::opAddAssign(float val, CDuiString &dest)
{
	dest += CDuiString(val,2);
	return dest;
}

CDuiString regCDuiString::opAddString(float val, const CDuiString &str)
{
	return str + val;
}

LPCTSTR regCDuiString::StringToLPCTSTR1(const CDuiString &b)
{
	return b.GetData();
}

LPCTSTR regCDuiString::StringToLPCTSTR2(const CDuiString &b)
{
	return b.GetData();
}

LPCTSTR regCDuiString::StringToLPCTSTR3(const CDuiString &b)
{
	return b.GetData();
}

LPCTSTR regCDuiString::StringToLPCTSTR4(const CDuiString &b)
{
	return b.GetData();
}

bool regCDuiString::Register(asIScriptEngine *engine)
{
	int r =0;

	//注册字符串类
	r = engine->RegisterObjectType("string", sizeof(CDuiString), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

	//注册字符串类工厂
	//r = engine->RegisterStringFactory("string", asFUNCTION(StringFactory), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterStringFactory("string", GetStdStringFactorySingleton());

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
	r = engine->RegisterObjectMethod("string", "LPCTSTR opImplConv() const", asFUNCTION(StringToLPCTSTR1), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "LPCTSTR opConv() const", asFUNCTION(StringToLPCTSTR2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "LPCTSTR opCast() const", asFUNCTION(StringToLPCTSTR3), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "LPCTSTR opImplCast() const", asFUNCTION(StringToLPCTSTR4), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	return r >= 0;	
}

} //namespace DuiLib {

