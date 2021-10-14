#include "StdAfx.h"
#include "UIDateTimeEx.h"
#include "UIDateTimeExWnd.h"
#include "atlconv.h"

namespace DuiLib
{
	class CDateTimeLabelUI : public CLabelUI
	{
	public:
		CDateTimeLabelUI(CDateTimeExUI *pOwner, int uFormatType) : m_pOwner(pOwner), m_uFormatStyle(uFormatType)
		{
		}

		virtual CDuiString GetText() const
		{
			CDuiString sText;
			SYSTEMTIME &st = m_pOwner->GetTime();

			if(m_uFormatStyle == UIDTS_DATE)
				sText.SmallFormat(_T("%4d - %02d - %02d"), st.wYear, st.wMonth, st.wDay);
			else if(m_uFormatStyle == UIDTS_TIME)
				sText.SmallFormat(_T("%02d : %02d : %02d"), st.wHour, st.wMinute, st.wSecond);
			else if(m_uFormatStyle == UIDTS_DATETIME)
				sText.SmallFormat(_T("%4d - %02d - %02d %2d : %02d : %02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			return sText;
		}
	private:
		CDateTimeExUI *m_pOwner;
		int m_uFormatStyle;
	};

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

		m_pLabelDate = new CDateTimeLabelUI(this, UIDTS_DATE);
		m_pLabelDate->SetTextPadding(CDuiRect(5,0,5,0));
		Add(m_pLabelDate);
		m_pLabelDate->OnEvent += MakeDelegate(this, &CDateTimeExUI::OnEventLabelDate);

		m_pLabelTime = new CDateTimeLabelUI(this, UIDTS_TIME);
		m_pLabelTime->SetTextPadding(CDuiRect(5,0,5,0));
		Add(m_pLabelTime);
		m_pLabelTime->OnEvent += MakeDelegate(this, &CDateTimeExUI::OnEventLabelTime);
		m_pLabelTime->SetVisible(false);
	}
 
	LPCTSTR CDateTimeExUI::GetClass() const
	{
		return DUI_CTR_DATETIMEEX;
	}

	LPVOID CDateTimeExUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_DATETIMEEX) == 0 ) return static_cast<CDateTimeExUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CDateTimeExUI::DoInit()
	{
		
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
			((CDateTimeExWnd *)m_pWindowDate)->Init(this, UIDTS_DATE);
			((CDateTimeExWnd *)m_pWindowDate)->ShowWindow();
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
					((CDateTimeExWnd *)m_pWindowDate)->Init(this, UIDTS_DATE);
					((CDateTimeExWnd *)m_pWindowDate)->ShowWindow();
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
			((CDateTimeExWnd *)m_pWindowTime)->Init(this, UIDTS_TIME);
			((CDateTimeExWnd *)m_pWindowTime)->ShowWindow();
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
					((CDateTimeExWnd *)m_pWindowTime)->Init(this, UIDTS_TIME);
					((CDateTimeExWnd *)m_pWindowTime)->ShowWindow();
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

		switch (m_uFormatStyle)
		{
		case UIDTS_DATETIME:
			m_pLabelDate->SetVisible(true);
			m_pLabelTime->SetVisible(true);
			break;
		case UIDTS_DATE:
			m_pLabelDate->SetVisible(true);
			m_pLabelTime->SetVisible(false);
			break;
		case UIDTS_TIME:
			m_pLabelDate->SetVisible(false);
			m_pLabelTime->SetVisible(true);
			break;
		}
	}

	UINT CDateTimeExUI::GetFormatStyle()
	{
		return m_uFormatStyle;
	}

	void CDateTimeExUI::SetText(LPCTSTR lpszDate)
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

	CDuiString CDateTimeExUI::GetText() const
	{
		CDuiString sText;

		if(m_uFormatStyle == UIDTS_DATE)
			sText.SmallFormat(_T("%4d - %02d - %02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
		else if(m_uFormatStyle == UIDTS_TIME)
			sText.SmallFormat(_T("%02d : %02d : %02d"), m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);
		else if(m_uFormatStyle == UIDTS_DATETIME)
			sText.SmallFormat(_T("%4d - %02d - %02d %2d : %02d : %02d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay, m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond);

		return sText;
	}

	void CDateTimeExUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		__super::SetPos(rc, bNeedInvalidate);
		if(m_pWindowDate != NULL)
		{
			RECT rcPos = ((CDateTimeExWnd *)m_pWindowDate)->CalPos();
			::SetWindowPos(m_pWindowDate->GetHWND(), NULL, rcPos.left, rcPos.top, 
				rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
		if(m_pWindowTime != NULL)
		{
			RECT rcPos = ((CDateTimeExWnd *)m_pWindowTime)->CalPos();
			::SetWindowPos(m_pWindowTime->GetHWND(), NULL, rcPos.left, rcPos.top, 
				rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);   
		}	
	}

	void CDateTimeExUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		__super::Move(szOffset, bNeedInvalidate);
		if(m_pWindowDate != NULL)
		{
			RECT rcPos = ((CDateTimeExWnd *)m_pWindowDate)->CalPos();
			::SetWindowPos(m_pWindowDate->GetHWND(), NULL, rcPos.left, rcPos.top, 
				rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
		if(m_pWindowTime != NULL)
		{
			RECT rcPos = ((CDateTimeExWnd *)m_pWindowTime)->CalPos();
			::SetWindowPos(m_pWindowTime->GetHWND(), NULL, rcPos.left, rcPos.top, 
				rcPos.right - rcPos.left, rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);   
		}
	}

	void CDateTimeExUI::SetVisible(bool bVisible)
	{
		__super::SetVisible(bVisible);
		if(!IsVisible() && (m_pWindowDate != NULL || m_pWindowTime != NULL))
			m_pManager->SetFocus(NULL);
	}

	void CDateTimeExUI::SetInternVisible(bool bVisible)
	{
		__super::SetInternVisible(bVisible);
		if(!IsVisible() && (m_pWindowDate != NULL || m_pWindowTime != NULL))
			m_pManager->SetFocus(NULL);
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

	CLabelUI *CDateTimeExUI::GetDateLabel() const
	{
		return m_pLabelDate;
	}

	CLabelUI *CDateTimeExUI::GetTimeLabel() const
	{
		return m_pLabelTime;
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
				SetFormatStyle(UIDTS_DATETIME);
				m_pLabelDate->SetVisible(true);
				m_pLabelTime->SetVisible(true);
			}
			else
			{
				SetFormatStyle(UIDTS_DATE);
				m_pLabelDate->SetVisible(true);
				m_pLabelTime->SetVisible(false);
			}
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
