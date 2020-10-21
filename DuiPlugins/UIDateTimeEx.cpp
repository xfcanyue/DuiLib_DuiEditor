#include "StdAfx.h"
#include "UIDateTimeEx.h"

namespace DuiLib
{
	//CDateTimeExUI::m_nDTUpdateFlag
#define DT_NONE   0
#define DT_UPDATE 1
#define DT_DELETE 2
#define DT_KEEP   3

	class CDateTimeExWnd : public CWindowWnd
	{
	public:
		CDateTimeExWnd();

		void Init(CDateTimeExUI* pOwner, UINT uFormatStyle);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		CDateTimeExUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDropOpen;
		SYSTEMTIME m_oldSysTime;
		UINT m_uFormatStyle;
	};

	CDateTimeExWnd::CDateTimeExWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDropOpen(false), m_uFormatStyle(UIDTS_DATE)
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
			SetWindowFont(m_hWnd, m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
		}

		memcpy(&m_oldSysTime, &m_pOwner->m_sysTime, sizeof(SYSTEMTIME));
		::SendMessage(m_hWnd, DTM_SETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);

		m_bInit = true;    
	}

	RECT CDateTimeExWnd::CalPos()
	{
		CDuiRect rcPos;
		if(m_uFormatStyle == UIDTS_DATE) rcPos = m_pOwner->m_pLabelDate->GetPos();
		else if(m_uFormatStyle == UIDTS_TIME) rcPos = m_pOwner->m_pLabelTime->GetPos();

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
			memcpy(&m_pOwner->m_sysTime, &m_oldSysTime, sizeof(SYSTEMTIME));
			m_pOwner->UpdateText();
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
					m_pOwner->UpdateText();
				}
				else if(pHeader->code == DTN_DROPDOWN) {
					m_bDropOpen = true;

				}
				else if(pHeader->code == DTN_CLOSEUP) {
					::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->m_sysTime);
					m_pOwner->UpdateText();
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
		else if( uMsg == WM_SETCURSOR)
		{
			return lRes;
		}
		else bHandled = FALSE;
		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}
	//////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CDateTimeExUI)

	CDateTimeExUI::CDateTimeExUI() 
	: m_uFormatStyle(UIDTS_DATE) //add by liqs99
	, m_iFont(-1)
	, m_pLabelDate(NULL)
	, m_pLabelTime(NULL)
	, m_pWindowDate(NULL)
	, m_pWindowTime(NULL)
	{
		::GetLocalTime(&m_sysTime);
		m_bReadOnly = false;

		m_pLabelDate = new CLabelUI;
		m_pLabelDate->SetTextPadding(CDuiRect(5,0,5,0));
		Add(m_pLabelDate);
		m_pLabelDate->OnEvent += MakeDelegate(this, &CDateTimeExUI::OnEventLabelDate);

		m_pLabelTime = new CLabelUI;
		m_pLabelTime->SetTextPadding(CDuiRect(5,0,5,0));
		Add(m_pLabelTime);
		m_pLabelTime->OnEvent += MakeDelegate(this, &CDateTimeExUI::OnEventLabelTime);
	}

	LPCTSTR CDateTimeExUI::GetClass() const
	{
		return _T("DateTimeExUI");
	}

	LPVOID CDateTimeExUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_DATETIMEEX) == 0 ) return static_cast<CDateTimeExUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CDateTimeExUI::DoInit()
	{
		UpdateText();
	}

	bool CDateTimeExUI::OnEventLabelDate(void* param)
	{
		TEventUI *pEvent = (TEventUI *)param;

		if( pEvent->Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return false;
		}
		if( pEvent->Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindowDate != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( pEvent->Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindowDate != NULL ) return false;
		}
		if( pEvent->Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindowDate ) return false;
			m_pWindowDate = new CDateTimeExWnd();
			ASSERT(m_pWindowDate);
			m_pWindowDate->Init(this, UIDTS_DATE);
			m_pWindowDate->ShowWindow();
		}
		if( pEvent->Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( pEvent->Type == UIEVENT_BUTTONDOWN || pEvent->Type == UIEVENT_DBLCLICK || pEvent->Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindowDate == NULL )
				{
					m_pWindowDate = new CDateTimeExWnd();
					ASSERT(m_pWindowDate);
				}
				if( m_pWindowDate != NULL )
				{
					m_pWindowDate->Init(this, UIDTS_DATE);
					m_pWindowDate->ShowWindow();
				}
			}
			return false;
		}

		return true;
	}

	bool CDateTimeExUI::OnEventLabelTime(void* param)
	{
		TEventUI *pEvent = (TEventUI *)param;

		if( pEvent->Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return false;
		}
		if( pEvent->Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindowTime != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( pEvent->Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindowTime != NULL ) return false;
		}
		if( pEvent->Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindowTime ) return false;
			m_pWindowTime = new CDateTimeExWnd();
			ASSERT(m_pWindowTime);
			m_pWindowTime->Init(this, UIDTS_TIME);
			m_pWindowTime->ShowWindow();
		}
		if( pEvent->Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( pEvent->Type == UIEVENT_BUTTONDOWN || pEvent->Type == UIEVENT_DBLCLICK || pEvent->Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindowTime == NULL )
				{
					m_pWindowTime = new CDateTimeExWnd();
					ASSERT(m_pWindowTime);
				}
				if( m_pWindowTime != NULL )
				{
					m_pWindowTime->Init(this, UIDTS_TIME);
					m_pWindowTime->ShowWindow();
				}
			}
			return false;
		}

		return true;
	}

	SYSTEMTIME& CDateTimeExUI::GetTime()
	{
		return m_sysTime;
	}

	void CDateTimeExUI::SetTime(SYSTEMTIME* pst)
	{
		m_sysTime = *pst;
		Invalidate();
		UpdateText();
	}

	void CDateTimeExUI::SetReadOnly(bool bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		Invalidate();
	}

	bool CDateTimeExUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	void CDateTimeExUI::SetFormatStyle(UINT uStyle)
	{
		m_uFormatStyle = uStyle;
	}

	UINT CDateTimeExUI::GetFormatStyle()
	{
		return m_uFormatStyle;
	}

	void CDateTimeExUI::UpdateText()
	{
		CDuiString sText;

		if(m_uFormatStyle == 0)
		{
			sText.SmallFormat(_T("%4d-%02d-%02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
			if(m_pLabelDate) m_pLabelDate->SetText(sText);
		}
		else if(m_uFormatStyle == 1)
		{
			sText.SmallFormat(_T("%2d:%02d:%02d"), m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);
			if(m_pLabelTime) m_pLabelTime->SetText(sText);
		}
		else if(m_uFormatStyle == 2)
		{
			sText.SmallFormat(_T("%4d-%02d-%02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
			if(m_pLabelDate) m_pLabelDate->SetText(sText);
			sText.SmallFormat(_T("%2d:%02d:%02d"), m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);
			if(m_pLabelTime) m_pLabelTime->SetText(sText);
		}
	}

	void CDateTimeExUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else __super::DoEvent(event);
			return;
		}
		__super::DoEvent(event);
	}

	void CDateTimeExUI::SetFont(int index)
	{
		m_iFont = index;
		Invalidate();
	}

	int CDateTimeExUI::GetFont() const
	{
		return m_iFont;
	}

	void CDateTimeExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		//add by liqs99
		if( _tcscmp(pstrName, _T("formatstyle")) == 0 ) 
		{
			if( _tcscmp(pstrValue, _T("date")) == 0)
			{
				SetFormatStyle(UIDTS_DATE);
				m_pLabelDate->SetVisible(true);
				m_pLabelTime->SetVisible(false);
			}
			else if( _tcscmp(pstrValue, _T("time")) == 0)
			{
				SetFormatStyle(UIDTS_TIME);
				m_pLabelDate->SetVisible(false);
				m_pLabelTime->SetVisible(true);
			}
			else if( _tcscmp(pstrValue, _T("datetime")) == 0)
			{
				SetFormatStyle(UIDTS_DateTimeEx);
				m_pLabelDate->SetVisible(true);
				m_pLabelTime->SetVisible(true);
			}
			else
			{
				SetFormatStyle(UIDTS_DATE);
				m_pLabelDate->SetVisible(true);
				m_pLabelTime->SetVisible(false);
			}

			UpdateText();
		}
		else if( _tcscmp(pstrName, _T("datelabelstyle")) == 0 ) 
		{
			if(m_pLabelDate) m_pLabelDate->ApplyAttributeList(pstrValue);
		}
		else if( _tcscmp(pstrName, _T("timelabelstyle")) == 0 ) 
		{
			if(m_pLabelTime) m_pLabelTime->ApplyAttributeList(pstrValue);
		}
		else __super::SetAttribute(pstrName, pstrValue);
	}
}
