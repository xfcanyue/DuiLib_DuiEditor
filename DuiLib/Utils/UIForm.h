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

	virtual void InitWindow(){};
	virtual CControlUI *CreateControl(LPCTSTR pstrClass) { return NULL; }

	void SetFrameWnd(CUIFrameWnd *pFrame) { m_pFrame = pFrame; }
	CUIFrameWnd *GetFrameWnd() const { return m_pFrame; }

	CPaintManagerUI *GetManager() { return GetFrameWnd()->GetManager(); }

	virtual void OnUpdateView() {}

protected:
	CPaintManagerUI *m_pManager;
	CUIFrameWnd *m_pFrame;
};

//}