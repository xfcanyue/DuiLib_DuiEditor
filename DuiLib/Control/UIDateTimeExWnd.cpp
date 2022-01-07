#include "StdAfx.h"
#include "UIDateTimeExWnd.h"

namespace DuiLib
{

CDateTimeExWnd::CDateTimeExWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDropOpen(false), m_uFormatStyle(UIDTS_DATE)
{
}

CDateTimeExWnd::~CDateTimeExWnd()
{
}

void CDateTimeExWnd::Init(CDateTimeExUI* pOwner, UINT uFormatStyle)
{
	m_pOwner = pOwner;
	m_uFormatStyle = uFormatStyle;

	if (m_hWnd == NULL)
	{
		RECT rcPos = CalPos();
		UINT uStyle = WS_CHILD;

		//add by liqs99
		if(m_uFormatStyle == UIDTS_TIME)
		{
			uStyle |= DTS_TIMEFORMAT;
		}

		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		m_font = MakeRefPtr<UIFont>(m_pOwner->GetManager()->CloneFont(m_pOwner->GetFont()));
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)m_font->GetHFont(m_pOwner->GetManager()), (LPARAM)TRUE);
	}

	memcpy(&m_oldSysTime, &m_pOwner->GetTime(), sizeof(SYSTEMTIME));
	memcpy(&m_dtTemp, &m_pOwner->GetTime(), sizeof(SYSTEMTIME));
	::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, 0, (LPARAM)&m_pOwner->GetTime());
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);

	m_bInit = true;    
}

RECT CDateTimeExWnd::CalPos()
{
	CDuiRect rcPos;
	if(m_uFormatStyle == UIDTS_DATE) rcPos = m_pOwner->GetDateLabel()->GetPos();
	else if(m_uFormatStyle == UIDTS_TIME) rcPos = m_pOwner->GetTimeLabel()->GetPos();

	CControlUI* pParent = m_pOwner;
	RECT rcParent;
	while( pParent = pParent->GetParent() ) {
		if( !pParent->IsVisible() ) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
			break;
		}
		rcParent = pParent->GetClientPos();
		if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
			rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
			break;
		}
	}

	return rcPos;
}

LPCTSTR CDateTimeExWnd::GetWindowClassName() const
{
	return _T("DateTimeExWnd");
}

LPCTSTR CDateTimeExWnd::GetSuperClassName() const
{
	return DATETIMEPICK_CLASS;
}

void CDateTimeExWnd::OnFinalMessage(HWND hWnd)
{
	if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
	//m_pOwner->GetManager()->RemoveNativeWindow(hWnd);
	if(m_uFormatStyle == UIDTS_DATE)
		m_pOwner->m_pWindowDate = NULL;
	else 
		m_pOwner->m_pWindowTime = NULL;
	delete this;
}

LRESULT CDateTimeExWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if( uMsg == WM_CREATE ) {
		//m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
		bHandled = FALSE;
	}
	else if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		memcpy(&m_pOwner->GetTime(), &m_oldSysTime, sizeof(SYSTEMTIME));
		PostMessage(WM_CLOSE);
		return lRes;
	}
	else if(uMsg == OCM_NOTIFY)
	{
		NMHDR* pHeader=(NMHDR*)lParam;
		if(pHeader != NULL && pHeader->hwndFrom == m_hWnd) {
			if(pHeader->code == DTN_DATETIMECHANGE) 
			{
// 				LPNMDATETIMECHANGE lpChage=(LPNMDATETIMECHANGE)lParam;
// 				::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->GetTime());
// 				m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);

				//CMsgWndUI::InsertMsg(_T("DTN_DATETIMECHANGE"));
				::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_dtTemp);
				if(memcmp(&m_pOwner->GetTime(), &m_dtTemp, sizeof(SYSTEMTIME)) != 0)
				{
					m_pOwner->GetTime() = m_dtTemp;
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
				}
			}
			else if(pHeader->code == DTN_DROPDOWN) {
				//CMsgWndUI::InsertMsg(_T("DTN_DROPDOWN"));
				m_bDropOpen = true;

			}
			else if(pHeader->code == DTN_CLOSEUP) {
				//::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->GetTime());
				//CMsgWndUI::InsertMsg(_T("DTN_CLOSEUP"));
				::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_dtTemp);
				if(memcmp(&m_pOwner->GetTime(), &m_dtTemp, sizeof(SYSTEMTIME)) != 0)
				{
					m_pOwner->GetTime() = m_dtTemp;
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
				}
				//PostMessage(WM_CLOSE);
				m_bDropOpen = false;
			}
		}
		bHandled = FALSE;
	}
	else if(uMsg == WM_KILLFOCUS)
	{
		if(!m_bDropOpen) {
			PostMessage(WM_CLOSE);
		}
		bHandled = FALSE;
	}
	else if( uMsg == WM_PAINT) {
		if (m_pOwner->GetManager()->IsLayered()) {
			//m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
		}
		bHandled = FALSE;
	}
	else if( uMsg == WM_SETCURSOR)
	{
		return lRes;
	}
	else bHandled = FALSE;
	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

}