#pragma once

namespace DuiLib
{

#define UIMSG_INSERT_MSG WM_USER + 123

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
	static void InsertMsg(LPCTSTR pstring, COLORREF cr);
};

}