#pragma once


namespace DuiLib
{

#define MAX_GRID_COLUMN_COUNT	128


typedef enum enumGridListCellType
{
	celltypeText			= 0,
	celltypeEdit			= 1,
	celltypeCheckBox		= 2,
	celltypeCombo			= 3,
	celltypeDateTime		= 4,
	celltypeDate			= 5,
	celltypeTime			= 6,
	celltypeContainer		= 7
}GridListCellType;

struct TGridCell
{
	int row;
	int col;
	bool IsValid() { return row >= 0 && col >= 0 && col < MAX_GRID_COLUMN_COUNT; }
};

class IGridListUI
{
public:
	IGridListUI(void)
	{
		m_bVirtualGrid = FALSE;

		m_nDefRowHeight = 30;
		m_nDefColWidth = 80;
		m_nFixedColCount = 1;
		m_nLeftFixedColWidth = 30;
		m_bViewListNo = TRUE;

		m_bDrawRowLine = TRUE;
		m_bDrawColumnLine = TRUE;
		m_dwLineColor = 0xff808080;
		m_bSelectRect = TRUE;
		m_dwDragRectColor = 0xff808080;

		m_bEnableSizeColumn = TRUE;
		m_bEnableSizeRow = TRUE;
		m_bEnableSizeColumnInBody = FALSE;
		m_bEnableSizeRowInBody = FALSE;

		m_bExpandColumnToFit = TRUE;
		m_bExpandRowToFit = FALSE;

		m_bListMode = TRUE;
		m_bSingleRowSelection = TRUE;

		m_bHeaderSort = TRUE;
		m_szSortIconSize.cx = 16;
		m_szSortIconSize.cy = 16;

		m_dwRowSelectedBkColor = UIRGB(164,195,235);
		m_dwRowSelectedTextColor = 0x00000000;
		m_nRowSelectedFont = -1;
		m_dwRowSelectedBorderColor = 0x00000000;

		m_dwCellBkColor = 0x00000000;
		m_dwCellTextColor = 0x00000000;
		m_nCellFont = -1;
		m_dwCellBorderColor = 0x00000000;

		m_dwCellSelectedBkColor = 0xffc0dcc0;
		m_dwCellSelectedTextColor = 0x00000000;
		m_nCellSelectedFont = -1;
		m_dwCellSelectedBorderColor = 0xff0000ff;

		m_dwFixedCellBkColor = 0xfff0f0f0;
		m_dwFixedCellTextColor = 0x00000000;
		m_nFixedCellFont = -1;
		m_dwFixedCellBorderColor = 0x00000000;

		m_dwFixedCellSelectedBkColor = 0xffd2d2d2;
		m_dwFixedCellSelectedTextColor = 0x00000000;
		m_nFixedCellSelectedFont = -1;
		m_dwFixedCellSelectedBorderColor = 0x00000000;
	}
	~IGridListUI(void){}

	virtual void Refresh()						= 0;

	virtual void SetVirtualGrid(BOOL bVirtual)	= 0;
	BOOL IsVirtualGrid()						{ return m_bVirtualGrid; }

	virtual BOOL SetRowCount(int nRows)			= 0;
	virtual int GetRowCount()					= 0;

	virtual BOOL SetColumnCount(int nCols)		= 0;
	virtual int GetColumnCount()				= 0;

	virtual BOOL SetFixedRowCount(int nRows)	= 0;
	virtual int GetFixedRowCount()				= 0;

	virtual void SetFixedColumnCount(int nCols)	{ m_nFixedColCount = nCols; }
	virtual int GetFixedColumnCount()			{ return m_nFixedColCount; }

	virtual void SetViewListNumber(BOOL bView)		{ m_bViewListNo = bView; }
	virtual BOOL IsViewListNumber()					{ return m_bViewListNo; }

	void SetDefRowHeight(int height)			{ m_nDefRowHeight = height; }
	int GetDefRowHeight() const					{ return m_nDefRowHeight; }
	void SetDefColWidth(int width)				{ m_nDefColWidth = width; }
	int GetDefColWidth() const					{ return m_nDefColWidth; }
	void SetLeftFixedColWidth(int width)		{ m_nLeftFixedColWidth = width; }
	int GetLeftFixedColWidth() const			{ return m_nLeftFixedColWidth; }

	void EnableDrawRowLine(BOOL bEnable)		{ m_bDrawRowLine = bEnable; }
	BOOL IsDrawRowLine()						{ return m_bDrawRowLine; }

	void EnableDrawColumnLine(BOOL bEnable)		{ m_bDrawColumnLine = bEnable; }
	BOOL IsDrawColumnLine()						{ return m_bDrawColumnLine; }

	void SetLineColor(DWORD clr)				{ m_dwLineColor = clr; }
	DWORD GetLineColor() const					{ return m_dwLineColor; }

	void EnableSelectRect(BOOL bEnable)			{ m_bSelectRect = bEnable; }
	BOOL IsEnableSelectRect()					{ return m_bSelectRect; }

	void SetDragRectColor(DWORD clr)			{ m_dwDragRectColor = clr; }
	DWORD GetDragRectColor() const				{ return m_dwDragRectColor; }

	virtual BOOL SetRowHeight(int nRow, int height)		= 0;
	virtual int  GetRowHeight(int nRow)					= 0;

	virtual BOOL SetColumnWidth(int nCol, int width)	= 0;
	virtual int  GetColumnWidth(int nCol)				= 0;

	void EnableSizeColumn(BOOL bEnable)			{ m_bEnableSizeColumn = bEnable; }
	BOOL IsEnableSizeColumn()					{ return m_bEnableSizeColumn; }

	void EnableSizeRow(BOOL bEnable)			{ m_bEnableSizeRow = bEnable; }
	BOOL IsEnableSizeRow()						{ return m_bEnableSizeRow; }

	void EnableSizeColumnInBody(BOOL bEnable)	{ m_bEnableSizeColumnInBody = bEnable; }
	BOOL IsEnableSizeColumnInBody()				{ return m_bEnableSizeColumnInBody; }

	void EnableSizeRowInBody(BOOL bEnable)		{ m_bEnableSizeRowInBody = bEnable; }
	BOOL IsEnableSizeRowInBody()				{ return m_bEnableSizeRowInBody; }

	void ExpandColumnToFit(BOOL bExpand)		{ m_bExpandColumnToFit = bExpand; }
	BOOL IsExpandColumnToFit()					{ return m_bExpandColumnToFit; }

	void ExpandRowToFit(BOOL bExpand)			{ m_bExpandRowToFit = bExpand; }
	BOOL IsExpandRowToFit()						{ return m_bExpandRowToFit; }

	void SetListMode(BOOL bList)				{ m_bListMode = bList; }
	BOOL IsListMode()							{ return m_bListMode; }

	void SetSingleRowSelection(BOOL bSingle)	{ m_bSingleRowSelection = bSingle; }
	BOOL IsSingleRowSelection()					{ return m_bSingleRowSelection; }

	void SetHeaderSort(BOOL bSort)				{ m_bHeaderSort = bSort; }
	BOOL IsHeaderSort()							{ return m_bHeaderSort; }

	void SetSortAscendingImage(LPCTSTR lpszImage)	{ m_sSortAscendingImage = lpszImage; }		
	LPCTSTR GetSortAscendingImage()					{ return m_sSortAscendingImage; }

	void SetSortDescendingImage(LPCTSTR lpszImage)	{ m_sSortDescendingImage = lpszImage; }		
	LPCTSTR GetSortDescendingImage()				{ return m_sSortDescendingImage; }

	void SetSortIconSize(SIZE sz)				{ m_szSortIconSize = sz; }
	SIZE GetSortIconSize()						{ return m_szSortIconSize; }
	//////////////////////////////////////////////////////////////////////////
	void SetRowSelectedBkColor(DWORD clr)		{ m_dwRowSelectedBkColor = clr; }
	DWORD GetRowSelectedBkColor()				{ return m_dwRowSelectedBkColor; }

	void SetRowSelectedTextColor(DWORD clr)		{ m_dwRowSelectedTextColor = clr; }
	DWORD GetRowSelectedTextColor()				{ return m_dwRowSelectedTextColor; }

	void SetRowSelectedFont(int nFont)			{ m_nRowSelectedFont = nFont; }
	int  GetRowSelectedFont()					{ return m_nRowSelectedFont; }

	void SetRowSelectedBorderColor(DWORD clr)	{ m_dwRowSelectedBorderColor = clr; }
	DWORD GetRowSelectedBorderColor()			{ return m_dwRowSelectedBorderColor; }

	//////////////////////////////////////////////////////////////////////////
	void SetCellBkColor(DWORD clr)		{ m_dwCellBkColor = clr; }
	DWORD GetCellBkColor()				{ return m_dwCellBkColor; }

	void SetCellTextColor(DWORD clr)	{ m_dwCellTextColor = clr; }
	DWORD GetCellTextColor()			{ return m_dwCellTextColor; }

	void SetCellFont(int nFont)			{ m_nCellFont = nFont; }
	int  GetCellFont()					{ return m_nCellFont; }

	void SetCellBorderColor(DWORD clr)	{ m_dwCellBorderColor = clr; }
	DWORD GetCellBorderColor()			{ return m_dwCellBorderColor; }

	//////////////////////////////////////////////////////////////////////////
	void SetCellSelectedBkColor(DWORD clr)		{ m_dwCellSelectedBkColor = clr; }
	DWORD GetCellSelectedBkColor()				{ return m_dwCellSelectedBkColor; }

	void SetCellSelectedTextColor(DWORD clr)	{ m_dwCellSelectedTextColor = clr; }
	DWORD GetCellSelectedTextColor()			{ return m_dwCellSelectedTextColor; }

	void SetCellSelectedFont(int nFont)			{ m_nCellSelectedFont = nFont; }
	int  GetCellSelectedFont()					{ return m_nCellSelectedFont; }

	void SetCellSelectedBorderColor(DWORD clr)	{ m_dwCellSelectedBorderColor = clr; }
	DWORD GetCellSelectedBorderColor()			{ return m_dwCellSelectedBorderColor; }

	//////////////////////////////////////////////////////////////////////////
	void SetFixedCellSelectedBkColor(DWORD clr)		{ m_dwFixedCellSelectedBkColor = clr; }
	DWORD GetFixedCellSelectedBkColor()				{ return m_dwFixedCellSelectedBkColor; }

	void SetFixedCellSelectedTextColor(DWORD clr)	{ m_dwFixedCellSelectedTextColor = clr; }
	DWORD GetFixedCellSelectedTextColor()			{ return m_dwFixedCellSelectedTextColor; }

	void SetFixedCellSelectedFont(int nFont)		{ m_nFixedCellSelectedFont = nFont; }
	int  GetFixedCellSelectedFont()					{ return m_nFixedCellSelectedFont; }

	void SetFixedCellSelectedBorderColor(DWORD clr)	{ m_dwFixedCellSelectedBorderColor = clr; }
	DWORD GetFixedCellSelectedBorderColor()			{ return m_dwFixedCellSelectedBorderColor; }

	//////////////////////////////////////////////////////////////////////////
	void SetFixedCellBkColor(DWORD clr)				{ m_dwFixedCellBkColor = clr; }
	DWORD GetFixedCellBkColor()						{ return m_dwFixedCellBkColor; }

	void SetFixedCellTextColor(DWORD clr)			{ m_dwFixedCellTextColor = clr; }
	DWORD GetFixedCellTextColor()					{ return m_dwFixedCellTextColor; }

	void SetFixedCellFont(int nFont)				{ m_nFixedCellFont = nFont; }
	int  GetFixedCellFont()							{ return m_nFixedCellFont; }

	void SetFixedCellBorderColor(DWORD clr)			{ m_dwFixedCellBorderColor = clr; }
	DWORD GetFixedCellBorderColor()					{ return m_dwFixedCellBorderColor; }

	//////////////////////////////////////////////////////////////////////////
	void SetStyleEdit(LPCTSTR pstrValue)			{ m_sStyleEdit = pstrValue; }
	LPCTSTR GetStyleEdit()							{ return m_sStyleEdit; }
	void SetStyleCheckBox(LPCTSTR pstrValue)		{ m_sStyleCheckBox = pstrValue; }
	LPCTSTR GetStyleCheckBox()						{ return m_sStyleCheckBox; }
	void SetStyleCombo(LPCTSTR pstrValue)			{ m_sStyleCombo = pstrValue; }
	LPCTSTR GetStyleCombo()							{ return m_sStyleCombo; }
	void SetStyleDateTime(LPCTSTR pstrValue)		{ m_sStyleDateTime = pstrValue; }
	LPCTSTR GetStyleDateTime()						{ return m_sStyleDateTime; }
	void SetStyleDate(LPCTSTR pstrValue)			{ m_sStyleDate = pstrValue; }
	LPCTSTR GetStyleDate()							{ return m_sStyleDate; }
	void SetStyleTime(LPCTSTR pstrValue)			{ m_sStyleTime = pstrValue; }
	LPCTSTR GetStyleTime()							{ return m_sStyleTime; }

	//////////////////////////////////////////////////////////////////////////
	void SetFont(int index)							{ m_iFont = index; }
	int GetFont() const								{ return m_iFont; }
protected:
	BOOL  m_bVirtualGrid;				//virtual grid

	int   m_nDefRowHeight;
	int   m_nDefColWidth;
	int   m_nLeftFixedColWidth;			//leftmost fixed column width
	BOOL  m_bViewListNo;				//view list index on leftmost fixed column
	int	  m_nFixedColCount;				//count of the fixed columns on the left side by grid

	BOOL  m_bDrawRowLine;				//enable drawing row's line
	BOOL  m_bDrawColumnLine;			//enable drawing column's line
	DWORD m_dwLineColor;				//grid line color
	BOOL  m_bSelectRect;				//enable drag rect to select rows or cells
	DWORD m_dwDragRectColor;			//color of drag rect to select rows or cells

	BOOL  m_bEnableSizeColumn;			//enable drag column size in grid header
	BOOL  m_bEnableSizeRow;				//enable drag row height in grid header
	BOOL  m_bEnableSizeColumnInBody;	//enable drag column size in grid body
	BOOL  m_bEnableSizeRowInBody;		//enable drag row height in grid body

	BOOL  m_bExpandColumnToFit;
	BOOL  m_bExpandRowToFit;

	BOOL  m_bListMode;
	BOOL  m_bSingleRowSelection;


	BOOL  m_bHeaderSort;				//enable click fixed row to sort column
	CDuiString m_sSortAscendingImage;
	CDuiString m_sSortDescendingImage;
	SIZE m_szSortIconSize;

	DWORD m_dwRowSelectedBkColor;
	DWORD m_dwRowSelectedTextColor;
	int   m_nRowSelectedFont;
	DWORD m_dwRowSelectedBorderColor;

	DWORD m_dwCellBkColor;
	DWORD m_dwCellTextColor;
	int   m_nCellFont;
	DWORD m_dwCellBorderColor;

	DWORD m_dwCellSelectedBkColor;
	DWORD m_dwCellSelectedTextColor;
	int   m_nCellSelectedFont;
	DWORD m_dwCellSelectedBorderColor;

	DWORD m_dwFixedCellBkColor;
	DWORD m_dwFixedCellTextColor;
	int   m_nFixedCellFont;
	DWORD m_dwFixedCellBorderColor;

	DWORD m_dwFixedCellSelectedBkColor;
	DWORD m_dwFixedCellSelectedTextColor;
	int   m_nFixedCellSelectedFont;
	DWORD m_dwFixedCellSelectedBorderColor;

	int		m_iFont;

	CDuiString m_sStyleEdit;
	CDuiString m_sStyleCheckBox;
	CDuiString m_sStyleCombo;
	CDuiString m_sStyleDateTime;
	CDuiString m_sStyleDate;
	CDuiString m_sStyleTime;
};



}