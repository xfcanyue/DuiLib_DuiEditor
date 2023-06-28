#ifndef __UICOMBO_GTK_H__
#define __UICOMBO_GTK_H__

#pragma once

#ifdef DUILIB_GTK
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class CComboWndGtk : public CWindowWnd, public INotifyUI
	{
	public:
		void Init(CComboUI* pOwner);
		void Notify(TNotifyUI& msg) override;
	};

} // namespace DuiLib
#endif //#ifdef DUILIB_GTK

#endif // __UICOMBO_GTK_H__
