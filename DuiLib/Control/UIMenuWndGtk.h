#ifndef __UIMENU_WND_GTK_H__
#define __UIMENU_WND_GTK_H__

#pragma once

#ifdef DUILIB_GTK
namespace DuiLib {
//////////////////////////////////////////////////////////////////////////////////////////////
//
//
class UILIB_API CMenuWndGtk : public CWindowWnd, public MenuReceiverImpl, public INotifyUI, public IDialogBuilderCallback
{
public:
	static MenuObserverImpl& GetGlobalContextMenuObserver()
	{
		static MenuObserverImpl s_context_menu_observer;
		return s_context_menu_observer;
	}
	static CMenuWndGtk* CreateMenu(CMenuElementUI* pOwner, STRINGorID xml, POINT point,
		CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo = NULL,
		DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top);
	static void DestroyMenu();
	static MenuItemInfo* SetMenuItemInfo(LPCTSTR pstrName, bool bChecked);

public:
	CMenuWndGtk();
	~CMenuWndGtk();
	void Close(UINT nRet = IDOK);
	bool isClosing;
	/*
	 *	@pOwner 一级菜单不要指定这个参数，这是菜单内部使用的
	 *	@xml	菜单的布局文件
	 *	@point	菜单的左上角坐标
	 *	@pMainPaintManager	菜单的父窗体管理器指针
	 *	@pMenuCheckInfo	保存菜单的单选和复选信息结构指针
	 *	@dwAlignment		菜单的出现位置，默认出现在鼠标的右下侧。
	 */

    void Init(CMenuElementUI* pOwner, STRINGorID xml, POINT point,
		CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo = NULL,
		DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top);
    LPCTSTR GetWindowClassName() const;
    void OnFinalMessage(UIWND hWnd);
	void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClassName);

	//modify by liqs99, 下面4个函数改为虚函数
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL Receive(ContextMenuParam param);

	// 获取根菜单控件，用于动态添加子菜单
	CMenuUI* GetMenuUI();

	// 重新调整菜单的大小
	void ResizeMenu();

	// 重新调整子菜单的大小
	void ResizeSubMenu();
	void setDPI(int DPI);

public:

	POINT			m_BasedPoint;
	STRINGorID		m_xml;
    DuiLibPaintManagerUI m_pm;
    CMenuElementUI* m_pOwner;
    CMenuUI*	m_pLayout;
	DWORD		m_dwAlignment;	//菜单对齐方式
	bool		m_bAutoDestroy;
};

} // namespace DuiLib

#endif // #ifdef DUILIB_GTK
#endif // __UIMENU_WND_GTK_H__

