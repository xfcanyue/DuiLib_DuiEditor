#ifndef __UISCROLLBAR_H__
#define __UISCROLLBAR_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CScrollBarUI : public CControlUI
	{
		DECLARE_DUICONTROL(CScrollBarUI)
	public:
		CScrollBarUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		CContainerUI* GetOwner() const;
		void SetOwner(CContainerUI* pOwner);

		virtual void SetVisible(bool bVisible = true) override;
		virtual void SetEnabled(bool bEnable = true) override;
		virtual void SetFocus() override;

		bool IsHorizontal();
		void SetHorizontal(bool bHorizontal = true);
		int GetScrollRange() const;
		void SetScrollRange(int nRange);
		int GetScrollPos() const;
		void SetScrollPos(int nPos);
		int GetLineSize() const;
		void SetLineSize(int nSize);

		bool GetShowButton1();
		void SetShowButton1(bool bShow);
		LPCTSTR GetButton1NormalImage();
		void SetButton1NormalImage(LPCTSTR pStrImage);
		LPCTSTR GetButton1HotImage();
		void SetButton1HotImage(LPCTSTR pStrImage);
		LPCTSTR GetButton1PushedImage();
		void SetButton1PushedImage(LPCTSTR pStrImage);
		LPCTSTR GetButton1DisabledImage();
		void SetButton1DisabledImage(LPCTSTR pStrImage);

		bool GetShowButton2();
		void SetShowButton2(bool bShow);
		LPCTSTR GetButton2NormalImage();
		void SetButton2NormalImage(LPCTSTR pStrImage);
		LPCTSTR GetButton2HotImage();
		void SetButton2HotImage(LPCTSTR pStrImage);
		LPCTSTR GetButton2PushedImage();
		void SetButton2PushedImage(LPCTSTR pStrImage);
		LPCTSTR GetButton2DisabledImage();
		void SetButton2DisabledImage(LPCTSTR pStrImage);

		LPCTSTR GetThumbNormalImage();
		void SetThumbNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbHotImage();
		void SetThumbHotImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbPushedImage();
		void SetThumbPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetThumbDisabledImage();
		void SetThumbDisabledImage(LPCTSTR pStrImage);

		LPCTSTR GetRailNormalImage();
		void SetRailNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetRailHotImage();
		void SetRailHotImage(LPCTSTR pStrImage);
		LPCTSTR GetRailPushedImage();
		void SetRailPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetRailDisabledImage();
		void SetRailDisabledImage(LPCTSTR pStrImage);

		LPCTSTR GetBkNormalImage();
		void SetBkNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetBkHotImage();
		void SetBkHotImage(LPCTSTR pStrImage);
		LPCTSTR GetBkPushedImage();
		void SetBkPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetBkDisabledImage();
		void SetBkDisabledImage(LPCTSTR pStrImage);

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		void PaintBk(UIRender *pRender);
		void PaintButton1(UIRender *pRender);
		void PaintButton2(UIRender *pRender);
		void PaintThumb(UIRender *pRender);
		void PaintRail(UIRender *pRender);

	protected:

		enum
		{ 
			DEFAULT_SCROLLBAR_SIZE = 16,
			DEFAULT_TIMERID = 10,
		};

		bool m_bHorizontal;
		int m_nRange;
		int m_nScrollPos;
		int m_nLineSize;
		CContainerUI* m_pOwner;
		int m_nLastScrollPos;
		int m_nLastScrollOffset;
		int m_nScrollRepeatDelay;

		CDuiString m_sBkNormalImage;
		CDuiString m_sBkHotImage;
		CDuiString m_sBkPushedImage;
		CDuiString m_sBkDisabledImage;

		bool m_bShowButton1;
		RECT m_rcButton1;
		UINT m_uButton1State;
		CDuiString m_sButton1NormalImage;
		CDuiString m_sButton1HotImage;
		CDuiString m_sButton1PushedImage;
		CDuiString m_sButton1DisabledImage;

		bool m_bShowButton2;
		RECT m_rcButton2;
		UINT m_uButton2State;
		CDuiString m_sButton2NormalImage;
		CDuiString m_sButton2HotImage;
		CDuiString m_sButton2PushedImage;
		CDuiString m_sButton2DisabledImage;

		RECT m_rcThumb;
		UINT m_uThumbState;
		CDuiString m_sThumbNormalImage;
		CDuiString m_sThumbHotImage;
		CDuiString m_sThumbPushedImage;
		CDuiString m_sThumbDisabledImage;

		CDuiString m_sRailNormalImage;
		CDuiString m_sRailHotImage;
		CDuiString m_sRailPushedImage;
		CDuiString m_sRailDisabledImage;

		CDuiString m_sImageModify;
	};
}

#endif // __UISCROLLBAR_H__