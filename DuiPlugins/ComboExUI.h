#pragma once

//下拉按钮单独定义
class UILIB_PLUGIN_API CComboExUI : public CComboUI
{
	DECLARE_DUICONTROL(CComboExUI)
public:
	CComboExUI(void);
	virtual ~CComboExUI(void);

	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	bool DrawDropButtonImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify=NULL);

	bool SelectString(LPCTSTR pstrText);

	virtual void SetText(LPCTSTR pstrText);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	LPCTSTR GetdbNormalImage() const;
	void SetdbNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetdbHotImage() const;
	void SetdbHotImage(LPCTSTR pStrImage);
	LPCTSTR GetdbPushedImage() const;
	void SetdbPushedImage(LPCTSTR pStrImage);
	LPCTSTR GetdbFocusedImage() const;
	void SetdbFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetdbDisabledImage() const;
	void SetdbDisabledImage(LPCTSTR pStrImage);

	CSize m_szDropButtonSize;
	CDuiString m_dbNormalImage;
	CDuiString m_dbHotImage;
	CDuiString m_dbPushedImage;
	CDuiString m_dbFocusedImage;
	CDuiString m_dbDisabledImage;
};

