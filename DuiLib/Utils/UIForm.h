#pragma once

namespace DuiLib {

class CUIFrameWnd;

//真正的虚拟窗口
class UILIB_API CUIForm : public CUIFrmBase
{
	friend class CUIFrameWnd;
public:
	CUIForm(void);
	virtual ~CUIForm(void);
	virtual CDuiString GetFormName() { return _T(""); }

	virtual void InitWindow(){};
	virtual CControlUI *CreateControl(LPCTSTR pstrClass) { return NULL; }

	void SetFrameWnd(CUIFrameWnd *pFrame) { m_pFrame = pFrame; }
	CUIFrameWnd *GetFrameWnd() const { return m_pFrame; }

	virtual CPaintManagerUI *GetManager() { return GetFrameWnd()->GetManager(); }
	virtual CControlUI *GetRoot() { ASSERT(m_pRoot); return m_pRoot; }

	virtual void OnUpdateView() {} //弃用，为了兼容保留

	BOOL IsForm(LPCTSTR formName) { return GetFormName() == formName; }

	virtual void OnActiveForm() {}	//页面由隐藏切换到显示时
	virtual void OnHideForm() {}	//页面由显示切换到隐藏时

protected:
	void __InitWindow();
protected:
	CPaintManagerUI *m_pManager;
	CControlUI *m_pRoot;
	CUIFrameWnd *m_pFrame;
};

} //namespace DuiLib {