#include "stdafx.h"
#include "UIGridCtrl.h"

namespace DuiLib {
	//add by liqs99 ±Ì∏Ò¿‡
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	static CGridCellUI grid_cell_null;

	IMPLEMENT_DUICONTROL(CGridCtrlUI)
	CGridCtrlUI::CGridCtrlUI() : m_pGridHeader(NULL), m_pGridBody(NULL)
	{
		m_TGridInfo.nRowCount = 0;
		m_TGridInfo.nColumnCount = 0;
		m_TGridInfo.dwHeaderBkColor =0xFFF4F5F9;
		m_TGridInfo.dwLineColor = 0xFFC0C0C0;

		m_pGridHeader = new CGridFixedRowUI;
		m_pGridBody = new CGridBodyUI;

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
		if( pControl->GetInterface(_T("GridFixedRow")) != NULL ) 
		{
			if( m_pGridHeader != pControl && m_pGridHeader->GetCount() == 0 ) 
			{
				CVerticalLayoutUI::Remove(m_pGridHeader);
				m_TGridInfo.nRowCount--;
				m_pGridHeader = static_cast<CGridFixedRowUI*>(pControl);
			}
			m_TGridInfo.nRowCount++;
			m_TGridInfo.nColumnCount = m_pGridHeader->GetCount();
			m_pGridHeader->SetOwner(this);
			return CVerticalLayoutUI::AddAt(pControl, 0);
		}

		if( _tcsstr(pControl->GetClass(), _T("GridFixedCellUI")) != NULL ) 
		{
			bool ret = m_pGridHeader->Add(pControl);
			m_TGridInfo.nColumnCount++;
			return ret;
		}

		return CVerticalLayoutUI::Add(pControl);
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
			CGridFixedRowUI *pRow = static_cast<CGridFixedRowUI*>(m_pGridHeader->GetItemAt(row));
			if(!pRow)	return NULL;
			return (CGridCellUI *)pRow->GetItemAt(col);
		}

		CGridRowUI *pRow = static_cast<CGridRowUI*>(m_pGridBody->GetItemAt(row));
		if(!pRow)	return NULL;
		return (CGridCellUI *)pRow->GetItemAt(row - m_pGridHeader->GetCount());
	}

	CGridCellUI &CGridCtrlUI::Cell(int row, int col)
	{	
		CGridCellUI *pCell = GetCell(row, col);
		if(pCell)
		{
			return *GetCell(row, col);
		}
		return grid_cell_null;
	}

	void CGridCtrlUI::SetRowCount(int rowcount)
	{
		if(m_TGridInfo.nRowCount == rowcount) return;

		m_TGridInfo.nRowCount = rowcount;
	}

	int  CGridCtrlUI::GetRowCount()
	{
		return m_TGridInfo.nRowCount;
	}

	void CGridCtrlUI::SetColumnCount(int columncount)
	{
		if(m_TGridInfo.nColumnCount == columncount)	return;
		m_TGridInfo.nColumnCount = columncount;

	}

	int CGridCtrlUI::GetColumnCount()
	{
		return m_TGridInfo.nColumnCount;
	}

	void CGridCtrlUI::SetFixedRowCount(int rowcount)
	{
		m_TGridInfo.nFixedColumnCount = rowcount;
	}

	int  CGridCtrlUI::GetFixedRowCount()
	{
		return m_TGridInfo.nFixedRowCount;
	}

	void CGridCtrlUI::SetFixedColumnCount(int columncount)
	{
		m_TGridInfo.nFixedColumnCount = columncount;
	}

	int CGridCtrlUI::GetFixedColumnCount()
	{
		return m_TGridInfo.nFixedColumnCount;
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
		if( _tcsicmp(pstrName, _T("rowcount")) == 0 ) 
		{
			SetRowCount(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("columncount")) == 0 ) 
		{
			SetColumnCount(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("headerbkcolor")) == 0 ) 
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

