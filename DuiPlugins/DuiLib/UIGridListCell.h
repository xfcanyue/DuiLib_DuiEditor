#pragma once
#include "IGridListUI.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CGridListCellUI : public CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CGridListCellUI)
public:
	CGridListCellUI();
	virtual ~CGridListCellUI(void);	
	
	void SetOwner(IGridListUI *pGrid) { m_pOwner = pGrid; }
	IGridListUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	int GetRowIndex();
	int GetColIndex();

	bool IsFixed();
	bool IsFixedRow();
	bool IsFixedColumn();

	void Selected(BOOL bSelect);
	BOOL IsSelected() { return m_bSelected; }

	bool IsMerged();							//if true, draw this cell, hide the others merged cells
	void SetMergeRange(const RECT &rcRange);	//to unmerge, set parameter with DuiRect(0,0,0,0)
	const RECT &GetMergeRange()	{ return m_rcMerge; }

	void SetMergedWithOther(bool bMerge);
	bool IsMergedWidthOthers();					// if ture, hide this cell

	RECT GetCellPos();

	void SetCellType(GridListCellType celltype);
	GridListCellType GetCellType();

	virtual CDuiString GetText() const;
	virtual void SetText(LPCTSTR pstrText);

	virtual int  GetText_to_int();
	virtual void SetText(int n);

	void SetTextStyle(UINT uStyle);
	UINT GetTextStyle() const;

	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);

	void CreateInnerControl();
	void DeleteInnerControl();
	CControlUI *GetInnerControl() const;

	bool OnEventInnerControl(void* param);
	bool OnNotifyInnerControl(void* param);

	void OnCellKillFocus();

	void OnClickCheckBox();

	virtual void DoInit();
	virtual SIZE EstimateSize(SIZE szAvailable);

	void DoEvent(TEventUI& event);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void PaintBkColor(HDC hDC);
	virtual void PaintBorder(HDC hDC);
	virtual void PaintText(HDC hDC);
	virtual void PaintForeImage(HDC hDC);

	bool SizeColumnOrRow(TEventUI& event);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
protected:
	IGridListUI *m_pOwner;
	RECT m_rcCellPos;
	BOOL m_bSelected;
	RECT m_rcMerge;
	bool m_bMergeWithOther;

	DWORD	m_dwTextColor;
	DWORD	m_dwDisabledTextColor;
	int		m_iFont;
	UINT	m_uTextStyle;
	RECT	m_rcTextPadding;
	bool	m_bShowHtml;

	GridListCellType m_celltype;
	CControlUI *m_pInnerControl;
private:
	int		m_nSeparatorType; //1=drag width, 2=drag height
};


}