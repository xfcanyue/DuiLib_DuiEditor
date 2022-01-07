#ifndef __UITEXTSCROLLEXH__
#define __UITEXTSCROLLEXH__

#pragma once

namespace DuiLib
{
	class UILIB_API CRollTextExUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CRollTextExUI)
	public:
		CRollTextExUI(void);
		~CRollTextExUI(void);

	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	public:	
		virtual void DoInit();
		void OnTimer(UINT_PTR idEvent);
		virtual void PaintText(UIRender *pRender) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void SetText(LPCTSTR pstrText) override;

		void SetRollDirection(int nDirect);
		int GetRollDirection() const;

		void SetRollSpeed(int nMillisecond);
		int GetRollSpeed() const;

		void SetRollStep(int nStep);
		int GetRollStep() const;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	public:
		void BeginRoll();
		void EndRoll();

	private:
		UINT_PTR m_idEventTimer;
		int m_nStep;
		int m_nScrollPos;
		BOOL m_bUseRoll;
		int m_nRollDirection;   
		int m_nText_W_H;

		bool m_bPauseRoll;
		RECT m_rcClient;
		int m_nRollSpeed;
	};

}	// namespace DuiLib

#endif // __UITEXTSCROLLH__