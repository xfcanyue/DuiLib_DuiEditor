#ifndef __UIAccordionPane_H__
#define __UIAccordionPane_H__

#pragma once

namespace DuiLib
{
	#define	ACCORDION_ANIMATION_ID	1

	class UILIB_PLUGIN_API CAccordionPaneUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CAccordionPaneUI)
		class CPaneCheckBox;
		class CAccordionBody;
	public:
		CAccordionPaneUI();
		~CAccordionPaneUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void DoInit();

		virtual bool Activate();
		virtual void DoEvent(TEventUI& event);

		virtual SIZE EstimateSize(SIZE szAvailable);

		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetCount() const;

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAll();

		CHorizontalLayoutUI *GetPaneHeader();
		CVerticalLayoutUI *GetPaneBody();

		void SetPaneVisible(bool bVisible, bool bAnimation = true);
		bool IsPaneVisible() const;
		virtual int GetFixedHeight() const;

		void SetFrameCount(int framecount);
		int GetFrameCount() const;
		void SetFrameDelay(int nDelay);
		int GetFrameDelay() const;

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		CHorizontalLayoutUI *m_pHeader;
		CLabelUI *m_pHeaderIcon;
		CLabelUI *m_pHeaderLabel;
		COptionUI *m_pHeaderCheckBox;
		CAnimationPaneUI	*m_pAnimPane;
		CVerticalLayoutUI	*m_pBodyVerti;
	};

} // namespace DuiLib

#endif // __UIAccordionPane_H__