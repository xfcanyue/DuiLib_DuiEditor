#pragma once
#include "KeyBoardUI.h"
class UILIB_PLUGIN_API CKeyBoardSimpleUI : public CKeyBoardUI
{
	DECLARE_DUICONTROL(CKeyBoardSimpleUI)
public:
	CKeyBoardSimpleUI(void);
	virtual ~CKeyBoardSimpleUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual int CalcKeyBoardWidth();
	virtual int CalcKeyBoardHeight();

	virtual void DoInit();
	virtual void InitKeyButton();
};

