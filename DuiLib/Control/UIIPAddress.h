#ifndef __UIIPADDRESS_H__
#define __UIIPADDRESS_H__

#pragma once

namespace DuiLib
{
	class CIPAddressWnd;

	/// IP¿Ø¼þ
	class UILIB_API CIPAddressUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CIPAddressUI)

		friend class CIPAddressWnd;
	public:
		CIPAddressUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		DWORD GetIP();
		void SetIP(DWORD dwIP);

		virtual void SetText(LPCTSTR pstrText);
		virtual CDuiString GetText() const;

		virtual int  GetTextN();
		virtual void SetTextN(int n);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void DoEvent(TEventUI& event);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		DWORD	m_dwIP;
		bool       m_bReadOnly;
		int		m_nIPUpdateFlag;

		CIPAddressWnd* m_pWindow;
	};
}
#endif // __UIIPADDRESS_H__