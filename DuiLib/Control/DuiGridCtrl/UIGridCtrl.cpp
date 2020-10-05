#include "stdafx.h"
#include "UIGridCtrl.h"

namespace DuiLib {
	//add by liqs99 ±Ì∏Ò¿‡
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	static CGridCellUI grid_cell_null;

	IMPLEMENT_DUICONTROL(CGridCtrlUI)
	CGridCtrlUI::CGridCtrlUI() : m_pGridHeader(NULL), m_pGridBody(NULL), m_bFixedScrollbar(false)//,
		//m_TGridInfo.dwHeaderBkColor(0xFFF4F5F9),
		//m_TGridInfo.dwLineColor(0xFFC0C0C0)
	{
		m_TGridInfo.nRowCount = 0;
		m_TGridInfo.nColumnCount = 0;
		m_TGridInfo.nFixedRowCount = 0;
		m_TGridInfo.nFixedColumnCount = 0;
		for(int i=0; i<DUI_GRID_MAX_COLUMN; i++) 
		{
			m_TGridInfo.rcColumn[i] = CDuiRect(0,0,0,0);
		}
		m_TGridInfo.dwHeaderBkColor =0xFFF4F5F9;
		m_TGridInfo.dwLineColor = 0xFFC0C0C0;

		m_pGridHeader = new CGridHeaderUI;
		m_pGridHeader->SetOwner(this);
		m_pGridBody = new CGridBodyUI;
		m_pGridBody->SetOwner(this);
		//m_pGridBody->SetBkColor(0xffa6caf0);

		Add(m_pGridHeader);
		Add(m_pGridBody);
	}

	LPCTSTR CGridCtrlUI::GetClass() const
	{
		return _T("GridCtrlUI");
	}

	UINT CGridCtrlUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	LPVOID CGridCtrlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("GridCtrl")) == 0 ) return static_cast<CGridCtrlUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CGridCtrlUI::DoInit()
	{

	}

	void CGridCtrlUI::DoEvent(TEventUI& event)
	{
		__super::DoEvent(event);
	}

	void CGridCtrlUI::Notify(TNotifyUI& msg)
	{
		
	}

	bool CGridCtrlUI::Add(CControlUI* pControl)
	{
		if( pControl->GetInterface(_T("GridHeader")) != NULL ) 
		{
			return __super::Add(pControl);
		}

		if( pControl->GetInterface(_T("GridBody")) != NULL ) 
		{
			return __super::Add(pControl);
		}

		if( pControl->GetInterface(_T("GridFixedRow")) != NULL ) 
		{
			CGridFixedRowUI *pFixedRow = static_cast<CGridFixedRowUI*>(pControl);
			m_TGridInfo.nRowCount++;
			m_TGridInfo.nFixedRowCount++;
			pFixedRow->SetOwner(this);
			m_TGridInfo.nColumnCount = MAX(m_TGridInfo.nColumnCount, pFixedRow->GetCount());
			return m_pGridHeader->Add(pControl);
		}
		if( pControl->GetInterface(_T("GridTextRow")) != NULL ) 
		{
			CGridTextRowUI *pRow = static_cast<CGridTextRowUI*>(pControl);
			m_TGridInfo.nRowCount++;
			pRow->SetOwner(this);
			return m_pGridBody->Add(pControl);
		}

		return false;
		//return CVerticalLayoutUI::Add(pControl);
	}

	void CGridCtrlUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		__super::SetPos(rc, bNeedInvalidate);

		CGridFixedRowUI *pFixedRow = (CGridFixedRowUI *)m_pGridHeader->GetItemAt(0);
		if(pFixedRow)
		{
			for( int it2 = 0; it2 < pFixedRow->GetCount(); it2++ ) 
			{
				CControlUI* pControl = static_cast<CControlUI*>(pFixedRow->GetItemAt(it2));
				m_TGridInfo.rcColumn[it2] = pControl->GetPos();
			}
		}		
	}

	int CGridCtrlUI::InsertRow()
	{
		CGridTextRowUI *pRow = new CGridTextRowUI;
		if(!Add(pRow)) return -1;

		return m_TGridInfo.nRowCount-1;
	}

	TGridInfoUI *CGridCtrlUI::GetGridInfo()
	{
		return &m_TGridInfo;
	}

	CGridCellUI *CGridCtrlUI::GetCell(int row, int col)
	{
		if(row<0 || col<0 || row >= m_TGridInfo.nRowCount || col >= m_TGridInfo.nColumnCount)
			return NULL;

		if(row < m_pGridHeader->GetCount())
		{
			CControlUI *pCtrl = m_pGridHeader->GetItemAt(row);
			CGridFixedRowUI *pRow = static_cast<CGridFixedRowUI*>(m_pGridHeader->GetItemAt(row));
			if(!pRow)	return NULL;
			//CGridFixedCellUI *pFixedCell = (CGridFixedCellUI *)pRow->GetCell(col);
			//return pFixedCell;
			return pRow->GetCell(col);
		}

		CGridRowUI *pRow = static_cast<CGridRowUI*>(m_pGridBody->GetItemAt(row - m_pGridHeader->GetCount()));
		if(!pRow)	return NULL;
		return (CGridCellUI *)pRow->GetCell(col);
	}

	CGridCellUI &CGridCtrlUI::Cell(int row, int col)
	{	
		CGridCellUI *pCell = GetCell(row, col);
		if(pCell)
		{
			return *pCell;
		}
		return grid_cell_null;
	}

	int CGridCtrlUI::GetRowCount()
	{
		return m_TGridInfo.nRowCount;
	}

	int CGridCtrlUI::GetColumnCount()
	{
		return m_TGridInfo.nColumnCount;
	}

	bool CGridCtrlUI::IsFixedScrollbar()
	{
		return m_bFixedScrollbar;
	}

	void CGridCtrlUI::SetFixedScrollbar(bool bFixed)
	{
		m_bFixedScrollbar = bFixed;
		Invalidate();
	}

	void CGridCtrlUI::SetHeaderBackColor(DWORD dwColor)
	{
		if(m_TGridInfo.dwHeaderBkColor == dwColor)	return;
		m_TGridInfo.dwHeaderBkColor = dwColor;
	}

	DWORD CGridCtrlUI::GetHeaderBackColor()
	{
		return m_TGridInfo.dwHeaderBkColor;
	}

	void CGridCtrlUI::SetLineColor(DWORD dwColor)
	{
		if(m_TGridInfo.dwLineColor == dwColor)	return;
		m_TGridInfo.dwLineColor = dwColor;
	}

	DWORD CGridCtrlUI::GetLineColor()
	{
		return m_TGridInfo.dwLineColor;
	}

	void CGridCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("headerbkcolor")) == 0 ) 
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHeaderBackColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("linecolor")) == 0 ) 
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetLineColor(clrColor);
		}
		else 
		{
			__super::SetAttribute(pstrName, pstrValue);
		}
	}
} // namespace DuiLib

