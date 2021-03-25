#pragma once


namespace DuiLib
{

class CComboEditWnd;
//下拉按钮单独定义
class UILIB_API CComboExUI : public CComboUI
{
	DECLARE_DUICONTROL(CComboExUI)
	friend class CComboEditWnd;
public:
	CComboExUI(void);
	virtual ~CComboExUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	bool DrawDropButtonImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify=NULL);

	CControlUI *AddString(LPCTSTR pstrText, UINT_PTR ptrItemData=0);
	int DeleteString(LPCTSTR pstrText); //返回删除项的下标


	bool SelectString(LPCTSTR pstrText);
	bool SetCurSel(int iIndex, bool bTakeFocus = false);
	bool SetCurSelFromItemData(UINT_PTR ptrItemData);
	bool SetItemData(CControlUI *pControl, UINT_PTR ptrItemData);
	UINT_PTR GetCurSelItemData();

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

	int GetDropType() const;
	void SetDropType(int type);

protected:
	virtual void DoEvent(TEventUI& event);
	RECT GetDropButtonRect();
	bool OnLbuttonDown(TEventUI& event);
	bool OnLbuttonUp(TEventUI& event);
	virtual bool Activate();
	virtual void PaintText(HDC hDC);

private:
	CDuiSize m_szDropButtonSize;
	CDuiString m_dbNormalImage;
	CDuiString m_dbHotImage;
	CDuiString m_dbPushedImage;
	CDuiString m_dbFocusedImage;
	CDuiString m_dbDisabledImage;

private:
	CComboEditWnd *m_pEditWindow;
	int m_type; //CBS_DROPDOWN CBS_DROPDOWNLIST
};


}