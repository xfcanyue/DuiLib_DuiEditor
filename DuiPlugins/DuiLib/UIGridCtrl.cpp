#include "StdAfx.h"
#include "UIGridCtrl.h"

namespace DuiLib
{

class CGridCtrlWnd : public CWindowWnd
{
public:
	CGridCtrlWnd(void) {}
	~CGridCtrlWnd(void) {}

	void Init(CGridCtrlUI* pOwner)
	{
		m_pOwner = pOwner;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD|WS_VISIBLE, 0, m_rcWindow);
		ASSERT(m_hWnd);

		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_BORDER));
	}

	void SetPos(RECT rc, bool bNeedInvalidate  = true)
	{
		m_rcWindow = rc;
		::MoveWindow(GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
	}

	LPCTSTR GetWindowClassName() const
	{
		return _T("GridCtrl");
	}

	void OnFinalMessage(HWND hWnd)
	{
		if(m_pGrid) { delete m_pGrid; m_pGrid = NULL; }
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;
		if( uMsg == WM_CREATE ) 
		{
			m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);

			CWnd *pWnd = CWnd::FromHandle(m_hWnd);
			m_pGrid = new CGridCtrl();
			m_pGrid->Create(m_rcWindow, pWnd, 1, WS_CHILD|WS_VISIBLE);
			ASSERT(m_pGrid);
			ASSERT(m_pGrid->m_hWnd);

		}
		else if( uMsg == WM_SIZE) 
		{
			CRect rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			m_pGrid->MoveWindow(rcClient);

			//m_pGrid->ExpandColumnsToFit(TRUE);
		}
		else if(uMsg == WM_LBUTTONDOWN)
		{
			m_pOwner->OnGridNotify(uMsg, wParam, lParam);
		}
		else if(uMsg == WM_NOTIFY)
		{
			m_pOwner->OnGridNotify(uMsg, wParam, lParam);
		}

		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

public:
	RECT m_rcWindow;
	CGridCtrlUI *m_pOwner;
	CGridCtrl *m_pGrid;
};

IMPLEMENT_DUICONTROL(CGridCtrlUI)
CGridCtrlUI::CGridCtrlUI(void) : m_pWindow(0)
{
	m_nRowCount = 1;
	m_nColumnCount = 1;
	m_nFixedRowCount = 1;
	m_nFixedColumnCount = 1;

	m_nDefaultRowHeight = 25;

	m_bEditable = false;
	m_bSingleRowSelection = true;
	m_bListMode = true;
	m_bViewListNo = true;
	m_bVirtualGrid = false;

	m_dwGridBkColor = 0xFFFFFFFF;
	m_dwGridLineColor = 0xFFc0c0c0;
	m_bHighLightBkColor = true;
	m_crHighLightBkColor = 0xFFa6caf0;
	m_bHighLightTextClr = true;
	m_crHighLightTextClr = 0xFF000000;
	m_bFocusCellBorder = true;
	m_dwFocusCellBorderColor = 0xFF4e00ff;
}

CGridCtrlUI::~CGridCtrlUI(void)
{
	if(m_pWindow) { delete m_pWindow; }
	for (int i=0; i<m_arrHeaderText.GetSize(); i++)
	{
		CDuiString *pStr = (CDuiString *)m_arrHeaderText.GetAt(i);
		delete pStr;
	}
}

LPCTSTR CGridCtrlUI::GetClass() const
{
	return _T("GridCtrlUI");
}

LPVOID CGridCtrlUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_GRIDCTRL) == 0 ) return static_cast<CGridCtrlUI*>(this);
	return __super::GetInterface(pstrName);
}

CGridCtrl *CGridCtrlUI::GetGrid()
{
	if(m_pWindow)
	{
		return ((CGridCtrlWnd *)m_pWindow)->m_pGrid;
	}
	return NULL;
}

CGridCell *CGridCtrlUI::GetCell(int nRow, int nCol)
{
	CGridCtrl *pGrid = GetGrid();
	return (CGridCell *)pGrid->GetCell(nRow, nCol);
}

CGridCtrl &CGridCtrlUI::Grid()
{
	return *GetGrid();
}

CGridCell &CGridCtrlUI::Cell(int nRow, int nCol)					//返回普通单元格引用
{
	return *GetCell(nRow, nCol);
}

CGridCellCombo &CGridCtrlUI::ComboBox(int nRow, int nCol)		//返回ComboBox单元格引用	
{
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellCombo)));
	return *(CGridCellCombo *)GetCell(nRow, nCol);
}

CGridCellCheck &CGridCtrlUI::CheckBox(int nRow, int nCol)		//返回CheckBox单元格引用	
{
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellCheck)));
	return *(CGridCellCheck *)GetCell(nRow, nCol);
}

CGridCellDateTime &CGridCtrlUI::DateTime(int nRow, int nCol)		//返回DateTime单元格引用
{
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellDateTime)));
	return *(CGridCellDateTime *)GetCell(nRow, nCol);
}

CGridDefaultCell &CGridCtrlUI::CellDef()							//返回默认单元格引用
{
	return GetGrid()->CellDef();
}

CGridDefaultCell &CGridCtrlUI::CellFixedRowDef()					//返回默认单元格引用
{
	return GetGrid()->CellFixedRowDef();
}

CGridDefaultCell &CGridCtrlUI::CellFixedColDef()					//返回默认单元格引用
{
	return GetGrid()->CellFixedColDef();
}

CGridDefaultCell &CGridCtrlUI::CellFixedRowColDef()				//返回默认单元格引用
{
	return GetGrid()->CellFixedRowColDef();
}

void CGridCtrlUI::DoInit()
{
	m_pWindow = new CGridCtrlWnd;
	((CGridCtrlWnd *)m_pWindow)->Init(this);

	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		CGridCtrl *pGrid = ((CGridCtrlWnd *)m_pWindow)->m_pGrid;
		if(pGrid)
		{
			CFont ft;
			ft.Attach(GetManager()->GetFont(m_iFont));
			pGrid->SetFont(&ft);
			ft.Detach();

			pGrid->CellDef().SetTextClr(UIARGB_2_RGB(m_dwTextColor));

			pGrid->SetVirtualMode(m_bVirtualGrid);

			pGrid->SetFixedColumnSelection(FALSE);	//不允许选中固定列时选中整列
			pGrid->SetFixedRowSelection(TRUE);		//允许选中固定行时选中整行
			pGrid->SetDefCellMargin(0);				//单元格内边距；单元格文字到单元格边框的大小

			pGrid->CellDef().SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pGrid->CellFixedRowDef().SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			pGrid->SetGridBkColor(RGB(UIARGB_GetRValue(m_dwGridBkColor), UIARGB_GetGValue(m_dwGridBkColor), UIARGB_GetBValue(m_dwGridBkColor)));
			pGrid->SetGridLineColor(RGB(UIARGB_GetRValue(m_dwGridLineColor), UIARGB_GetGValue(m_dwGridLineColor), UIARGB_GetBValue(m_dwGridLineColor)));
			pGrid->SetEnableHighLightBkColor(m_bHighLightBkColor);
			pGrid->SetHighLightBkColor(RGB(UIARGB_GetRValue(m_crHighLightBkColor), UIARGB_GetGValue(m_crHighLightBkColor), UIARGB_GetBValue(m_crHighLightBkColor)));
			pGrid->SetEnableHighLightTextColor(m_bHighLightTextClr);
			pGrid->SetHighLightTextClr(RGB(UIARGB_GetRValue(m_crHighLightTextClr), UIARGB_GetGValue(m_crHighLightTextClr), UIARGB_GetBValue(m_crHighLightTextClr)));

			pGrid->SetFrameFocusCell(m_bFocusCellBorder);		//选中单元格外边框
			pGrid->SetFrameFocusCellColor(UIARGB_2_RGB(m_dwFocusCellBorderColor));		//选中单元格外边框

			pGrid->SetEditable(m_bEditable);
			pGrid->SetSingleRowSelection(m_bSingleRowSelection);
			pGrid->SetFrameFocusCell(m_bFocusCellBorder);
			pGrid->SetListMode(m_bListMode);
			pGrid->SetViewFixedColNo(m_bViewListNo);

			pGrid->CellDef().SetHeight(m_nDefaultRowHeight);

			pGrid->SetRowCount(m_nRowCount);
			pGrid->SetColumnCount(m_nColumnCount);
			pGrid->SetFixedRowCount(m_nFixedRowCount);
			pGrid->SetFixedColumnCount(m_nFixedColumnCount);
			if(m_nColumnCount > 0)
			{
				pGrid->SetColumnWidth(0,30);
			}

			for(int i=0; i<m_arrHeaderText.GetSize(); i++)
			{
				CDuiString *pstr = (CDuiString *)m_arrHeaderText.GetAt(i);
				if(i+1< pGrid->GetColumnCount())
				{
					pGrid->Cell(0,i+1).SetText(*pstr);
				}
			}

		}
	}
}

void CGridCtrlUI::DoEvent(TEventUI& event)
{
	__super::DoEvent(event);
}

LRESULT CGridCtrlUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if(uMsg == WM_NOTIFY)
	{
		int x = 0;
	}
	return 0;
}

void CGridCtrlUI::SetPos(RECT rc, bool bNeedInvalidate /* = true */)
{
	__super::SetPos(rc, bNeedInvalidate);

	if(m_pWindow)
		((CGridCtrlWnd *)m_pWindow)->SetPos(rc, bNeedInvalidate);
}

void CGridCtrlUI::SetInternVisible(bool bVisible)
{
	if(m_pWindow)
		::ShowWindow(m_pWindow->GetHWND(), bVisible);
}

//////////////////////////////////////////////////////////////////////////
void CGridCtrlUI::SetRowHeight(int nHeight)
{
	m_nDefaultRowHeight = nHeight;
}

int CGridCtrlUI::GetRowHeight()
{
	return m_nDefaultRowHeight;
}

bool CGridCtrlUI::SetRowCount(int count)
{
	m_nRowCount = count;

	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		return ((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetRowCount(m_nRowCount) == TRUE;
	}
	return false;
}

int CGridCtrlUI::GetRowCount()
{
	return m_nRowCount;
}

bool CGridCtrlUI::SetColumnCount(int count)
{
	m_nColumnCount = count;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		return ((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetColumnCount(m_nColumnCount) == TRUE;
	}
	return false;
}

int CGridCtrlUI::GetColumnCount()
{
	return m_nColumnCount;
}

bool CGridCtrlUI::SetFixedRowCount(int count)
{
	m_nFixedRowCount = count;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		return ((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetFixedRowCount(m_nFixedRowCount) == TRUE;
	}
	return false;
}

int CGridCtrlUI::GetFixedRowCount()
{
	return m_nFixedRowCount;
}

bool CGridCtrlUI::SetFixedColumnCount(int count)
{
	m_nFixedColumnCount = count;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		return ((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetFixedColumnCount(m_nFixedColumnCount) == TRUE;
	}
	return false;
}

int CGridCtrlUI::GetFixedColumnCount()
{
	return m_nFixedColumnCount;
}

void CGridCtrlUI::SetEditable(bool bEditable)
{
	m_bEditable = bEditable;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetEditable(m_bEditable);
	}
}

bool CGridCtrlUI::GetEditable()
{
	return m_bEditable;
}

void CGridCtrlUI::SetSingleRowSelection(bool bSingleRow)
{
	m_bSingleRowSelection = bSingleRow;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetSingleRowSelection(m_bSingleRowSelection);
}

bool CGridCtrlUI::GetSingleRowSelection()
{
	return m_bSingleRowSelection;
}

void CGridCtrlUI::SetListMode(bool bListMode)
{
	m_bListMode = bListMode;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetListMode(m_bListMode);
}

bool CGridCtrlUI::GetListMode()
{
	return m_bListMode;
}

void CGridCtrlUI::SetViewListNumber(bool bView)
{
	m_bViewListNo = bView;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetViewFixedColNo(m_bViewListNo);
}

bool CGridCtrlUI::GetViewListNumber()
{
	return m_bViewListNo;
}

void CGridCtrlUI::SetFont(int index)
{
	m_iFont = index;

	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		CFont ft;
		ft.Attach(GetManager()->GetFont(m_iFont));
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetFont(&ft);
		ft.Detach();
		Refresh();
	}
}


void CGridCtrlUI::SetVirtualGrid(bool bVirtual)
{
	m_bVirtualGrid = bVirtual;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetVirtualMode(m_bVirtualGrid);
		Refresh();
	}
}

bool CGridCtrlUI::GetVirtualGrid()
{
	return m_bVirtualGrid;
}

void  CGridCtrlUI::SetGridBkColor(DWORD dwColor)
{
	m_dwGridBkColor = dwColor;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetGridBkColor(RGB(UIARGB_GetRValue(m_dwGridBkColor), 
															UIARGB_GetGValue(m_dwGridBkColor), 
															UIARGB_GetBValue(m_dwGridBkColor)));
		Refresh();
	}
	
}

DWORD CGridCtrlUI::GetGridBkColor()
{
	return m_dwGridBkColor;
}

void  CGridCtrlUI::SetGridLineColor(DWORD dwColor)
{
	m_dwGridLineColor = dwColor;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetGridLineColor(RGB(UIARGB_GetRValue(m_dwGridLineColor), 
																UIARGB_GetGValue(m_dwGridLineColor), 
																UIARGB_GetBValue(m_dwGridLineColor)));
		Refresh();
	}
}

DWORD CGridCtrlUI::GetGridLineColor()
{
	return m_dwGridLineColor;
}

void CGridCtrlUI::SetEnableHighLightBkColor(bool bValue)
{
	m_bHighLightBkColor = bValue;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetEnableHighLightBkColor(m_bHighLightBkColor);
	}
}

bool CGridCtrlUI::GetEnableHighLightBkColor()
{
	return m_bHighLightBkColor;
}

void CGridCtrlUI::SetHighLightBkColor(DWORD dwColor)		
{
	m_crHighLightBkColor = dwColor;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetHighLightBkColor(RGB(UIARGB_GetRValue(m_crHighLightBkColor), 
			UIARGB_GetGValue(m_crHighLightBkColor), 
			UIARGB_GetBValue(m_crHighLightBkColor)));
		Refresh();
	}
}

DWORD CGridCtrlUI::GetHighLightBkColor()	
{
	return m_crHighLightBkColor;
}

void CGridCtrlUI::SetEnableHighLightTextClr(bool bValue)
{
	m_bHighLightTextClr = bValue;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetEnableHighLightTextColor(m_bHighLightTextClr);
	}
}

bool CGridCtrlUI::GetEnableHighLightTextClr()
{
	return m_bHighLightTextClr;
}

void CGridCtrlUI::SetHighLightTextClr(DWORD dwColor)
{
	m_crHighLightTextClr = dwColor;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetHighLightTextClr(RGB(UIARGB_GetRValue(m_crHighLightTextClr), 
			UIARGB_GetGValue(m_crHighLightTextClr), 
			UIARGB_GetBValue(m_crHighLightTextClr)));
		Refresh();
	}
}

DWORD CGridCtrlUI::GetHighLightTextClr()
{
	return m_crHighLightTextClr;
}

void CGridCtrlUI::SetFocusCellBorder(bool bBorder)
{
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetFrameFocusCell(bBorder);
}

bool CGridCtrlUI::GetFocusCellBorder()
{
	return m_bFocusCellBorder;
}

void CGridCtrlUI::SetFocusCellBorderColor(DWORD dwColor)
{
	m_dwFocusCellBorderColor = dwColor;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetFrameFocusCellColor(RGB(UIARGB_GetRValue(m_dwFocusCellBorderColor), 
			UIARGB_GetGValue(m_dwFocusCellBorderColor), 
			UIARGB_GetBValue(m_dwFocusCellBorderColor)));
		Refresh();
	}
}

DWORD CGridCtrlUI::GetFocusCellBorderColor()
{
	return m_dwFocusCellBorderColor;
}
//////////////////////////////////////////////////////////////////////////
void CGridCtrlUI::Refresh()
{
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->Refresh();
}

void CGridCtrlUI::SetAutoRedraw(bool bAuto)
{
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetAutoReDraw(bAuto);
}

void CGridCtrlUI::ExpandColumnsToFit(BOOL bExpandFixed)
{
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
		((CGridCtrlWnd *)m_pWindow)->m_pGrid->ExpandColumnsToFit(bExpandFixed);
}

int  CGridCtrlUI::InsertRow(int rowPosition)
{
	int row = -1;
	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		row = ((CGridCtrlWnd *)m_pWindow)->m_pGrid->InsertRow(_T(""), rowPosition);
		if(row >= 0)
		{
			((CGridCtrlWnd *)m_pWindow)->m_pGrid->SetRowHeight(row, GetRowHeight());
		}
	}

	return row;
}

void CGridCtrlUI::SetHeaderTexts(LPCTSTR pstrValue)
{
	for (int i=0; i<m_arrHeaderText.GetSize(); i++)
	{
		CDuiString *pStr = (CDuiString *)m_arrHeaderText.GetAt(i);
		delete pStr;
	}
	m_arrHeaderText.Empty();

	while( *pstrValue != _T('\0') ) 
	{
		CDuiString sValue;
		while( *pstrValue == _T(',')) pstrValue = ::CharNext(pstrValue);

		while( *pstrValue != _T('\0') && *pstrValue != _T(',')) {
			LPTSTR pstrTemp = ::CharNext(pstrValue);
			while( pstrValue < pstrTemp) {
				sValue += *pstrValue++;
			}
		}

		CDuiString *pStr = new CDuiString;
		*pStr = sValue;
		m_arrHeaderText.Add(pStr);	
	}

	if(m_pWindow && ((CGridCtrlWnd *)m_pWindow)->m_pGrid)
	{
		CGridCtrl *pGrid = ((CGridCtrlWnd *)m_pWindow)->m_pGrid;
		for(int i=0; i<m_arrHeaderText.GetSize(); i++)
		{
			CDuiString *pstr = (CDuiString *)m_arrHeaderText.GetAt(i);
			if(i+1< pGrid->GetColumnCount())
			{
				pGrid->Cell(0,i+1).SetText(*pstr);
			}
		}
		Refresh();
	}
}

void CGridCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{		
	if( _tcsicmp(pstrName, _T("virtualgrid")) == 0 ) 
		SetVirtualGrid(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("rowcount")) == 0 ) 
		SetRowCount(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("columncount")) == 0 ) 
		SetColumnCount(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("fixedrowcount")) == 0 ) 
		SetFixedRowCount(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("fixedcolumncount")) == 0 ) 
		SetFixedColumnCount(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("fixedcolumntext")) == 0 ) 
		SetHeaderTexts(pstrValue);
	else if( _tcsicmp(pstrName, _T("rowheight")) == 0 ) 
		SetRowHeight(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("editable")) == 0 ) 
		SetEditable(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("singlerowselection")) == 0 ) 
		SetSingleRowSelection(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("listmode")) == 0 ) 
		SetListMode(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("viewlistnumber")) == 0 ) 
		SetViewListNumber(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("textcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("gridbkcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetGridBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("gridlinecolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetGridLineColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("enablehighlightbkcolor")) == 0 ) 
		SetEnableHighLightBkColor(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("highlightbkcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetHighLightBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("enablehighlighttextColor")) == 0 ) 
		SetEnableHighLightTextClr(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("highlighttextColor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetHighLightTextClr(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("enablefocuscellborder")) == 0 ) 
		SetFocusCellBorder(_tcsicmp(pstrValue, _T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("focuscellbordercolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFocusCellBorderColor(clrColor);
	}
	else 
	{
		__super::SetAttribute(pstrName, pstrValue);
	}
}

void CGridCtrlUI::OnGridNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)lParam;
	switch (pItem->hdr.code)
	{
	case NM_CLICK:
		::PostMessage(GetManager()->GetPaintWindow(), WM_LBUTTONDOWN, (WPARAM)pItem->hdr.hwndFrom, lParam);
		::PostMessage(GetManager()->GetPaintWindow(), WM_LBUTTONUP, (WPARAM)pItem->hdr.hwndFrom, lParam);
		GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case NM_RCLICK:
		{
			Grid().SelectRow(pItem->iRow);
			GetManager()->SendNotify(this, DUI_MSGTYPE_RCLICK, (WPARAM)pItem->hdr.hwndFrom, lParam);
		}
		break;
	case NM_DBLCLK:
		GetManager()->SendNotify(this, DUI_MSGTYPE_DBCLICK, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case GVN_BEGINLABELEDIT:
		GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_STARTEDIT, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case GVN_ENDLABELEDIT:
		GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_ENDEDIT, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case GVN_SELCHANGING:
		GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_STARTSELCHANGE, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case GVN_SELCHANGED:
		GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_ENDSELCHANGE, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case CBS_DROPDOWNLIST:
		GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_DROPDOWNLIST, (WPARAM)pItem->hdr.hwndFrom, lParam);
		break;
	case GVN_GETDISPINFO:
		{
			GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
			if(Grid().IsCellFixed(pDispInfo->item.row, pDispInfo->item.col))
			{
				//固定行填入预设的文本
				if(pDispInfo->item.row == 0 && pDispInfo->item.col > 0)
				{
					if(m_arrHeaderText.GetSize() >= pDispInfo->item.col)
					{
						CDuiString *pstr = (CDuiString *)m_arrHeaderText.GetAt(pDispInfo->item.col-1);
						pDispInfo->item.strText = pstr->GetData();
					}
				}
			}
			else
			{
				GetManager()->SendNotify(this, DUI_MSGTYPE_GRID_GETDISPINFO, (WPARAM)pItem->hdr.hwndFrom, lParam);
			}
		}
		break;
	default:
		break;
	}
}

}