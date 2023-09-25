#include "StdAfx.h"

namespace DuiLib{

CUIFrmBase::CUIFrmBase(void)
{
	
}

CUIFrmBase::~CUIFrmBase(void)
{
	
}

CMenuWnd *CUIFrmBase::CreateMenu(STRINGorID xml)
{
	CDuiPoint pt2;
	CPlatform::GetCursorPos(&pt2);
	CMenuWnd *pMenuWnd = CMenuWnd::CreateMenu(NULL, xml, pt2,  GetManager());
	pMenuWnd->m_bAutoDestroy = true;
	if(pMenuWnd) pMenuWnd->ResizeMenu();
	return pMenuWnd;
}

void CUIFrmBase::Notify(TNotifyUI& msg)
{
	CNotifyPump::NotifyPump(msg);
}

DUI_BEGIN_MESSAGE_MAP(CUIFrmBase,CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,			CUIFrmBase::OnNotifyClick)			//(_T("click"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RCLICK,			CUIFrmBase::OnNotifyRClick)			//(_T("rclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DBCLICK,			CUIFrmBase::OnNotifyDbClick)		//(_T("dbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MENU,			CUIFrmBase::OnNotifyMenu)			//(_T("menu"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_LINK,			CUIFrmBase::OnNotifyLink)			//(_T("link"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER,			CUIFrmBase::OnNotifyTimer)			//(_T("timer"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN,			CUIFrmBase::OnNotifyReturn)			//(_T("return"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SCROLL,			CUIFrmBase::OnNotifyScroll)			//(_T("scroll"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_PREDROPDOWN,		CUIFrmBase::OnNotifyPreDropDown)	//(_T("predropdown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DROPDOWN,		CUIFrmBase::OnNotifyDropDown)		//(_T("dropdown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SETFOCUS,		CUIFrmBase::OnNotifySetFocus)		//(_T("setfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_KILLFOCUS,		CUIFrmBase::OnNotifyKillFocus)		//(_T("killfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,		CUIFrmBase::OnNotifyItemClick)		//(_T("itemclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMRCLICK,		CUIFrmBase::OnNotifyItemRClick)		//(_T("itemrclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TABSELECT,		CUIFrmBase::OnNotifyTabSelect)		//(_T("tabselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,		CUIFrmBase::OnNotifyItemSelect)		//(_T("itemselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMEXPAND,		CUIFrmBase::OnNotifyItemExpand)		//(_T("itemexpand"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWINIT,		CUIFrmBase::OnNotifyWindowPrepare)	//(_T("windowinit"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWSIZE,      CUIFrmBase::OnNotifyWindowSize)     //(_T("windowsize"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_BUTTONDOWN,		CUIFrmBase::OnNotifyButtonDown)		//(_T("buttondown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSEENTER,		CUIFrmBase::OnNotifyMouseEnter)		//(_T("mouseenter"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSELEAVE,		CUIFrmBase::OnNotifyMouseLeave)		//(_T("mouseleave"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED,		CUIFrmBase::OnNotifyTextChanged)	//(_T("textchanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK,		CUIFrmBase::OnNotifyHeaderClick)	//(_T("headerclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMDBCLICK,		CUIFrmBase::OnNotifyItemDBClick)	//(_T("itemdbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SHOWACTIVEX,		CUIFrmBase::OnNotifyShowActivex)	//(_T("showactivex"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCOLLAPSE,	CUIFrmBase::OnNotifyItemCollapse)	//(_T("itemcollapse"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE,	CUIFrmBase::OnNotifyItemActivate)	//(_T("itemactivate"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED,	CUIFrmBase::OnNotifyValueChanged)	//(_T("valuechanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,	CUIFrmBase::OnNotifySelectChanged)	//(_T("selectchanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SIZECHANGED,		CUIFrmBase::OnNotifySizeChanged)	//(_T("sizechanged"))

	DUI_ON_MSGTYPE(DUI_MSGTYPE_ACCEPTFILE, CUIFrmBase::OnNotifyAcceptFile)	//(_T("selectchanged"))

	DUI_ON_MSGTYPE(DUI_MSGTYPE_INITCELL, CUIFrmBase::OnNotifyInitCell)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DRAWITEM, CUIFrmBase::OnNotifyDrawItem)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SORTITEM, CUIFrmBase::OnNotifySortItem)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_STARTEDIT, CUIFrmBase::OnNotifyStartEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ENDEDIT, CUIFrmBase::OnNotifyEndEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DELETEITEM, CUIFrmBase::OnNotifyDeleteItem)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_STARTSELCHANGE, CUIFrmBase::OnNotifyStartSelChange)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ENDSELCHANGE, CUIFrmBase::OnNotifyEndSelChange)

	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_STARTEDIT, CUIFrmBase::OnNotityGridStartEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_ENDEDIT, CUIFrmBase::OnNotityGridEndEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_STARTSELCHANGE, CUIFrmBase::OnNotityGridStartSelChange)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_ENDSELCHANGE, CUIFrmBase::OnNotityGridEndSelChange)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_DROPDOWNLIST, CUIFrmBase::OnNotityGridDropDownList)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_GETDISPINFO, CUIFrmBase::OnNotityGridGetDispInfo)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SCI_NOTIFY, CUIFrmBase::OnNotifySciNotify)
DUI_END_MESSAGE_MAP()


} //namespace DuiLib{