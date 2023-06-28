#ifndef __UI_WINDOW_GTK_H__
#define __UI_WINDOW_GTK_H__


#pragma once

#ifdef DUILIB_GTK
namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//
class UILIB_API CWindowGtk : public CWindowBase
{
public:
	CWindowGtk();
	virtual ~CWindowGtk();

	virtual UIWND Create(UIWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc);
	virtual UIWND Create(UIWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = 0, int y = 0, int cx = 0, int cy = 0);
	UIWND CreateDuiWindow(UIWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle =0, DWORD dwExStyle =0);

	virtual LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) override;
	virtual LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) override;


	virtual void Close(UINT nRet = IDOK) override;
	virtual void SetCursor(int nCursor) override;
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	void CenterWindow();	// 居中，支持扩展屏幕

	virtual BOOL SetTimer(UINT uElapse, TIMERINFO *pTimer) override;
	virtual BOOL KillTimer(TIMERINFO *pTimer) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};	

} // namespace DuiLib
#endif //#ifdef DUILIB_GTK

#endif // __UIBASE_H__
