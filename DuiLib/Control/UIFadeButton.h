#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#include "UIAnimation.h"
#pragma once

namespace DuiLib {

	class UILIB_API CFadeButtonUI : public CButtonUI
	{
		DECLARE_DUICONTROL(CFadeButtonUI)
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual void SetNormalImage(LPCTSTR pStrImage) override;

		virtual void DoEvent(TEventUI& event) override;
		void OnTimer( int nTimerID );
		virtual void PaintStatusImage(UIRender *pRender) override;

		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) override {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID) override;
		virtual void OnAnimationStop(INT nAnimationID) override {}
		
	protected:
		CDuiString m_sOldImage;
		CDuiString m_sNewImage;
		CDuiString m_sLastImage;
		BYTE       m_bFadeAlpha;
		BOOL       m_bMouseHove;
		BOOL       m_bMouseLeave;
		enum{
			FADE_IN_ID			= 8,
			FADE_OUT_ID			= 9,
			FADE_ELLAPSE		= 10,
			FADE_FRAME_COUNT	= 30,
		};
	};

} // namespace DuiLib

#endif // __UIFADEBUTTON_H__