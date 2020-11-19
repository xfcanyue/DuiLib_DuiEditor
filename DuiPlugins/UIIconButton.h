#pragma once

namespace DuiLib
{

class UILIB_PLUGIN_API CIconButtonUI : public CButtonUI
{
	DECLARE_DUICONTROL(CIconButtonUI)
public:
	CIconButtonUI(void);
	virtual ~CIconButtonUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);
	bool DrawIconImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify=NULL);

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

	CSize m_szIcon;
	UINT m_uIconAlign;
	CDuiString m_IconNormalImage;
	CDuiString m_IconHotImage;
	CDuiString m_IconPushedImage;
	CDuiString m_IconFocusedImage;
	CDuiString m_IconDisabledImage;
};

}

