#include "StdAfx.h"
#include "UILabel.h"

#include <atlconv.h>
namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CLabelHLayoutUI)

	LPCTSTR CLabelHLayoutUI::GetClass() const
	{
		return _T("LabelHLayoutUI");
	}

	LPVOID CLabelHLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_LABEL_HLAYOUT) == 0 ) return static_cast<CLabelHLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CLabelVLayoutUI)

	LPCTSTR CLabelVLayoutUI::GetClass() const
	{
		return _T("LabelVLayoutUI");
	}

	LPVOID CLabelVLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_LABEL_VLAYOUT) == 0 ) return static_cast<CLabelVLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CLabelLayoutUI)

	LPCTSTR CLabelLayoutUI::GetClass() const
	{
		return _T("LabelLayoutUI");
	}

	LPVOID CLabelLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_LABEL_LAYOUT) == 0 ) return static_cast<CLabelLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_DUICONTROL(CLabelUI)

	CLabelUI::CLabelUI()
	{
		m_uTextStyle = DT_VCENTER | DT_SINGLELINE;
	}

	CLabelUI::~CLabelUI()
	{
	}

	LPCTSTR CLabelUI::GetClass() const
	{
		return _T("LabelUI");
	}

	LPVOID CLabelUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_LABEL) == 0 ) return static_cast<CLabelUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}
	
	UINT CLabelUI::GetControlFlags() const
	{
		return IsEnabled() ? UIFLAG_SETCURSOR : 0;
	}

	SIZE CLabelUI::EstimateSize(SIZE szAvailable)
	{
		if (IsAutoCalcWidth() || IsAutoCalcHeight()) 
		{
			RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
			GetManager()->Render()->DrawText(rcText, m_rcTextPadding, GetText(), m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);

			if(IsAutoCalcWidth())
				m_cxyFixed.cx = rcText.right - rcText.left;
		
			if(IsAutoCalcHeight())
			{
				m_cxyFixed.cy = rcText.bottom - rcText.top;		
			}
			
			if(m_pManager)
				return GetManager()->GetDPIObj()->Scale(m_cxyFixed);
			return m_cxyFixed;
		}

		//注释掉了，不需要自动计算高度，modify by liqs99
		//if( m_cxyFixed.cy == 0 ) return CDuiSize(GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx), m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 4);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CLabelUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			SetFocusState(true);
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			SetFocusState(false);
			return;
		}
		CControlUI::DoEvent(event);
	}

	void CLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		__super::SetAttribute(pstrName, pstrValue);
	}

	void CLabelUI::PaintText(UIRender *pRender)
	{
		CDuiString sText = GetText();
		if(sText.IsEmpty()) return;

		RECT rcText = m_rcItem;
		DWORD dwColor = 0;
		int iFont = -1;

		//////////////////////////////////////////////////////////////////////////
		if( !IsEnabled() )
			iFont = GetDisabledFont();

		else if( IsSelectedState() )
			iFont = GetSelectedFont();

		else if( IsPushedState() )
			iFont = GetPushedFont();

		else if( IsHotState() )
			iFont = GetHotFont();

		else if( IsFocused() )
			iFont = GetFocusedFont();

		if(iFont == -1)
			iFont = GetFont();

		//////////////////////////////////////////////////////////////////////////
		if( !IsEnabled() )
			dwColor = GetDisabledTextColor();

		else if( IsSelectedState() )
			dwColor = GetSelectedTextColor();

		else if( IsPushedState() )
			dwColor = GetPushedTextColor();

		else if( IsHotState() )
			dwColor = GetHotTextColor();

		else if( IsFocused() )
			dwColor = GetFocusedTextColor();

		if(dwColor == 0)
			dwColor = GetTextColor();

		if(dwColor == 0 && m_pManager)
			dwColor = m_pManager->GetDefaultFontColor();

		pRender->DrawText(rcText, GetTextPadding(), sText, dwColor, iFont, GetTextStyle());
		return;
	}
}