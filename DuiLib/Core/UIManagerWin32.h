#ifndef __UIMANAGER_WIN32_H__
#define __UIMANAGER_WIN32_H__
#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API CPaintManagerWin32UI : public CPaintManagerUI, public CIDropTarget
	{
	public:
		CPaintManagerWin32UI();
		virtual ~CPaintManagerWin32UI();

		virtual void Init(HWND hWnd, LPCTSTR pstrName = NULL, CWindowWnd *pWindow=NULL) override;
		virtual UIRender *Render() override;

		virtual BOOL InvalidateRect(UIWND hWnd, const RECT *lpRect, BOOL bErase) override;
		virtual HDC GetPaintDC() const override;

		virtual void SetLayered(bool bLayered) override;

		virtual void SetCursor(int nCursor) override;

		bool InitDragDrop(); // 初始化拖拽
		virtual bool OnDropOver(DWORD grfKeyState, LPDWORD pdwEffect) override;
		virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect) override;

		virtual void SetCapture() override;
		virtual void ReleaseCapture() override;
		virtual bool IsCaptured() override;

		virtual bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnApp1(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnClose(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnPaint(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnEraseBkgnd(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnPrintClient(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnGetMaxMinInfo(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnSize(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnTimer(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnLButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnLButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnLButtonDbClick(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnRButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnRButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnRButtonDbClick(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMouseOver(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMouseLeave(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMouseMove(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnMouseWheel(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnContextMenu(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnChar(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnKeyDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnKeyUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnSetCursor(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnSetFocus(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnKillFocus(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnNotify(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnCommand(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnCtlColorEdit(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;
		virtual bool OnCtlColorStatic(WPARAM wParam, LPARAM lParam, LRESULT& lRes) override;

	public:
		//创建控件的图标，比如拖拽时的图片
		virtual UIBitmap* CreateControlBitmap(CControlUI* pControl, DWORD dwFilterColor = 0, CControlUI* pStopControl = NULL) override;

		//构造内部窗口的背景画刷
		virtual UIBitmap* CreateControlBackBitmap(CControlUI* pControl, const RECT &rcWnd, DWORD dwFilterColor) override;

	protected:
		HDC m_hDcPaint;
		TOOLINFO m_ToolTip;
	};


	//先锁定窗口更新，然后强制刷新整个窗口
	class UILIB_API CLockWindowUpdateUI
	{
	public:
		CLockWindowUpdateUI(CPaintManagerUI* pManager);
		~CLockWindowUpdateUI();
	private:
		CPaintManagerUI* m_pManager;
	};
} // namespace DuiLib
#endif //#ifdef DUILIB_WIN32

#endif // __UIMANAGER_H__
