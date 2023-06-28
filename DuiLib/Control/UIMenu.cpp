#include "StdAfx.h"

#include "UIMenu.h"

namespace DuiLib {

	//modify by liqs99
	//1, lstrcpy 改为 _tcscpy
	/////////////////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CMenuUI)

	CMenuUI::CMenuUI():
	m_pWindow(NULL), m_nIconWidth(26), m_dwIconBkColor(0xFFE8EDEE)
	{
		m_ListInfo.dwDisabledBkColor = 0x00000000;

		if (GetHeader() != NULL)
			GetHeader()->SetVisible(false);
	}

	CMenuUI::~CMenuUI()
	{

	}

	LPCTSTR CMenuUI::GetClass() const
	{
		return _T("MenuUI");
	}

	LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("Menu")) == 0 ) return static_cast<CMenuUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	UINT CMenuUI::GetListType()
	{
		return LT_MENU;
	}

	void CMenuUI::DoEvent(TEventUI& event)
	{
		return CListUI::DoEvent(event);
	}

	bool CMenuUI::Add(CControlUI* pControl)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		for (int i = 0; i < pMenuItem->GetCount(); ++i)
		{
			if (pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{
				(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
			}
		}
		return CListUI::Add(pControl);
	}

	bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		for (int i = 0; i < pMenuItem->GetCount(); ++i)
		{
			if (pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{
				(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
			}
		}
		return CListUI::AddAt(pControl, iIndex);
	}

	int CMenuUI::GetItemIndex(CControlUI* pControl) const
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return -1;

		return CListUI::GetItemIndex(pControl);
	}

	bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		return CListUI::SetItemIndex(pControl, iIndex);
	}

	bool CMenuUI::Remove(CControlUI* pControl, bool bDoNotDestroy )
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		return CListUI::Remove(pControl);
	}

	SIZE CMenuUI::EstimateSize(SIZE szAvailable)
	{
		int cxFixed = 0;
		int cyFixed = 0;
		for( int it = 0; it < GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if( !pControl->IsVisible() ) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cyFixed += sz.cy;
			if( cxFixed < sz.cx )
				cxFixed = sz.cx;
		}

		for (int it = 0; it < GetCount(); it++) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if (!pControl->IsVisible()) continue;

			pControl->SetFixedWidth(MulDiv(cxFixed, 100, GetManager()->GetDPIObj()->GetScale()));
		}

		return CDuiSize(cxFixed, cyFixed);
	}

	void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("expandicon")) == 0){
			SetExpandIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("checkicon")) == 0){
			SetCheckIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconwidth")) == 0){
			SetIconWidth(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("iconbkcolor")) == 0){
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetIconBkColor(clrColor);
		}
		else
			CListUI::SetAttribute(pstrName, pstrValue);
	}

	void CMenuUI::SetExpandIcon(LPCTSTR strIcon) 
	{ 
		m_strExpandIcon = strIcon; 
	}
	CDuiString CMenuUI::GetExpandIcon() 
	{ 
		return m_strExpandIcon; 
	}
	void CMenuUI::SetCheckIcon(LPCTSTR strIcon) 
	{ 
		m_strCheckIcon = strIcon;
	}
	CDuiString CMenuUI::GetCheckIcon() 
	{ 
		return m_strCheckIcon; 
	}
	void CMenuUI::SetIconWidth(int nWidth)
	{
		m_nIconWidth = nWidth;
	}
	int CMenuUI::GetIconWidth()
	{
		return m_nIconWidth;
	}
	void CMenuUI::SetIconBkColor(DWORD dwColor)
	{
		m_dwIconBkColor = dwColor;
	}
	DWORD CMenuUI::GetIconBkColor()
	{
		return m_dwIconBkColor;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CMenuElementUI)

	CMenuElementUI::CMenuElementUI():
	m_pWindow(NULL),
		m_bDrawLine(false),
		m_dwLineColor(DEFAULT_LINE_COLOR),
		m_bCheckItem(false), m_bCheck(false),
		m_bShowExplandIcon(false)
	{
		m_cxyFixed.cy = ITEM_DEFAULT_HEIGHT;
		m_cxyFixed.cx = ITEM_DEFAULT_WIDTH;
		m_szIconSize.cy = ITEM_DEFAULT_ICON_SIZE;
		m_szIconSize.cx = ITEM_DEFAULT_ICON_SIZE;

		m_rcLinePadding.top = m_rcLinePadding.bottom = 0;
		m_rcLinePadding.left = DEFAULT_LINE_LEFT_INSET;
		m_rcLinePadding.right = DEFAULT_LINE_RIGHT_INSET;
	}

	CMenuElementUI::~CMenuElementUI()
	{}

	LPCTSTR CMenuElementUI::GetClass() const
	{
		return _T("MenuElementUI");
	}

	LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("MenuElement")) == 0 ) return static_cast<CMenuElementUI*>(this);    
		return CListContainerElementUI::GetInterface(pstrName);
	}

	bool CMenuElementUI::DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
	{
		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cy);
		RECT m_rcLinePadding = CMenuElementUI::m_rcLinePadding;
		GetManager()->GetDPIObj()->ScaleRect(&m_rcLinePadding);

		CDuiRect rcTemp;
		//if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;
		if( !rcTemp.Intersect(rcPaint, m_rcItem) ) return true;

		if(m_bDrawLine)
		{
			RECT rcLine = { m_rcItem.left +  m_rcLinePadding.left, m_rcItem.top + m_cxyFixed.cy/2, m_rcItem.right - m_rcLinePadding.right, m_rcItem.top + m_cxyFixed.cy/2 };
			pRender->DrawLine(rcLine, 1, m_dwLineColor);
		}
		else
		{
			//CMenuElementUI::DrawItemBk(hDC, m_rcItem);
			//DrawItemText(hDC, m_rcItem);
			//DrawItemIcon(hDC, m_rcItem);
			//DrawItemExpland(hDC, m_rcItem);
			//for (int i = 0; i < GetCount(); ++i)
			//{
			//	if (GetItemAt(i)->GetInterface(_T("MenuElement")) == NULL) {
			//		GetItemAt(i)->DoPaint(hDC, rcPaint);
			//	}
			//}

			UIClip clip;
			clip.GenerateClip(pRender, rcTemp);
			CMenuElementUI::DrawItemBk(pRender, m_rcItem);
			DrawItemText(pRender, m_rcItem);
			DrawItemBorder(pRender, rcTemp);
			DrawItemIcon(pRender, m_rcItem);
			DrawItemExpland(pRender, m_rcItem);

			if( m_items.GetSize() > 0 ) {
				RECT rc = m_rcItem;
				RECT rcInset = GetInset();
				rc.left += rcInset.left;
				rc.top += rcInset.top;
				rc.right -= rcInset.right;
				rc.bottom -= rcInset.bottom;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

				//if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
				if( !rcTemp.Intersect(rcPaint, rc) ) {
					for( int it = 0; it < m_items.GetSize(); it++ ) {
						CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
						if( pControl == pStopControl ) return false;
						if( !pControl->IsVisible() ) continue;
						if( pControl->GetInterface(_T("MenuElement")) != NULL ) continue;
						//if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
						if( !rcTemp.Intersect(rcPaint, pControl->GetPos()) ) continue;
						if( pControl->IsFloat() ) {
							//if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
							if( !rcTemp.Intersect(m_rcItem, pControl->GetPos()) ) continue;
							if( !pControl->Paint(pRender, rcPaint, pStopControl) ) return false;
						}
					}
				}
				else {
					UIClip childClip;
					childClip.GenerateClip(pRender, rcTemp);
					for( int it = 0; it < m_items.GetSize(); it++ ) {
						CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
						if( pControl == pStopControl ) return false;
						if( !pControl->IsVisible() ) continue;
						if( pControl->GetInterface(_T("MenuElement")) != NULL ) continue;
						//if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
						if( !rcTemp.Intersect(rcPaint, pControl->GetPos()) ) continue;
						if( pControl->IsFloat() ) {
							//if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
							if( !rcTemp.Intersect(m_rcItem, pControl->GetPos()) ) continue;
							childClip.UseOldClipBegin(pRender);
							if( !pControl->Paint(pRender, rcPaint, pStopControl) ) return false;
							childClip.UseOldClipEnd(pRender);
						}
						else {
							//if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
							if( !rcTemp.Intersect(rc, pControl->GetPos()) ) continue;
							if( !pControl->Paint(pRender, rcPaint, pStopControl) ) return false;
						}
					}
				}
			}

		}

		if( m_pVerticalScrollBar != NULL ) {
			if( m_pVerticalScrollBar == pStopControl ) return false;
			if (m_pVerticalScrollBar->IsVisible()) {
				//if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
				if( rcTemp.Intersect(rcPaint, m_pVerticalScrollBar->GetPos()) ) {
					if( !m_pVerticalScrollBar->Paint(pRender, rcPaint, pStopControl) ) return false;
				}
			}
		}

		if( m_pHorizontalScrollBar != NULL ) {
			if( m_pHorizontalScrollBar == pStopControl ) return false;
			if (m_pHorizontalScrollBar->IsVisible()) {
				//if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
				if( rcTemp.Intersect(rcPaint, m_pHorizontalScrollBar->GetPos()) ) {
					if( !m_pHorizontalScrollBar->Paint(pRender, rcPaint, pStopControl) ) return false;
				}
			}
		}
		return true;
	}

	void CMenuElementUI::DrawItemIcon(UIRender *pRender, const RECT& rcItem)
	{
		CDuiString strIcon = m_strIcon;
		if(GetCheckItem())	//如果是复选框
		{
			if(GetChecked())
			{
				//如果当前没有设置图标, 使用默认的
				if(m_strIcon.IsEmpty() && GetMenuUI()) 
				{
					strIcon = GetMenuUI()->GetCheckIcon();
				}
			}
			else
			{
				strIcon = m_strUnCheckIcon;
			}
		}
		else
		{
			strIcon = m_strIcon;
		}

		if(strIcon.IsEmpty()) return;

		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cy);

		SIZE m_szIconSize = CMenuElementUI::m_szIconSize;
		m_szIconSize.cx = GetManager()->GetDPIObj()->ScaleInt(m_szIconSize.cx);
		m_szIconSize.cy = GetManager()->GetDPIObj()->ScaleInt(m_szIconSize.cy);

		int nWidth = 0;
		if(GetMenuUI())
			nWidth = GetManager()->GetDPIObj()->ScaleInt(GetMenuUI()->GetIconWidth());
		int padding = (nWidth - m_szIconSize.cx) / 2;
		RECT rcDest =
		{
			padding,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2,
			padding + m_szIconSize.cx,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2 + m_szIconSize.cy
		};
		GetManager()->GetDPIObj()->ScaleRectBack(&rcDest);
		/*
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		RECT rcTextPadding = pInfo->rcTextPadding;
		GetManager()->GetDPIObj()->Scale(&rcTextPadding);

		int padding = (rcTextPadding.left - m_szIconSize.cx) / 2;
		RECT rcDest =
		{
			padding,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2,
			padding + m_szIconSize.cx,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2 + m_szIconSize.cy
		};
		GetManager()->GetDPIObj()->ScaleBack(&rcDest);
		*/

		CDuiString pStrImage;
		pStrImage.Format(_T("dest='%d,%d,%d,%d'"), rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
		//if(IsEnabled())
			DrawImage(pRender, strIcon, pStrImage);	
		//else
		//	DrawDisableItemIcon(hDC, strIcon, pStrImage);
	}

	void CMenuElementUI::DrawItemExpland(UIRender *pRender, const RECT& rcItem)
	{
		if (m_bShowExplandIcon && IsEnabled())
		{
			CDuiString strExplandIcon;
			strExplandIcon = GetManager()->GetDefaultAttributeList(_T("ExplandIcon"));
			if (strExplandIcon.IsEmpty()) 
			{
				if(GetMenuUI())
					strExplandIcon = GetMenuUI()->GetExpandIcon();
				if (strExplandIcon.IsEmpty()) 
					return;
			}
			SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
			m_cxyFixed.cx = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cx);
			m_cxyFixed.cy = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cy);
			int padding = GetManager()->GetDPIObj()->ScaleInt(ITEM_DEFAULT_EXPLAND_ICON_WIDTH) / 3;
			const TDrawInfo* pDrawInfo = GetManager()->GetDrawInfo((LPCTSTR)strExplandIcon, NULL);
			const UIImage *pImageInfo = GetManager()->GetImageEx(pDrawInfo->sImageName, NULL, 0);
			if (!pImageInfo) {
				return;
			}
			RECT rcDest =
			{
				m_cxyFixed.cx - pImageInfo->nWidth - padding,
				(m_cxyFixed.cy - pImageInfo->nHeight) / 2,
				m_cxyFixed.cx - pImageInfo->nWidth - padding + pImageInfo->nWidth,
				(m_cxyFixed.cy - pImageInfo->nHeight) / 2 + pImageInfo->nHeight
			};
			GetManager()->GetDPIObj()->ScaleRectBack(&rcDest);
			CDuiString pStrImage;
			pStrImage.Format(_T("dest='%d,%d,%d,%d'"), rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
			DrawImage(pRender, strExplandIcon, pStrImage);
		}
	}

	void CMenuElementUI::DrawItemText(UIRender *pRender, const RECT& rcItem)
	{
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		if( m_pOwner == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		DWORD iTextColor = pInfo->dwTextColor;
		if( IsHotState() ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}

		RECT rcText = rcItem;
 		RECT rcTextPadding = pInfo->rcTextPadding;
 		GetManager()->GetDPIObj()->ScaleRect(&rcTextPadding);
// 		rcText.left += rcTextPadding.left;
// 		rcText.right -= rcTextPadding.right;
// 		rcText.top += rcTextPadding.top;
// 		rcText.bottom -= rcTextPadding.bottom;
		
		pRender->DrawText(rcText, rcTextPadding, sText, iTextColor, \
			pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
	}

	void CMenuElementUI::DrawItemBorder(UIRender *pRender, const RECT& rcItem)
	{
		DWORD dwColor = 0;

		if(dwColor == 0 && !IsEnabled() && GetDisableBorderColor() != 0)
			dwColor = GetDisableBorderColor();

		if(dwColor == 0 && IsFocused() && GetFocusBorderColor() != 0)
			dwColor = GetFocusBorderColor();

		if(dwColor == 0 && IsHotState() && GetHotBorderColor() != 0)
			dwColor = GetHotBorderColor();

		if(dwColor == 0) 
			dwColor = GetBorderColor();

		if(dwColor == 0) 
			return;

		pRender->DrawBorder(rcItem, GetBorderSize(), GetBorderRound(), 
			GetBorderRectSize(), GetAdjustColor(dwColor), GetBorderStyle());
		/*
		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if(m_dwBorderColor != 0 || m_dwFocusBorderColor != 0 || m_dwHotBorderColor != 0) {
			//画圆角边框
			if(nBorderSize > 0 && ( cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 )) {
				if (IsFocused() && m_dwFocusBorderColor != 0)
					pRender->DrawRoundRect(rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);	
				//////////////////////////////////////////////////////////////////////////
				//add by liqs99
				else if (IsHotState() && m_dwHotBorderColor != 0)
					pRender->DrawRoundRect(rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
				//////////////////////////////////////////////////////////////////////////
				else
					pRender->DrawRoundRect(rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
			}
			else {
				if (IsFocused() && m_dwFocusBorderColor != 0 && nBorderSize > 0) { 
					pRender->DrawRect(rcItem, nBorderSize, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);
				}
				//////////////////////////////////////////////////////////////////////////
				//add by liqs99
				else if (IsHotState() && m_dwHotBorderColor != 0 && nBorderSize > 0) { 
					pRender->DrawRect(rcItem, nBorderSize, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
				}
				//////////////////////////////////////////////////////////////////////////
				else if(rcBorderSize.left > 0 || rcBorderSize.top > 0 || rcBorderSize.right > 0 || rcBorderSize.bottom > 0) {
					RECT rcBorder;

					if(rcBorderSize.left > 0){
						rcBorder		= rcItem;
						rcBorder.right	= rcBorder.left;
						pRender->DrawLine(rcBorder,rcBorderSize.left,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.top > 0){
						rcBorder		= rcItem;
						rcBorder.bottom	= rcBorder.top;
						pRender->DrawLine(rcBorder,rcBorderSize.top,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.right > 0){
						rcBorder		= rcItem;
						rcBorder.right -= 1;
						rcBorder.left	= rcBorder.right;
						pRender->DrawLine(rcBorder,rcBorderSize.right,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.bottom > 0){
						rcBorder		= rcItem;
						rcBorder.bottom -= 1;
						rcBorder.top	= rcBorder.bottom;
						pRender->DrawLine(rcBorder,rcBorderSize.bottom,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
				}
				else if(nBorderSize > 0) {
					pRender->DrawRect(rcItem, nBorderSize, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
				}
			}
		}*/
	}

	SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
	{
		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->ScaleInt(m_cxyFixed.cy);
		SIZE cXY = {0};
		for( int it = 0; it < GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if( !pControl->IsVisible() ) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cXY.cy += sz.cy;
			if( cXY.cx < sz.cx )
				cXY.cx = sz.cx;
		}

		if(m_pOwner == NULL) //不判断下面就崩了，暂时先这样啊
		{
			return m_cxyFixed;
		}

		if(cXY.cy == 0) {
			TListInfoUI* pInfo = m_pOwner->GetListInfo();

			DWORD iTextColor = pInfo->dwTextColor;
			if( IsHotState() ) {
				iTextColor = pInfo->dwHotTextColor;
			}
			if( IsSelected() ) {
				iTextColor = pInfo->dwSelectedTextColor;
			}
			if( !IsEnabled() ) {
				iTextColor = pInfo->dwDisabledTextColor;
			}
			CDuiString sText = GetText();

			RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
			RECT rcTextPadding = pInfo->rcTextPadding;
			GetManager()->GetDPIObj()->ScaleRect(&rcTextPadding);
			rcText.left += rcTextPadding.left;
			rcText.right -= rcTextPadding.right;
			
				GetManager()->Render()->DrawText(rcText, pInfo->rcTextPadding, sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
			
			cXY.cx = rcText.right - rcText.left + rcTextPadding.left + rcTextPadding.right ;
			cXY.cy = rcText.bottom - rcText.top + rcTextPadding.top + rcTextPadding.bottom;
		}

		if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
		if ( cXY.cx < m_cxyFixed.cx )
			cXY.cx =  m_cxyFixed.cx;

		CMenuElementUI::m_cxyFixed.cy = MulDiv(cXY.cy, 100, GetManager()->GetDPIObj()->GetScale());
		CMenuElementUI::m_cxyFixed.cx = MulDiv(cXY.cx, 100, GetManager()->GetDPIObj()->GetScale());
		return cXY;
	}

	void CMenuElementUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			CListContainerElementUI::DoEvent(event);
			if( m_pWindow ) return;
			SetHotState(true);
			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i )
			{
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
				{
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu && IsEnabled())
			{
				m_pOwner->SelectItem(GetIndex(), true);
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 2;
				CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				m_pOwner->SelectItem(GetIndex(), true);
			}
			return;
		}


		if (event.Type == UIEVENT_MOUSELEAVE) {
			SetHotState(false);
			bool hasSubMenu = false;
			for (int i = 0; i < GetCount(); ++i)
			{
				if (GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
				{
					
					hasSubMenu = true;
				}
			}

			if (!hasSubMenu) {
				m_pOwner->SelectItem(-1, true);
			}
		}

		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( IsEnabled() ){
				CListContainerElementUI::DoEvent(event);

				if( m_pWindow ) return;

				bool hasSubMenu = false;
				for( int i = 0; i < GetCount(); ++i ) {
					if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL ) {
						(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
						(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

						hasSubMenu = true;
					}
				}
				if( hasSubMenu )
				{
					CreateMenuWnd();
				}
				else
				{
					SetChecked(!GetChecked());

					
					bool isClosing = false;
					CMenuUI* menuUI=static_cast<CMenuUI*>(GetManager()->GetRoot());
					isClosing = (menuUI->m_pWindow->isClosing);
					if (CPlatform::IsWindow(GetManager()->GetPaintWindow()) && !isClosing) {
						if (CMenuWnd::GetGlobalContextMenuObserver().GetManager() != NULL)
						{

							MenuCmd* pMenuCmd = new MenuCmd();
							pMenuCmd->szName		= GetName();
							pMenuCmd->szUserData	= GetUserData();
							pMenuCmd->szText		= GetText();
							pMenuCmd->tag			= GetTag();
							pMenuCmd->bChecked		= GetChecked();
							if (!CPlatform::PostMessage(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), UIMSG_MENUCLICK, (WPARAM)pMenuCmd, (LPARAM)this))
							{
								delete pMenuCmd;
								pMenuCmd = NULL;
							}
						}
					}
					ContextMenuParam param;
					param.hWnd = m_pManager->GetPaintWindow();
					param.wParam = 1;
					CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				}
			}

			return;
		}

		if ( event.Type == UIEVENT_KEYDOWN && event.chKey == VK_RIGHT )
		{
			if( m_pWindow ) return;
			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i )
			{
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
				{
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);
					hasSubMenu = true;
				}
			}
			if( hasSubMenu )
			{
				m_pOwner->SelectItem(GetIndex(), true);
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 2;
				CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				m_pOwner->SelectItem(GetIndex(), true);
			}

			return;
		}

		CListContainerElementUI::DoEvent(event);
	}

	CMenuWnd* CMenuElementUI::GetMenuWnd()
	{
		return m_pWindow;
	}

	CMenuUI* CMenuElementUI::GetMenuUI()
	{
		//return dynamic_cast<CMenuUI*>(GetManager()->GetRoot());
		return dynamic_cast<CMenuUI*>(GetOwner());
	}

	void CMenuElementUI::CreateMenuWnd()
	{
		if( m_pWindow ) return;

		m_pWindow = new CMenuWnd();
		ASSERT(m_pWindow);

		ContextMenuParam param;
		param.hWnd = m_pManager->GetPaintWindow();
		param.wParam = 2;
		CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);

		m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), CDuiPoint(), NULL);
	}

	void CMenuElementUI::SetLineType()
	{
		m_bDrawLine = true;
		if (m_cxyFixed.cy == 0 || m_cxyFixed.cy == ITEM_DEFAULT_HEIGHT)
			SetFixedHeight(DEFAULT_LINE_HEIGHT);

		SetMouseChildEnabled(false);
		SetMouseEnabled(false);
		SetEnabled(false);
	}

	void CMenuElementUI::SetLineColor(DWORD color)
	{
		m_dwLineColor = color;
	}

	DWORD CMenuElementUI::GetLineColor() const
	{
		return m_dwLineColor;
	}
	void CMenuElementUI::SetLinePadding(RECT rcInset)
	{
		m_rcLinePadding = rcInset;
	}

	RECT CMenuElementUI::GetLinePadding() const
	{
		return m_rcLinePadding;
	}

	void CMenuElementUI::SetIcon(LPCTSTR strIcon)
	{
		m_strIcon = strIcon;
	}

	void CMenuElementUI::SetUnCheckIcon(LPCTSTR strIcon)
	{
		m_strUnCheckIcon = strIcon;
	}

	void CMenuElementUI::SetIconSize(LONG cx, LONG cy)
	{
		m_szIconSize.cx = cx;
		m_szIconSize.cy = cy;
	}

	void CMenuElementUI::SetChecked(bool bCheck/* = true*/)
	{
		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if(mCheckInfos == NULL)
		{
			m_bCheck = bCheck;
			return;
		}
		SetItemInfo(GetName(), bCheck);
	}

	bool CMenuElementUI::GetChecked()
	{
		LPCTSTR pstrName = GetName();
		if(pstrName == NULL || _tcslen(pstrName) <= 0) return false;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo != NULL) {
				return pItemInfo->bChecked;
			}
		}
		else
		{
			return m_bCheck;
		}
		return false;
	}

	void CMenuElementUI::SetCheckItem(bool bCheckItem/* = false*/)
	{
		m_bCheckItem = bCheckItem;
	}

	bool CMenuElementUI::GetCheckItem() const
	{
		return m_bCheckItem;
	}

	void CMenuElementUI::SetShowExplandIcon(bool bShow)
	{
		m_bShowExplandIcon = bShow;
	}

	void CMenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("icon")) == 0){
			SetIcon(pstrValue);
		}
		if( _tcsicmp(pstrName, _T("uncheckicon")) == 0){
			SetUnCheckIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconsize")) == 0 ) {
			LPTSTR pstr = NULL;
			LONG cx = 0, cy = 0;
			cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			SetIconSize(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("checkitem")) == 0 ) {		
			SetCheckItem(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);		
		}
		else if( _tcsicmp(pstrName, _T("ischeck")) == 0 ) {		
			CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
			if (mCheckInfos != NULL)
			{
				bool bFind = false;
				for(int i = 0; i < mCheckInfos->GetSize(); i++) {
					MenuItemInfo* itemInfo = (MenuItemInfo*)mCheckInfos->GetAt(i);
					if(_tcsicmp(itemInfo->szName, GetName()) == 0) {
						bFind = true;
						break;
					}
				}
				if(!bFind) SetChecked(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);
			}
		}	
		else if( _tcsicmp(pstrName, _T("linetype")) == 0){
			if (_tcsicmp(pstrValue, _T("true")) == 0)
				SetLineType();
		}
		else if( _tcsicmp(pstrName, _T("expland")) == 0 ) {
			SetShowExplandIcon(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);
		}
		else if( _tcsicmp(pstrName, _T("linecolor")) == 0){
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			SetLineColor(_tcstoul(pstrValue, &pstr, 16));
		}
		else if( _tcsicmp(pstrName, _T("linepadding")) == 0 ) {
			RECT rcInset = { 0 };
			LPTSTR pstr = NULL;
			rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetLinePadding(rcInset);
		}
		else if	( _tcsicmp(pstrName, _T("height")) == 0){
			SetFixedHeight(_ttoi(pstrValue));
		}
		else
			CListContainerElementUI::SetAttribute(pstrName, pstrValue);
	}


	MenuItemInfo* CMenuElementUI::GetItemInfo(LPCTSTR pstrName)
	{
		if(pstrName == NULL || _tcslen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo != NULL) {
				return pItemInfo;
			}
		}

		return NULL;
	}

	MenuItemInfo* CMenuElementUI::SetItemInfo(LPCTSTR pstrName, bool bChecked)
	{
		if(pstrName == NULL || _tcslen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo == NULL) {
				pItemInfo = new MenuItemInfo;
				_tcscpy(pItemInfo->szName, pstrName);
				pItemInfo->bChecked = bChecked;
				mCheckInfos->Insert(pstrName, pItemInfo);
			}
			else {
				pItemInfo->bChecked = bChecked;
			}

			return pItemInfo;
		}
		return NULL;
	}

	void CMenuElementUI::OnUpdateCommandUI()
	{
		//发消息给父窗口，返回check状态
		if (!CPlatform::IsWindow(GetManager()->GetPaintWindow())) 
			return;

		if (CMenuWnd::GetGlobalContextMenuObserver().GetManager() == NULL)
			return;

		//if(!GetCheckItem())
		//	return;

		CMenuCmdUI cmdUI(this);
		LRESULT lRet = CPlatform::SendMessage(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), UIMSG_MENU_UPDATE_COMMAND_UI, (WPARAM)&cmdUI, (LPARAM)this);		
	}

	void CMenuElementUI::DrawDisableItemIcon(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		//if ((m_pManager == NULL) || (pRender->GetDC() == NULL)) return;
		if ((m_pManager == NULL)) return;
		const TDrawInfo* pDrawInfo = m_pManager->GetDrawInfo(pStrImage, pStrModify);

		CPaintManagerUI *pManager = m_pManager;
		RECT rcItem = m_rcItem;
		RECT rcPaint = m_rcItem;
		//return DrawImageInfo(hDC, pManager, rcItem, rcPaint, pDrawInfo, instance);

		RECT rcDest = rcItem;
		if( pDrawInfo->rcDest.left != 0 || pDrawInfo->rcDest.top != 0 ||
			pDrawInfo->rcDest.right != 0 || pDrawInfo->rcDest.bottom != 0 ) {
				rcDest.left = rcItem.left + pDrawInfo->rcDest.left;
				rcDest.top = rcItem.top + pDrawInfo->rcDest.top;
				rcDest.right = rcItem.left + pDrawInfo->rcDest.right;
				if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;
				rcDest.bottom = rcItem.top + pDrawInfo->rcDest.bottom;
				if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;
		}
		bool bRet = _DrawImageMenuDisableIcon(pRender, pManager, rcItem, rcPaint, pDrawInfo->sImageName, pDrawInfo->sResType, rcDest, \
			pDrawInfo->rcSource, pDrawInfo->rcCorner, pDrawInfo->dwMask, pDrawInfo->uFade, pDrawInfo->bHole, pDrawInfo->bTiledX, pDrawInfo->bTiledY, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, m_instance);

	}
	bool CMenuElementUI::_DrawImageMenuDisableIcon(UIRender *pRender, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, const CDuiString& sImageName, \
		const CDuiString& sImageResType, RECT rcItem, RECT rcBmpPart, RECT rcCorner, DWORD dwMask, BYTE bFade, \
		bool bHole, bool bTiledX, bool bTiledY, int width, int height, DWORD fillcolor, HINSTANCE instance)
	{
		if (sImageName.IsEmpty()) {
			return false;
		}
		const UIImage* data = NULL;
		if( sImageResType.IsEmpty() ) {
			data = pManager->GetImageEx((LPCTSTR)sImageName, NULL, dwMask, width, height, fillcolor, false, instance);
		}
		else {
			data = pManager->GetImageEx((LPCTSTR)sImageName, (LPCTSTR)sImageResType, dwMask, width, height, fillcolor, false, instance);
		}
		if( !data ) return false;    

		if( rcBmpPart.left == 0 && rcBmpPart.right == 0 && rcBmpPart.top == 0 && rcBmpPart.bottom == 0 ) {
			rcBmpPart.right = data->nWidth;
			rcBmpPart.bottom = data->nHeight;
		}
		if (rcBmpPart.right > data->nWidth) rcBmpPart.right = data->nWidth;
		if (rcBmpPart.bottom > data->nHeight) rcBmpPart.bottom = data->nHeight;

		CDuiRect rcTemp;
		//if( !::IntersectRect(&rcTemp, &rcItem, &rc) ) return true;
		if( !rcTemp.Intersect(rcItem, rc) ) return true;
		//if( !::IntersectRect(&rcTemp, &rcItem, &rcPaint) ) return true;
		if( !rcTemp.Intersect(rcItem, rcPaint) ) return true;

		pRender->DrawBitmap(data->bitmap, rcItem, rcPaint, rcBmpPart, rcCorner, pManager->IsLayered() ? true : data->bAlpha, bFade, bHole, bTiledX, bTiledY);

		//需要把data->hBitmap转为灰度图
//		CRenderEngine::DrawImage(hDC, hbmp, rcItem, rcPaint, rcBmpPart, rcCorner, pManager->IsLayered() ? true : data->bAlpha, bFade, bHole, bTiledX, bTiledY);

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	CMenuCmdUI::CMenuCmdUI(CMenuElementUI *p) : pMenuElement(p)//m_bEnable(true), m_bCheck(false)
	{
	}

	void CMenuCmdUI::Enable(BOOL bEnable)
	{
		//m_bEnable = bOn;
		pMenuElement->SetEnabled(bEnable == TRUE);
	}

	BOOL CMenuCmdUI::IsEnable()
	{
		return pMenuElement->IsEnabled();
	}

	void CMenuCmdUI::SetCheck(BOOL bCheck)
	{
		//m_bCheck = bCheck;
		pMenuElement->SetChecked(bCheck == TRUE);
		
	}

	BOOL CMenuCmdUI::IsCheck()
	{
		//return m_bCheck;
		return pMenuElement->GetChecked();
	}

	void CMenuCmdUI::SetText(LPCTSTR lpszText)
	{
		//m_sText = lpszText;
		return pMenuElement->SetText(lpszText);
	}

	CDuiString CMenuCmdUI::GetText()
	{
		//return m_sText;
		return pMenuElement->GetText();
	}

	CDuiString CMenuCmdUI::GetName()
	{
		//return m_sName;
		return pMenuElement->GetName();
	}

	CMenuElementUI *CMenuCmdUI::GetMenuElementUI() const
	{
		return pMenuElement;
	}
} // namespace DuiLib


