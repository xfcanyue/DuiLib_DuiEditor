#pragma once


namespace DuiLib {

class CMenuElementExUI : public CMenuElementUI
{
public:
	CMenuElementExUI(void);
	virtual ~CMenuElementExUI(void);

	void DoEvent(TEventUI& event);

	void CreateMenuWnd();
};

}
