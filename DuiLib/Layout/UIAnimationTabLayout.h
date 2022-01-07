#ifndef __UIANIMATIONTABLAYOUT_H__
#define __UIANIMATIONTABLAYOUT_H__

namespace DuiLib
{
	class UILIB_API CAnimationTabLayoutUI : public CTabLayoutUI
	{
		DECLARE_DUICONTROL(CAnimationTabLayoutUI)
	public:
		CAnimationTabLayoutUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual bool SelectItem( int iIndex ) override;
		void AnimationSwitch();
		virtual void DoEvent(TEventUI& event) override;
		void OnTimer( int nTimerID );

		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) override {}
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) override;
		virtual void OnAnimationStop(int nAnimationID) override;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	protected:
		int m_nPositiveDirection;
		RECT m_rcCurPos;
		RECT m_rcItemOld;
		CControlUI* m_pCurrentControl;
		bool m_bControlVisibleFlag;
	};
}
#endif // __UIANIMATIONTABLAYOUT_H__