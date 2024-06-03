#pragma once

#define DEFUNC(x)	#x

#define REG_OBJECT_TYPE(class) { \
	r = engine->RegisterObjectType(#class, 0, asOBJ_REF|asOBJ_NOCOUNT);	\
	assert( r >= 0 );	\
}

//定义类工厂函数
#define DECL_FACTORY(x) static x *x##_Ref_Factory() { return new x(); }

//注册类工厂
#define REG_FACTORY(x) {	\
	r = engine->RegisterObjectBehaviour(#x, asBEHAVE_FACTORY, DEFUNC(x##@ f()), asFUNCTION( x##_Ref_Factory), asCALL_CDECL);  \
	assert( r >= 0 );	\
}

//注册AddRef
#define DECL_ADD_REF(class, fun) {	\
	r = engine->RegisterObjectBehaviour(#class, asBEHAVE_ADDREF, "void f()", asMETHOD(class, fun), asCALL_THISCALL); \
	assert( r >= 0 );	\
}

//注册Release
#define DECL_RELEASE_REF(class, fun) { \
	r = engine->RegisterObjectBehaviour(#class, asBEHAVE_RELEASE, "void f()", asMETHOD(class, fun), asCALL_THISCALL); \
	assert( r >= 0 );	\
}

/*
注：注册到脚本中的函数，参数或返回值包含LPCTSTR的, 将转为string类型
*/

//////////////////////////////////////////////////////////////////////////
//注册类函数
#define REG_METHOD_FUNPR(class, ret, n, p)	{	\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");						\
	sFun.Replace("LPCTSTR", "string");			\
	r = engine->RegisterObjectMethod(#class, sFun, asMETHODPR(class, n, p, ret), asCALL_THISCALL);  \
	assert( r >= 0 ); \
}

//注册类函数, 直接使用类名
#define REG_METHOD_FUNPR2(regclassname, class, ret, n, p)	{	\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");						\
	sFun.Replace("LPCTSTR", "string");			\
	r = engine->RegisterObjectMethod(regclassname, sFun, asMETHODPR(class, n, p, ret), asCALL_THISCALL);  \
	assert( r >= 0 ); \
}

//注册类成员
#define REG_OBJECT_PROPERTY(class, type, value) { \
	CStringA sFun = DEFUNC(type value);	\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");						\
	sFun.Replace("LPCTSTR", "string");			\
	r = engine->RegisterObjectProperty(#class, sFun, asOFFSET(class, value)); \
	assert( r >= 0 ); \
}

//////////////////////////////////////////////////////////////////////////
//注册全局函数, 注册时若是类的静态函数如Class::Fun(), 需要把"Class::"移除，注册为全局函数;
#define REG_GLOBAL_FUNPR(ret, n, p)	{			\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");						\
	sFun.Replace("LPCTSTR", "string");			\
	if(sFun.Find("::") >= 0)					\
	{											\
		int start = sFun.Find(" ");				\
		if(start < 0) start = 0;				\
		sFun.Delete(start+1, sFun.Find("::")+2-start-1);	\
	}											\
	r = engine->RegisterGlobalFunction(sFun, asFUNCTIONPR(n, p, ret), asCALL_CDECL);  \
	assert( r >= 0 ); \
}

//定义控件类工厂
#define DECL_CONTROL_FACTORY(x)	static x *x##_Ref_Factory() { return new x(); }
#define DECL_CONTROL_REGFACT(x) virtual void reg_factory() { int r = 0; REG_FACTORY(x); }
#define REG_CONTROL_FACTORY() reg_factory()


//注册控件的成员函数
#define REG_CONTROL_FUNPR(ret,n,p)	{			\
	CStringA sFun = DEFUNC(ret n##p);			\
	sFun.Replace("CDuiString", "string");		\
	sFun.Replace("CDuiPoint", "CPoint");		\
	sFun.Replace("CDuiRect", "CRect");			\
	sFun.Replace("CDuiSize", "CSize");			\
	sFun.Replace("*", "@");	\
	sFun.Replace("LPCTSTR", "string");			\
	r = engine->RegisterObjectMethod(classname, sFun, asMETHODPR(T, n, p, ret), asCALL_THISCALL);  \
	assert( r >= 0 ); \
}

//注册duilib控件
#define REGISTER_CONTROL(x) { reg##x<x> ctrl; ctrl.classname=#x;  ctrl.reg(engine); }	


#define DECL_PROP_BOOL(m)		static bool m##property = m;		//定义bool全局变量
#define DECL_PROP_INT(m)		static int m##property = m;			//定义int全局变量
#define DECL_PROP_UINT(m)		static UINT m##property = m;		//定义UINT全局变量
#define DECL_PROP_STRING(m)		static CDuiString m##property = m;	//定义string全局变量
#define PROP_PARAPETER(type,value)	type##value	//连接typevalue字符串
#define REGI_PROP_BOOL(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const bool ",#m), &##m##property); assert( r >= 0 );		//注册bool全局变量
#define REGI_PROP_INT(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const int ",#m), &##m##property); assert( r >= 0 );		//注册int全局变量
#define REGI_PROP_UINT(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const uint ",#m), &##m##property); assert( r >= 0 );		//注册UINT全局变量
#define REGI_PROP_STRING(m)		r = engine->RegisterGlobalProperty(PROP_PARAPETER("const string ",#m), &##m##property); assert( r >= 0 );	//注册string全局变量


// Example opCast behaviour
template<class A, class B>
B* refCast(A* a)
{
	// If the handle already is a null handle, then just return the null handle
	if( !a ) return 0;

	// Now try to dynamically cast the pointer to the wanted type
	B* b = dynamic_cast<B*>(a);
	// 	if( b != 0 )
	// 	{
	// 		// Since the cast was made, we need to increase the ref counter for the returned handle
	// 		b->addref();
	// 	}
	return b;
}
//注册类的层次关系: 继承
#define REG_CLASS_HIERARCHIES(base,derived) {	\
	r = engine->RegisterObjectMethod(#base, DEFUNC(derived@ opCast()), asFUNCTION((refCast<base,derived>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );					\
	r = engine->RegisterObjectMethod(#derived, DEFUNC(base@ opImplCast()), asFUNCTION((refCast<derived,base>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );				\
	r = engine->RegisterObjectMethod(#base, DEFUNC(const derived@ opCast() const), asFUNCTION((refCast<base,derived>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );		\
	r = engine->RegisterObjectMethod(#derived, DEFUNC(const base@ opImplCast() const), asFUNCTION((refCast<derived,base>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );	\
}

