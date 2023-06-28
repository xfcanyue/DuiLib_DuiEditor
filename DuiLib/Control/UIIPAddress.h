#ifndef __UIIPADDRESS_H__
#define __UIIPADDRESS_H__

#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib
{
	//class CIPAddressWnd;

	/// IP控件
	class UILIB_API CIPAddressUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CIPAddressUI)

		friend class CIPAddressWnd;
	public:
		CIPAddressUI();
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoInit() override;
		DWORD GetIP();
		void SetIP(DWORD dwIP);

		BOOL IsStaticWindow() const;
		void SetStaticWindow(BOOL bStaticWindow);

		virtual void SetText(LPCTSTR pstrText) override;
		virtual CDuiString GetText() const override;

		virtual int  GetTextN() override;
		virtual void SetTextN(int n) override;

		void UpdateText();

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual void SetVisible(bool bVisible = true) override;
		virtual void SetInternVisible(bool bVisible = true) override;
		virtual void DoEvent(TEventUI& event) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	protected:
		DWORD	m_dwIP;
		int		m_nIPUpdateFlag;

		CIPAddressWnd* m_pWindow;
		BOOL m_bStaicWindow;		//失去焦点时，不要隐藏内部窗口
	};
}
#endif //#ifdef DUILIB_WIN32
#endif // __UIIPADDRESS_H__
