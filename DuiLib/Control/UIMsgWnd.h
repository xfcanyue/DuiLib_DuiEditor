#pragma once

namespace DuiLib
{


class UILIB_API CMsgWndUI : public CRichEditUI
{
	DECLARE_DUICONTROL(CMsgWndUI)
	struct TMsgWnd
	{
		CDuiString sText;
		DWORD dwColor;
	};
public:
	CMsgWndUI(void);
	virtual ~CMsgWndUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual void DoEvent(TEventUI& event) override; 

	bool OnInsertMsg(void* param);

	void ShowDateTime(bool bShow);
	bool IsShowDateTime() const;

	void ShowDate(bool bShow);
	bool IsShowDate() const;

	void ShowTime(bool bShow);
	bool IsShowTime() const;
	

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
protected:
	bool m_bWithDate; //在每条日志前是否插入日期
	bool m_bWithTime; //在每条日志前是否插入时间
public:
	static CMsgWndUI *m_pStaticWindow;
	static void InsertMsg(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));
	static void InsertMsgV(LPCTSTR lpszFormat, ...);
	static void ClearMsg();
};

}