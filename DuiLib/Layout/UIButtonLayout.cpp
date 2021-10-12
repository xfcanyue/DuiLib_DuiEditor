#include "StdAfx.h"
#include "UIButtonLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CButtonLayoutUI)

	CButtonLayoutUI::CButtonLayoutUI()
		: m_iHotFont(-1)
		, m_iPushedFont(-1)
		, m_iFocusedFont(-1)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		, m_dwHotBkColor(0)
		, m_dwPushedBkColor(0)
		, m_dwPushedBorderColor(0)
		, m_dwDisableBorderColor(0)
		, m_iBindTabIndex(-1)
		, m_nStateCount(0)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	LPCTSTR CButtonLayoutUI::GetClass() const
	{
		return _T("ButtonLayoutUI");
	}

	LPVOID CButtonLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON_LAYOUT) == 0 ) return static_cast<CButtonLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	UINT CButtonLayoutUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonLayoutUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else __super::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}		
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}	
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
		}
		__super::DoEvent(event);
	}

	bool CButtonLayoutUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL )
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			BindTriggerTabSel();

			SwitchControlVisible();
		}
		return true;
	}

	void CButtonLayoutUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	
	void CButtonLayoutUI::SetHotFont(int index)
	{
		m_iHotFont = index;
		Invalidate();
	}

	int CButtonLayoutUI::GetHotFont() const
	{
		return m_iHotFont;
	}

	void CButtonLayoutUI::SetPushedFont(int index)
	{
		m_iPushedFont = index;
		Invalidate();
	}

	int CButtonLayoutUI::GetPushedFont() const
	{
		return m_iPushedFont;
	}

	void CButtonLayoutUI::SetFocusedFont(int index)
	{
		m_iFocusedFont = index;
		Invalidate();
	}

	int CButtonLayoutUI::GetFocusedFont() const
	{
		return m_iFocusedFont;
	}

	void CButtonLayoutUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
		Invalidate();
	}

	DWORD CButtonLayoutUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}
	
	void CButtonLayoutUI::SetPushedBkColor( DWORD dwColor )
	{
		m_dwPushedBkColor = dwColor;
		Invalidate();
	}

	DWORD CButtonLayoutUI::GetPushedBkColor() const
	{
		return m_dwPushedBkColor;
	}
		
	void CButtonLayoutUI::SetDisabledBkColor( DWORD dwColor )
	{
		m_dwDisabledBkColor = dwColor;
		Invalidate();
	}

	DWORD CButtonLayoutUI::GetDisabledBkColor() const
	{
		return m_dwDisabledBkColor;
	}
	
	void CButtonLayoutUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CButtonLayoutUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CButtonLayoutUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CButtonLayoutUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CButtonLayoutUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CButtonLayoutUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	LPCTSTR CButtonLayoutUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CButtonLayoutUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonLayoutUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CButtonLayoutUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonLayoutUI::GetPushedImage()
	{
		return m_sPushedImage;
	}

	void CButtonLayoutUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonLayoutUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CButtonLayoutUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonLayoutUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CButtonLayoutUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonLayoutUI::GetHotForeImage()
	{
		return m_sHotForeImage;
	}

	void CButtonLayoutUI::SetHotForeImage( LPCTSTR pStrImage )
	{
		m_sHotForeImage = pStrImage;
		Invalidate();
	}

	void CButtonLayoutUI::SetStateCount(int nCount)
	{
		m_nStateCount = nCount;
		Invalidate();
	}

	int CButtonLayoutUI::GetStateCount() const
	{
		return m_nStateCount;
	}

	LPCTSTR CButtonLayoutUI::GetStateImage()
	{
		return m_sStateImage;
	}

	void CButtonLayoutUI::SetStateImage( LPCTSTR pStrImage )
	{
		m_sNormalImage.Empty();
		m_sStateImage = pStrImage;
		Invalidate();
	}


	void CButtonLayoutUI::SwitchControlVisible()
	{
		if(m_sSwitchControlVisible.IsEmpty()) 
			return;

		CControlUI* pControl = GetManager()->FindControl(m_sSwitchControlVisible);
		if(!pControl) return;
		pControl->SetVisible(!pControl->IsVisible());
	}

	void CButtonLayoutUI::SetSwitchControlVisible(LPCTSTR ControlName)
	{
		m_sSwitchControlVisible = ControlName;
	}

	void CButtonLayoutUI::BindTabIndex(int _BindTabIndex )
	{
		if( _BindTabIndex >= 0)
			m_iBindTabIndex	= _BindTabIndex;
	}

	void CButtonLayoutUI::BindTabIndexName(LPCTSTR _BindTabIndexName)
	{
		m_sBindTabIndexName = _BindTabIndexName;
	}

	void CButtonLayoutUI::BindTabLayoutName( LPCTSTR _TabLayoutName )
	{
		if(_TabLayoutName)
			m_sBindTabLayoutName = _TabLayoutName;
	}

	void CButtonLayoutUI::BindTriggerTabSel( int _SetSelectIndex /*= -1*/ )
	{
		LPCTSTR pstrName = GetBindTabLayoutName();
// 		if(pstrName == NULL || (GetBindTabLayoutIndex() < 0 && _SetSelectIndex < 0))
// 			return;

// 		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
// 		if(!pTabLayout) return;
// 		pTabLayout->SelectItem(_SetSelectIndex >=0?_SetSelectIndex:GetBindTabLayoutIndex());

		if(GetBindTabLayoutIndex() >= 0 || _SetSelectIndex >= 0 )
		{
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			pTabLayout->SelectItem(_SetSelectIndex >=0?_SetSelectIndex:GetBindTabLayoutIndex());
		}
		else if(!m_sBindTabIndexName.IsEmpty())
		{
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			CControlUI *pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sBindTabIndexName));
			if(pControl)
				pTabLayout->SelectItem(pControl);
		}
	}

	void CButtonLayoutUI::RemoveBindTabIndex()
	{
		m_iBindTabIndex	= -1;
		m_sBindTabLayoutName.Empty();
	}

	int CButtonLayoutUI::GetBindTabLayoutIndex()
	{
		return m_iBindTabIndex;
	}

	LPCTSTR CButtonLayoutUI::GetBindTabLayoutName()
	{
		return m_sBindTabLayoutName;
	}

	void CButtonLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("stateimage")) == 0 ) SetStateImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("statecount")) == 0 ) SetStateCount(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("switchcontrolvisible")) == 0 ) SetSwitchControlVisible(pstrValue);
		else if( _tcsicmp(pstrName, _T("bindtabindex")) == 0 ) BindTabIndex(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bindtablayoutname")) == 0 ) BindTabLayoutName(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disabledbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hotfont")) == 0 ) SetHotFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("pushedfont")) == 0 ) SetPushedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("focuedfont")) == 0 ) SetFocusedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("pushedbordercolor")) == 0 ) //add by liqs99
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disablebordercolor")) == 0 ) //add by liqs99
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisableBorderColor(clrColor);
		}
		
		else __super::SetAttribute(pstrName, pstrValue);
	}

	void CButtonLayoutUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		RECT rcTextPadding = GetTextPadding();;
		GetManager()->GetDPIObj()->Scale(&rcTextPadding);
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += rcTextPadding.left;
		rc.right -= rcTextPadding.right;
		rc.top += rcTextPadding.top;
		rc.bottom -= rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;
		
		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
			clrColor = GetHotTextColor();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		int iFont = GetFont();
		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedFont() != -1) )
			iFont = GetPushedFont();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotFont() != -1) )
			iFont = GetHotFont();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedFont() != -1) )
			iFont = GetFocusedFont();

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, clrColor, \
			NULL, NULL, nLinks, iFont, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, clrColor, \
			iFont, m_uTextStyle);
	}

	void CButtonLayoutUI::PaintBkColor(HDC hDC)
	{
		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if(m_dwDisabledBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
				return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if(m_dwPushedBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwPushedBkColor));
				return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if(m_dwHotBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				return;
			}
		}

		return CControlUI::PaintBkColor(hDC);
	}

	void CButtonLayoutUI::PaintStatusImage(HDC hDC)
	{
		if(!m_sStateImage.IsEmpty() && m_nStateCount > 0)
		{
			TDrawInfo info;
			info.Parse(m_sStateImage, _T(""), m_pManager);
			const TImageInfo* pImage = m_pManager->GetImageEx(info.sImageName, info.sResType, info.dwMask, info.width, info.height, info.fillcolor, info.bHSL);
			if(m_sNormalImage.IsEmpty() && pImage != NULL)
			{
				SIZE szImage = {pImage->nX, pImage->nY};
				SIZE szStatus = {pImage->nX / m_nStateCount, pImage->nY};
				if( szImage.cx > 0 && szImage.cy > 0 )
				{
					RECT rcSrc = {0, 0, szImage.cx, szImage.cy};
					if(m_nStateCount > 0) {
						int iLeft = rcSrc.left + 0 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sNormalImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 1) {
						int iLeft = rcSrc.left + 1 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sHotImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
						m_sPushedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 2) {
						int iLeft = rcSrc.left + 2 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sPushedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
					if(m_nStateCount > 3) {
						int iLeft = rcSrc.left + 3 * szStatus.cx;
						int iRight = iLeft + szStatus.cx;
						int iTop = rcSrc.top;
						int iBottom = iTop + szStatus.cy;
						m_sDisabledImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
					}
				}
			}
		}

		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;
		if(!::IsWindowEnabled(m_pManager->GetPaintWindow())) {
			m_uButtonState &= UISTATE_DISABLED;
		}
		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
		}
	}

	void CButtonLayoutUI::PaintForeImage(HDC hDC)
	{
		if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if( !m_sPushedForeImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sPushedForeImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !m_sHotForeImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotForeImage) ) {}
				else return;
			}
		}
		if(!m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) {}
		}
	}

	void CButtonLayoutUI::PaintBorder(HDC hDC)
	{
		DWORD dwColor = 0;

		if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
			dwColor = m_dwPushedBorderColor;

		if(dwColor == 0)	
		{
			__super::PaintBorder(hDC);
			return;
		}

		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if(nBorderSize > 0)
		{
			if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 ) 
			{
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(dwColor), m_nBorderStyle);
			}
			else
			{
				CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(dwColor), m_nBorderStyle);
			}
		}
		else
		{
			RECT rcBorder;

			if(rcBorderSize.left > 0){
				rcBorder		= m_rcItem;
				rcBorder.right	= rcBorder.left;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.left,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.top > 0){
				rcBorder		= m_rcItem;
				rcBorder.bottom	= rcBorder.top;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.top,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.right > 0){
				rcBorder		= m_rcItem;
				rcBorder.right -= 1;
				rcBorder.left	= rcBorder.right;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.right,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.bottom > 0){
				rcBorder		= m_rcItem;
				rcBorder.bottom -= 1;
				rcBorder.top	= rcBorder.bottom;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.bottom,GetAdjustColor(dwColor),m_nBorderStyle);
			}
		}
	}
}