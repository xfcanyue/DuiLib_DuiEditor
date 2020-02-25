#include "StdAfx.h"
#include "MsgWndUI.h"


CMsgWndUI *CMsgWndUI::m_pStaticWindow = NULL;
void CMsgWndUI::InsertMsg(LPCTSTR pstring, COLORREF cr)
{
	if(m_pStaticWindow == NULL)
		return;

	CString strRet;
	SYSTEMTIME _Time;
	GetLocalTime(&_Time);
	strRet.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d : %s"), 
		_Time.wYear, _Time.wMonth, _Time.wDay,
		_Time.wHour, _Time.wMinute, _Time.wSecond, _Time.wMilliseconds, 
		pstring);

	m_pStaticWindow->InsertText(-1, strRet);
	m_pStaticWindow->InsertText(-1, _T("\r\n"));
	m_pStaticWindow->TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0,0);
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
	__super::DoInit();
}

void CMsgWndUI::DoEvent(TEventUI& event)
{
	__super::DoEvent(event);
}