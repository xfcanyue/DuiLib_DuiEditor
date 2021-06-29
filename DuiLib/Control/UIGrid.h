#pragma once
#include "UIGridHeader.h"
#include "UIGridBody.h"
#include "UIGridRowUI.h"
#include "UIGridCell.h"

namespace DuiLib
{

class UILIB_API CGridUI : public CVerticalLayoutUI, public IGridUI
{
	DECLARE_DUICONTROL(CGridUI)
public:
	CGridUI(void); 
	~CGridUI(void);

	LPCTSTR GetClass() const;
	virtual UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void Refresh(bool bNeedUpdate = false);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	int InsertRow(int nIndex = -1);
	bool DeleteRow(int nIndex);

	int InsertColumn(int nIndex = -1);
	void DeleteColumn(int nIndex);

	void ResetGrid();
	void ResetGridBody();

	virtual BOOL SetRowCount(int rows);
	virtual int GetRowCount();
	virtual BOOL SetColumnCount(int cols);
	virtual int GetColumnCount();

	virtual BOOL SetFixedRowCount(int rows);
	virtual int GetFixedRowCount();

	virtual BOOL SetRowHeight(int row, int height);
	virtual int  GetRowHeight(int row);

	virtual BOOL SetColumnWidth(int col, int width);
	virtual int  GetColumnWidth(int col);

	//设置整列单元格类型
	void SetCellType(int col, GridCellType cellType);

	//获取整列单元格类型
	GridCellType GetCellType(int col);

	//设置单元格类型
	void SetCellType(int row, int col, GridCellType cellType);

	//获取单元格类型
	GridCellType GetCellType(int row, int col);

	virtual void SetVirtualGrid(BOOL bVirtual) override;
	virtual void SetVirtualRowCount(int nRows);

	void SetRowTag(int row, UINT_PTR tag);
	UINT_PTR GetRowTag(int row);
	int FindRowFromRowTag(UINT_PTR tag);

	BOOL IsMergedCell(int row, int col);
	TGridMergeRange GetCellMergeRange(int row, int col);
	void MergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol);

	CGridCellUI *GetCellUI(int row, int col);
	CGridCellUI *GetCellUI(const TCellID &cellID);
	CGridCellUI *GetCellUI(TCellData *pCellData);
	CGridCellUI *GetCellUIFromPoint(const POINT &pt);

	CGridRowUI *GetRowUI(int row);

	CGridCellUI *GetHotCell() { return m_pHotCell; }
	CGridRowUI *GetHotRow() { return m_pHotRow; }

	BOOL IsFixedRow(int row);
	BOOL IsFixedCol(int col);

	virtual void SetFocusCell(int row, int col);
	virtual void SetFocusCell(const TCellID &cellID);
	virtual const TCellID &GetFocusCell() const;

	//设置col列是否允许点击表头排序
	void SetColumnSort(int col, BOOL bSort);

	//获取col列是否允许点击表头排序
	BOOL IsColumnSort(int col) const;

	//设置自定义排序算法回调函数，默认按单元格字符串比较，_tcscmp()
	void SetSortCallbackFun(PFNLVCOMPARE pfnCompare);
	//获取排序算法回调函数
	PFNLVCOMPARE GetSortCallbackFun() const;

	//执行按col列进行排序
	void SortItems(int col);
	virtual void OnSortItem(int col, BOOL bAscending);

	//获取当前排序的列
	int GetSortColumn() const		{ return m_nSortCol; }

	//TRUE=升序排列，FALSE=降序排列
	BOOL GetSortAscending() const	{ return m_bSortAscending; }
protected:
	BOOL SortItems(PFNLVCOMPARE pfnCompare, int col, BOOL bAscending, LPARAM data, int low, int high);
	static int CALLBACK pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:
	virtual void DoInit();
	virtual void DoEvent(TEventUI& event);
	bool OnSizeColumnOrRow(TEventUI& event);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual void BuildRows(RECT rc, bool bNeedInvalidate = true);
	virtual void OnDrawItem(int nBeginRow, int nEndRow);
	virtual void SetScrollPos(SIZE szPos, bool bMsg);
	virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void PaintBorder(HDC hDC);

public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	CGridHeaderUI *m_pHeader;
	CGridBodyUI *m_pBody;
	int m_nRowCount;
	int m_nColCount;
	int m_mapColumnWidth[MAX_GRID_COLUMN_COUNT];
	int m_mapColWidthFixed[MAX_GRID_COLUMN_COUNT];
	GridCellType m_mapColumnCellType[MAX_GRID_COLUMN_COUNT];
	BOOL m_mapColumnSort[MAX_GRID_COLUMN_COUNT];

	CDuiRect m_rcTracker; //拖动鼠标进行选择的框框

	int m_nSortCol;
	BOOL m_bSortAscending;
	PFNLVCOMPARE m_pfnCompare;

private:
	CGridCellUI *m_pCellLButtonDown;
	CGridRowUI *m_pHotRow;
	CGridCellUI *m_pHotCell;
	int		m_nSeparatorType; //1=drag width, 2=drag height
};


}