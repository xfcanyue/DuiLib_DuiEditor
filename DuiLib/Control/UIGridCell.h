#pragma once
#include "IGridUI.h"

namespace DuiLib
{

//单元格内部的CheckBox
class UILIB_API CGridCellInnerCheckBoxUI : public CCheckBoxUI
{
public:
	CGridCellInnerCheckBoxUI();

	void SetOwner(CControlUI *pCellUI) { m_pOwner = pCellUI; }
	CControlUI *GetOwner() const { return m_pOwner; }

	virtual bool Activate();
	virtual bool IsSelected() const;
	virtual void Selected(bool bSelected, bool bTriggerEvent=true);

	CControlUI *m_pOwner;
};

class UILIB_API CGridCellUI : public COptionLayoutUI
{
	DECLARE_DUICONTROL(CGridCellUI)
public:
	CGridCellUI();
	virtual ~CGridCellUI(void);	
	
	void SetOwner(IGridUI *pGrid) { m_pOwner = pGrid; }
	IGridUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual CDuiString GetText() const;
	virtual void SetText(LPCTSTR pstrText);

	bool IsMergedWithOthers();

	void SetCellIndex(int row, int col);
	void InitCell();

	int GetRow() const { return m_row; }
	int GetCol() const { return m_col; }

	BOOL IsFixedRow() const;
	BOOL IsFixedCol() const;

	virtual void SetFixedWidth(int cx);
	virtual bool IsSelected() const;
	virtual void Selected(bool bSelected, bool bTriggerEvent=true);
	virtual bool IsFocused() const;
	virtual bool IsHot() const;

	RECT GetCellPos();

	void CreateInnerControl();
	void DeleteInnerControl();
	CControlUI *GetInnerControl() const;
	bool OnEventInnerControl(void* param);
	bool OnNotifyInnerControl(void* param);
	void OnCellSetFocus();
	void OnCellKillFocus();

	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual SIZE EstimateSize(SIZE szAvailable);

	void DoEvent(TEventUI& event);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void PaintBkColor(HDC hDC);
	virtual void PaintBkImage(HDC hDC);
	virtual void PaintStatusImage(HDC hDC);
	virtual void PaintForeColor(HDC hDC);
	virtual void PaintForeImage(HDC hDC);
	virtual void PaintText(HDC hDC);
	virtual void PaintBorder(HDC hDC);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
protected:
	IGridUI *m_pOwner;
	int m_row, m_col;
	CControlUI *m_pInnerControl;
	int		m_nSeparatorType; //1=drag width, 2=drag height
	RECT m_rcPaint2;
protected:
	RECT m_rcMerge;
	bool m_bMergeWithOther;
	GridCellType m_celltype;
};


}