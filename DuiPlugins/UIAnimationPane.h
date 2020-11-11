#ifndef __UIAnimationPane_H__
#define __UIAnimationPane_H__

#pragma once

namespace DuiLib
{
	#define PANE_ANIMATION_ID  2
	class UILIB_PLUGIN_API CAnimationPaneUI : public CContainerUI, public CUIAnimation
	{
		DECLARE_DUICONTROL(CAnimationPaneUI)
	public:
		CAnimationPaneUI();
		~CAnimationPaneUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void DoInit();

		virtual void DoEvent(TEventUI& event);
		void OnTimer( int nTimerID );
		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID);

		virtual SIZE EstimateSize(SIZE szAvailable);

		void SetPaneVisible(bool bVisible, bool bAnimation = true);
		bool IsPaneVisible() const;
		virtual bool IsVisible() const;
		virtual void SetVisible(bool bVisible = true);
		virtual int GetFixedHeight() const;
		virtual int GetFixedWidth() const;

		void SetFrameCount(int framecount);
		int GetFrameCount() const;
		void SetFrameDelay(int nDelay);
		int GetFrameDelay() const;

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		int m_animation; //动画方向 0 = 无动画， 0 = 上下， 1=左右
		bool m_bPaneVisible;	

		RECT m_rcItemOld;

		int m_nFrameCount;
		int m_nFrameDelay;
	};

} // namespace DuiLib

#endif // __UIAnimationPane_H__