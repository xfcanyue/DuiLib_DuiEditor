#include "StdAfx.h"
#include "UIDateTime.h"

//#include <atlbase.h> 
#include <atlstr.h> 
#include <atlconv.h>

namespace DuiLib
{
	class CDateTimeWnd : public CWindowWnd
	{
	public:
		CDateTimeWnd();

		void Init(CDateTimeUI* pOwner);
		RECT CalPos();


		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		CDateTimeUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDropOpen;
		SYSTEMTIME m_oldSysTime;
	};

	CDateTimeWnd::CDateTimeWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDropOpen(false)
	{
	}

	void CDateTimeWnd::Init(CDateTimeUI* pOwner)
	{
		m_pOwner = pOwner;

		if (m_hWnd == NULL)
		{
			RECT rcPos = CalPos();
			UINT uStyle = WS_CHILD;

			//add by liqs99
			int nFormat = pOwner->GetFormatStyle();
			if(nFormat == 1)
			{
				uStyle |= DTS_TIMEFORMAT;
			}

			Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
			SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
		}

		memcpy(&m_oldSysTime, &m_pOwner->m_sysTime, sizeof(SYSTEMTIME));
		::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);

		m_bInit = true;    
	}

	RECT CDateTimeWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();

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

	LPCTSTR CDateTimeWnd::GetWindowClassName() const
	{
		return _T("DateTimeWnd");
	}

	LPCTSTR CDateTimeWnd::GetSuperClassName() const
	{
		return DATETIMEPICK_CLASS;
	}

	void CDateTimeWnd::OnFinalMessage(HWND hWnd)
	{
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		//m_pOwner->GetManager()->RemoveNativeWindow(hWnd);
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CDateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_CREATE ) {
			//m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			bHandled = FALSE;
		}
		else if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
		{
			memcpy(&m_pOwner->m_sysTime, &m_oldSysTime, sizeof(SYSTEMTIME));
			PostMessage(WM_CLOSE);
			return lRes;
		}
		else if(uMsg == OCM_NOTIFY)
		{
			NMHDR* pHeader=(NMHDR*)lParam;
			if(pHeader != NULL && pHeader->hwndFrom == m_hWnd) {
				if(pHeader->code == DTN_DATETIMECHANGE) {
					LPNMDATETIMECHANGE lpChage=(LPNMDATETIMECHANGE)lParam;
					::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
				}
				else if(pHeader->code == DTN_DROPDOWN) {
					m_bDropOpen = true;

				}
				else if(pHeader->code == DTN_CLOSEUP) {
					::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
					PostMessage(WM_CLOSE);
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
		else if( uMsg == WM_SETCURSOR) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}
	//////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CDateTimeUI)

	CDateTimeUI::CDateTimeUI() 
	: m_uFormatStyle(0) //add by liqs99
	{
		::GetLocalTime(&m_sysTime);
		m_bReadOnly = false;
		m_pWindow = NULL;
	}

	LPCTSTR CDateTimeUI::GetClass() const
	{
		return _T("DateTimeUI");
	}

	LPVOID CDateTimeUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_DATETIME) == 0 ) return static_cast<CDateTimeUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	SYSTEMTIME& CDateTimeUI::GetTime()
	{
		return m_sysTime;
	}

	void CDateTimeUI::SetTime(SYSTEMTIME* pst)
	{
		m_sysTime = *pst;
		Invalidate();
	}

	void CDateTimeUI::SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		Invalidate();
	}

	bool CDateTimeUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CDateTimeUI::SetFormatStyle(UINT uStyle)
	{
		m_uFormatStyle = uStyle;
	}

	UINT CDateTimeUI::GetFormatStyle()
	{
		return m_uFormatStyle;
	}

	void CDateTimeUI::SetText(LPCTSTR lpszDate)
	{
		DATE dt;
		USES_CONVERSION_EX;
		LPCTSTR pszDate = ( lpszDate == NULL ) ? _T("") : lpszDate;

		HRESULT hr;
		LPOLESTR p = T2OLE_EX((LPTSTR)pszDate, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);

#ifndef _UNICODE
		if( p == NULL )
		{
			dt = 0;
			return;
		}
#endif // _UNICODE

		DWORD dwFlags = 0;
		LCID lcid = LANG_USER_DEFAULT;
		if (FAILED(hr = VarDateFromStr( p, lcid, dwFlags, &dt )))
		{
			if (hr == DISP_E_TYPEMISMATCH)
			{
				// Can't convert string to date, set 0 and invalidate
				dt = 0;
				return;
			}
			else if (hr == DISP_E_OVERFLOW)
			{
				// Can't convert string to date, set -1 and invalidate
				dt = -1;
				return;
			}
			else
			{
				// Can't convert string to date, set -1 and invalidate
				dt = -1;
				return;
			}
		}
		::VariantTimeToSystemTime(dt, &m_sysTime);
	}

	CDuiString CDateTimeUI::GetText() const
	{
		CDuiString sText;

		if(m_uFormatStyle == 0)
			sText.SmallFormat(_T("%4d - %02d - %02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
		else if(m_uFormatStyle == 1)
			sText.SmallFormat(_T("%02d : %02d : %02d"), m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);
		else if(m_uFormatStyle == 2)
			sText.SmallFormat(_T("%4d - %02d - %02d %2d : %02d : %02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay, m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);

		return sText;
	}

	void CDateTimeUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		__super::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CDateTimeUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		__super::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CDateTimeUI::SetVisible(bool bVisible)
	{
		__super::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CDateTimeUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CDateTimeUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindow ) return;
			m_pWindow = new CDateTimeWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			m_pWindow->ShowWindow();
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CDateTimeWnd();
					ASSERT(m_pWindow);
				}
				if( m_pWindow != NULL )
				{
					m_pWindow->Init(this);
					m_pWindow->ShowWindow();
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			return;
		}

		CLabelUI::DoEvent(event);
	}

	void CDateTimeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		//add by liqs99
		if( _tcscmp(pstrName, _T("formatstyle")) == 0 ) 
		{
			if( _tcscmp(pstrValue, _T("date")) == 0)
				SetFormatStyle(0);
			else if( _tcscmp(pstrValue, _T("time")) == 0)
				SetFormatStyle(1);
			else if( _tcscmp(pstrValue, _T("datetime")) == 0)
				SetFormatStyle(2);
			else
				SetFormatStyle(0);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}
}
