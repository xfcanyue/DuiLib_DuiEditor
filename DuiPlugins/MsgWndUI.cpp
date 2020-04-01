#include "StdAfx.h"
#include "MsgWndUI.h"


CMsgWndUI *CMsgWndUI::m_pStaticWindow = NULL;
void CMsgWndUI::InsertMsg(LPCTSTR pstring, COLORREF cr)
{
	if(m_pStaticWindow == NULL)
		return;

	CDuiString strRet;
	SYSTEMTIME _Time;
	GetLocalTime(&_Time);
	strRet.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d : %s"), 
		_Time.wYear, _Time.wMonth, _Time.wDay,
		_Time.wHour, _Time.wMinute, _Time.wSecond, _Time.wMilliseconds, 
		pstring);

	CDuiString *pNewString = new CDuiString;
	*pNewString = strRet;
	m_pStaticWindow->GetManager()->SendNotify(m_pStaticWindow, _T("CMsgWndUI::InsertMsg"), 0, (LPARAM)pNewString, true);
}

IMPLEMENT_DUICONTROL(CMsgWndUI)

CMsgWndUI::CMsgWndUI(void)
{
	SetBkColor(UIARGB(255,255,255,255));

	m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL |  ES_AUTOVSCROLL;

	SetMultiLine(true);
	SetWantCtrlReturn(true);
	SetWantReturn(true);
	SetInset(CRect(5,5,5,5));

	m_pStaticWindow = this;
}


CMsgWndUI::~CMsgWndUI(void)
{
	m_pStaticWindow = NULL;
}

LPCTSTR CMsgWndUI::GetClass() const
{
	//return _T("MsgWindow");
	return __super::GetClass();
}

LPVOID CMsgWndUI::GetInterface(LPCTSTR pstrName)
{
	//if( _tcscmp(pstrName, _T("MsgWindow")) == 0 ) return static_cast<CMsgWndUI*>(this);
	return __super::GetInterface(pstrName);
}

void CMsgWndUI::DoInit()
{
	OnNotify += MakeDelegate(this,&CMsgWndUI::OnInsertMsg);
	__super::DoInit();
}

void CMsgWndUI::DoEvent(TEventUI& event)
{
	__super::DoEvent(event);
}

bool CMsgWndUI::OnInsertMsg(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if(_tcsicmp(pMsg->sType, _T("CMsgWndUI::InsertMsg")) == 0) 
	{
		CDuiString *pString = (CDuiString *)(pMsg->lParam);
		InsertText(-1, *pString);
		InsertText(-1, _T("\r\n"));
		TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0,0);
		delete pString;
		return true;
	}
	return false;
}