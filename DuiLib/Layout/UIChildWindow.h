#pragma once

namespace DuiLib
{

class UILIB_API CChildWindowUI :public CContainerUI
{
	DECLARE_DUICONTROL(CChildWindowUI)
public:
	CChildWindowUI(void);
	~CChildWindowUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual void SetVisible(bool bVisible);
	virtual void SetInternVisible(bool bVisible = true);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);
	virtual void DoEvent(TEventUI& event);	
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	CWindowWnd *CreateWnd();
	void DestroyWnd();
	CWindowWnd *GetWnd();
	void RefreshWindow(); //强制刷新窗口，如视频断连之后

	HDC GetWndDC() const;
public:
	CWindowWnd *m_pWindow;
};


}