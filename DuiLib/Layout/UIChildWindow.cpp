#include "StdAfx.h"
#include "UIChildWindow.h"

#ifdef DUILIB_WIN32
namespace DuiLib
{

class CChildWnd : public CWindowWnd
{
public:
	CChildWnd(): m_pOwner(NULL)
	{

	}

	void Init(CChildWindowUI* pOwner)
	{
		m_pOwner = pOwner;
		UINT uStyle = WS_CHILD | WS_VISIBLE;
		Create(m_pOwner->GetManager()->GetPaintWindow(), _T(""), uStyle, 0, 0, 0, 0, NULL);
	}

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("ChildWindow");
	}

	void OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pWindow = NULL;
		CWindowWnd::OnFinalMessage(hWnd);
		delete this;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;

		if( uMsg == WM_CREATE )
		{
			m_pm.Init(GetHWND(), NULL, this);
		}
		else if( uMsg == WM_PAINT)
		{
			RECT rc = m_pOwner->GetPos();
			PAINTSTRUCT ps = { 0 };
			::BeginPaint(m_hWnd, &ps);

			RECT rcPaint;
			::GetClientRect(m_hWnd, &rcPaint);

			m_pm.Render()->Resize(rcPaint);
			if(m_pOwner->GetBkColor() == 0)
				m_pm.Render()->DrawColor(ps.rcPaint, CDuiSize(0,0), 0xFF000000);
			else
				m_pm.Render()->DrawColor(ps.rcPaint, CDuiSize(0,0), m_pOwner->GetBkColor());
			::BitBlt(ps.hdc, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, m_pm.Render()->GetDC(), rcPaint.left, rcPaint.top, SRCCOPY);

			::EndPaint(m_hWnd, &ps);
		}
		else if( uMsg == WM_SIZE)
		{
			m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_WINDOWSIZE, 0, 0);
		}
		else if( uMsg == WM_CLOSE)
		{
			::DestroyWindow(m_hWnd);
			bHandled = TRUE;
		}

		if( !bHandled ) 
		{
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}
		return lRes;
	}

	RECT CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetInset();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;

		CContainerUI* pParent = m_pOwner;
		RECT rcParent;
		while( pParent = (CContainerUI*)pParent->GetParent() ) {
			if( !pParent->IsVisible() ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			CDuiString s = pParent->GetName();
			//rcParent = pParent->GetClientPos();
			rcParent = pParent->GetPos(); //子窗口不可超出父窗口区域，所以不要计入滚动条
			CDuiRect rcInset = pParent->GetInset();
			rcParent.left += rcInset.left;
			rcParent.top += rcInset.top;
			rcParent.right -= rcInset.right;
			rcParent.bottom -= rcInset.bottom;
			//if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
			if( !rcPos.Intersect(rcPos, rcParent) ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
	}
	DuiLibPaintManagerUI m_pm;
private:
	CChildWindowUI *m_pOwner;
};

///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CChildWindowUI)

CChildWindowUI::CChildWindowUI(void) : m_pWindow(NULL)
{

}


CChildWindowUI::~CChildWindowUI(void)
{	
	DestroyWnd();
}

void CChildWindowUI::DoInit()
{
	CreateWnd();
}

void CChildWindowUI::SetVisible(bool bVisible)
{
	CControlUI::SetVisible(bVisible);
	SetInternVisible(bVisible);
}

void CChildWindowUI::SetInternVisible(bool bVisible)
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

LPCTSTR CChildWindowUI::GetClass() const
{
	return _T("ChildWindowUI");
}

LPVOID CChildWindowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_CHILDWINDOW) == 0 ) return static_cast<CChildWindowUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CChildWindowUI::DoEvent(TEventUI& event)
{
	CContainerUI::DoEvent(event);
}

void CChildWindowUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CContainerUI::SetPos(rc, bNeedInvalidate);

	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		RECT rcPos = ((CChildWnd *)m_pWindow)->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);   
// 		::SetWindowPos(m_pWindow->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left, 
// 			rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);  
	}
}

void CChildWindowUI::Move(SIZE szOffset, bool bNeedInvalidate)
{
	CContainerUI::Move(szOffset, bNeedInvalidate);

	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		RECT rcPos = ((CChildWnd *)m_pWindow)->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);  
	}
}

void CChildWindowUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
// 	if (_tcscmp(pstrName, _T("homepage")) == 0)
// 	{
// 		m_strURL = pstrValue;
// 	}
// 	else 
		CContainerUI::SetAttribute(pstrName,pstrValue);
}


CWindowWnd *CChildWindowUI::CreateWnd()
{
	if(m_pWindow != NULL)
		return m_pWindow;

	m_pWindow = new CChildWnd();
	((CChildWnd *)m_pWindow)->Init(this);
	return m_pWindow;
}

void CChildWindowUI::DestroyWnd()
{
	if(m_pWindow && ::IsWindow(m_pWindow->GetHWND()))
	{
		::SendMessage(m_pWindow->GetHWND(), WM_CLOSE, 0, 0);
		m_pWindow = NULL;
	}
}

CWindowWnd *CChildWindowUI::GetWnd()
{
	return m_pWindow;
}

void CChildWindowUI::RefreshWindow()
{
	if(m_pWindow)
		::InvalidateRect(m_pWindow->GetHWND(), NULL, TRUE);
}

}
#endif //#ifdef DUILIB_WIN32
