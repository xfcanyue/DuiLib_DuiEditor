#include "stdafx.h"
#include "UIGridCtrl.h"

namespace DuiLib {

	class CGridWnd : public CWindowWnd
	{
	public:
		CGridWnd() : m_pOwner(NULL)
		{		
		}

		void Init(CGridCtrlUI* pOwner)
		{
			m_pOwner = pOwner;
			UINT uStyle = WS_CHILD | WS_VISIBLE;
			Create(m_pOwner->GetManager()->GetPaintWindow(), _T(""), uStyle, 0, 0, 0, 0, NULL);
		}

		virtual LPCTSTR GetWindowClassName() const
		{
			return _T("GridWnd");
		}

		virtual LPCTSTR GetSuperClassName() const
		{
			return NULL;
		}

		void OnFinalMessage(HWND hWnd)
		{
			m_pOwner->m_pWindow = NULL;
			delete this;
		}

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lRes = 0;
			BOOL bHandled = FALSE;
			/*
			if( uMsg == WM_PAINT)
			{
				HDC hDcPaint = ::GetDC(m_hWnd);

				RECT rc = m_pOwner->GetPos();
				PAINTSTRUCT ps = { 0 };
				::BeginPaint(m_hWnd, &ps);
				CRenderEngine::DrawColor(hDcPaint, ps.rcPaint, 0xFF000000);
				::EndPaint(m_hWnd, &ps);

				::ReleaseDC(m_hWnd, hDcPaint);
			}
			*/

			if(uMsg == WM_CREATE)
			{
				CWnd *pWnd;
				pWnd->Attach(m_hWnd);
				m_pGrid = new CGridCtrl;
				m_pGrid->Create(0,0,100,100, pWnd, 0);
				pWnd->Detach();
			}

			if( !bHandled ) 
			{
				return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			}

			return lRes;
		}

	private:
		CGridCtrlUI *m_pOwner;
		CGridCtrl *m_pGrid;
	};
	//add by liqs99 ±Ì∏Ò¿‡
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CGridCtrlUI)
	CGridCtrlUI::CGridCtrlUI() : m_pWindow(NULL)
	{
	}

	LPCTSTR CGridCtrlUI::GetClass() const
	{
		return _T("GridCtrlUI");
	}

	UINT CGridCtrlUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	LPVOID CGridCtrlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("GridCtrl")) == 0 ) return static_cast<CGridCtrlUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CGridCtrlUI::DoInit()
	{
		m_pWindow = new CGridWnd();
		((CGridWnd *)m_pWindow)->Init(this);
	}

	void CGridCtrlUI::DoEvent(TEventUI& event)
	{
		__super::DoEvent(event);
	}

	void CGridCtrlUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		SetInternVisible(bVisible);
	}

	void CGridCtrlUI::SetInternVisible(bool bVisible)
	{
		CControlUI::SetInternVisible(bVisible);
		if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
		{
			if(bVisible)
				::ShowWindow(m_pWindow->GetHWND(), SW_SHOW);
			else
				::ShowWindow(m_pWindow->GetHWND(), SW_HIDE);
		}
	}

	void CGridCtrlUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		__super::SetPos(rc, bNeedInvalidate);

		if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
		{
			::MoveWindow(m_pWindow->GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
		}		
	}

	void CGridCtrlUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);

		if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
		{
			RECT rc = GetPos();
			::MoveWindow(m_pWindow->GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
		}
	}

	void CGridCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{		
		__super::SetAttribute(pstrName, pstrValue);
	}
} // namespace DuiLib

