#ifndef __UIGRIDCTRL_H__
#define __UIGRIDCTRL_H__

//add by liqs99 ±Ì∏Ò¿‡
#pragma once

namespace DuiLib {

	class UILIB_API CGridCtrlUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CGridCtrlUI)

	public:
		CGridCtrlUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		virtual void DoInit();
		virtual void DoEvent(TEventUI& event);

		virtual void SetVisible(bool bVisible);
		virtual void SetInternVisible(bool bVisible = true);
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);
	public:
	public:
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	public:
		CWindowWnd *m_pWindow;
	};
	
} // namespace DuiLib

#endif // __UIGRIDCTRL_H__
