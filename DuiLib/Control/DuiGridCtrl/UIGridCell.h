#pragma once

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridCellUI
{
public:
	CGridCellUI(void);
	virtual ~CGridCellUI(void);

	void SetOwner(CControlUI *pOwnder);
	CControlUI *GetOwner();

	void DrawCellText(HDC hDC, const RECT &rcItem);

	virtual void SetText(LPCTSTR pstrText);
	virtual CDuiString GetText();

	DWORD GetTextStyle() const;
	void SetTextStyle(UINT uStyle);

	DWORD GetTextColor() const;
	void SetTextColor(DWORD dwTextColor);

	void SetTextPadding(RECT rc);
	RECT GetTextPadding() const;

	void SetFont(int index);

	bool IsShowHtml();
	void SetShowHtml(bool bShowHtml = true);
protected:
	CDuiString m_sText;
	DWORD m_dwTextColor;
	int m_iFont;
	UINT m_uTextStyle;
	bool m_bShowHtml;
	RECT m_rcTextPadding;

private:
	CControlUI *m_pOwner;
};


}
