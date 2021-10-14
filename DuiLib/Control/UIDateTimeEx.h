#ifndef __UIDateTimeEx_H__
#define __UIDateTimeEx_H__

#pragma once

#define UIDTS_DATE		0
#define UIDTS_TIME		1
#define UIDTS_DATETIME	2


namespace DuiLib
{
	//class CDateTimeExWnd;

	/// 时间选择控件
	class UILIB_API CDateTimeExUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CDateTimeExUI)
	public:
		CDateTimeExUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual void DoInit();

		virtual bool OnEventLabelDate(void* param);
		virtual bool OnEventLabelTime(void* param);

		SYSTEMTIME& GetTime();
		void SetTime(SYSTEMTIME* pst);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void SetFormatStyle(UINT uStyle);	//UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99
		UINT GetFormatStyle();				//UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99

		virtual void SetText(LPCTSTR pstrText);
		virtual CDuiString GetText() const;

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void DoEvent(TEventUI& event);

		void SetFont(int index);
		int GetFont() const;

		CLabelUI *GetDateLabel() const;
		CLabelUI *GetTimeLabel() const;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	protected:
		SYSTEMTIME m_sysTime;
		int		m_iFont;
		bool       m_bReadOnly;
		int        m_uFormatStyle; //UIDTS_DATE=date, UIDTS_TIME=time, UIDTS_DateTimeEx=DateTimeEx  add by liqs99
		
		CLabelUI *m_pLabelDate;
		CLabelUI *m_pLabelTime;
// 		CDateTimeExWnd* m_pWindowDate;
// 		CDateTimeExWnd* m_pWindowTime;
	public:
		CWindowWnd* m_pWindowDate;
		CWindowWnd* m_pWindowTime;
	};
}
#endif // __UIDateTimeEx_H__