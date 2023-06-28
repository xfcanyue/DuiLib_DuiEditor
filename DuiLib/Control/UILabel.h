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
			return T::IsEnabled() ? UIFLAG_SETCURSOR : 0;
		}

		virtual SIZE EstimateSize(SIZE szAvailable) override
		{
			if (T::IsAutoCalcWidth() || T::IsAutoCalcHeight())
			{			
				SIZE szFact = {szAvailable.cx, szAvailable.cy};

				if(T::IsAutoCalcWidth())
				{
					RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
					RECT rcTextPadding = T::GetTextPadding();
					T::GetManager()->Render()->DrawText(rcText, rcTextPadding, T::GetText(), T::GetTextColor(), T::GetFont(), DT_CALCRECT | T::GetTextStyle());

					szFact.cx = rcText.right - rcText.left;
				}

				if(T::IsAutoCalcHeight())
				{
					UIFont *pFont = T::GetManager()->GetFont(T::GetFont());
					int h = pFont->GetHeight(T::GetManager());
					szFact.cy = T::m_rcTextPadding.top + T::m_rcTextPadding.bottom + h + 4;
				}

				return T::GetManager()->GetDPIObj()->ScaleSize(szFact);
			}

			return CControlUI::EstimateSize(szAvailable);
		}

		virtual void DoEvent(TEventUI& event) override
		{
			if( event.Type == UIEVENT_SETFOCUS ) 
			{
				T::SetFocusState(true);
				return;
			}
			if( event.Type == UIEVENT_KILLFOCUS ) 
			{
				T::SetFocusState(false);
				return;
			}
			CControlUI::DoEvent(event);
		}

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override
		{
			T::SetAttribute(pstrName, pstrValue);
		}

		virtual void PaintText(UIRender *pRender) override
		{
			CDuiString sText = T::GetText();
			if(sText.IsEmpty()) return;

			RECT rcText = T::GetPos();
			DWORD dwColor = 0;
			int iFont = -1;

			//////////////////////////////////////////////////////////////////////////
			if( !T::IsEnabled() )
				iFont = T::GetDisabledFont();

			else if(T::IsSelectedState() )
				iFont = T::GetSelectedFont();

			else if(T::IsPushedState() )
				iFont = T::GetPushedFont();

			else if(T::IsHotState() )
				iFont = T::GetHotFont();

			else if(T::IsFocused() )
				iFont = T::GetFocusedFont();

			if(iFont == -1)
				iFont = T::GetFont();

			//////////////////////////////////////////////////////////////////////////
			if( !T::IsEnabled() )
				dwColor = T::GetDisabledTextColor();

			else if(T::IsSelectedState() )
				dwColor = T::GetSelectedTextColor();

			else if(T::IsPushedState() )
				dwColor = T::GetPushedTextColor();

			else if(T::IsHotState() )
				dwColor = T::GetHotTextColor();

			else if(T::IsFocused() )
				dwColor = T::GetFocusedTextColor();

			if(dwColor == 0)
				dwColor = T::GetTextColor();

			if(dwColor == 0 && T::GetManager())
				dwColor = T::GetManager()->GetDefaultFontColor();

			pRender->DrawText(rcText, T::GetTextPadding(), sText, dwColor, iFont, T::GetTextStyle());
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