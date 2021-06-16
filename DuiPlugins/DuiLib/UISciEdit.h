#pragma once
#define INCLUDE_DEPRECATED_FEATURES
#include "../../3rd/scintilla/SciApi.h"

namespace DuiLib
{

class UILIB_API CSciEditUI : public CContainerUI, public SciApi
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

	void SetViewLineNumber();
	void SetViewBreakPoint();
	void SetViewFolder();

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	CWindowWnd *m_pWindow;
	int m_nCodePage;
	bool m_bLineNumber;
	DWORD m_dwLineNumberColor;
	DWORD m_dwLineNumberBkColor;
	bool m_bBreakPoint;
	bool m_bFolder;
};


}