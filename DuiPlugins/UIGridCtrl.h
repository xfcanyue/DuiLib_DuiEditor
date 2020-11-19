#pragma once
#include "UIGridCtrlWnd.h"

namespace DuiLib
{

class UILIB_PLUGIN_API CGridCtrlUI : public CContainerUI
{
	DECLARE_DUICONTROL(CGridCtrlUI)
	friend class CGridCtrlWnd;
public:
	CGridCtrlUI(void);
	virtual ~CGridCtrlUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	CGridCtrl *GetGrid();
	CGridCell *GetCell(int nRow, int nCol);

	CGridCtrl &Grid();
	CGridCell &Cell(int nRow, int nCol);				//返回普通单元格引用
	CGridCellCombo &ComboBox(int nRow, int nCol);		//返回ComboBox单元格引用	
	CGridCellCheck &CheckBox(int nRow, int nCol);		//返回CheckBox单元格引用	
	CGridCellDateTime &DateTime(int nRow, int nCol);	//返回DateTime单元格引用
	
	CGridDefaultCell &CellDef();			//返回默认单元格引用
	CGridDefaultCell &CellFixedRowDef();	//返回默认单元格引用
	CGridDefaultCell &CellFixedColDef();	//返回默认单元格引用
	CGridDefaultCell &CellFixedRowColDef();	//返回默认单元格引用
	

	virtual void DoInit();
	virtual void DoEvent(TEventUI& event);  
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void SetPos(RECT rc, bool bNeedInvalidate  = true);
	void SetInternVisible(bool bVisible);

protected:
	void OnGridNotify(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	//////////////////////////////////////////////////////////////////////////
	//属性
	//////////////////////////////////////////////////////////////////////////
	void SetRowHeight(int nHeight);
	int GetRowHeight();

	bool SetRowCount(int count);
	int GetRowCount();

	bool SetColumnCount(int count);
	int GetColumnCount();

	bool SetFixedRowCount(int count);
	int GetFixedRowCount();

	bool SetFixedColumnCount(int count);
	int GetFixedColumnCount();


	void SetEditable(bool bEditable);
	bool GetEditable();

	void SetSingleRowSelection(bool bSingleRow);
	bool GetSingleRowSelection();

	void SetListMode(bool bListMode);
	bool GetListMode();

	void SetViewListNumber(bool bView);
	bool GetViewListNumber();

	void SetFont(int index);
	int GetFont() const;

	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;

	void SetVirtualGrid(bool bVirtual);
	bool GetVirtualGrid();

	void  SetGridBkColor(DWORD dwColor);
	DWORD GetGridBkColor();

	void  SetGridLineColor(DWORD dwColor);
	DWORD GetGridLineColor();

	void SetEnableHighLightBkColor(bool bValue);
	bool GetEnableHighLightBkColor();

	void SetHighLightBkColor(DWORD dwColor);
	DWORD GetHighLightBkColor();

	void SetEnableHighLightTextClr(bool bValue);
	bool GetEnableHighLightTextClr();

	void SetHighLightTextClr(DWORD dwColor);
	DWORD GetHighLightTextClr();

	void SetFocusCellBorder(bool bBorder);
	bool GetFocusCellBorder();

	void SetFocusCellBorderColor(DWORD dwColor);
	DWORD GetFocusCellBorderColor();

//////////////////////////////////////////////////////////////////////////
//操作
//////////////////////////////////////////////////////////////////////////
public:
	void Refresh();
	void SetAutoRedraw(bool bAuto);
	void ExpandColumnsToFit(BOOL bExpandFixed = TRUE);
	int  InsertRow(int rowPosition=-1);
	void SetHeaderTexts(LPCTSTR pstrValue);

public:
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

public:
	CGridCtrlWnd *m_pWindow;


private:
	bool m_bVirtualGrid;

	int m_nRowCount;
	int m_nColumnCount;
	int m_nFixedRowCount;
	int m_nFixedColumnCount;

	int m_nDefaultRowHeight;

	bool m_bEditable;
	bool m_bSingleRowSelection;
	bool m_bListMode;
	bool m_bViewListNo;

	DWORD	m_dwTextColor;
	int		m_iFont;

	DWORD m_dwGridBkColor;
	DWORD m_dwGridLineColor;


	DWORD m_crHighLightBkColor;
	bool m_bHighLightBkColor;
	DWORD m_crHighLightTextClr;
	bool m_bHighLightTextClr;

	bool m_bFocusCellBorder;
	DWORD m_dwFocusCellBorderColor;

	CStdPtrArray m_arrHeaderText;
};

}