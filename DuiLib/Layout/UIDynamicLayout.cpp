#include "StdAfx.h"
#include "UIDynamicLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CDynamicLayoutUI)
	CDynamicLayoutUI::CDynamicLayoutUI() : m_iSepWidth(0), m_iSepHeight(0), m_bImmMode(false)
	{
		m_ptLastMouse.x = m_ptLastMouse.y = 0;
		memset(&m_rcNewPos, 0, sizeof(m_rcNewPos));

		m_pCalcControl = NULL;
		memset(&m_rcCalcChild, 0, sizeof(RECT));
		m_bCalcResult = false;

		m_eLayout = Layout_HorizontalLayout;
		m_eSepAction = eSepNull;
	}

	LPCTSTR CDynamicLayoutUI::GetClass() const
	{
		return _T("DynamicLayoutUI");
	}

	LPVOID CDynamicLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_DYNAMICLAYOUT) == 0 ) return static_cast<CDynamicLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CDynamicLayoutUI::GetControlFlags() const
	{
		if( IsEnabled() && m_iSepWidth != 0 ) return UIFLAG_SETCURSOR;
		else return 0;
	}

	void CDynamicLayoutUI::SetLayout(emLayoutType eMode) 
	{ 
		if(m_eLayout == eMode) return;
		m_eLayout = eMode;
		NeedUpdate();
	}

	emLayoutType CDynamicLayoutUI::GetLayout() const { return m_eLayout; }

	SIZE CDynamicLayoutUI::EstimateSize(SIZE szAvailable)
	{
		return CContainerUI::EstimateSize(szAvailable);
	}

	void CDynamicLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		if(GetLayout() == Layout_HorizontalLayout)
		{
			SetPosHorizontalLayout(rc, bNeedInvalidate);
		}
		else if(GetLayout() == Layout_VerticalLayout)
		{
			SetPosVerticalLayout(rc, bNeedInvalidate);
		}
	}

	void CDynamicLayoutUI::DoPostPaint(UIRender *pRender, const RECT& rcPaint)
	{
		if( IsCaptureState() && !m_bImmMode ) {
			RECT rcSeparator = GetThumbRect(true);
			pRender->DrawColor(rcSeparator, CDuiSize(0,0), 0xAA000000);
		}
	}

	void CDynamicLayoutUI::SetSepHeight(int iHeight)
	{
		m_iSepHeight = iHeight;
	}

	int CDynamicLayoutUI::GetSepHeight() const
	{
		return m_iSepHeight;
	}

	void CDynamicLayoutUI::SetSepWidth(int iWidth)
	{
		m_iSepWidth = iWidth;
	}

	int CDynamicLayoutUI::GetSepWidth() const
	{
		return m_iSepWidth;
	}

	void CDynamicLayoutUI::SetSepImmMode(bool bImmediately)
	{
		if( m_bImmMode == bImmediately ) return;
		if( IsCaptureState() && !m_bImmMode && m_pManager != NULL ) {
			m_pManager->RemovePostPaint(this);
		}

		m_bImmMode = bImmediately;
	}

	bool CDynamicLayoutUI::IsSepImmMode() const
	{
		return m_bImmMode;
	}

	void CDynamicLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("layout")) == 0 )
		{
			if(_tcsicmp(pstrValue, _T("VerticalLayout")) == 0) SetLayout(Layout_VerticalLayout);
			else SetLayout(Layout_HorizontalLayout);
		}
		else if( _tcsicmp(pstrName, _T("sepwidth")) == 0 ) SetSepWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("sepheight")) == 0 ) SetSepHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("sepimm")) == 0 ) SetSepImmMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CDynamicLayoutUI::DoEvent(TEventUI& event)
	{
		if(m_iSepWidth !=0 || m_iSepHeight != 0)
		{
			if( event.Type == UIEVENT_SETCURSOR )
			{
				m_eSepAction = eSepWidth;
				CDuiRect rcSeparator = GetThumbRect(false);
				//if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
				if( IsEnabled() && rcSeparator.PtInRect(event.ptMouse) ) 
				{
					//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
					GetManager()->SetCursor(DUI_SIZEWE);
					return;
				}

				m_eSepAction = eSepHeight;
				rcSeparator = GetThumbRect(false);
				//if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
				if( IsEnabled() && rcSeparator.PtInRect(event.ptMouse) ) {
					//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					GetManager()->SetCursor(DUI_SIZENS);
					return;
				}

				m_eSepAction = eSepNull;
			}
		}

		if( m_eSepAction == eSepWidth ) {
			if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
			{
				CDuiRect rcSeparator = GetThumbRect(false);
				//if( ::PtInRect(&rcSeparator, event.ptMouse) ) 
				if( rcSeparator.PtInRect(event.ptMouse) ) 
				{
					SetCaptureState(true);
					m_ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
					return;
				}
			}
			if( event.Type == UIEVENT_BUTTONUP )
			{
				if( IsCaptureState() ) {
					SetCaptureState(false);
					m_rcItem = m_rcNewPos;
					if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( IsCaptureState() ) {
					LONG cx = event.ptMouse.x - m_ptLastMouse.x;
					m_ptLastMouse = event.ptMouse;
					RECT rc = m_rcNewPos;
					if( m_iSepWidth >= 0 ) {
						if( cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepWidth ) return;
						if( cx < 0 && event.ptMouse.x > m_rcNewPos.right ) return;
						rc.right += cx;
						if( rc.right - rc.left <= GetMinWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
							rc.right = rc.left + GetMinWidth();
						}
						if( rc.right - rc.left >= GetMaxWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
							rc.right = rc.left + GetMaxWidth();
						}
					}
					else {
						if( cx > 0 && event.ptMouse.x < m_rcNewPos.left ) return;
						if( cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepWidth ) return;
						rc.left += cx;
						if( rc.right - rc.left <= GetMinWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
							rc.left = rc.right - GetMinWidth();
						}
						if( rc.right - rc.left >= GetMaxWidth() ) {
							if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
							rc.left = rc.right - GetMaxWidth();
						}
					}

					CDuiRect rcInvalidate = GetThumbRect(true);
					m_rcNewPos = rc;
					m_cxyFixed.cx = GetManager()->GetDPIObj()->ScaleIntBack(m_rcNewPos.right - m_rcNewPos.left);

					if( m_bImmMode ) {
						m_rcItem = m_rcNewPos;
						NeedParentUpdate();
					}
					else {
						rcInvalidate.Join(GetThumbRect(true));
						rcInvalidate.Join(GetThumbRect(false));
						if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
					}
					return;
				}
			}		
		}

		if( m_eSepAction == eSepHeight ) {
			if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
			{
				CDuiRect rcSeparator = GetThumbRect(false);
				//if( ::PtInRect(&rcSeparator, event.ptMouse) ) {
				if( rcSeparator.PtInRect(event.ptMouse) ) {
					SetCaptureState(true);
					m_ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if( !m_bImmMode && m_pManager ) m_pManager->AddPostPaint(this);
					return;
				}
			}
			if( event.Type == UIEVENT_BUTTONUP )
			{
				if( IsCaptureState() ) {
					SetCaptureState(false);
					m_rcItem = m_rcNewPos;
					if( !m_bImmMode && m_pManager ) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if( event.Type == UIEVENT_MOUSEMOVE )
			{
				if( IsCaptureState() ) {
					LONG cy = event.ptMouse.y - m_ptLastMouse.y;
					m_ptLastMouse = event.ptMouse;
					RECT rc = m_rcNewPos;
					if( m_iSepHeight >= 0 ) {
						if( cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight ) return;
						if( cy < 0 && event.ptMouse.y > m_rcNewPos.bottom ) return;
						rc.bottom += cy;
						if( rc.bottom - rc.top <= GetMinHeight() ) {
							if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
							rc.bottom = rc.top + GetMinHeight();
						}
						if( rc.bottom - rc.top >= GetMaxHeight() ) {
							if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
							rc.bottom = rc.top + GetMaxHeight();
						}
					}
					else {
						if( cy > 0 && event.ptMouse.y < m_rcNewPos.top ) return;
						if( cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight ) return;
						rc.top += cy;
						if( rc.bottom - rc.top <= GetMinHeight() ) {
							if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) return;
							rc.top = rc.bottom - GetMinHeight();
						}
						if( rc.bottom - rc.top >= GetMaxHeight() ) {
							if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) return;
							rc.top = rc.bottom - GetMaxHeight();
						}
					}

					CDuiRect rcInvalidate = GetThumbRect(true);
					m_rcNewPos = rc;
					m_cxyFixed.cy = GetManager()->GetDPIObj()->ScaleIntBack(m_rcNewPos.bottom - m_rcNewPos.top);

					if( m_bImmMode ) {
						m_rcItem = m_rcNewPos;
						NeedParentUpdate();
					}
					else {
						rcInvalidate.Join(GetThumbRect(true));
						rcInvalidate.Join(GetThumbRect(false));
						if( m_pManager ) m_pManager->Invalidate(rcInvalidate);
					}
					return;
				}
			}
			if( event.Type == UIEVENT_SETCURSOR )
			{
				m_eSepAction = eSepHeight;
				CDuiRect rcSeparator = GetThumbRect(false);
				//if( IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse) ) {
				if( IsEnabled() && rcSeparator.PtInRect(event.ptMouse) ) {
					//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					GetManager()->SetCursor(DUI_SIZENS);
					return;
				}
				m_eSepAction = eSepNull;
			}
		}

		CContainerUI::DoEvent(event);
	}

	RECT CDynamicLayoutUI::GetThumbRect(bool bUseNew) const
	{
		if(m_eSepAction == eSepWidth)
		{
			if( IsCaptureState() && bUseNew) 
			{
				if( m_iSepWidth >= 0 ) return CDuiRect(m_rcNewPos.right - m_iSepWidth, m_rcNewPos.top, m_rcNewPos.right, m_rcNewPos.bottom);
				else return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.left - m_iSepWidth, m_rcNewPos.bottom);
			}
			else 
			{
				if( m_iSepWidth >= 0 ) return CDuiRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
				else return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
			}
		}
		else if(m_eSepAction == eSepHeight)
		{
			if( IsCaptureState() && bUseNew) 
			{
				if( m_iSepHeight >= 0 ) 
					return CDuiRect(m_rcNewPos.left, MAX(m_rcNewPos.bottom - m_iSepHeight, m_rcNewPos.top), 
					m_rcNewPos.right, m_rcNewPos.bottom);
				else 
					return CDuiRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.right, 
					MIN(m_rcNewPos.top - m_iSepHeight, m_rcNewPos.bottom));
			}
			else 
			{
				if( m_iSepHeight >= 0 ) 
					return CDuiRect(m_rcItem.left, MAX(m_rcItem.bottom - m_iSepHeight, m_rcItem.top), m_rcItem.right, 
					m_rcItem.bottom);
				else 
					return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, 
					MIN(m_rcItem.top - m_iSepHeight, m_rcItem.bottom));

			}
		}
		return CDuiRect(0,0,0,0);
	}
}

