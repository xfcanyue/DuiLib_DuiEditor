#include "StdAfx.h"
#include "UITableLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CTableLayoutUI)
	CTableLayoutUI::CTableLayoutUI()
	{
		m_nDefRowHeight = 30;
		m_bAutoCalcHeight = true;
		for (int i=0; i<MAX_TABLE_COLUMN_COUNT; i++)
		{
			m_aColWidth[i] = 0;
			m_aColMinWidth[i] = 0;
			m_aColMaxWidth[i] = 9999;
			m_aColAutoCalcWidth[i] = false;
			m_aColInset[i] = CDuiRect(0,0,0,0);
			m_aColTextPadding[i] = CDuiRect(0,0,0,0);
		}
	}

	LPCTSTR CTableLayoutUI::GetClass() const
	{
		return _T("TableLayoutUI");
	}

	LPVOID CTableLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_TABLELAYOUT) == 0 ) return static_cast<CTableLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CTableLayoutUI::SetColWidth(int col, int nWidth)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColWidth[col] = nWidth;
	}

	int CTableLayoutUI::GetColWidth(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return 0;
		return m_aColWidth[col];
	}

	void CTableLayoutUI::SetColMinWidth(int col, int nWidth)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColMinWidth[col] = nWidth;
	}

	int CTableLayoutUI::GetColMinWidth(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return 0;
		return m_aColMinWidth[col];
	}

	void CTableLayoutUI::SetColMaxWidth(int col, int nWidth)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColMaxWidth[col] = nWidth;
	}

	int CTableLayoutUI::GetColMaxWidth(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return 0;
		return m_aColMaxWidth[col];
	}

	bool CTableLayoutUI::IsColAutoCalcWidth(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return 0;
		return m_aColAutoCalcWidth[col];
	}

	void CTableLayoutUI::SetColAutoCalcWidth(int col, bool bAutoCalcWidth)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColAutoCalcWidth[col] = bAutoCalcWidth;
	}

	RECT CTableLayoutUI::GetColInset(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return CDuiRect(0,0,0,0);
		return m_aColInset[col];
	}

	void CTableLayoutUI::SetColInset(int col, RECT rcInset)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColInset[col] = rcInset;
	}

	RECT CTableLayoutUI::GetColTextPadding(int col) const
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return CDuiRect(0,0,0,0);
		return m_aColTextPadding[col];
	}

	void CTableLayoutUI::SetColTextPadding(int col, RECT rc)
	{
		if(col < 0 || col >= MAX_TABLE_COLUMN_COUNT) return;
		m_aColTextPadding[col] = rc;
	}

	void CTableLayoutUI::DoInit()
	{

	}

	void CTableLayoutUI::DoEvent(TEventUI& event)
	{
		__super::DoEvent(event);
	}

	void CTableLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

		if( m_items.GetSize() == 0) {
			ProcessScrollBar(rc, 0, 0);
			return;
		}

		// Determine the minimum size
		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
			szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
			szAvailable.cy += m_pVerticalScrollBar->GetScrollRange();

		int cxNeeded = 0;
		int nAdjustables = 0;
		int cyFixed = 0;
		int nEstimateNum = 0;
		SIZE szControlAvailable;
		int iControlMaxWidth = 0;
		int iControlMaxHeight = 0;
		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) continue;
			szControlAvailable = szAvailable;
			RECT rcPadding = pControl->GetPadding();
			szControlAvailable.cx -= rcPadding.left + rcPadding.right;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if( sz.cy == 0 ) {
				nAdjustables++;
			}
			else {
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;

			sz.cx = MAX(sz.cx, 0);
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			cxNeeded = MAX(cxNeeded, sz.cx + rcPadding.left + rcPadding.right);
			nEstimateNum++;
		}
		cyFixed += (nEstimateNum - 1) * m_iChildPadding;

		// Place elements
		int cyNeeded = 0;
		int cyExpand = 0;
		if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
		// Position the elements
		SIZE szRemaining = szAvailable;
		int iPosY = rc.top;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
			iPosY -= m_pVerticalScrollBar->GetScrollPos();
		}
		else {
			// 子控件垂直对其方式
			if(nAdjustables <= 0) {
				UINT iChildAlign = GetChildVAlign(); 
				if (iChildAlign == DT_VCENTER) {
					iPosY += (szAvailable.cy -cyFixed) / 2;
				}
				else if (iChildAlign == DT_BOTTOM) {
					iPosY += (szAvailable.cy - cyFixed);
				}
			}
		}
		int iEstimate = 0;
		int iAdjustable = 0;
		int cyFixedRemaining = cyFixed;
		for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) {
				SetFloatPos(it2);
				continue;
			}

			iEstimate += 1;
			RECT rcPadding = pControl->GetPadding();
			szRemaining.cy -= rcPadding.top;

			szControlAvailable = szRemaining;
			szControlAvailable.cx -= rcPadding.left + rcPadding.right;
			iControlMaxWidth = pControl->GetFixedWidth();
			iControlMaxHeight = pControl->GetFixedHeight();
			if (iControlMaxWidth <= 0) iControlMaxWidth = pControl->GetMaxWidth(); 
			if (iControlMaxHeight <= 0) iControlMaxHeight = pControl->GetMaxHeight();
			if (szControlAvailable.cx > iControlMaxWidth) szControlAvailable.cx = iControlMaxWidth;
			if (szControlAvailable.cy > iControlMaxHeight) szControlAvailable.cy = iControlMaxHeight;
			cyFixedRemaining = cyFixedRemaining - (rcPadding.top + rcPadding.bottom);
			if (iEstimate > 1) cyFixedRemaining = cyFixedRemaining - m_iChildPadding;
			SIZE sz = pControl->EstimateSize(szControlAvailable);
			if( sz.cy == 0 ) {
				iAdjustable++;
				sz.cy = cyExpand;
				// Distribute remaining to last element (usually round-off left-overs)
				if( iAdjustable == nAdjustables ) {
					sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
				} 
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			}
			else {
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
				cyFixedRemaining -= sz.cy;
			}

			sz.cx = MAX(sz.cx, 0);
			if( sz.cx == 0 ) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
			if( sz.cx > szControlAvailable.cx ) sz.cx = szControlAvailable.cx;
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();

			UINT iChildAlign = GetChildAlign(); 
			if (iChildAlign == DT_CENTER) {
				int iPosX = (rc.right + rc.left) / 2;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
					iPosX += m_pHorizontalScrollBar->GetScrollRange() / 2;
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX - sz.cx/2, iPosY + rcPadding.top, iPosX + sz.cx - sz.cx/2, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else if (iChildAlign == DT_RIGHT) {
				int iPosX = rc.right;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
					iPosX += m_pHorizontalScrollBar->GetScrollRange();
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX - rcPadding.right - sz.cx, iPosY + rcPadding.top, iPosX - rcPadding.right, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}
			else {
				int iPosX = rc.left;
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
					iPosX -= m_pHorizontalScrollBar->GetScrollPos();
				}
				RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top };
				pControl->SetPos(rcCtrl, false);
			}

			iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
			cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
			szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
		}
		cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

		// Process the scrollbar
		ProcessScrollBar(rc, cxNeeded, cyNeeded);
	}

	void CTableLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("rowheight")) == 0 )
		{
			SetDefRowHeight(_ttoi(pstrValue));
		}
		else
			__super::SetAttribute(pstrName, pstrValue);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CTRUI)
	CTRUI::CTRUI()
	{
	}

	LPCTSTR CTRUI::GetClass() const
	{
		return _T("TRUI");
	}

	LPVOID CTRUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_TABLEROW) == 0 ) return static_cast<CTRUI*>(this);
		return __super::GetInterface(pstrName);
	}

	UINT CTRUI::GetControlFlags() const
	{
		return 0;
	}


	CTableLayoutUI *CTRUI::GetTable() const
	{
		CTableLayoutUI *pTable = NULL;

		if(GetParent() && GetParent()->GetInterface(DUI_CTR_TABLELAYOUT))
		{
			pTable = static_cast<CTableLayoutUI *>(GetParent());
		}
		else if(GetExtraParent() && GetExtraParent()->GetInterface(DUI_CTR_TABLELAYOUT))
		{
			pTable = static_cast<CTableLayoutUI *>(GetExtraParent());
		}

		return pTable;
	}

	bool CTRUI::Add(CControlUI* pControl)
	{
		if(!pControl->GetInterface(DUI_CTR_TABLECOL))
			return false;

		return __super::Add(pControl);
	}

	bool CTRUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if(!pControl->GetInterface(DUI_CTR_TABLECOL))
			return false;

		return __super::AddAt(pControl, iIndex);
	}

	void CTRUI::DoInit()
	{

	}

	SIZE CTRUI::EstimateSize(SIZE szAvailable)
	{
		if(GetFixedHeight() == 0 && GetParent() && GetParent()->GetInterface(DUI_CTR_TABLELAYOUT))
		{
			SIZE sz = {0};
			CTableLayoutUI *pTable = (CTableLayoutUI *)GetParent();
			sz.cy = pTable->GetDefRowHeight();
	
			if(IsAutoCalcWidth())
			{
				for (int it=0; it<GetCount(); it++)
				{
					SIZE szControl = {0};
					CControlUI *pControl = GetItemAt(it);
					if(!pControl->IsVisible()) continue;
					szControl = pControl->EstimateSize(szAvailable);
					RECT padding = pControl->GetPadding();
					sz.cx += szControl.cx + padding.left + padding.right;
				}
			}

			return sz;
		}
		return __super::EstimateSize(szAvailable);
	}

	void CTRUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		__super::SetAttribute(pstrName, pstrValue);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CTDUI)
	CTDUI::CTDUI()
	{
		SetTextStyle(DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}

	LPCTSTR CTDUI::GetClass() const
	{
		return _T("TDUI");
	}

	LPVOID CTDUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_TABLECOL) == 0 ) return static_cast<CTDUI*>(this);
		return __super::GetInterface(pstrName);
	}

	UINT CTDUI::GetControlFlags() const
	{
		return 0;
	}

	CTRUI *CTDUI::GetRow() const
	{
		CTRUI *pRow = NULL;
		if(GetParent() && GetParent()->GetInterface(DUI_CTR_TABLEROW))
		{
			pRow = static_cast<CTRUI *>(GetParent());
		}
		else if(GetExtraParent() && GetExtraParent()->GetInterface(DUI_CTR_TABLEROW))
		{
			pRow = static_cast<CTRUI *>(GetExtraParent());
		}
		return pRow;
	}

	CTableLayoutUI *CTDUI::GetTable() const
	{
		CTableLayoutUI *pTable = NULL;
		CTRUI *pRow = GetRow();
		if(!pRow) return NULL;

		if(pRow->GetParent() && pRow->GetParent()->GetInterface(DUI_CTR_TABLELAYOUT))
		{
			pTable = static_cast<CTableLayoutUI *>(GetParent()->GetParent());
		}
		else if(pRow->GetExtraParent() && pRow->GetExtraParent()->GetInterface(DUI_CTR_TABLELAYOUT))
		{
			pTable = static_cast<CTableLayoutUI *>(pRow->GetExtraParent());
		}

		return pTable;
	}

	void CTDUI::SetFixedWidth(int nWidth)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColWidth(pRow->GetItemIndex(this), nWidth);
		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	int CTDUI::GetFixedWidth() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return 0;

		return pTable->GetColWidth(pRow->GetItemIndex((CTDUI *)this));
	}

	void CTDUI::SetMinWidth(int nWidth)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColMinWidth(pRow->GetItemIndex(this), nWidth);

		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	int CTDUI::GetMinWidth() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return 0;

		return pTable->GetColMinWidth(pRow->GetItemIndex((CTDUI *)this));
	}

	void CTDUI::SetMaxWidth(int nWidth)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColMaxWidth(pRow->GetItemIndex(this), nWidth);

		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	int CTDUI::GetMaxWidth() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return 0;

		return pTable->GetColMaxWidth(pRow->GetItemIndex((CTDUI *)this));
	}

	bool CTDUI::IsAutoCalcWidth() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return 0;

		return pTable->IsColAutoCalcWidth(pRow->GetItemIndex((CTDUI *)this));
	}

	void CTDUI::SetAutoCalcWidth(bool bAutoCalcWidth)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColAutoCalcWidth(pRow->GetItemIndex(this), bAutoCalcWidth);

		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	RECT CTDUI::GetInset() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return CDuiRect(0,0,0,0);

		return pTable->GetColInset(pRow->GetItemIndex((CTDUI *)this));
	}

	void CTDUI::SetInset(RECT rcInset)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColInset(pRow->GetItemIndex(this), rcInset);

		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	RECT CTDUI::GetTextPadding() const
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return CDuiRect(0,0,0,0);

		return pTable->GetColTextPadding(pRow->GetItemIndex((CTDUI *)this));
	}

	void CTDUI::SetTextPadding(RECT rc)
	{
		CTRUI *pRow = GetRow();
		CTableLayoutUI *pTable = GetTable();
		if(!pRow || !pTable) return;

		pTable->SetColTextPadding(pRow->GetItemIndex(this), rc);

		if(GetParent() && GetParent()->GetParent())
			pTable->NeedUpdate();
	}

	void CTDUI::DoInit()
	{

	}

	SIZE CTDUI::EstimateSize(SIZE szAvailable)
	{
		//return __super::EstimateSize(szAvailable);
		if(IsAutoCalcWidth())
		{
			int needWidth = 0;

			if(GetCount() > 0)
			{
				for (int it=0; it<GetCount(); it++)
				{
					SIZE szControl = {0};
					CControlUI *pControl = GetItemAt(it);
					if(!pControl->IsVisible()) continue;
					szControl = pControl->EstimateSize(szAvailable);
					needWidth += szControl.cx;
				}
			}
			else
			{
				CDuiString sText = GetText();
				RECT rcTextPadding = GetTextPadding();
				RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
				int nLinks = 0;
				if( m_bShowHtml ) CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, m_dwTextColor, NULL, NULL, nLinks, m_iFont, DT_CALCRECT | m_uTextStyle);
				else CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, m_dwTextColor, m_iFont, DT_CALCRECT | m_uTextStyle);
				needWidth = MulDiv(rcText.right - rcText.left + GetManager()->GetDPIObj()->Scale(rcTextPadding.left) + GetManager()->GetDPIObj()->Scale(rcTextPadding.right), 100, GetManager()->GetDPIObj()->GetScale());
			}

			RECT rcInset = GetInset();
			needWidth += rcInset.left + rcInset.right;
			if(needWidth > GetFixedWidth())
			{
				SetFixedWidth(needWidth);
			}
		}

		SIZE sz;
		sz.cx = GetFixedWidth();
		sz.cy = szAvailable.cy;
		return m_pManager->GetDPIObj()->Scale(sz);	
	}

	void CTDUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("width")) == 0 ) SetFixedWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("minwidth")) == 0 ) SetMinWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("maxwidth")) == 0 ) SetMaxWidth(_ttoi(pstrValue));
		else __super::SetAttribute(pstrName, pstrValue);
	}
}
