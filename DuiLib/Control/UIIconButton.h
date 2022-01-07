#pragma once

namespace DuiLib
{

class UILIB_API CIconButtonUI : public CButtonLayoutUI
{
	DECLARE_DUICONTROL(CIconButtonUI)
public:
	CIconButtonUI(void);
	virtual ~CIconButtonUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual SIZE EstimateSize(SIZE szAvailable) override;

	virtual void PaintText(UIRender *pRender) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

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

