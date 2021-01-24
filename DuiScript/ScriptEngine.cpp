#include "StdAfx.h"
#include "ScriptEngine.h"

#include "angelscript/add_on/scriptstdtime/AsTime.h"
#include "angelscript/add_on/scriptstdtime/scriptstdtime.h"
#include "angelscript/add_on/scriptmath/scriptmath.h"
#include "angelscript/add_on/scriptmath/scriptmath.cpp"
#include "angelscript/add_on/scriptarray/scriptarray.h"
#include "angelscript/add_on/scriptarray/scriptarray.cpp"
#include "RegCDuiString.h"
#include "RegPoint.h"
#include "regSIZE.h"
#include "regRECT.h"
#include "RegTNotifyUI.h"
#include "RegTEvent.h"
#include "RegCControlUI.h"
#include "regCLangManagerUI.h"
#include "regPaintManagerUI.h"
#include "regCRenderEngine.h"
#include "regTFontInfo.h"
#include "regTImageInfo.h"
#include "regTDrawInfo.h"

namespace DuiLib
{

static CDuiString formatdatetime(const CDuiString &strtime, const datetime &dt)
{
	TCHAR szBuffer[128];
	struct tm ptmTemp;

	if (_localtime64_s(&ptmTemp, &dt.m_time) != 0)
	{
		return _T("");
	}

	if (!_tcsftime(szBuffer, 128, strtime, &ptmTemp))
	{
		szBuffer[0] = '\0';
	}

	return szBuffer;
}

static void ScriptMsgBox(const CDuiString &str)
{
	MessageBox(NULL, str, _T("duilib angel script"), MB_OK);
}

static void ScriptMsgBox(LPCTSTR str)
{
	MessageBox(NULL, str, _T("duilib angel script"), MB_OK);
}

//////////////////////////////////////////////////////////////////////////
//注册windows消息
DECL_PROP_UINT( WM_CREATE );
DECL_PROP_UINT( WM_CLOSE );
DECL_PROP_UINT( WM_DESTROY );
DECL_PROP_UINT( WM_NCACTIVATE );
DECL_PROP_UINT( WM_NCCALCSIZE );
DECL_PROP_UINT( WM_NCPAINT );
DECL_PROP_UINT( WM_NCHITTEST );
DECL_PROP_UINT( WM_GETMINMAXINFO );
DECL_PROP_UINT( WM_MOUSEWHEEL );
DECL_PROP_UINT( WM_SIZE );	
DECL_PROP_UINT( WM_CHAR );
DECL_PROP_UINT( WM_SYSCOMMAND );
DECL_PROP_UINT( WM_KEYDOWN );
DECL_PROP_UINT( WM_KILLFOCUS );
DECL_PROP_UINT( WM_SETFOCUS );
DECL_PROP_UINT( WM_LBUTTONUP );
DECL_PROP_UINT( WM_LBUTTONDOWN );
DECL_PROP_UINT( WM_MOUSEMOVE );
DECL_PROP_UINT( WM_MOUSEHOVER );

//////////////////////////////////////////////////////////////////////////
//注册DuiLib消息类型
DECL_PROP_STRING( DUI_MSGTYPE_MENU );//					( _T( "menu" ) )
DECL_PROP_STRING( DUI_MSGTYPE_LINK );//					 ( _T( "link" ) )

DECL_PROP_STRING( DUI_MSGTYPE_TIMER );//                  ( _T( "timer" ) )
DECL_PROP_STRING( DUI_MSGTYPE_CLICK );//                  ( _T( "click" ) )
DECL_PROP_STRING( DUI_MSGTYPE_RCLICK );//                  ( _T( "rclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_DBCLICK );//                ( _T( "dbclick" ) )

DECL_PROP_STRING( DUI_MSGTYPE_RETURN );//                 ( _T( "return" ) )
DECL_PROP_STRING( DUI_MSGTYPE_SCROLL );//                 ( _T( "scroll" ) )

DECL_PROP_STRING( DUI_MSGTYPE_PREDROPDOWN );//            ( _T( "predropdown" ) )
DECL_PROP_STRING( DUI_MSGTYPE_DROPDOWN );//               ( _T( "dropdown" ) )
DECL_PROP_STRING( DUI_MSGTYPE_SETFOCUS );//               ( _T( "setfocus" ) )

DECL_PROP_STRING( DUI_MSGTYPE_KILLFOCUS );//              ( _T( "killfocus" ) )
DECL_PROP_STRING( DUI_MSGTYPE_ITEMCLICK );// 		   	   ( _T( "itemclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_ITEMRCLICK );// 			   ( _T( "itemrclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_TABSELECT ); //             ( _T( "tabselect" ) )

DECL_PROP_STRING( DUI_MSGTYPE_ITEMSELECT );// 		   	   ( _T( "itemselect" ) )
DECL_PROP_STRING( DUI_MSGTYPE_ITEMEXPAND );//             ( _T( "itemexpand" ) )
DECL_PROP_STRING( DUI_MSGTYPE_WINDOWINIT );//             ( _T( "windowinit" ) )
DECL_PROP_STRING( DUI_MSGTYPE_WINDOWSIZE );//             ( _T( "windowsize" ) )
DECL_PROP_STRING( DUI_MSGTYPE_BUTTONDOWN );//		   	   ( _T( "buttondown" ) )
DECL_PROP_STRING( DUI_MSGTYPE_MOUSEENTER );//			   ( _T( "mouseenter" ) )
DECL_PROP_STRING( DUI_MSGTYPE_MOUSELEAVE );//			   ( _T( "mouseleave" ) )

DECL_PROP_STRING( DUI_MSGTYPE_TEXTCHANGED );//            ( _T( "textchanged" ) )
DECL_PROP_STRING( DUI_MSGTYPE_HEADERCLICK );//            ( _T( "headerclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_ITEMDBCLICK );//           ( _T( "itemdbclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_SHOWACTIVEX );//            ( _T( "showactivex" ) )

DECL_PROP_STRING( DUI_MSGTYPE_ITEMCOLLAPSE );//           ( _T( "itemcollapse" ) )
DECL_PROP_STRING( DUI_MSGTYPE_ITEMACTIVATE );//           ( _T( "itemactivate" ) )
DECL_PROP_STRING( DUI_MSGTYPE_VALUECHANGED );//           ( _T( "valuechanged" ) )
DECL_PROP_STRING( DUI_MSGTYPE_VALUECHANGED_MOVE );//      ( _T( "movevaluechanged" ) )

DECL_PROP_STRING( DUI_MSGTYPE_SELECTCHANGED );// 		   ( _T( "selectchanged" ) )
DECL_PROP_STRING( DUI_MSGTYPE_UNSELECTED );//	 		   ( _T( "unselected" ) )

DECL_PROP_STRING( DUI_MSGTYPE_TREEITEMDBCLICK );// 		( _T( "treeitemdbclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_CHECKCLICK );//				( _T( "checkclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_TEXTROLLEND );// 			( _T( "textrollend" ) )
DECL_PROP_STRING( DUI_MSGTYPE_COLORCHANGED );//		    ( _T( "colorchanged" ) )

DECL_PROP_STRING( DUI_MSGTYPE_LISTITEMSELECT );// 		   	( _T( "listitemselect" ) )
DECL_PROP_STRING( DUI_MSGTYPE_LISTITEMCHECKED );// 		( _T( "listitemchecked" ) )
DECL_PROP_STRING( DUI_MSGTYPE_COMBOITEMSELECT );// 		( _T( "comboitemselect" ) )
DECL_PROP_STRING( DUI_MSGTYPE_LISTHEADERCLICK );//			( _T( "listheaderclick" ) )
DECL_PROP_STRING( DUI_MSGTYPE_LISTHEADITEMCHECKED );//		( _T( "listheaditemchecked" ) )
DECL_PROP_STRING( DUI_MSGTYPE_LISTPAGECHANGED );//			( _T( "listpagechanged" ) )

DECL_PROP_STRING( DUI_MSGTYPE_GRID_STARTEDIT );//          ( _T( "gridstartedit" ) )		
DECL_PROP_STRING( DUI_MSGTYPE_GRID_ENDEDIT );//			( _T( "gridendedit" ) )			
DECL_PROP_STRING( DUI_MSGTYPE_GRID_STARTSELCHANGE );//		( _T( "gridstartselchange" ) )	
DECL_PROP_STRING( DUI_MSGTYPE_GRID_ENDSELCHANGE );//		( _T( "gridendselchange" ) )	
DECL_PROP_STRING( DUI_MSGTYPE_GRID_DROPDOWNLIST );//       ( _T( "griddropdownlist" ) )	
DECL_PROP_STRING( DUI_MSGTYPE_GRID_GETDISPINFO );//        ( _T( "gridgetdispinfo" ) )		

DECL_PROP_STRING( DUI_MSGTYPE_SCI_NOTIFY );//				( _T( "scinotify" ) )		

//////////////////////////////////////////////////////////////////////////
//注册鼠标光标定义
DECL_PROP_UINT( DUI_ARROW );
DECL_PROP_UINT( DUI_IBEAM );
DECL_PROP_UINT( DUI_WAIT );
DECL_PROP_UINT( DUI_CROSS );
DECL_PROP_UINT( DUI_UPARROW );
DECL_PROP_UINT( DUI_SIZE );
DECL_PROP_UINT( DUI_ICON );
DECL_PROP_UINT( DUI_SIZENWSE );
DECL_PROP_UINT( DUI_SIZENESW );
DECL_PROP_UINT( DUI_SIZEWE );
DECL_PROP_UINT( DUI_SIZENS );
DECL_PROP_UINT( DUI_SIZEALL );
DECL_PROP_UINT( DUI_NO );
DECL_PROP_UINT( DUI_HAND );

//////////////////////////////////////////////////////////////////////////
//注册DuiLib事件类型
DECL_PROP_UINT( UIEVENT__FIRST );
DECL_PROP_UINT( UIEVENT__KEYBEGIN );
DECL_PROP_UINT( UIEVENT_KEYDOWN );
DECL_PROP_UINT( UIEVENT_KEYUP );
DECL_PROP_UINT( UIEVENT_CHAR );
DECL_PROP_UINT( UIEVENT_SYSKEY );
DECL_PROP_UINT( UIEVENT__KEYEND );
DECL_PROP_UINT( UIEVENT__MOUSEBEGIN );
DECL_PROP_UINT( UIEVENT_MOUSEMOVE );
DECL_PROP_UINT( UIEVENT_MOUSELEAVE );
DECL_PROP_UINT( UIEVENT_MOUSEENTER );
DECL_PROP_UINT( UIEVENT_MOUSEHOVER );
DECL_PROP_UINT( UIEVENT_BUTTONDOWN );
DECL_PROP_UINT( UIEVENT_BUTTONUP );
DECL_PROP_UINT( UIEVENT_RBUTTONDOWN );
DECL_PROP_UINT( UIEVENT_RBUTTONUP );
DECL_PROP_UINT( UIEVENT_MBUTTONDOWN );
DECL_PROP_UINT( UIEVENT_MBUTTONUP );
DECL_PROP_UINT( UIEVENT_DBLCLICK );
DECL_PROP_UINT( UIEVENT_CONTEXTMENU );
DECL_PROP_UINT( UIEVENT_SCROLLWHEEL );
DECL_PROP_UINT( UIEVENT__MOUSEEND );
DECL_PROP_UINT( UIEVENT_KILLFOCUS );
DECL_PROP_UINT( UIEVENT_SETFOCUS );
DECL_PROP_UINT( UIEVENT_WINDOWSIZE );
DECL_PROP_UINT( UIEVENT_SETCURSOR );
DECL_PROP_UINT( UIEVENT_TIMER );
DECL_PROP_UINT( UIEVENT__LAST );

//////////////////////////////////////////////////////////////////////////
CScriptEngine g_ScriptEngine;

CScriptEngine::CScriptEngine(void)
{
	engine = NULL;
}


CScriptEngine::~CScriptEngine(void)
{
	if( engine )
	{
		engine->ShutDownAndRelease();
		engine = NULL;
	}
}

asIScriptEngine *CScriptEngine::GetEngine()
{
	if(engine == NULL)
		Init();
	return engine;
}

void CScriptEngine::MessageCallback(const asSMessageInfo &msg)
{
	if( msg.type == asMSGTYPE_ERROR )
	{
		LSSTRING_CONVERSION;
		ATL::CStringA temp;
		temp.Format("row = %d\r\ncol = %d\r\nsection=%s \r\nmessage = %s\r\n", 
			msg.row, msg.col, msg.section, msg.message);
		MessageBoxA(NULL, LSUTF82A(temp), "Duilib script error", MB_OK);
	}
}

void CScriptEngine::Init()
{
	if(engine) return;

	engine = asCreateScriptEngine();

	int r = 0;
	r = engine->SetMessageCallback(asMETHOD(CScriptEngine, MessageCallback), this, asCALL_THISCALL); assert( r >= 0 );

	//脚本代码的字符编码  0 - ASCII, 1 - UTF8. Default: 1 (UTF8). 
	r = engine->SetEngineProperty(asEP_SCRIPT_SCANNER, 1); assert( r >= 0 );

	//脚本内部字符串的字符编码 0 - UTF8/ASCII, 1 - UTF16. Default: 0 (UTF8) 
#ifdef _UNICODE
	r = engine->SetEngineProperty(asEP_STRING_ENCODING, 1); assert( r >= 0 );
#endif

	r = engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, 1);

	//注册基础类型
	r = engine->RegisterTypedef("long", "int");		assert( r >= 0 );
	r = engine->RegisterTypedef("LONG", "int");		assert( r >= 0 );
	r = engine->RegisterTypedef("UINT", "uint");	assert( r >= 0 );
	r = engine->RegisterTypedef("WPARAM", "uint");	assert( r >= 0 );
	r = engine->RegisterTypedef("LPARAM", "uint");	assert( r >= 0 );
	r = engine->RegisterTypedef("DWORD", "int");	assert( r >= 0 );
	r = engine->RegisterTypedef("WORD", "int16");	assert( r >= 0 );
	r = engine->RegisterTypedef("BYTE", "int8");	assert( r >= 0 );
	r = engine->RegisterTypedef("char", "int8");	assert( r >= 0 );
	r = engine->RegisterTypedef("short", "int8");	assert( r >= 0 );
	r = engine->RegisterTypedef("wchar_t", "int16");assert( r >= 0 );
	r = engine->RegisterTypedef("UINT_PTR", "int64");assert( r >= 0 );
	r = engine->RegisterTypedef("BOOL", "int");assert( r >= 0 );
	r = engine->RegisterObjectType("LPVOID", sizeof(LPVOID), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );

#ifdef _UNICODE
	r = engine->RegisterTypedef("TCHAR", "int16");	assert( r >= 0 );
#else
	r = engine->RegisterTypedef("TCHAR", "int8");	assert( r >= 0 );
#endif

	//把 const char * 注册进去
#ifdef _UNICODE
	r = engine->RegisterObjectType("LPCTSTR", sizeof(const wchar_t *), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );
#else
	r = engine->RegisterObjectType("LPCTSTR", sizeof(const char *), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);		assert( r >= 0 );
#endif

	r = engine->RegisterObjectType("HINSTANCE", sizeof(HINSTANCE), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );
	r = engine->RegisterObjectType("HDC", sizeof(HDC), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );
	r = engine->RegisterObjectType("HBITMAP", sizeof(HBITMAP), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );
	r = engine->RegisterObjectType("HFONT", sizeof(HFONT), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );
	r = engine->RegisterObjectType("CLSID", sizeof(CLSID), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE);	assert( r >= 0 );

	//把CDuiString注册到脚本中，使成为脚本标准字符串类型
	regCDuiString::Register(engine);

	//时间类
	RegisterStdTime(engine);
	//需要把datetime格式字符串也改成CDuiString
	r = engine->RegisterObjectMethod("datetime", "string Format(string &in)", asFUNCTION(formatdatetime), asCALL_CDECL_OBJLAST);assert( r >= 0 );
	
	//注册一些数学公式
	RegisterScriptMath_Native(engine);

	//数组类
	RegisterScriptArray(engine, true);

	//注册各种define/enum,由于脚本中不能注册define，全部都定义成全局属性
	reg_GlobalProperty();


	regPOINT::Register(engine);
	regRECT::Register(engine);
	regSIZE::Register(engine);

	regCLangManagerUI::Register(engine);
	regCPaintManagerUI::Register_Prepare(engine);
	regTEventUI::Register_Prepare(engine);

	REGISTER_CONTROL( CControlUI ); 
	REGISTER_CONTROL( CActiveXUI );
	REGISTER_CONTROL( CWebBrowserUI );
	REGISTER_CONTROL( CColorPaletteUI );
	REGISTER_CONTROL( CContainerUI );
	REGISTER_CONTROL( CGifAnimUI );
	REGISTER_CONTROL( CLabelUI );

	REGISTER_CONTROL( CListElementUI );
	REGISTER_CONTROL( CListLabelElementUI );

	REGISTER_CONTROL( CScrollBarUI );

	REGISTER_CONTROL( CVerticalLayoutUI );
	REGISTER_CONTROL( CGroupBoxUI );
	//REGISTER_CONTROL( CListBodyUI );
	REGISTER_CONTROL( CHorizontalLayoutUI );
	REGISTER_CONTROL( CListContainerElementUI );
	REGISTER_CONTROL( CListContainerHeaderItemUI );
	REGISTER_CONTROL( CListHeaderUI );
	REGISTER_CONTROL( CListUI );

	// 	REGISTER_CONTROL( CTreeNodeUI );
	// 	REGISTER_CONTROL( CTreeViewUI );

	REGISTER_CONTROL( CAnimationPaneUI );
	REGISTER_CONTROL( CChildLayoutUI );
	REGISTER_CONTROL( CChildWindowUI );
	REGISTER_CONTROL( CComboUI );
	REGISTER_CONTROL( CLabelLayoutUI );
	REGISTER_CONTROL( CButtonLayoutUI );
	REGISTER_CONTROL( COptionLayoutUI );
	REGISTER_CONTROL( CListHeaderItemUI );
	REGISTER_CONTROL( CRichEditUI );
	REGISTER_CONTROL( CTabLayoutUI );
	REGISTER_CONTROL( CAnimationTabLayoutUI );
	REGISTER_CONTROL( CTileLayoutUI );

	REGISTER_CONTROL( CButtonUI );
	REGISTER_CONTROL( CFadeButtonUI );
	REGISTER_CONTROL( COptionUI );
	REGISTER_CONTROL( CCheckBoxUI );
	REGISTER_CONTROL( CTabCtrlUI );
	REGISTER_CONTROL( CDateTimeUI );
	REGISTER_CONTROL( CEditUI );
	REGISTER_CONTROL( CHotKeyUI );
	REGISTER_CONTROL( CIPAddressUI );
	REGISTER_CONTROL( CProgressUI );
	REGISTER_CONTROL( CSliderUI );
	//REGISTER_CONTROL( CRingUI );
	REGISTER_CONTROL( CRollTextUI );
	REGISTER_CONTROL( CTextUI );
	
	reg_ControlHierarchies(); 

	regCPaintManagerUI::Register_Extra(engine);
	regTNotifyUI::Register(engine);
	regTEventUI::Register_Extra(engine);
	regTFontInfo::Register(engine);
	regTImageInfo::Register(engine);
	regTDrawInfo::Register(engine);
	regCRenderEngine::Register(engine);
	
	//注册MsgBox函数
	r = engine->RegisterGlobalFunction("void MsgBox(const string &in)", asFUNCTIONPR(ScriptMsgBox, (const CDuiString&), void), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterGlobalFunction("void MsgBox(LPCTSTR)", asFUNCTIONPR(ScriptMsgBox, (LPCTSTR), void), asCALL_CDECL); assert( r >= 0 );
	
}

void CScriptEngine::reg_GlobalProperty()
{
	int r = 0;
	//////////////////////////////////////////////////////////////////////////
	//注册windows消息
	REGI_PROP_UINT( WM_CREATE );
	REGI_PROP_UINT( WM_CLOSE );
	REGI_PROP_UINT( WM_DESTROY );
	REGI_PROP_UINT( WM_NCACTIVATE );
	REGI_PROP_UINT( WM_NCCALCSIZE );
	REGI_PROP_UINT( WM_NCPAINT );
	REGI_PROP_UINT( WM_NCHITTEST );
	REGI_PROP_UINT( WM_GETMINMAXINFO );
	REGI_PROP_UINT( WM_MOUSEWHEEL );
	REGI_PROP_UINT( WM_SIZE );	
	REGI_PROP_UINT( WM_CHAR );
	REGI_PROP_UINT( WM_SYSCOMMAND );
	REGI_PROP_UINT( WM_KEYDOWN );
	REGI_PROP_UINT( WM_KILLFOCUS );
	REGI_PROP_UINT( WM_SETFOCUS );
	REGI_PROP_UINT( WM_LBUTTONUP );
	REGI_PROP_UINT( WM_LBUTTONDOWN );
	REGI_PROP_UINT( WM_MOUSEMOVE );
	REGI_PROP_UINT( WM_MOUSEHOVER );

	//////////////////////////////////////////////////////////////////////////
	//注册DuiLib消息类型
	REGI_PROP_STRING( DUI_MSGTYPE_MENU );//					( _T( "menu" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_LINK );//					 ( _T( "link" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_TIMER );//                  ( _T( "timer" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_CLICK );//                  ( _T( "click" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_RCLICK );//                  ( _T( "rclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_DBCLICK );//                ( _T( "dbclick" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_RETURN );//                 ( _T( "return" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_SCROLL );//                 ( _T( "scroll" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_PREDROPDOWN );//            ( _T( "predropdown" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_DROPDOWN );//               ( _T( "dropdown" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_SETFOCUS );//               ( _T( "setfocus" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_KILLFOCUS );//              ( _T( "killfocus" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_ITEMCLICK );// 		   	   ( _T( "itemclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_ITEMRCLICK );// 			   ( _T( "itemrclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_TABSELECT ); //             ( _T( "tabselect" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_ITEMSELECT );// 		   	   ( _T( "itemselect" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_ITEMEXPAND );//             ( _T( "itemexpand" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_WINDOWINIT );//             ( _T( "windowinit" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_WINDOWSIZE );//             ( _T( "windowsize" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_BUTTONDOWN );//		   	   ( _T( "buttondown" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_MOUSEENTER );//			   ( _T( "mouseenter" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_MOUSELEAVE );//			   ( _T( "mouseleave" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_TEXTCHANGED );//            ( _T( "textchanged" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_HEADERCLICK );//            ( _T( "headerclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_ITEMDBCLICK );//           ( _T( "itemdbclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_SHOWACTIVEX );//            ( _T( "showactivex" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_ITEMCOLLAPSE );//           ( _T( "itemcollapse" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_ITEMACTIVATE );//           ( _T( "itemactivate" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_VALUECHANGED );//           ( _T( "valuechanged" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_VALUECHANGED_MOVE );//      ( _T( "movevaluechanged" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_SELECTCHANGED );// 		   ( _T( "selectchanged" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_UNSELECTED );//	 		   ( _T( "unselected" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_TREEITEMDBCLICK );// 		( _T( "treeitemdbclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_CHECKCLICK );//				( _T( "checkclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_TEXTROLLEND );// 			( _T( "textrollend" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_COLORCHANGED );//		    ( _T( "colorchanged" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_LISTITEMSELECT );// 		   	( _T( "listitemselect" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_LISTITEMCHECKED );// 		( _T( "listitemchecked" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_COMBOITEMSELECT );// 		( _T( "comboitemselect" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_LISTHEADERCLICK );//			( _T( "listheaderclick" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_LISTHEADITEMCHECKED );//		( _T( "listheaditemchecked" ) )
	REGI_PROP_STRING( DUI_MSGTYPE_LISTPAGECHANGED );//			( _T( "listpagechanged" ) )

	REGI_PROP_STRING( DUI_MSGTYPE_GRID_STARTEDIT );//          ( _T( "gridstartedit" ) )		
	REGI_PROP_STRING( DUI_MSGTYPE_GRID_ENDEDIT );//			( _T( "gridendedit" ) )			
	REGI_PROP_STRING( DUI_MSGTYPE_GRID_STARTSELCHANGE );//		( _T( "gridstartselchange" ) )	
	REGI_PROP_STRING( DUI_MSGTYPE_GRID_ENDSELCHANGE );//		( _T( "gridendselchange" ) )	
	REGI_PROP_STRING( DUI_MSGTYPE_GRID_DROPDOWNLIST );//       ( _T( "griddropdownlist" ) )	
	REGI_PROP_STRING( DUI_MSGTYPE_GRID_GETDISPINFO );//        ( _T( "gridgetdispinfo" ) )		

	REGI_PROP_STRING( DUI_MSGTYPE_SCI_NOTIFY );//				( _T( "scinotify" ) )		

	//////////////////////////////////////////////////////////////////////////
	//注册鼠标光标定义
	REGI_PROP_UINT( DUI_ARROW );
	REGI_PROP_UINT( DUI_IBEAM );
	REGI_PROP_UINT( DUI_WAIT );
	REGI_PROP_UINT( DUI_CROSS );
	REGI_PROP_UINT( DUI_UPARROW );
	REGI_PROP_UINT( DUI_SIZE );
	REGI_PROP_UINT( DUI_ICON );
	REGI_PROP_UINT( DUI_SIZENWSE );
	REGI_PROP_UINT( DUI_SIZENESW );
	REGI_PROP_UINT( DUI_SIZEWE );
	REGI_PROP_UINT( DUI_SIZENS );
	REGI_PROP_UINT( DUI_SIZEALL );
	REGI_PROP_UINT( DUI_NO );
	REGI_PROP_UINT( DUI_HAND );

	//////////////////////////////////////////////////////////////////////////
	//注册DuiLib事件类型
	REGI_PROP_UINT( UIEVENT__FIRST );
	REGI_PROP_UINT( UIEVENT__KEYBEGIN );
	REGI_PROP_UINT( UIEVENT_KEYDOWN );
	REGI_PROP_UINT( UIEVENT_KEYUP );
	REGI_PROP_UINT( UIEVENT_CHAR );
	REGI_PROP_UINT( UIEVENT_SYSKEY );
	REGI_PROP_UINT( UIEVENT__KEYEND );
	REGI_PROP_UINT( UIEVENT__MOUSEBEGIN );
	REGI_PROP_UINT( UIEVENT_MOUSEMOVE );
	REGI_PROP_UINT( UIEVENT_MOUSELEAVE );
	REGI_PROP_UINT( UIEVENT_MOUSEENTER );
	REGI_PROP_UINT( UIEVENT_MOUSEHOVER );
	REGI_PROP_UINT( UIEVENT_BUTTONDOWN );
	REGI_PROP_UINT( UIEVENT_BUTTONUP );
	REGI_PROP_UINT( UIEVENT_RBUTTONDOWN );
	REGI_PROP_UINT( UIEVENT_RBUTTONUP );
	REGI_PROP_UINT( UIEVENT_MBUTTONDOWN );
	REGI_PROP_UINT( UIEVENT_MBUTTONUP );
	REGI_PROP_UINT( UIEVENT_DBLCLICK );
	REGI_PROP_UINT( UIEVENT_CONTEXTMENU );
	REGI_PROP_UINT( UIEVENT_SCROLLWHEEL );
	REGI_PROP_UINT( UIEVENT__MOUSEEND );
	REGI_PROP_UINT( UIEVENT_KILLFOCUS );
	REGI_PROP_UINT( UIEVENT_SETFOCUS );
	REGI_PROP_UINT( UIEVENT_WINDOWSIZE );
	REGI_PROP_UINT( UIEVENT_SETCURSOR );
	REGI_PROP_UINT( UIEVENT_TIMER );
	REGI_PROP_UINT( UIEVENT__LAST );
}

void CScriptEngine::reg_ControlHierarchies()
{
	int r = 0;
	//////////////////////////////////////////////////////////////////////////
	//注册类的层次关系
	//REG_CONTROL_HIERARCHIES(父类, 子类);
	REG_CLASS_HIERARCHIES(CControlUI, CActiveXUI);
	REG_CLASS_HIERARCHIES(CControlUI, CColorPaletteUI);
	REG_CLASS_HIERARCHIES(CControlUI, CContainerUI);
	REG_CLASS_HIERARCHIES(CControlUI, CGifAnimUI);
	REG_CLASS_HIERARCHIES(CControlUI, CLabelUI);
	REG_CLASS_HIERARCHIES(CControlUI, CListElementUI);
	REG_CLASS_HIERARCHIES(CControlUI, CScrollBarUI);

	REG_CLASS_HIERARCHIES(CActiveXUI, CWebBrowserUI);

	REG_CLASS_HIERARCHIES(CContainerUI, CAnimationPaneUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CChildLayoutUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CChildWindowUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CComboUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CHorizontalLayoutUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CLabelLayoutUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CListHeaderItemUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CRichEditUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CTabLayoutUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CTileLayoutUI);
	REG_CLASS_HIERARCHIES(CContainerUI, CVerticalLayoutUI);

	REG_CLASS_HIERARCHIES(CHorizontalLayoutUI, CListContainerElementUI);
	REG_CLASS_HIERARCHIES(CHorizontalLayoutUI, CListContainerHeaderItemUI);
	REG_CLASS_HIERARCHIES(CHorizontalLayoutUI, CListHeaderUI);

	REG_CLASS_HIERARCHIES(CLabelLayoutUI, CButtonLayoutUI);
	REG_CLASS_HIERARCHIES(CButtonLayoutUI, COptionLayoutUI);

	REG_CLASS_HIERARCHIES(CTabLayoutUI, CAnimationTabLayoutUI);

	REG_CLASS_HIERARCHIES(CVerticalLayoutUI, CGroupBoxUI);
	//REG_CLASS_HIERARCHIES(CVerticalLayoutUI, CListBodyUI);
	REG_CLASS_HIERARCHIES(CVerticalLayoutUI, CListUI);

	REG_CLASS_HIERARCHIES(CLabelUI, CButtonUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CDateTimeUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CEditUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CHotKeyUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CIPAddressUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CProgressUI);
	//REG_CLASS_HIERARCHIES(CLabelUI, CRingUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CRollTextUI);
	REG_CLASS_HIERARCHIES(CLabelUI, CTextUI);

	REG_CLASS_HIERARCHIES(CButtonUI, CFadeButtonUI);
	REG_CLASS_HIERARCHIES(CButtonUI, COptionUI);

	REG_CLASS_HIERARCHIES(COptionUI, CCheckBoxUI);
	REG_CLASS_HIERARCHIES(COptionUI, CTabCtrlUI);

	REG_CLASS_HIERARCHIES(CProgressUI, CSliderUI);

	REG_CLASS_HIERARCHIES(CListElementUI, CListLabelElementUI);
	
}


} //namespace DuiLib