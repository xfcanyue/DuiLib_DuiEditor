#include "StdAfx.h"
#include "UIMsgWnd.h"
#include "TxtWinHost.h"


namespace DuiLib
{

CMsgWndUI *CMsgWndUI::m_pStaticWindow = NULL;
void CMsgWndUI::InsertMsg(LPCTSTR pstring, COLORREF cr)
{
	if(m_pStaticWindow == NULL)
		return;

	CDuiString strRet;
	if(m_pStaticWindow->IsShowDateTime())
	{
		SYSTEMTIME _Time;
		GetLocalTime(&_Time);
		strRet.Format(_T("%04d-%02d-%02d %02d:%02d:%02d.%03d : %s"), 
			_Time.wYear, _Time.wMonth, _Time.wDay,
			_Time.wHour, _Time.wMinute, _Time.wSecond, _Time.wMilliseconds, 
			pstring);
	}
	else if(m_pStaticWindow->IsShowDate())
	{
		SYSTEMTIME _Time;
		GetLocalTime(&_Time);
		strRet.Format(_T("%04d-%02d-%02d : %s"), 
			_Time.wYear, _Time.wMonth, _Time.wDay,
			pstring);
	}
	else if(m_pStaticWindow->IsShowTime())
	{
		SYSTEMTIME _Time;
		GetLocalTime(&_Time);
		strRet.Format(_T("%02d:%02d:%02d.%03d : %s"), 
			_Time.wHour, _Time.wMinute, _Time.wSecond, _Time.wMilliseconds, 
			pstring);
	}
	else
	{
		strRet = pstring;
	}

	TMsgWnd* pMsg = new TMsgWnd;
	pMsg->sText = strRet;
	pMsg->dwColor = cr;
	::PostMessage(m_pStaticWindow->GetManager()->GetPaintWindow(), UIMSG_INSERT_MSG, (WPARAM)m_pStaticWindow, (LPARAM)pMsg);
}

void CMsgWndUI::InsertMsgV(LPCTSTR lpszFormat, ...)
{
	CDuiString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.InnerFormat(lpszFormat, argList);
	va_end(argList);
	InsertMsg(strText);
}

void CMsgWndUI::ClearMsg()
{
	if(m_pStaticWindow == NULL)
		return;
	m_pStaticWindow->SetText(_T(""));
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

	m_bWithDate = true;
	m_bWithTime = true;
}


CMsgWndUI::~CMsgWndUI(void)
{
	m_pStaticWindow = NULL;
}

LPCTSTR CMsgWndUI::GetClass() const
{
	return _T("MsgWndUI");
	return __super::GetClass();
}

LPVOID CMsgWndUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_MSGWND) == 0 ) return static_cast<CMsgWndUI*>(this);
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
	CMsgWndUI *pControl = (CMsgWndUI *)pMsg->wParam;
	if(pControl == this && _tcsicmp(pMsg->sType, _T("CMsgWndUI::InsertMsg")) == 0) 
	{
		if(GetLineCount() >= 5000)
		{
			SetSel(0,-1);
			Clear();
		}
		TMsgWnd* pMsgWnd = (TMsgWnd*)(pMsg->lParam);

// 		long nStart, nEnd;
// 		GetSel(nStart, nEnd);

		InsertText(-1, pMsgWnd->sText);
		InsertText(-1, _T("\r\n"));

// 		long nStart2, nEnd2;
// 		GetSel(nStart2, nEnd2);
// 		SetSel(nEnd, nEnd2);
// 
// 		CHARFORMAT2   cf;
// 		memset(&cf, 0, sizeof(CHARFORMAT2));
// 		cf.cbSize = sizeof(CHARFORMAT2);
// 		cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFE_BOLD;
// 		cf.dwEffects = (unsigned   long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
// 		cf.crTextColor = pMsgWnd->dwColor; //RGB(255,   0,   0);  
// 		SetSelectionCharFormat(cf);

// 		CHARFORMAT2W   cf;
// 		memset(&cf, 0, sizeof(CHARFORMAT2W));
// 		cf.cbSize = sizeof(CHARFORMAT2W);
// 		cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFE_BOLD;
// 		cf.dwEffects = (unsigned   long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
// 		cf.crTextColor = RGB(255, 0, 0); // pMsgWnd->dwColor; //RGB(255,   0,   0); 
// 		TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION | SCF_WORD, (LPARAM)&cf, 0);
// 		ReplaceSel(pMsgWnd->sText, FALSE);
// 		ReplaceSel(_T("\r\n"), FALSE);

		TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0,0);
		delete pMsgWnd;
		return true;
	}
	return false;
}

void CMsgWndUI::ShowDateTime(bool bShow)
{
	m_bWithDate = bShow;
	m_bWithTime = bShow;
}

bool CMsgWndUI::IsShowDateTime() const
{
	return m_bWithDate && m_bWithTime;
}

void CMsgWndUI::ShowDate(bool bShow)
{
	m_bWithDate = bShow;
}

bool CMsgWndUI::IsShowDate() const
{
	return m_bWithDate;
}

void CMsgWndUI::ShowTime(bool bShow)
{
	m_bWithTime = bShow;
}

bool CMsgWndUI::IsShowTime() const
{
	return m_bWithTime;
}

void CMsgWndUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("showdate")) == 0 ) 
		ShowDate(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("showtime")) == 0 ) 
		ShowDate(_tcsicmp(pstrValue, _T("true")) == 0);
	else __super::SetAttribute(pstrName, pstrValue);
}

}