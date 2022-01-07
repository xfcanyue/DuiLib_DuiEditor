#include "StdAfx.h"
#include "UIGroupBox.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CGroupBoxUI)

	//////////////////////////////////////////////////////////////////////////
	//
	CGroupBoxUI::CGroupBoxUI()
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
		SetInset(CDuiRect(20, 25, 20, 20));
	}

	CGroupBoxUI::~CGroupBoxUI()
	{
	}

	LPCTSTR CGroupBoxUI::GetClass() const
	{
		return _T("GroupBoxUI");
	}

	LPVOID CGroupBoxUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("GroupBox")) == 0 ) return static_cast<CGroupBoxUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	void CGroupBoxUI::PaintText(UIRender *pRender)
	{
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) {
			return;
		}

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();
		if( sText.IsEmpty() ) return;

		CDuiRect rcText = m_rcItem;
		rcText.Deflate(5,5);
		SIZE szAvailable = { rcText.right - rcText.left, rcText.bottom - rcText.top };
		SIZE sz = CalcrectSize(szAvailable);

		//¼ÆËãÎÄ×ÖÇøÓò
		rcText.left = rcText.left + 15;
		rcText.top = rcText.top - 5;
		rcText.right = rcText.left + sz.cx;
		rcText.bottom = rcText.top + sz.cy;

		DWORD dwTextColor = m_dwTextColor;
		if(!IsEnabled()) dwTextColor = m_dwDisabledTextColor;
		pRender->DrawText(rcText, CDuiRect(0,0,0,0), sText, dwTextColor, m_iFont, m_uTextStyle, GetAdjustColor(m_dwBackColor));
	}

	void CGroupBoxUI::PaintBorder(UIRender *pRender)
	{
		int nBorderSize = GetBorderSize();

		if( nBorderSize > 0 )
		{
			CDuiRect rcItem = m_rcItem;
			rcItem.Deflate(5, 5);

			DWORD dwColor = 0;

			if(dwColor == 0 && !IsEnabled() && GetDisableBorderColor() != 0)
				dwColor = GetDisableBorderColor();

			if(dwColor == 0 && IsFocused() && GetFocusBorderColor() != 0)
				dwColor = GetFocusBorderColor();

			if(dwColor == 0 && IsHotState() && GetHotBorderColor() != 0)
				dwColor = GetHotBorderColor();

			if(dwColor == 0) 
				dwColor = GetBorderColor();

			if(dwColor != 0) 
			{
				pRender->DrawBorder(rcItem, nBorderSize, GetBorderRound(), 
					GetBorderRectSize(), GetAdjustColor(dwColor), GetBorderStyle());
			}
		}

		PaintText(pRender);
	}

	SIZE CGroupBoxUI::CalcrectSize(SIZE szAvailable)
	{
		SIZE cxyFixed = GetFixedXY();
		RECT rcText = { 0, 0, MAX(szAvailable.cx, cxyFixed.cx), 20 };
		
		CDuiString sText = GetText();

		GetManager()->Render()->DrawText(rcText, CDuiRect(0,0,0,0), sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
		SIZE cXY = {rcText.right - rcText.left, rcText.bottom - rcText.top};
		return cXY;
	}

	void CGroupBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		__super::SetAttribute(pstrName, pstrValue);
	}
}
