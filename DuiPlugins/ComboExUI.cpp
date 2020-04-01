#include "StdAfx.h"
#include "ComboExUI.h"


IMPLEMENT_DUICONTROL(CComboExUI)
CComboExUI::CComboExUI(void)
{
	m_szDropButtonSize.cx = 16;
	m_szDropButtonSize.cy = 16;
}


CComboExUI::~CComboExUI(void)
{
}

bool CComboExUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if(!CControlUI::DoPaint(hDC, rcPaint, pStopControl))
		return false;

	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if( !m_dbDisabledImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbDisabledImage) )
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if( !m_dbPushedImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbPushedImage) )
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_dbHotImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbHotImage) ) 
				return true;
		}
	}
	else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
		if( !m_dbFocusedImage.IsEmpty() ) {
			if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbFocusedImage) )
				return true;
		}
	}

	if( !m_dbNormalImage.IsEmpty() ) {
		if( DrawDropButtonImage(hDC, (LPCTSTR)m_dbNormalImage) )
			return true;
	}

	return true;
}

bool CComboExUI::DrawDropButtonImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
{
	RECT rc = m_rcItem;
	rc.top++;
	rc.bottom--;
	rc.right--;
	rc.left = rc.right - (rc.bottom - rc.top);

	SIZE sz = m_szDropButtonSize;
	RECT rcButton;
	rcButton.left = rc.left + (rc.right - rc.left)/2 - sz.cx/2;
	rcButton.right = rcButton.left + sz.cx;
	rcButton.top = rc.top + (rc.bottom - rc.top)/2 - sz.cy/2;
	rcButton.bottom = rcButton.top + sz.cy;

	return CRenderEngine::DrawImageString(hDC, m_pManager, rcButton, rcButton, pStrImage, pStrModify, m_instance);
}

bool CComboExUI::SelectString(LPCTSTR pstrText)
{
	for( int it = 0; it < GetCount(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
		if( !pControl->IsVisible() ) continue;
		
		if(pControl->GetText() == pstrText)
		{
			SelectItem(it);
			return true;
		}
	}
	return false;
}

void CComboExUI::SetText(LPCTSTR pstrText)
{
	SelectString(pstrText);
}

void CComboExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("dropbuttonsize")) == 0 )
	{
		SIZE cx = { 0 };
		LPTSTR pstr = NULL;
		cx.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		cx.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		m_szDropButtonSize = cx;
	}
	else if( _tcsicmp(pstrName, _T("dropbuttonnormalimage")) == 0 ) SetdbNormalImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonhotimage")) == 0 ) SetdbHotImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonpushedimage")) == 0 ) SetdbPushedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttonfocusedimage")) == 0 ) SetdbFocusedImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("dropbuttondisabledimage")) == 0 ) SetdbDisabledImage(pstrValue);
	else CComboUI::SetAttribute(pstrName, pstrValue);
}

LPCTSTR CComboExUI::GetdbNormalImage() const
{
	return m_dbNormalImage;
}

void CComboExUI::SetdbNormalImage(LPCTSTR pStrImage)
{
	m_dbNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbHotImage() const
{
	return m_dbHotImage;
}

void CComboExUI::SetdbHotImage(LPCTSTR pStrImage)
{
	m_dbHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbPushedImage() const
{
	return m_dbPushedImage;
}

void CComboExUI::SetdbPushedImage(LPCTSTR pStrImage)
{
	m_dbPushedImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbFocusedImage() const
{
	return m_dbFocusedImage;
}

void CComboExUI::SetdbFocusedImage(LPCTSTR pStrImage)
{
	m_dbFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CComboExUI::GetdbDisabledImage() const
{
	return m_dbDisabledImage;
}

void CComboExUI::SetdbDisabledImage(LPCTSTR pStrImage)
{
	m_dbDisabledImage = pStrImage;
	Invalidate();
}