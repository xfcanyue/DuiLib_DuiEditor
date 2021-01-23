#include <assert.h>
#include <sstream>
#include "scriptstdtime.h"
#include "AsTime.h"

using namespace std;

BEGIN_AS_NAMESPACE

static datetime *Ref_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new datetime();
}

static void ConstructDateTime(datetime *time)
{
	new (time) datetime();
}

static void CopyConstructDateTime(const datetime &other, datetime *thisPointer)
{
	new(thisPointer) datetime(other);
}

static void DestructDateTime(datetime *thisPointer)
{
	thisPointer->~datetime();
}

static int DateTimeopEquals(const datetime &a, const datetime &b)
{
	int cmp = 0;
	if( a == b ) 
		cmp = 1;
	else //a!=b
		cmp = 0;
	return cmp;
}

static int DateTimeCmp(const datetime &a, const datetime &b)
{
	int cmp = 0;
	if( a < b ) cmp = -1;
	else if( a > b ) cmp = 1;
	return cmp;
}
//////////////////////////////////////////////////////////////////////////
void RegisterStdTime(asIScriptEngine * engine)
{
 	int r;

	// Register the datetime reference type
 	r = engine->RegisterObjectType("datetime", sizeof(datetime), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(ConstructDateTime), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT,  "void f(const datetime &in)", asFUNCTION(CopyConstructDateTime), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(DestructDateTime),  asCALL_CDECL_OBJLAST); assert( r >= 0 );


/*	//定义reference
	r = engine->RegisterObjectType("datetime", sizeof(datetime), asOBJ_REF); assert( r >= 0 );
	
	// Registering the factory behaviour
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_FACTORY, "datetime@ f()", asFUNCTION(Ref_Factory), asCALL_CDECL); assert( r >= 0 );
	
	//	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(ConstructDateTime), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_CONSTRUCT,  "void f(const datetime &in)", asFUNCTION(CopyConstructDateTime), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_DESTRUCT,   "void f()", asFUNCTION(DestructDateTime),  asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	// Registering the addref/release behaviours
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_ADDREF, "void f()", asMETHOD(datetime,AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("datetime", asBEHAVE_RELEASE, "void f()", asMETHOD(datetime,Release), asCALL_THISCALL); assert( r >= 0 );
*/
	// = 拷贝构造函数
	r = engine->RegisterObjectMethod("datetime", "datetime &opAssign(const datetime &in)", asMETHODPR(datetime, operator =, (const datetime&), datetime&), asCALL_THISCALL); assert( r >= 0 );
 	// == ; !=
	r = engine->RegisterObjectMethod("datetime", "bool opEquals(const datetime &in) const", asFUNCTION(DateTimeopEquals), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	// < ; > ; <= ; >=
	r = engine->RegisterObjectMethod("datetime", "int opCmp(const datetime &in) const", asFUNCTION(DateTimeCmp), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		
	r = engine->RegisterObjectMethod("datetime", "void GetCurrentTime()", asMETHOD(datetime,GetTime), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)", asMETHOD(datetime,SetDateTime), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool SetDate(int nYear, int nMonth, int nDay)", asMETHOD(datetime,SetDate), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool SetTime(int nHour, int nMin, int nSec)", asMETHOD(datetime,SetTime), asCALL_THISCALL);assert( r >= 0 );
	
	r = engine->RegisterObjectMethod("datetime", "int GetYear()", asMETHOD(datetime,GetYear), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetMonth()", asMETHOD(datetime,GetMonth), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetDay()", asMETHOD(datetime,GetDay), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetHour()", asMETHOD(datetime,GetHour), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetMin()", asMETHOD(datetime,GetMin), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetSec()", asMETHOD(datetime,GetSec), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetDayOfWeek()", asMETHOD(datetime,GetDayOfWeek), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetDayOfYear()", asMETHOD(datetime,GetDayOfYear), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int GetDays(const datetime &in)",	asMETHOD(datetime,GetDays), asCALL_THISCALL);assert( r >= 0 );

	r = engine->RegisterObjectMethod("datetime", "bool AddDay(int n)", asMETHOD(datetime,AddDay), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool AddHour(int n)", asMETHOD(datetime,AddHour), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool AddMin(int n)", asMETHOD(datetime,AddMin), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "bool AddSec(int n)", asMETHOD(datetime,AddSec), asCALL_THISCALL);assert( r >= 0 );

	//r = engine->RegisterObjectMethod("datetime", "string Format(string &in)", asMETHOD(datetime,Format), asCALL_THISCALL);assert( r >= 0 );

	r = engine->RegisterObjectMethod("datetime", "int DiffTotalDays(const datetime &in)", asMETHOD(datetime,DiffTotalDays), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffTotalDaysEx(const datetime &in)", asMETHOD(datetime,DiffTotalDaysEx), asCALL_THISCALL);assert( r >= 0 );

	r = engine->RegisterObjectMethod("datetime", "int DiffTotalHours(const datetime &in)", asMETHOD(datetime,DiffTotalHours), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffTotalHoursEx(const datetime &in)", asMETHOD(datetime,DiffTotalHoursEx), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffHours(const datetime &in)", asMETHOD(datetime,DiffHours), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffHoursEx(const datetime &in)", asMETHOD(datetime,DiffHoursEx), asCALL_THISCALL);assert( r >= 0 );

	r = engine->RegisterObjectMethod("datetime", "int DiffTotalMins(const datetime &in)", asMETHOD(datetime,DiffTotalMins), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffTotalMinsEx(const datetime &in)", asMETHOD(datetime,DiffTotalMinsEx), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffMins(const datetime &in)", asMETHOD(datetime,DiffMins), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffMinsEx(const datetime &in)", asMETHOD(datetime,DiffMinsEx), asCALL_THISCALL);assert( r >= 0 );

	r = engine->RegisterObjectMethod("datetime", "int DiffTotalSecs(const datetime &in)", asMETHOD(datetime,DiffTotalSecs), asCALL_THISCALL);assert( r >= 0 );
	r = engine->RegisterObjectMethod("datetime", "int DiffSecs(const datetime &in)", asMETHOD(datetime,DiffSecs), asCALL_THISCALL);assert( r >= 0 );


	//全局函数
	r = engine->RegisterGlobalFunction("int DiffTotalDays(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalDays), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffTotalDaysEx(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalDaysEx), asCALL_CDECL);assert( r >= 0 );
	
	r = engine->RegisterGlobalFunction("int DiffTotalHours(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalHours), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffTotalHoursEx(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalHoursEx), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffHours(const datetime &in, const datetime &in)", asFUNCTION(DiffHours), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffHoursEx(const datetime &in, const datetime &in)", asFUNCTION(DiffHoursEx), asCALL_CDECL);assert( r >= 0 );
	
	r = engine->RegisterGlobalFunction("int DiffTotalMins(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalMins), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffTotalMinsEx(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalMinsEx), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffMins(const datetime &in, const datetime &in)", asFUNCTION(DiffMins), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffMinsEx(const datetime &in, const datetime &in)", asFUNCTION(DiffMinsEx), asCALL_CDECL);assert( r >= 0 );
	
	r = engine->RegisterGlobalFunction("int DiffTotalSecs(const datetime &in, const datetime &in)", asFUNCTION(DiffTotalSecs), asCALL_CDECL);assert( r >= 0 );
	r = engine->RegisterGlobalFunction("int DiffSecs(const datetime &in, const datetime &in)", asFUNCTION(DiffSecs), asCALL_CDECL);assert( r >= 0 );

}

END_AS_NAMESPACE




