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

		void BindTabIndex(int _BindTabIndex);
		void BindTabLayoutName(LPCTSTR _TabLayoutName);
		void BindTriggerTabSel(int _SetSelectIndex = -1);
		void RemoveBindTabIndex();
		int	 GetBindTabLayoutIndex();
		LPCTSTR GetBindTabLayoutName();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		int			m_iBindTabIndex;
		CDuiString	m_sBindTabLayoutName;
	};

} // namespace DuiLib

#endif // __UITABCTRL_H__