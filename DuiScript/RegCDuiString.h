#pragma once

namespace DuiLib {

class regCDuiString
{
protected:
	static CDuiString StringFactory(asUINT length, LPCTSTR s);
	static void String_Construct_DuiString(CDuiString *thisPointer);
	static void String_CopyConstruct(const CDuiString &other, CDuiString *thisPointer);
	static void String_Construct_LPCTSTR(LPCTSTR str, CDuiString *thisPointer);
	static void String_Construct_bool(bool val, CDuiString *thisPointer);
	static void String_Construct_int(int val, CDuiString *thisPointer);
	static void String_Construct_int64(DuiLib::Int64 val, CDuiString *thisPointer);
	static void String_Construct_double(double val, int base, CDuiString *thisPointer);
	static void String_Construct_float(float val, int base, CDuiString *thisPointer);
	static void String_Destruct(CDuiString *thisPointer);
	static CDuiString &SetAt(int n, CDuiString ch, CDuiString &dest);
	static CDuiString GetAt(int n, CDuiString &dest);
	static CDuiString opAddString(const CDuiString &str, const CDuiString &dest);
	static CDuiString opAddString(bool val, const CDuiString &str);
	static CDuiString opAddString(int val, const CDuiString &str);
	static CDuiString opAddString(DuiLib::Int64 val, const CDuiString &str);
	static CDuiString &opAssign(double val, CDuiString &dest);
	static CDuiString &opAddAssign(double val, CDuiString &dest);
	static CDuiString opAddString(double val, const CDuiString &str);
	static CDuiString &opAssign(float val, CDuiString &dest);
	static CDuiString &opAddAssign(float val, CDuiString &dest);
	static CDuiString opAddString(float val, const CDuiString &str);
	static LPCTSTR StringToLPCTSTR1(const CDuiString &b);
	static LPCTSTR StringToLPCTSTR2(const CDuiString &b);
	static LPCTSTR StringToLPCTSTR3(const CDuiString &b);
	static LPCTSTR StringToLPCTSTR4(const CDuiString &b);

public:
	static bool Register(asIScriptEngine *engine);
};

} //namespace DuiLib {

