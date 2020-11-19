#include "StdAfx.h"
#include "UIIconButton.h"


namespace DuiLib
{

IMPLEMENT_DUICONTROL(CIconButtonUI)

CIconButtonUI::CIconButtonUI(void) : m_szIcon(16,16), m_uIconAlign(DT_LEFT)
{
}


CIconButtonUI::~CIconButtonUI(void)
{
}

LPCTSTR CIconButtonUI::GetClass() const
{
	return DUI_CTR_ICONBUTTON;
}

LPVOID CIconButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_ICONBUTTON) == 0 ) return static_cast<CIconButtonUI*>(this);
	return __super::GetInterface(pstrName);
}

void CIconButtonUI::PaintText(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	CDuiString sText = GetText();
	if( sText.IsEmpty() ) return;

	RECT rcText = m_rcItem;
	//rcText.left += m_szIcon.cx;
	rcText.left += m_rcItem.bottom - m_rcItem.top;

	RECT m_rcTextPadding = CButtonUI::m_rcTextPadding;
	GetManager()->GetDPIObj()->Scale(&m_rcTextPadding);
	int nLinks = 0;
	RECT rc = rcText;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

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

void CIconButtonUI::PaintStatusImage(HDC hDC)
{
	__super::PaintStatusImage(hDC);

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_IconDisabledImage.IsEmpty() ) {
			if( DrawIconImage(hDC, (LPCTSTR)m_IconDisabledImage) )
				return;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if( !m_IconPushedImage.IsEmpty() ) {
			if( DrawIconImage(hDC, (LPCTSTR)m_IconPushedImage) )
				return;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_IconHotImage.IsEmpty() ) {
			if( DrawIconImage(hDC, (LPCTSTR)m_IconHotImage) ) 
				return;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_IconFocusedImage.IsEmpty() ) {
			if( DrawIconImage(hDC, (LPCTSTR)m_IconFocusedImage) )
				return;
		}
	}

	if( !m_IconNormalImage.IsEmpty() ) {
		if( DrawIconImage(hDC, (LPCTSTR)m_IconNormalImage) )
			return;
	}
}

bool CIconButtonUI::DrawIconImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify/*=NULL*/)
{
	RECT rcItem = m_rcItem;
	RECT rcImage;

	rcItem.top++;
	rcItem.bottom--;
	rcItem.left++;
	rcItem.right = rcItem.left + rcItem.bottom - rcItem.top;

// 	rcImage.left = rcItem.left + (rcItem.right - rcItem.left)/2 - m_szIcon.cx/2;
// 	rcImage.right = rcImage.left + m_szIcon.cx;
// 	rcImage.top = rcItem.top + (rcItem.bottom - rcItem.top)/2 - m_szIcon.cy/2;
// 	rcImage.bottom = rcImage.top + m_szIcon.cy;

	rcImage.left = rcItem.right - m_szIcon.cx;
	rcImage.right = rcItem.right;
	rcImage.top = rcItem.top + (rcItem.bottom - rcItem.top)/2 - m_szIcon.cy/2;
	rcImage.bottom = rcImage.top + m_szIcon.cy;
		
	return CRenderEngine::DrawImageString(hDC, m_pManager, rcImage, rcImage, pStrImage, pStrModify, m_instance);
}

void CIconButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("iconsize")) == 0 )
	{
		SIZE cx = { 0 };
		LPTSTR pstr = NULL;
		cx.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		cx.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		m_szIcon = cx;
	}
	else if( _tcsicmp(pstrName, _T("iconnormalimage")) == 0 ) SetIconNormalImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("iconhotimage")) == 0 ) SetIconHotImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("iconpushedimage")) == 0 ) SetIconPushedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("iconfocusedimage")) == 0 ) SetIconFocusedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("icondisabledimage")) == 0 ) SetIconDisabledImage(pstrValue);
	else __super::SetAttribute(pstrName, pstrValue);
}

LPCTSTR CIconButtonUI::GetIconNormalImage() const
{
	return m_IconNormalImage;
}

void CIconButtonUI::SetIconNormalImage(LPCTSTR pStrImage)
{
	m_IconNormalImage = pStrImage;
}

LPCTSTR CIconButtonUI::GetIconHotImage() const
{
	return m_IconHotImage;
}

void CIconButtonUI::SetIconHotImage(LPCTSTR pStrImage)
{
	m_IconHotImage = pStrImage;
}

LPCTSTR CIconButtonUI::GetIconPushedImage() const
{
	return m_IconPushedImage;
}

void CIconButtonUI::SetIconPushedImage(LPCTSTR pStrImage)
{
	m_IconPushedImage = pStrImage;
}

LPCTSTR CIconButtonUI::GetIconFocusedImage() const
{
	return m_IconFocusedImage;
}

void CIconButtonUI::SetIconFocusedImage(LPCTSTR pStrImage)
{
	m_IconFocusedImage = pStrImage;
}

LPCTSTR CIconButtonUI::GetIconDisabledImage() const
{
	return m_IconDisabledImage;
}

void CIconButtonUI::SetIconDisabledImage(LPCTSTR pStrImage)
{
	m_IconDisabledImage = pStrImage;
}

}