#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

//namespace DuiLib{

CUIForm::CUIForm(void)
{
	
}

CUIForm::~CUIForm(void)
{
	
}

DUI_BEGIN_MESSAGE_MAP(CUIForm, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,			OnNotifyClick)			//(_T("click"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RCLICK,			OnNotifyRClick)			//(_T("rclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DBCLICK,			OnNotifyDbClick)		//(_T("dbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MENU,			OnNotifyMenu)			//(_T("menu"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_LINK,			OnNotifyLink)			//(_T("link"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER,			OnNotifyTimer)			//(_T("timer"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN,			OnNotifyReturn)			//(_T("return"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SCROLL,			OnNotifyScroll)			//(_T("scroll"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DROPDOWN,		OnNotifyDropDown)		//(_T("dropdown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SETFOCUS,		OnNotifySetFocus)		//(_T("setfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_KILLFOCUS,		OnNotifyKillFocus)		//(_T("killfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,		OnNotifyItemClick)		//(_T("itemclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMRCLICK,		OnNotifyItemRClick)		//(_T("itemrclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TABSELECT,		OnNotifyTabSelect)		//(_T("tabselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,		OnNotifyItemSelect)		//(_T("itemselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMEXPAND,		OnNotifyItemExpand)		//(_T("itemexpand"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWINIT,		OnNotifyWindowPrepare)		//(_T("windowinit"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_BUTTONDOWN,		OnNotifyButtonDown)		//(_T("buttondown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSEENTER,		OnNotifyMouseEnter)		//(_T("mouseenter"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSELEAVE,		OnNotifyMouseLeave)		//(_T("mouseleave"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED,		OnNotifyTextChanged)	//(_T("textchanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK,		OnNotifyHeaderClick)	//(_T("headerclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMDBCLICK,		OnNotifyItemDBClick)	//(_T("itemdbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SHOWACTIVEX,		OnNotifyShowActivex)	//(_T("showactivex"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCOLLAPSE,	OnNotifyItemCollapse)	//(_T("itemcollapse"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE,	OnNotifyItemActivate)	//(_T("itemactivate"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED,	OnNotifyValueChanged)	//(_T("valuechanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,	OnNotifySelectChanged)	//(_T("selectchanged"))
//	DUI_ON_MSGTYPE(DUI_MSGTYPE_MENUCOMMAND,		OnNotifyMenuCommand)	//(_T("menucommand"))
DUI_END_MESSAGE_MAP()

CMenuWnd *CUIForm::CreateMenu(STRINGorID xml)
{
	CDuiPoint pt2;
	::GetCursorPos(&pt2);
	CMenuWnd *pMenuWnd = CMenuWnd::CreateMenu(NULL, xml, pt2,  GetManager());
	if(pMenuWnd) pMenuWnd->ResizeMenu();
	return pMenuWnd;
}

void CUIForm::Notify(TNotifyUI& msg)
{
	return CNotifyPump::NotifyPump(msg);
}