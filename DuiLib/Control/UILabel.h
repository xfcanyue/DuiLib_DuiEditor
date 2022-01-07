#ifndef __UILABEL_H__
#define __UILABEL_H__

#pragma once

namespace DuiLib
{
	template< typename T = CControlUI>
	class TemplateLabelUI : public T
	//class TemplateLabelUI : public CContainerUI
	{
	public:
		TemplateLabelUI() {}
		virtual ~TemplateLabelUI() {}

		virtual UINT GetControlFlags() const override
		{
			return IsEnabled() ? UIFLAG_SETCURSOR : 0;
		}

		virtual SIZE EstimateSize(SIZE szAvailable) override
		{
			if (IsAutoCalcWidth() || IsAutoCalcHeight()) 
			{
				RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
				GetManager()->Render()->DrawText(rcText, m_rcTextPadding, GetText(), m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
				
				if(IsAutoCalcWidth())
					m_cxyFixed.cx = rcText.right - rcText.left;
				if(IsAutoCalcHeight())
					m_cxyFixed.cy = rcText.bottom - rcText.top;	

				return GetManager()->GetDPIObj()->Scale(m_cxyFixed);
			}

			return CControlUI::EstimateSize(szAvailable);
		}

		virtual void DoEvent(TEventUI& event) override
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

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override
		{
			__super::SetAttribute(pstrName, pstrValue);
		}

		virtual void PaintText(UIRender *pRender) override
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
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CLabelHLayoutUI : public TemplateLabelUI<CHorizontalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CLabelHLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CLabelVLayoutUI : public TemplateLabelUI<CVerticalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CLabelVLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CLabelLayoutUI : public TemplateLabelUI<CDynamicLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CLabelLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CLabelUI : public CControlUI
	{
		DECLARE_DUICONTROL(CLabelUI)
	public:
		CLabelUI();
		~CLabelUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual void PaintText(UIRender *pRender) override;
	};
}

#endif // __UILABEL_H__