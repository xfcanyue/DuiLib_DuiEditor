#include "stdafx.h"
#include "UIRollTextEx.h"

namespace DuiLib
{
	static std::map<UINT_PTR, CControlUI *> g_MapTimerID_TO_CRollTextExUI;
	static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR idEvent, DWORD dwTime)  
	{
		std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CRollTextExUI.find(idEvent);
		if(it != g_MapTimerID_TO_CRollTextExUI.end())
		{
			CRollTextExUI *pObject = (CRollTextExUI *)it->second;
			pObject->OnTimer(idEvent);
		}
	}

	IMPLEMENT_DUICONTROL(CRollTextExUI)

	CRollTextExUI::CRollTextExUI(void)
	{
		m_idEventTimer = 0;
		m_nRollSpeed = 100;
		m_bPauseRoll = false;
		m_rcClient.left = 0;
		m_rcClient.top = 0;
		m_rcClient.right = 0;
		m_rcClient.bottom = 0;

		m_nScrollPos = 0;
		m_nText_W_H = 0;
		m_nStep = 5;
		m_bUseRoll = FALSE;
		m_nRollDirection = ROLLTEXT_LEFT;
	}

	CRollTextExUI::~CRollTextExUI(void)
	{
		EndRoll();
	}

	LPCTSTR CRollTextExUI::GetClass() const
	{
		return DUI_CTR_ROLLTEXTEX;
	}

	LPVOID CRollTextExUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_ROLLTEXTEX) == 0 ) return static_cast<CRollTextExUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CRollTextExUI::DoInit()
	{
		BeginRoll();
	}

	void CRollTextExUI::BeginRoll()
	{
		if(m_idEventTimer == 0)
		{
			m_nScrollPos = 0;
			m_nText_W_H = 0;
			m_idEventTimer = ::SetTimer(NULL, NULL, m_nRollSpeed, TimerProc);
			g_MapTimerID_TO_CRollTextExUI[m_idEventTimer] = this;
		}
	}

	void CRollTextExUI::EndRoll()
	{
		if(m_idEventTimer != 0)
		{
			::KillTimer(NULL, m_idEventTimer);
			std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CRollTextExUI.find(m_idEventTimer);
			if(it != g_MapTimerID_TO_CRollTextExUI.end())
				g_MapTimerID_TO_CRollTextExUI.erase(it);
			m_idEventTimer = 0;
		}
	}

	void CRollTextExUI::SetPos(RECT rc)
	{
		CLabelUI::SetPos(rc);
		m_nText_W_H = 0;
	}

	void CRollTextExUI::SetText( LPCTSTR pstrText )
	{
		CLabelUI::SetText(pstrText);
		m_nText_W_H = 0;
	}

	void CRollTextExUI::SetRollDirection(int nDirect)
	{
		if(m_nRollDirection == nDirect) return;
		m_nRollDirection = nDirect;
		m_nText_W_H = 0;
		EndRoll();
		BeginRoll();
	}

	int CRollTextExUI::GetRollDirection() const
	{
		return m_nRollDirection;
	}

	void CRollTextExUI::SetRollSpeed(int nMillisecond)
	{
		if(m_nRollDirection == nMillisecond) return;
		m_nRollSpeed = nMillisecond;
		m_nText_W_H = 0;
		EndRoll();
		BeginRoll();
	}

	int CRollTextExUI::GetRollSpeed() const { return m_nRollSpeed; }

	void CRollTextExUI::SetRollStep(int nStep)
	{
		if(m_nStep == nStep) return;
		m_nStep = nStep;
		EndRoll();
		BeginRoll();
	}

	int CRollTextExUI::GetRollStep() const { return m_nStep; }

	void CRollTextExUI::DoEvent(TEventUI& event)
	{
		if(event.Type == UIEVENT_MOUSEENTER)
			m_bPauseRoll = true;
		else if(event.Type == UIEVENT_MOUSELEAVE)
			m_bPauseRoll = false;
		else if(event.Type == UIEVENT_MOUSEMOVE)
			Invalidate();

		__super::DoEvent(event);
	}

	void CRollTextExUI::OnTimer(UINT_PTR idEvent)
	{
		if(m_idEventTimer != idEvent) return;
		if(!IsVisible()) return;
		if(m_bPauseRoll) 
		{
			Invalidate();
			return;
		}

		RECT rcTextPadding = GetTextPadding();
		CDuiRect  rcClient;
		rcClient = m_rcItem;
		rcClient.left += rcTextPadding.left;
		rcClient.right -= rcTextPadding.right;
		rcClient.top += rcTextPadding.top;
		rcClient.bottom -= rcTextPadding.bottom;

		int nScrollRange = 0;

		if(m_nRollDirection == ROLLTEXT_LEFT)
		{
			nScrollRange = rcClient.GetWidth();
			m_rcClient.top = rcClient.top;
			m_rcClient.bottom = rcClient.bottom;
			m_rcClient.left = rcClient.right - m_nScrollPos;
			m_rcClient.right = rcClient.right + m_nText_W_H;
		}
		else if(m_nRollDirection == ROLLTEXT_RIGHT)
		{
			nScrollRange = rcClient.GetWidth();
			m_rcClient.top = rcClient.top;
			m_rcClient.bottom = rcClient.bottom;
			m_rcClient.left = rcClient.left + m_nScrollPos - m_nText_W_H;
			m_rcClient.right = rcClient.left + m_nScrollPos;
		}
		else if(m_nRollDirection == ROLLTEXT_UP)
		{
			nScrollRange = rcClient.GetHeight();
			m_rcClient.left = rcClient.left;
			m_rcClient.right = rcClient.right;
			m_rcClient.top = rcClient.bottom - m_nScrollPos;
			m_rcClient.bottom = rcClient.bottom - m_nScrollPos + m_nText_W_H;
		}
		else if(m_nRollDirection == ROLLTEXT_DOWN)
		{
			nScrollRange = rcClient.GetHeight();
			m_rcClient.left = rcClient.left;
			m_rcClient.right = rcClient.right;
			m_rcClient.top = rcClient.top + m_nScrollPos - m_nText_W_H;
			m_rcClient.bottom = rcClient.top + m_nScrollPos;
		}

		m_nScrollPos += m_nStep;
		if (m_nScrollPos > nScrollRange + m_nText_W_H + 20)
		{
			m_nScrollPos = 0;
		}

		Invalidate();
	}

	void CRollTextExUI::PaintText(HDC hDC)
	{
		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		DWORD dwTextColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;
		
		UINT uTextStyle = DT_WORDBREAK | DT_EDITCONTROL ;

		if(m_nText_W_H == 0)	//第一次计算文本宽度或高度
		{
			RECT rcTextPadding = GetTextPadding();
			CDuiRect  rcClient;
			rcClient = m_rcItem;
			rcClient.left += rcTextPadding.left;
			rcClient.right -= rcTextPadding.right;
			rcClient.top += rcTextPadding.top;
			rcClient.bottom -= rcTextPadding.bottom;

			RECT rc = rcClient;
			uTextStyle |= DT_CALCRECT;		//第一次计算文本宽度或高度
// 			if (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) {	//左面移动
// 				rc.right += 10000;
// 			} else {																		//上下移动
// 				rc.bottom += 10000;
// 			}

			if( m_bShowHtml ) {
				int nLinks = 0;
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, dwTextColor, NULL, NULL, nLinks, m_iFont, uTextStyle);
			} else {
				CRenderEngine::DrawText(hDC, m_pManager, rc, sText, dwTextColor, m_iFont, uTextStyle);
			}	

			m_nText_W_H = (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) ? (rc.right - rc.left) : (rc.bottom - rc.top);		//计算文本宽度或高度

		}
		else
		{
			if( m_bShowHtml ) {
				int nLinks = 0;
				CRenderEngine::DrawHtmlText(hDC, m_pManager, m_rcClient, sText, dwTextColor, NULL, NULL, nLinks, m_iFont, uTextStyle);
			} else {
				CRenderEngine::DrawText(hDC, m_pManager, m_rcClient, sText, dwTextColor, m_iFont, uTextStyle);
			}	
		}	
	}

	void CRollTextExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("rollspeed")) == 0 ) 
		{
			SetRollSpeed(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("rolldirection")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("left")) == 0 )
				SetRollDirection(ROLLTEXT_LEFT);
			else if( _tcsicmp(pstrValue, _T("right")) == 0 )
				SetRollDirection(ROLLTEXT_RIGHT);
			else if( _tcsicmp(pstrValue, _T("up")) == 0 )
				SetRollDirection(ROLLTEXT_UP);
			else if( _tcsicmp(pstrValue, _T("down")) == 0 )
				SetRollDirection(ROLLTEXT_DOWN);
		}
		else if( _tcsicmp(pstrName, _T("rollstep")) == 0 ) {
			SetRollStep(_ttoi(pstrValue));
		}
		else
			__super::SetAttribute(pstrName, pstrValue);
	}
}