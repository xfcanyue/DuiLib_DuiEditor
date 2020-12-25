#pragma once
#define INCLUDE_DEPRECATED_FEATURES
#include "../3rd/scintilla/SciApi.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CSciEditUI : public CContainerUI, public SciApi
{
	DECLARE_DUICONTROL(CSciEditUI)
public:
	CSciEditUI(void);
	virtual ~CSciEditUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoInit();

	void SetPos(RECT rc, bool bNeedInvalidate  = true);
	void SetInternVisible(bool bVisible);

	virtual void OnSciNotify(SCNotification *pMsg);

	virtual CDuiString GetText() const;
	virtual void SetText(LPCTSTR pstrText);

private:
	CWindowWnd *m_pWindow;
};


}