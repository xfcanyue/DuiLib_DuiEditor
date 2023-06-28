#ifndef __UIBASE_H__
#define __UIBASE_H__


#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

#ifdef DUILIB_WIN32

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

#else

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (0)
#define UI_WNDSTYLE_CHILD      (0)
#define UI_WNDSTYLE_DIALOG     (0)

#define UI_WNDSTYLE_EX_FRAME   (0)
#define UI_WNDSTYLE_EX_DIALOG  (0)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (0)
#define UI_CLASSSTYLE_CHILD      (0)
#define UI_CLASSSTYLE_DIALOG     (0)

#endif


	/////////////////////////////////////////////////////////////////////////////////////
	//
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef DUITRACE
#define DUITRACE DUI__Trace
#endif
#define DUITRACEMSG DUI__TraceMsg
#else
#ifndef DUITRACE
#define DUITRACE
#endif
#define DUITRACEMSG _T("")
#endif

	void UILIB_API DUI__Trace(LPCTSTR pstrFormat, ...);
	LPCTSTR UILIB_API DUI__TraceMsg(UINT uMsg);

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CNotifyPump
	{
	public:
		bool AddVirtualWnd(CDuiString strName,CNotifyPump* pObject);
		bool RemoveVirtualWnd(CDuiString strName);
		void NotifyPump(TNotifyUI& msg);
		bool LoopDispatch(TNotifyUI& msg);
		DUI_DECLARE_MESSAGE_MAP()
	private:
		CStdStringPtrMap m_VirtualWndMap;
	};

	class UILIB_API CWindowBase
	{
	public:
		CWindowBase();

		UIWND GetHWND() const;
		operator UIWND() const;

		virtual LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) { return 0; }
		virtual LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L) { return 0; }

		virtual void Close(UINT nRet = IDOK) = 0;

		virtual void SetCursor(int nCursor) = 0;
		virtual BOOL SetWindowPos(UIWND hWndInsertAfter,int x, int y, int cx, int cy, UINT uFlags) { return FALSE; }

		virtual BOOL SetTimer(UINT uElapse, TIMERINFO *pTimer) = 0;
		virtual BOOL KillTimer(TIMERINFO *pTimer) = 0;
	protected:
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return 0;}
		virtual void OnFinalMessage(UIWND hWnd) {}

	public:
		UIWND m_hWnd;
	};

} // namespace DuiLib

#endif // __UIBASE_H__
