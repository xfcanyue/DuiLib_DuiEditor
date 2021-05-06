#pragma once

namespace DuiLib
{


class UILIB_API CMsgWndUI : public CRichEditUI
{
	DECLARE_DUICONTROL(CMsgWndUI)
public:
	CMsgWndUI(void);
	virtual ~CMsgWndUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual void DoEvent(TEventUI& event); 

	bool OnInsertMsg(void* param);
public:
	static CMsgWndUI *m_pStaticWindow;
	static void InsertMsg(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));
	static void InsertMsgV(LPCTSTR lpszFormat, ...);
};

}