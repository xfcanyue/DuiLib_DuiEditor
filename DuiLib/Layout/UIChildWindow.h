#pragma once

namespace DuiLib
{

class UILIB_API CChildWindowUI :public CContainerUI
{
	DECLARE_DUICONTROL(CChildWindowUI)
public:
	CChildWindowUI(void);
	~CChildWindowUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID	GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual void SetVisible(bool bVisible) override;
	virtual void SetInternVisible(bool bVisible = true) override;
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
	virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
	virtual void DoEvent(TEventUI& event) override;	
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	CWindowWnd *CreateWnd();
	void DestroyWnd();
	CWindowWnd *GetWnd();
	void RefreshWindow(); //强制刷新窗口，如视频断连之后

public:
	CWindowWnd *m_pWindow;
};


}