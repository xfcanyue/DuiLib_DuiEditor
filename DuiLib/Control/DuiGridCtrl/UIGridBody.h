#pragma once

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridBodyUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridBodyUI)
public:
	CGridBodyUI(void);
	virtual ~CGridBodyUI(void);

	void SetOwner(CControlUI *pOwnder);
	CControlUI *GetOwner() const;

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	int GetScrollStepSize() const;
	void SetScrollPos(SIZE szPos, bool bMsg = true);
	void SetPos(RECT rc, bool bNeedInvalidate = true);
	void DoEvent(TEventUI& event);

private:
	CControlUI *m_pOwner;
};


}
