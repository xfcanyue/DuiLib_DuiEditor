#pragma once


class UILIB_PLUGIN_API COptionExUI : public CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CMsgWndUI)
public:
	COptionExUI(void);
	virtual ~COptionExUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	virtual void DoInit();
	void DoEvent(TEventUI& event);

	bool Activate();
	void PaintBkColor(HDC hDC);

public:
	void SetHotBkColor(DWORD dwColor);
	DWORD GetHotBkColor() const;
	void SetSelectedBkColor(DWORD dwBkColor);
	DWORD GetSelectBkColor();

	LPCTSTR GetGroup() const;
	void SetGroup(LPCTSTR pStrGroupName = NULL);
	bool IsSelected() const;
	virtual void Selected(bool bSelected);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
private:
	bool			m_bSelected;
	CDuiString		m_sGroupName;

	DWORD m_dwHotBkColor;
	DWORD m_dwSelectedBkColor;

	CControlUI *m_pLeftIcon;
	CLabelUI *m_pCenterLabel;
	COptionUI *m_pRightIcon;
};

