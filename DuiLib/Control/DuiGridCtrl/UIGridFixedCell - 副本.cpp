	#include "StdAfx.h"
#include "UIGridFixedCell.h"


namespace DuiLib {
//add by liqs99 表格类
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CGridFixedCellUI)
CGridFixedCellUI::CGridFixedCellUI(void) : m_bDragable(true), m_uButtonState(0), m_iSepWidth(4),m_nScale(0)
{
	SetTextPadding(CDuiRect(2, 0, 2, 0));
	ptLastMouse.x = ptLastMouse.y = 0;
	SetMinWidth(16);
}


CGridFixedCellUI::~CGridFixedCellUI(void)
{
}

LPCTSTR CGridFixedCellUI::GetClass() const
{
	return _T("GridFixedCellUI");
}

UINT CGridFixedCellUI::GetControlFlags() const
{
	if( IsEnabled() && m_iSepWidth != 0 ) return UIFLAG_SETCURSOR;
	else return 0;
}

LPVOID CGridFixedCellUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridFixedCell")) == 0 ) return static_cast<CGridFixedCellUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridFixedCellUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
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
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
	{
		if( !IsEnabled() ) return;
		RECT rcSeparator = GetThumbRect();
		if (m_iSepWidth>=0)
			rcSeparator.left-=4;
		else
			rcSeparator.right+=4;
		if( ::PtInRect(&rcSeparator, event.ptMouse) ) {
			if( m_bDragable ) {
				m_uButtonState |= UISTATE_CAPTURED;
				ptLastMouse = event.ptMouse;
			}
		}
		else {
			m_uButtonState |= UISTATE_PUSHED;
			m_pManager->SendNotify(this, DUI_MSGTYPE_HEADERCLICK);
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			m_uButtonState &= ~UISTATE_CAPTURED;
			if( GetParent() ) 
				GetParent()->NeedParentUpdate();
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			RECT rc = m_rcItem;
			if( m_iSepWidth >= 0 ) {
				rc.right -= ptLastMouse.x - event.ptMouse.x;
			}
			else {
				rc.left -= ptLastMouse.x - event.ptMouse.x;
			}

			if( rc.right - rc.left > GetMinWidth() ) {
				m_cxyFixed.cx = rc.right - rc.left;
				ptLastMouse = event.ptMouse;
				if( GetParent() ) 
					GetParent()->NeedParentUpdate();
			}
		}
		return;
	}
	if( event.Type == UIEVENT_SETCURSOR )
	{
		RECT rcSeparator = GetThumbRect();
		if (m_iSepWidth>=0)//111024 by cddjr, 增加分隔符区域，方便用户拖动
			rcSeparator.left-=4;
		else
			rcSeparator.right+=4;
		if( IsEnabled() && m_bDragable && ::PtInRect(&rcSeparator, event.ptMouse) ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
			return;
		}
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CContainerUI::DoEvent(event);
}

SIZE CGridFixedCellUI::EstimateSize(SIZE szAvailable)
{
	if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo()->tm.tmHeight + 14);
	return CContainerUI::EstimateSize(szAvailable);
}

RECT CGridFixedCellUI::GetThumbRect() const
{
	if( m_iSepWidth >= 0 ) return CDuiRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
	else return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
}

void CGridFixedCellUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;

	if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if( m_sPushedImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
		if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) {}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( m_sHotImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
		if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( m_sFocusedImage.IsEmpty() && !m_sNormalImage.IsEmpty() ) DrawImage(hDC, (LPCTSTR)m_sNormalImage);
		if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
	}
	else {
		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
		}
	}

	if( !m_sSepImage.IsEmpty() ) {
		RECT rcThumb = GetThumbRect();
		rcThumb.left -= m_rcItem.left;
		rcThumb.top -= m_rcItem.top;
		rcThumb.right -= m_rcItem.left;
		rcThumb.bottom -= m_rcItem.top;

		m_sSepImageModify.Empty();
		m_sSepImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
		if( !DrawImage(hDC, (LPCTSTR)m_sSepImage, (LPCTSTR)m_sSepImageModify) ) {}
	}
}

void CGridFixedCellUI::PaintText(HDC hDC)
{
	DrawCellText(hDC, m_rcItem);
	/*
	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();

	RECT rcText = m_rcItem;
	rcText.left += m_rcTextPadding.left;
	rcText.top += m_rcTextPadding.top;
	rcText.right -= m_rcTextPadding.right;
	rcText.bottom -= m_rcTextPadding.bottom;

	CDuiString sText = GetText();
	if( sText.IsEmpty() ) return;
	int nLinks = 0;
	if( m_bShowHtml )
		CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, sText, m_dwTextColor, \
		NULL, NULL, nLinks, m_iFont, m_uTextStyle);
	else
		CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, m_dwTextColor, \
		m_iFont, m_uTextStyle);
	*/
}

void CGridFixedCellUI::SetText(LPCTSTR pstrText)
{
	CGridCellUI::SetText(pstrText);
}

CDuiString CGridFixedCellUI::GetText()
{
	return CGridCellUI::GetText();
}

void CGridFixedCellUI::SetEnabled(bool bEnable)
{
	CContainerUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

bool CGridFixedCellUI::IsDragable() const
{
	return m_bDragable;
}

void CGridFixedCellUI::SetDragable(bool bDragable)
{
	m_bDragable = bDragable;
	if ( !m_bDragable ) m_uButtonState &= ~UISTATE_CAPTURED;
}

DWORD CGridFixedCellUI::GetSepWidth() const
{
	return m_iSepWidth;
}

void CGridFixedCellUI::SetSepWidth(int iWidth)
{
	m_iSepWidth = iWidth;
}

LPCTSTR CGridFixedCellUI::GetNormalImage() const
{
	return m_sNormalImage;
}

void CGridFixedCellUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CGridFixedCellUI::GetHotImage() const
{
	return m_sHotImage;
}

void CGridFixedCellUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CGridFixedCellUI::GetPushedImage() const
{
	return m_sPushedImage;
}

void CGridFixedCellUI::SetPushedImage(LPCTSTR pStrImage)
{
	m_sPushedImage = pStrImage;
	Invalidate();
}

LPCTSTR CGridFixedCellUI::GetFocusedImage() const
{
	return m_sFocusedImage;
}

void CGridFixedCellUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CGridFixedCellUI::GetSepImage() const
{
	return m_sSepImage;
}

void CGridFixedCellUI::SetSepImage(LPCTSTR pStrImage)
{
	m_sSepImage = pStrImage;
	Invalidate();
}

void CGridFixedCellUI::SetScale(int nScale)
{
	m_nScale = nScale;
}

int CGridFixedCellUI::GetScale() const
{
	return m_nScale;
}


void CGridFixedCellUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("dragable")) == 0 ) SetDragable(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("sepwidth")) == 0 ) SetSepWidth(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("align")) == 0 ) {
		if( _tcsstr(pstrValue, _T("left")) != NULL ) {
			m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
			m_uTextStyle |= DT_LEFT;
		}
		if( _tcsstr(pstrValue, _T("center")) != NULL ) {
			m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
			m_uTextStyle |= DT_CENTER;
		}
		if( _tcsstr(pstrValue, _T("right")) != NULL ) {
			m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
			m_uTextStyle |= DT_RIGHT;
		}
	}
	else if( _tcsicmp(pstrName, _T("endellipsis")) == 0 ) {
		if( _tcsicmp(pstrValue, _T("true")) == 0 ) m_uTextStyle |= DT_END_ELLIPSIS;
		else m_uTextStyle &= ~DT_END_ELLIPSIS;
	}    
	else if( _tcsicmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("textcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) {
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetTextPadding(rcTextPadding);
	}
	else if( _tcsicmp(pstrName, _T("showhtml")) == 0 ) SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("sepimage")) == 0 ) SetSepImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("scale")) == 0 ) {
		LPTSTR pstr = NULL;
		SetScale(_tcstol(pstrValue, &pstr, 10)); 

	}
	else __super::SetAttribute(pstrName, pstrValue);
}

}