#ifndef __UICOMBO_WND_WIN32_H__
#define __UICOMBO_WND_WIN32_H__

#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CComboWndWin32 : public CWindowWnd, public INotifyUI
	{
	public:
		void Init(CComboUI* pOwner);
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(UIWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Notify(TNotifyUI& msg) override;

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

#if(_WIN32_WINNT >= 0x0501)
		virtual UINT GetClassStyle() const;
#endif
		bool IsHitItem(POINT ptMouse);
	public:
		DuiLibPaintManagerUI m_pm;
		CComboUI* m_pOwner;
		CVerticalLayoutUI* m_pLayout;
		int m_iOldSel;
		bool m_bHitItem;
	};

} // namespace DuiLib
#endif // #ifdef DUILIB_WIN32
#endif // __UICOMBO_H__

