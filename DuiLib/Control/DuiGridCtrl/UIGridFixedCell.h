#pragma once
#include "UIGridCell.h"

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridFixedCellUI : public CGridCellUI, public CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CGridFixedCellUI)
public:
	CGridFixedCellUI(void);
	virtual ~CGridFixedCellUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);
	RECT GetThumbRect() const;

	virtual void PaintBkColor(HDC hDC);
	virtual void PaintText(HDC hDC);
	virtual void PaintStatusImage(HDC hDC);

	virtual void SetText(LPCTSTR pstrText);
	virtual CDuiString GetText();

	void SetEnabled(bool bEnable = true);
	bool IsDragable() const;
	void SetDragable(bool bDragable);
	DWORD GetSepWidth() const;
	void SetSepWidth(int iWidth);
	
	LPCTSTR GetNormalImage() const;
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage() const;
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetPushedImage() const;
	void SetPushedImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage() const;
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetSepImage() const;
	void SetSepImage(LPCTSTR pStrImage);
	void SetScale(int nScale);
	int GetScale() const;

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	POINT ptLastMouse;
	bool m_bDragable;
	UINT m_uButtonState;
	int m_iSepWidth;
	
	CDuiString m_sNormalImage;
	CDuiString m_sHotImage;
	CDuiString m_sPushedImage;
	CDuiString m_sFocusedImage;
	CDuiString m_sSepImage;
	CDuiString m_sSepImageModify;
	int m_nScale;
};


}
