#pragma once

namespace DuiLib
{

class UILIB_API CIconButtonUI : public CButtonLayoutUI
{
	DECLARE_DUICONTROL(CIconButtonUI)
public:
	CIconButtonUI(void);
	virtual ~CIconButtonUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	SIZE EstimateSize(SIZE szAvailable);

	void PaintText(HDC hDC);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	LPCTSTR GetIconNormalImage() const;
	void SetIconNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetIconHotImage() const;
	void SetIconHotImage(LPCTSTR pStrImage);
	LPCTSTR GetIconPushedImage() const;
	void SetIconPushedImage(LPCTSTR pStrImage);
	LPCTSTR GetIconFocusedImage() const;
	void SetIconFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetIconDisabledImage() const;
	void SetIconDisabledImage(LPCTSTR pStrImage);

	CButtonUI *m_pIconUI;
	CDuiSize m_szIcon;
};

}

