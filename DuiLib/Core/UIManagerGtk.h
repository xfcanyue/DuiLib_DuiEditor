#ifndef __UIMANAGER_GTK_H__
#define __UIMANAGER_GTK_H__
#pragma once

#ifdef DUILIB_GTK
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CPaintManagerGtkUI : public CPaintManagerUI
	{
	public:
		CPaintManagerGtkUI();
		virtual ~CPaintManagerGtkUI();

		virtual void Init(UIWND hWnd, LPCTSTR pstrName = NULL, CWindowWnd *pWindow=NULL) override;
		virtual UIRender *Render() override;

		virtual BOOL InvalidateRect(UIWND hWnd, const RECT *lpRect, BOOL bErase) override;
		virtual void SetCursor(int nCursor) override;

		virtual void SetCapture() override;
		virtual void ReleaseCapture() override;
		virtual bool IsCaptured() override;

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
	};
} // namespace DuiLib

#endif //#ifdef DUILIB_GTK
#endif // __UIMANAGER_GTK_H__
