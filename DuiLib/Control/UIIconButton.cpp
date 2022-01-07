#include "StdAfx.h"
#include "UIIconButton.h"


namespace DuiLib
{

IMPLEMENT_DUICONTROL(CIconButtonUI)

CIconButtonUI::CIconButtonUI(void) : m_szIcon(16,16)
{
	SetChildVAlign(DT_VCENTER);
	Add(m_pIconUI = new CButtonUI);
	m_pIconUI->SetFixedWidth(m_szIcon.cx);
	m_pIconUI->SetFixedHeight(m_szIcon.cy);
	m_pIconUI->SetMouseEnabled(false);
}


CIconButtonUI::~CIconButtonUI(void)
{
}

LPCTSTR CIconButtonUI::GetClass() const
{
	return _T("IconButtonUI");
}

LPVOID CIconButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_ICONBUTTON) == 0 ) return static_cast<CIconButtonUI*>(this);
	return __super::GetInterface(pstrName);
}

void CIconButtonUI::DoInit()
{
	m_pIconUI->SetFixedWidth(m_szIcon.cx);
	m_pIconUI->SetFixedHeight(m_szIcon.cy);
}

SIZE CIconButtonUI::EstimateSize(SIZE szAvailable)
{
	if (IsAutoCalcWidth() || IsAutoCalcHeight()) 
	{
		CDuiString sText = GetText();

		RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
		int nLinks = 0;

		GetManager()->Render()->DrawText(rcText, GetTextPadding(), sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);

		RECT rcTextPadding = GetTextPadding();
		RECT rcInset = GetInset();

		if(IsAutoCalcWidth())
		{	
			int cx = rcText.right - rcText.left + rcInset.left + rcInset.right + m_szIcon.cx;
			m_cxyFixed.cx = cx;
		}

		if(IsAutoCalcHeight())
		{
			int cy = rcText.bottom - rcText.top + rcInset.top + rcInset.bottom + m_szIcon.cy;
			m_cxyFixed.cy = cy;
		}

		return CDuiSize(GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx), GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy));
	}

	return __super::EstimateSize(szAvailable);
}

void CIconButtonUI::PaintText(UIRender *pRender)
{
	CDuiString sText = GetText();
	if(sText.IsEmpty()) return;

	RECT rcText = m_rcItem;
	DWORD dwColor = 0;
	int iFont = -1;

	//////////////////////////////////////////////////////////////////////////
	if( !IsEnabled() )
		iFont = GetDisabledFont();

	else if( IsPushedState() )
		iFont = GetPushedFont();

	else if( IsHotState() )
		iFont = GetHotFont();

	else if( IsSelectedState() )
		iFont = GetSelectedFont();

	else if( IsFocused() )
		iFont = GetFocusedFont();

	if(iFont == -1)
		iFont = GetFont();

	//////////////////////////////////////////////////////////////////////////
	if( !IsEnabled() )
		dwColor = GetDisabledTextColor();

	else if( IsHotState() )
		dwColor = GetHotTextColor();

	else if( IsPushedState() )
		dwColor = GetPushedTextColor();

	else if( IsSelectedState() )
		dwColor = GetSelectedTextColor();

	else if( IsFocused() )
		dwColor = GetFocusedTextColor();

	if(dwColor == 0)
		dwColor = GetTextColor();

	if(dwColor == 0 && m_pManager)
		dwColor = m_pManager->GetDefaultFontColor();

	//////////////////////////////////////////////////////////////////////////
	RECT rcInset = GetInset();
	rcText.left += rcInset.left;
	rcText.right -= rcInset.right;
	rcText.top += rcInset.top;
	rcText.bottom -= rcInset.bottom;

	rcText.left += m_szIcon.cx;

	pRender->DrawText(rcText, GetTextPadding(), sText, dwColor, iFont, GetTextStyle());
	return;
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
		m_pIconUI->SetFixedWidth(m_szIcon.cx);
		m_pIconUI->SetFixedHeight(m_szIcon.cy);
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
	return m_pIconUI->GetNormalImage();
}

void CIconButtonUI::SetIconNormalImage(LPCTSTR pStrImage)
{
	m_pIconUI->SetNormalImage(pStrImage);
}

LPCTSTR CIconButtonUI::GetIconHotImage() const
{
	return m_pIconUI->GetHotImage();
}

void CIconButtonUI::SetIconHotImage(LPCTSTR pStrImage)
{
	m_pIconUI->SetHotImage(pStrImage);
}

LPCTSTR CIconButtonUI::GetIconPushedImage() const
{
	return m_pIconUI->GetPushedImage();
}

void CIconButtonUI::SetIconPushedImage(LPCTSTR pStrImage)
{
	m_pIconUI->SetPushedImage(pStrImage);
}

LPCTSTR CIconButtonUI::GetIconFocusedImage() const
{
	return m_pIconUI->GetFocusedImage();
}

void CIconButtonUI::SetIconFocusedImage(LPCTSTR pStrImage)
{
	m_pIconUI->SetFocusedImage(pStrImage);
}

LPCTSTR CIconButtonUI::GetIconDisabledImage() const
{
	return m_pIconUI->GetDisabledImage();
}

void CIconButtonUI::SetIconDisabledImage(LPCTSTR pStrImage)
{
	m_pIconUI->SetDisabledImage(pStrImage);
}

}