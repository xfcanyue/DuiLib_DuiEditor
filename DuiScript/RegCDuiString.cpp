#include "StdAfx.h"
#include "RegCDuiString.h"


static void CDuiStringFactoryGeneric(asIScriptGeneric *gen)
{
	asUINT length = gen->GetArgDWord(0);
	LPCTSTR s = (LPCTSTR)gen->GetArgAddress(1);

	// Return a string value
#ifdef _UNICODE
	new (gen->GetAddressOfReturnLocation()) CDuiString(s, length/2);
#else
	new (gen->GetAddressOfReturnLocation()) CDuiString(s, length);
#endif
}


static void ConstructStringGeneric(asIScriptGeneric * gen)
{
	new (gen->GetObject()) CDuiString();
}

static void CopyConstructStringGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetArgObject(0));
	new (gen->GetObject()) CDuiString(a->GetData(), a->GetLength());
}

static void DestructStringGeneric(asIScriptGeneric * gen)
{
	CDuiString * ptr = static_cast<CDuiString *>(gen->GetObject());
	ptr->~CDuiString();
}

static void AssignStringGeneric(asIScriptGeneric *gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetArgObject(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	*self = *a;
	gen->SetReturnAddress(self);
}

static void AddAssignStringGeneric(asIScriptGeneric *gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetArgObject(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	*self += *a;
	gen->SetReturnAddress(self);
}

static void StringEqualsGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	CDuiString * b = static_cast<CDuiString *>(gen->GetArgAddress(0));
	*(bool*)gen->GetAddressOfReturnLocation() = (*a == *b);
}

static void StringCmpGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	CDuiString * b = static_cast<CDuiString *>(gen->GetArgAddress(0));

	int cmp = 0;
	if( *a < *b ) cmp = -1;
	else if( *a > *b ) cmp = 1;

	*(int*)gen->GetAddressOfReturnLocation() = cmp;
}

static void StringAddGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	CDuiString * b = static_cast<CDuiString *>(gen->GetArgAddress(0));
	CDuiString ret_val = *a + *b;
	gen->SetReturnObject(&ret_val);
}

static void StringLengthGeneric(asIScriptGeneric * gen)
{
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	*static_cast<asUINT *>(gen->GetAddressOfReturnLocation()) = (asUINT)self->GetLength();
}

static void StringIsEmptyGeneric(asIScriptGeneric * gen)
{
	CDuiString * self = reinterpret_cast<CDuiString *>(gen->GetObject());
	*reinterpret_cast<bool *>(gen->GetAddressOfReturnLocation()) = self->IsEmpty();
}

static void StringResizeGeneric(asIScriptGeneric * gen)
{
	//CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	//self->resize(*static_cast<asUINT *>(gen->GetAddressOfArg(0)));
}

static void StringInsert_Generic(asIScriptGeneric *gen)
{
	// 	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	// 	asUINT pos = gen->GetArgDWord(0);
	// 	CDuiString *other = reinterpret_cast<CDuiString*>(gen->GetArgAddress(1));
	// 	StringInsert(pos, *other, *self);
}

static void StringErase_Generic(asIScriptGeneric *gen)
{
	// 	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());
	// 	asUINT pos = gen->GetArgDWord(0);
	// 	int count = int(gen->GetArgDWord(1));
	// 	StringErase(pos, count, *self);
}

static void StringFindFirst_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirst(*find, start, *self);
}

static void StringFindLast_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLast(*find, start, *self);
}

static void StringFindFirstOf_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirstOf(*find, start, *self);
}

static void StringFindLastOf_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLastOf(*find, start, *self);
}

static void StringFindFirstNotOf_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindFirstNotOf(*find, start, *self);
}

static void StringFindLastNotOf_Generic(asIScriptGeneric * gen)
{
	CDuiString *find = reinterpret_cast<CDuiString*>(gen->GetArgAddress(0));
	asUINT start = gen->GetArgDWord(1);
	CDuiString *self = reinterpret_cast<CDuiString *>(gen->GetObject());
	//	*reinterpret_cast<int *>(gen->GetAddressOfReturnLocation()) = StringFindLastNotOf(*find, start, *self);
}

static void StringCharAtGeneric(asIScriptGeneric * gen)
{
	unsigned int index = gen->GetArgDWord(0);
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	if (index >= (unsigned int)self->GetLength())
	{
		// Set a script exception
		asIScriptContext *ctx = asGetActiveContext();
		ctx->SetException("Out of range");

		gen->SetReturnAddress(0);
	}
	else
	{
		TCHAR ch = (*self)[index];
		gen->SetReturnAddress(&ch);
		//gen->SetReturnAddress(&(self->operator [](index)));
	}
}

//////////////////////////////////////////////////////////////////////////
static void AssignInt8_to_StringGeneric(asIScriptGeneric *gen)
{
	asINT64 *a = static_cast<asINT64*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	self->Format(_T("%c"), *a);

	gen->SetReturnAddress(self);
}

static void AddAssignInt8_to_StringGeneric(asIScriptGeneric * gen)
{
	asINT64 * a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	self->Format(_T("%s%c"), self->GetData(), TCHAR(*a));

	gen->SetReturnAddress(self);
}

static void AddString_to_Int8Generic(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	asINT64 * b = static_cast<asINT64 *>(gen->GetAddressOfArg(0));

	CDuiString str;
	str.Format(_T("%c"), *b);
	CDuiString ret_val = *a + str;

	gen->SetReturnObject(&ret_val);
}

static void AddInt8_to_StringGeneric(asIScriptGeneric * gen)
{
	asINT64* a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val.Format(_T("%c"), *a);
	ret_val += *b;

	gen->SetReturnObject(&ret_val);
}

//////////////////////////////////////////////////////////////////////////
static void AssignInt2StringGeneric(asIScriptGeneric *gen)
{
	asINT64 *a = static_cast<asINT64*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	self->Format(_T("%d"), *a);

	gen->SetReturnAddress(self);
}

static void AssignUInt2StringGeneric(asIScriptGeneric *gen)
{
	asQWORD *a = static_cast<asQWORD*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	self->Format(_T("%u"), *a);

	gen->SetReturnAddress(self);
}

static void AssignDouble2StringGeneric(asIScriptGeneric *gen)
{
	double *a = static_cast<double*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	self->Format(_T("%lf"), *a);

	gen->SetReturnAddress(self);
}

static void AssignFloat2StringGeneric(asIScriptGeneric *gen)
{
	float *a = static_cast<float*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	self->Format(_T("%f"), *a);

	gen->SetReturnAddress(self);
}

static void AssignBool2StringGeneric(asIScriptGeneric *gen)
{
	bool *a = static_cast<bool*>(gen->GetAddressOfArg(0));
	CDuiString *self = static_cast<CDuiString*>(gen->GetObject());

	*self = (*a ? _T("true") : _T("false"));

	gen->SetReturnAddress(self);
}

static void AddAssignDouble2StringGeneric(asIScriptGeneric * gen)
{
	double * a = static_cast<double *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	self->Format(_T("%s%lf"), self->GetData(), *a);

	gen->SetReturnAddress(self);
}

static void AddAssignFloat2StringGeneric(asIScriptGeneric * gen)
{
	float * a = static_cast<float *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	self->Format(_T("%s%f"), self->GetData(), *a);

	gen->SetReturnAddress(self);
}

static void AddAssignInt2StringGeneric(asIScriptGeneric * gen)
{
	asINT64 * a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	// 	CDuiString str;
	// 	str.Format(_T("%d"), *a);
	// 	*self += str;

	self->Format(_T("%s%ld"), self->GetData(), *a);

	gen->SetReturnAddress(self);
}

static void AddAssignUInt2StringGeneric(asIScriptGeneric * gen)
{
	asQWORD * a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	self->Format(_T("%s%u"), self->GetData(), *a);

	gen->SetReturnAddress(self);
}

static void AddAssignBool2StringGeneric(asIScriptGeneric * gen)
{
	bool * a = static_cast<bool *>(gen->GetAddressOfArg(0));
	CDuiString * self = static_cast<CDuiString *>(gen->GetObject());

	*self += *a ? _T("true") : _T("false");

	gen->SetReturnAddress(self);
}

static void AddString2DoubleGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	double * b = static_cast<double *>(gen->GetAddressOfArg(0));

	CDuiString str;
	str.Format(_T("%lf"), *b);
	CDuiString ret_val = *a + str;
	//CDuiString ret_val;
	//ret_val.Format(_T("%s%lf"), a->GetData(), *b);

	gen->SetReturnObject(&ret_val);
}

static void AddString2FloatGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	float * b = static_cast<float *>(gen->GetAddressOfArg(0));

	CDuiString str;
	str.Format(_T("%f"), *b);
	CDuiString ret_val = *a + str;
	//CDuiString ret_val;
	//ret_val.Format(_T("%s%f"), a->GetData(), *b);

	gen->SetReturnObject(&ret_val);
}

static void AddString2IntGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	asINT64 * b = static_cast<asINT64 *>(gen->GetAddressOfArg(0));

	CDuiString str;
	str.Format(_T("%d"), *b);
	CDuiString ret_val = *a + str;
	//CDuiString ret_val;
	//ret_val.Format(_T("%s%d"), a->GetData(), *b);

	gen->SetReturnObject(&ret_val);
}

static void AddString2UIntGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	asQWORD * b = static_cast<asQWORD *>(gen->GetAddressOfArg(0));

	CDuiString str;
	str.Format(_T("%u"), *b);
	CDuiString ret_val = *a + str;
	//CDuiString ret_val;
	//ret_val.Format(_T("%s%u"), a->GetData(), *b);

	gen->SetReturnObject(&ret_val);
}

static void AddString2BoolGeneric(asIScriptGeneric * gen)
{
	CDuiString * a = static_cast<CDuiString *>(gen->GetObject());
	bool * b = static_cast<bool *>(gen->GetAddressOfArg(0));

	CDuiString ret_val = *a;
	ret_val += *a ? _T("true") : _T("false");

	gen->SetReturnObject(&ret_val);
}

static void AddDouble2StringGeneric(asIScriptGeneric * gen)
{
	double* a = static_cast<double *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val.Format(_T("%lf"), *a);
	ret_val += *b;

	gen->SetReturnObject(&ret_val);
}

static void AddFloat2StringGeneric(asIScriptGeneric * gen)
{
	float* a = static_cast<float *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val.Format(_T("%f"), *a);
	ret_val += *b;

	gen->SetReturnObject(&ret_val);
}

static void AddInt2StringGeneric(asIScriptGeneric * gen)
{
	asINT64* a = static_cast<asINT64 *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val.Format(_T("%d"), *a);
	ret_val += *b;

	// 	CDuiString ret_val;
	// 	ret_val.Format(_T("%d%s"), *a, b->GetData());

	gen->SetReturnObject(&ret_val);
}

static void AddUInt2StringGeneric(asIScriptGeneric * gen)
{
	asQWORD* a = static_cast<asQWORD *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val.Format(_T("%u"), *a);
	ret_val += *b;

	gen->SetReturnObject(&ret_val);
}

static void AddBool2StringGeneric(asIScriptGeneric * gen)
{
	bool* a = static_cast<bool *>(gen->GetAddressOfArg(0));
	CDuiString * b = static_cast<CDuiString *>(gen->GetObject());

	CDuiString ret_val;
	ret_val += *a ? _T("true") : _T("false");
	ret_val += *b;

	gen->SetReturnObject(&ret_val);
}

static void StringSubString_Generic(asIScriptGeneric *gen)
{
	// Get the arguments
	CDuiString *str   = (CDuiString*)gen->GetObject();
	asUINT  start = *(int*)gen->GetAddressOfArg(0);
	int     count = *(int*)gen->GetAddressOfArg(1);

	// Return the substring
	//	new(gen->GetAddressOfReturnLocation()) CDuiString(StringSubString(start, count, *str));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CRegCDuiString::CRegCDuiString(void)
{
}


CRegCDuiString::~CRegCDuiString(void)
{
}

bool CRegCDuiString::Register(asIScriptEngine *engine)
{
	int r =0;

	// Register the string type
	r = engine->RegisterObjectType("string", sizeof(CDuiString), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK); assert( r >= 0 );

	// Register the string factory
	r = engine->RegisterStringFactory("string", asFUNCTION(CDuiStringFactoryGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the object operator overloads
	r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f()",                    asFUNCTION(ConstructStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT,  "void f(const string &in)",    asFUNCTION(CopyConstructStringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT,   "void f()",                    asFUNCTION(DestructStringGeneric),  asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asFUNCTION(AssignStringGeneric),    asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asFUNCTION(AddAssignStringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "bool opEquals(const string &in) const", asFUNCTION(StringEqualsGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asFUNCTION(StringCmpGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asFUNCTION(StringAddGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the object methods
	r = engine->RegisterObjectMethod("string", "uint length() const", asFUNCTION(StringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "void resize(uint)",   asFUNCTION(StringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "uint get_length() const", asFUNCTION(StringLengthGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "void set_length(uint)", asFUNCTION(StringResizeGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "bool isEmpty() const", asFUNCTION(StringIsEmptyGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Register the index operator, both as a mutator and as an inspector
	r = engine->RegisterObjectMethod("string", "uint8 &opIndex(uint)", asFUNCTION(StringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "const uint8 &opIndex(uint) const", asFUNCTION(StringCharAtGeneric), asCALL_GENERIC); assert( r >= 0 );

	// Automatic conversion from values
	r = engine->RegisterObjectMethod("string", "string &opAssign(double)", asFUNCTION(AssignDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(double)", asFUNCTION(AddAssignDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(double) const", asFUNCTION(AddString2DoubleGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(double) const", asFUNCTION(AddDouble2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(float)", asFUNCTION(AssignFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(float)", asFUNCTION(AddAssignFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(float) const", asFUNCTION(AddString2FloatGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(float) const", asFUNCTION(AddFloat2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(int8)", asFUNCTION(AssignInt8_to_StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(int8)", asFUNCTION(AddAssignInt8_to_StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(int8) const", asFUNCTION(AddString_to_Int8Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(int8) const", asFUNCTION(AddInt8_to_StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(int16)", asFUNCTION(AssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(int16)", asFUNCTION(AddAssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(int16) const", asFUNCTION(AddString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(int16) const", asFUNCTION(AddInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(int)", asFUNCTION(AssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(int)", asFUNCTION(AddAssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(int) const", asFUNCTION(AddString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(int) const", asFUNCTION(AddInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(int64)", asFUNCTION(AssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(int64)", asFUNCTION(AddAssignInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(int64) const", asFUNCTION(AddString2IntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(int64) const", asFUNCTION(AddInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(uint64)", asFUNCTION(AssignUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(uint64)", asFUNCTION(AddAssignUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(uint64) const", asFUNCTION(AddString2UIntGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(uint64) const", asFUNCTION(AddUInt2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string &opAssign(bool)", asFUNCTION(AssignBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string &opAddAssign(bool)", asFUNCTION(AddAssignBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd(bool) const", asFUNCTION(AddString2BoolGeneric), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectMethod("string", "string opAdd_r(bool) const", asFUNCTION(AddBool2StringGeneric), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectMethod("string", "string substr(uint start = 0, int count = -1) const", asFUNCTION(StringSubString_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findFirst(const string &in, uint start = 0) const", asFUNCTION(StringFindFirst_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findFirstOf(const string &in, uint start = 0) const", asFUNCTION(StringFindFirstOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findFirstNotOf(const string &in, uint start = 0) const", asFUNCTION(StringFindFirstNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findLast(const string &in, int start = -1) const", asFUNCTION(StringFindLast_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findLastOf(const string &in, int start = -1) const", asFUNCTION(StringFindLastOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "int findLastNotOf(const string &in, int start = -1) const", asFUNCTION(StringFindLastNotOf_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "void insert(uint pos, const string &in other)", asFUNCTION(StringInsert_Generic), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterObjectMethod("string", "void erase(uint pos, int count = -1)", asFUNCTION(StringErase_Generic), asCALL_GENERIC); assert(r >= 0);



	return r >= 0;	
}