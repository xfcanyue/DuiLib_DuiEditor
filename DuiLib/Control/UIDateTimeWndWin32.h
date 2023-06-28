#ifndef __UIDATETIME_WND_WIN32_H__
#define __UIDATETIME_WND_WIN32_H__

#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib
{
	class CDateTimeWndWin32 : public CWindowWnd
	{
	public:
		CDateTimeWndWin32();

		void Init(CDateTimeUI* pOwner);
		RECT CalPos();


		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		CDateTimeUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDropOpen;
		SYSTEMTIME m_oldSysTime;
		CStdRefPtr<UIFont> m_font;
	};
}
#endif // #ifdef DUILIB_WIN32
#endif // __UIDATETIME_H__
