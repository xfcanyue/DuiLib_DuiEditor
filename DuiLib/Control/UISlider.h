#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CSliderUI : public CProgressUI
	{
		DECLARE_DUICONTROL(CSliderUI)
	public:
		CSliderUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		int GetChangeStep();
		void SetChangeStep(int step);
		void SetThumbSize(SIZE szXY);
		RECT GetThumbRect() const;
		LPCTSTR GetThumbImage() const;
		void SetThumbImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbHotImage() const;
		void SetThumbHotImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbPushedImage() const;
		void SetThumbPushedImage(LPCTSTR pStrImage);

		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual void PaintForeImage(UIRender *pRender) override;

		void SetValue(int nValue);
		void SetCanSendMove(bool bCanSend);
		bool GetCanSendMove() const;
	protected:
		SIZE m_szThumb;
		int m_nStep;

		CDuiString m_sThumbImage;
		CDuiString m_sThumbHotImage;
		CDuiString m_sThumbPushedImage;

		CDuiString m_sImageModify;
		bool	   m_bSendMove;
	};
}

#endif // __UISLIDER_H__