#pragma once
#define INCLUDE_DEPRECATED_FEATURES
#include "../3rd/scintilla/include/SciLexer.h"
#include "../3rd/scintilla/include/Scintilla.h"
#define DLL_SCIWND_
#include "../3rd/scintilla/SciWnd.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CSciEditUI : public CContainerUI
{
	DECLARE_DUICONTROL(CSciEditUI)
public:
	CSciEditUI(void);
	virtual ~CSciEditUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	CSciWnd *GetSciWnd() const;

	virtual void DoInit();

	void SetPos(RECT rc, bool bNeedInvalidate  = true);
	void SetInternVisible(bool bVisible);

	virtual CDuiString GetText() const;
	virtual void SetText(LPCTSTR pstrText);

private:
	CWindowWnd *m_pWindow;
};


}