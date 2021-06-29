#ifndef __UITABCTRL_H__
#define __UITABCTRL_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTabCtrlUI : public COptionUI
	{
		DECLARE_DUICONTROL(CTabCtrlUI)
	public:
		CTabCtrlUI();
		~CTabCtrlUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Activate();
		void DoEvent(TEventUI& event);

		virtual bool IsSelected() const;	
		virtual void BindTriggerTabSel(int _SetSelectIndex = -1);	

		void SetActiveMouseOn(bool bActive); // Û±Í–¸Õ£ ±«–ªª“≥√Ê
		bool IsActiveMouseOn();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		bool		m_bActiveMouseOn;
	};

} // namespace DuiLib

#endif // __UITABCTRL_H__