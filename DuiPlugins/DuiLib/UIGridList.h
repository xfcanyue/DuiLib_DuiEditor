#pragma once
#include "UIGridListHeader.h"
#include "UIGridListBody.h"
#include "UIGridListRowUI.h"
#include "UIGridListCell.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CGridListUI : public CVerticalLayoutUI, public IGridListUI
{
	DECLARE_DUICONTROL(CGridListUI)
	friend class CGridListHeaderUI;
	friend class CGridListBodyUI;
public:
	CGridListUI(void); 
	~CGridListUI(void);

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	int InsertRow(bool bFixed = false, int nIndex = -1);
	bool DeleteRow(int nIndex);

	int InsertColumn(int nIndex = -1);
	void DeleteColumn(int nIndex);

	CGridListHeaderUI *GetHeader();
	CGridListBodyUI *GetBody();
	CGridListRowUI *GetRow(int nRow);
	CGridListCellUI *GetCell(int nRow, int nCol);
	CGridListCellUI &Cell(int nRow, int nCol);
	CGridListCellUI *GetCellFromPt(POINT pt);
	POINT FindWhoMergeMe(CGridListCellUI *pMe);

	void ResetGrid();
	void ResetGridBody();

	virtual BOOL SetRowCount(int nRows);
	virtual int GetRowCount();
	virtual BOOL SetColumnCount(int nCols);
	virtual int GetColumnCount();

	virtual BOOL SetFixedRowCount(int nRows);
	virtual int GetFixedRowCount();

	virtual BOOL SetRowHeight(int nRow, int height);
	virtual int  GetRowHeight(int nRow);

	virtual BOOL SetColumnWidth(int nCol, int width);
	virtual int  GetColumnWidth(int nCol);

	void ClearSelectedRows();
	void SelectRow(int row, bool bClearSelectionIfCurSelected = true);
	BOOL IsSelectedRow(int row);

	void ClearSelectedCells();
	void SelectCell(int row, int col, bool bClearSelectionIfCurSelected = true);
	BOOL IsSelectedCell(int row, int col);

	void SetFocusCell(int row, int col);
	TGridCell GetFocusCell() const;

	void SetFocusRow(int row);
	int GetFocusRow() const;

	void SelectRect(const RECT &rc);

	int GetSelectRowCount();
	CGridListRowUI *GetSelectRow();
	CGridListRowUI *GetNextSelectRow();

	int GetSelectCellCount();
	CGridListCellUI *GetSelectCell();
	CGridListCellUI *GetNextSelectCell();

	void MergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol); //merge the selected cells

	void SortItems(int col);
	int GetSortColumn() const		{ return m_nSortCol; }
	BOOL GetSortAscending() const	{ return m_bSortAscending; }

	virtual void Refresh();
	virtual void SetVirtualGrid(BOOL bVirtual) override;
	void ResetVirtualOrder(bool bForceReset=true);

	void SetColumnCellType(int col, GridListCellType cellType);
	GridListCellType GetColumnCellType(int col);

	//load grid from xml string content, virtual grid will be failed
	bool LoadXmlString(LPCTSTR lpstring);

	//save grid to xml string content, virtual grid will be failed
	CDuiString SaveXmlString();					

	//load grid from xml file, virtual grid will be failed
	//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
	bool LoadXmlFile(LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

	//save grid to xml file, virtual grid will be failed
	//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
	bool SaveXmlFile(LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

	//load grid from excel file, virtual grid will be failed
	//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
	bool LoadExcelFile(LPCTSTR filename=NULL, bool bOpenFileDialog=true);		

	//save grid to excel file, virtual grid will be failed
	//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
	bool SaveExcelFile(LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high);
	static int CALLBACK pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	virtual void DoInit();
	virtual void DoEvent(TEventUI& event);
	virtual void Notify(TNotifyUI& msg);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual void SetScrollPos(SIZE szPos, bool bMsg);
	virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
	void BuildVirtualRow();
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void PaintBorder(HDC hDC);
	void PaintMergeCells(HDC hDC, CGridListCellUI *pCell);

protected:
	CGridListHeaderUI *m_pHeader;
	CGridListBodyUI *m_pBody;
	int m_nRowCount;
	int m_mapColumnWidth[MAX_GRID_COLUMN_COUNT];
	int m_mapColumnWidthFixed[MAX_GRID_COLUMN_COUNT];
	GridListCellType m_mapColumnCellType[MAX_GRID_COLUMN_COUNT];
private:
	TGridCell m_cellFocus;
	int m_iFocusRow;

	CStdPtrArray m_aSelectedRow;	//int
	int m_iteratorRow;

	CStdPtrArray m_aSelectedCellX;	//int
	CStdPtrArray m_aSelectedCellY;	//int
	int m_iteratorCell;

	CDuiRect m_rcTracker;

	int m_nSortCol;
	BOOL m_bSortAscending;
	PFNLVCOMPARE m_pfnCompare;
};


}