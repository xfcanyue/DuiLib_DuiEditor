#ifndef __UIAccordionPane_H__
#define __UIAccordionPane_H__

#pragma once

namespace DuiLib
{
	class UILIB_PLUGIN_API CAccordionPaneUI : public CVerticalLayoutUI, public CUIAnimation
	{
		DECLARE_DUICONTROL(CAccordionPaneUI)
	public:
		CAccordionPaneUI();
		~CAccordionPaneUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	
		virtual void DoEvent(TEventUI& event);
		void OnTimer( int nTimerID );
		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID);

		virtual SIZE EstimateSize(SIZE szAvailable);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		CHorizontalLayoutUI *GetPaneHeader();
		CVerticalLayoutUI *GetPaneBody();

		void SetPaneVisible(bool bVisible);
		bool IsPaneVisible();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		CHorizontalLayoutUI *m_pHeader;
		CLabelUI *m_pHeaderIcon;
		CLabelUI *m_pHeaderLabel;
		COptionUI *m_pHeaderButton;

		CVerticalLayoutUI	*m_pBody;
		bool m_bPaneVisible;		
		RECT m_rcCurPos;
		RECT m_rcItemOld;
		SIZE m_szPaneSize;
		enum
		{
			TAB_ANIMATION_ID = 1,

			TAB_ANIMATION_ELLAPSE = 5,
			TAB_ANIMATION_FRAME_COUNT = 20,
		};
	};

} // namespace DuiLib

#endif // __UIAccordionPane_H__