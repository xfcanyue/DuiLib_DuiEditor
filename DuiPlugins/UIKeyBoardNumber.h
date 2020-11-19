#pragma once
#include "UIKeyBoard.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CKeyBoardNumberUI : public CKeyBoardUI
{
	DECLARE_DUICONTROL(CKeyBoardNumberUI)
public:
	CKeyBoardNumberUI(void);
	virtual ~CKeyBoardNumberUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual int CalcKeyBoardWidth();
	virtual int CalcKeyBoardHeight();

	virtual void DoInit();
	virtual void InitKeyButton();

	virtual CDuiString GetKeyText(BYTE vkCode);
};

}