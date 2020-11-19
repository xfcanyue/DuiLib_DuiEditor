#pragma once

namespace DuiLib
{

class CGridCtrlUI;
class UILIB_PLUGIN_API CGridCtrlWnd : public CWindowWnd
{
public:
	CGridCtrlWnd(void);
	~CGridCtrlWnd(void);

	void Init(CGridCtrlUI* pOwner);
	void SetPos(RECT rc, bool bNeedInvalidate  = true);

	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);	

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	RECT m_rcWindow;
	CGridCtrlUI *m_pOwner;
	CGridCtrl *m_pGrid;
};

}