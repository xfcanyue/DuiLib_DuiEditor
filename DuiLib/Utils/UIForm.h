#pragma once

//namespace DuiLib{
using namespace DuiLib;

class CUIFrameWnd;

//真正的虚拟窗口
class UILIB_API CUIForm : public CUIFrmBase
{

public:
	CUIForm(void);
	virtual ~CUIForm(void);
	virtual CDuiString GetFormName() { return _T(""); }

	virtual void InitWindow(){};
	virtual CControlUI *CreateControl(LPCTSTR pstrClass) { return NULL; }

	void SetFrameWnd(CUIFrameWnd *pFrame) { m_pFrame = pFrame; }
	CUIFrameWnd *GetFrameWnd() const { return m_pFrame; }

	CPaintManagerUI *GetManager() { return GetFrameWnd()->GetManager(); }

	virtual void OnUpdateView() {} //弃用，为了兼容保留

	BOOL IsForm(LPCTSTR formName) { return GetFormName() == formName; }

	virtual void OnActiveForm() {}	//页面由隐藏切换到显示时
	virtual void OnHideForm() {}	//页面由显示切换到隐藏时
protected:
	CPaintManagerUI *m_pManager;
	CUIFrameWnd *m_pFrame;
};

//}