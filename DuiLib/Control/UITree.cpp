#include "StdAfx.h"
#include "UITree.h"

namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CTreeBodyUI)

CTreeBodyUI::CTreeBodyUI(void) : m_pOwner(NULL)
{
	EnableScrollBar(false,false);
	SetAutoCalcWidth(true);
	m_nCxNeeded = 0;
	m_nCyNeeded = 0;
}

CTreeBodyUI::~CTreeBodyUI(void)
{

}

LPCTSTR CTreeBodyUI::GetClass() const
{
	return DUI_CTR_TREEBODY;
}

LPVOID CTreeBodyUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_TREEBODY) == 0 ) return static_cast<CTreeBodyUI*>(this);
	return __super::GetInterface(pstrName);
}

void CTreeBodyUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	if(!GetOwner()) return;

	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->BuildRows(rc, bNeedInvalidate);
}

void CTreeBodyUI::LineUp()
{
	if(!GetOwner()) return __super::LineUp();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->LineUp();
}

void CTreeBodyUI::LineDown()
{
	if(!GetOwner()) return __super::LineDown();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->LineDown();
}

void CTreeBodyUI::PageUp()
{
	if(!GetOwner()) return __super::PageUp();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->PageUp();
}

void CTreeBodyUI::PageDown()
{
	if(!GetOwner()) return __super::PageDown();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->PageDown();
}

void CTreeBodyUI::HomeUp()
{
	if(!GetOwner()) return __super::HomeUp();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->HomeUp();
}

void CTreeBodyUI::EndDown()
{
	if(!GetOwner()) return __super::EndDown();
	CTreeUI *pTree = (CTreeUI *)GetOwner();
	pTree->EndDown();
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
IMPLEMENT_DUICONTROL(CTreeUI)

CTreeUI::CTreeUI( void )
{
	Add(m_pBody = new CTreeBodyUI);
	m_pBody->SetOwner(this);
}

CTreeUI::~CTreeUI( void )
{

}


LPCTSTR CTreeUI::GetClass() const
{
	return _T("TreelUI");
}

LPVOID CTreeUI::GetInterface( LPCTSTR pstrName )
{
	if( _tcsicmp(pstrName, DUI_CTR_TREE) == 0 ) return static_cast<CTreeUI*>(this);
	return __super::GetInterface(pstrName);
}

UINT CTreeUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

void CTreeUI::Refresh(bool bNeedUpdate)
{
	if(bNeedUpdate) NeedUpdate();
	else Invalidate();
}

void CTreeUI::SendNotify(LPCTSTR pstrMessage, WPARAM wParam, LPARAM lParam, bool bAsync)
{
	if(GetManager())
		GetManager()->SendNotify(this, pstrMessage, wParam, lParam, bAsync);
}

void CTreeUI::DoInit()
{
	EnableScrollBar(true, true);
	m_pVerticalScrollBar->SetVisible(false);
	m_pHorizontalScrollBar->SetVisible(false);
}

void CTreeUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else __super::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN ) 
	{
		if( IsEnabled() ) 
		{
			ClearSeletedNodes();
		}
		return;
	}

	__super::DoEvent(event);
}

void CTreeUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	//return __super::SetPos(rc, bNeedInvalidate);
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

	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
	int iPosX = rc.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
	{
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}

	RECT rcBody = {iPosX, rc.top, iPosX + m_pBody->GetCxNeeded(), rc.bottom};
	m_pBody->SetPos(rcBody);

	if(m_pBody->GetCxNeeded() < rc.right - rc.left)
	{
		rcBody.left = iPosX;
		rcBody.right = iPosX + rc.right - rc.left;
		m_pBody->SetPos(rcBody);
	}

	//CMsgWndUI::InsertMsgV(_T("CxNeeded: %d"), m_pBody->GetCxNeeded());
	int cxNeeded = m_pBody->GetCxNeeded();
	int cyNeeded = m_pBody->GetCyNeeded();
	ProcessScrollBar(rc, cxNeeded, cyNeeded);
}

void CTreeUI::BuildRows(RECT rc, bool bNeedInvalidate)
{
	int iPosX = rc.left;
	int iPosY = rc.top;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
	{
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}
	if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
	{
		iPosY -= GetVerticalScrollBar()->GetScrollPos();
	}

	//显示的第一行是哪个？
	int cyNeeded = 0;
	int nNodeCount = 0;  //总共可能需要绘制几行
	BOOL bFindBeginRow = FALSE;
	TNodeData *pNode = GetRootNode();
	TNodeData *pBeginNode = pNode; //从哪一行开始绘制
	while (pNode)
	{
		if(!bFindBeginRow)
		{
			if(iPosY + GetNodeHeight() + GetChildPadding() > rc.top)
			{
				pBeginNode = pNode;
				bFindBeginRow = TRUE;
			}
			else
			{
				iPosY += GetNodeHeight() + GetChildPadding();
			}
		}
		cyNeeded += GetNodeHeight() + GetChildPadding();
		nNodeCount++;
		pNode = GetNextNode(pNode,true);
	}
	cyNeeded -= GetChildPadding();

	//可以显示几行？
	int nNeedRows = 0;
	int iTempPosY = iPosY;
	pNode = pBeginNode;
	while (iTempPosY < rc.bottom && pNode)
	{
		nNeedRows++;

		iTempPosY += GetNodeHeight() + GetChildPadding();
		pNode = GetNextNode(pNode,true);
	}

	//创建实际的TreeNodeUI
	int addedRows = nNeedRows - m_pBody->GetCount();
	if(addedRows > 0)
	{
		while(m_pBody->GetCount() < nNeedRows) 
		{
			CTreeItemUI *pRow = new CTreeItemUI();
			pRow->SetOwner(this);
			m_pBody->Add(pRow);
			pRow->ApplyAttributeList(GetStyleNode());
		}
	}
	else
	{
		while(m_pBody->GetCount() > 0 && m_pBody->GetCount() > nNeedRows) 
		{
			m_pBody->RemoveAt(GetCount()-1);
		}
	}

	//子项SetPos并填充数据
	int cxNeeded = 0;
	pNode = pBeginNode;
	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CTreeItemUI *pRowUI = (CTreeItemUI *)m_pBody->GetItemAt(i);
		pRowUI->SetNodeData(pNode);

		int nNodeWidth = pRowUI->GetCxNeeded(CDuiSize(rc.right-rc.left, GetNodeHeight()));
		cxNeeded = MAX(cxNeeded, nNodeWidth);

		RECT rcRow = { rc.left, iPosY, rc.right, iPosY + GetNodeHeight() };
		pRowUI->SetPos(rcRow, bNeedInvalidate);

		iPosY += GetNodeHeight() + GetChildPadding();
		pNode = GetNextNode(pNode,true);
	}
	
	m_pBody->SetCxNeeded(cxNeeded);
	m_pBody->SetCyNeeded(cyNeeded);
}

void CTreeUI::SetScrollPos(SIZE szPos, bool bMsg)
{
	int cx = 0;
	int cy = 0;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( cx == 0 && cy == 0 ) return;

	RECT rcPos;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;

		rcPos = pControl->GetPos();
		rcPos.left -= cx;
		rcPos.right -= cx;
		//rcPos.top -= cy;	//竖向滚动时，不改变竖向位置
		//rcPos.bottom -= cy;
		pControl->SetPos(rcPos);
	}

	Invalidate();

	if(m_pVerticalScrollBar)
	{
		// 发送滚动消息
		if( m_pManager != NULL && bMsg ) {
			int nPage = (m_pVerticalScrollBar->GetScrollPos() + m_pVerticalScrollBar->GetLineSize()) / m_pVerticalScrollBar->GetLineSize();
			m_pManager->SendNotify(this, DUI_MSGTYPE_SCROLL, (WPARAM)nPage);
		}
	}
}
void CTreeUI::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
{
	while (m_pHorizontalScrollBar)
	{
		// Scroll needed
		if (cxRequired > rc.right - rc.left && !m_pHorizontalScrollBar->IsVisible())
		{
			m_pHorizontalScrollBar->SetVisible(true);
			m_pHorizontalScrollBar->SetScrollRange(cxRequired - (rc.right - rc.left));
			m_pHorizontalScrollBar->SetScrollPos(0);
			SetPos(m_rcItem);
			break;
		}

		// No scrollbar required
		if (!m_pHorizontalScrollBar->IsVisible()) break;

		// Scroll not needed anymore?
		int cxScroll = cxRequired - (rc.right - rc.left);
		if (cxScroll <= 0)
		{
			m_pHorizontalScrollBar->SetVisible(false);
			m_pHorizontalScrollBar->SetScrollPos(0);
			m_pHorizontalScrollBar->SetScrollRange(0);
			SetPos(m_rcItem);
		}
		else
		{
			RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight() };
			m_pHorizontalScrollBar->SetPos(rcScrollBarPos);

			if (m_pHorizontalScrollBar->GetScrollRange() != cxScroll) 
			{
				int iScrollPos = m_pHorizontalScrollBar->GetScrollPos();
				m_pHorizontalScrollBar->SetScrollRange(::abs(cxScroll)); // if scrollpos>range then scrollpos=range
				if(iScrollPos > m_pHorizontalScrollBar->GetScrollPos()) 
				{
					SetPos(m_rcItem);
				}
			}
		}
		break;
	}

	RECT rcBody = m_pBody->GetPos();
	int nBodyHeight = rcBody.bottom - rcBody.top;
	while (m_pVerticalScrollBar)
	{
		// Scroll needed
		if (cyRequired > nBodyHeight && !m_pVerticalScrollBar->IsVisible()) 
		{
			m_pVerticalScrollBar->SetVisible(true);
			m_pVerticalScrollBar->SetScrollRange(cyRequired - nBodyHeight);
			m_pVerticalScrollBar->SetScrollPos(0);
			SetPos(m_rcItem);
			break;
		}

		// No scrollbar required
		if (!m_pVerticalScrollBar->IsVisible()) break;

		// Scroll not needed anymore?
		int cyScroll = cyRequired - nBodyHeight;
		if (cyScroll <= 0) 
		{
			m_pVerticalScrollBar->SetVisible(false);
			m_pVerticalScrollBar->SetScrollPos(0);
			m_pVerticalScrollBar->SetScrollRange(0);
			SetPos(m_rcItem);
			break;
		}

		RECT rcTree = GetPos();
		RECT rcScrollBarPos = { rcTree.right-m_pVerticalScrollBar->GetFixedWidth(), rcTree.top, rcTree.right, rcTree.bottom };
//		RECT rcScrollBarPos = { rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom };
		m_pVerticalScrollBar->SetPos(rcScrollBarPos);

		if (m_pVerticalScrollBar->GetScrollRange() != cyScroll)
		{
			int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll)); // if scrollpos>range then scrollpos=range
			if(iScrollPos > m_pVerticalScrollBar->GetScrollPos()) 
			{
				SetPos(m_rcItem);
			}
		}
		break;
	}
}


void CTreeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("indentwidth")) == 0 )
	{
		SetIndentWidth(_ttoi(pstrValue));
	}
	else if(_tcsicmp(pstrName,_T("viewline")) == 0)
		SetViewLine(_tcsicmp(pstrValue,_T("true")) == 0);
	else if( _tcsicmp(pstrName, _T("nodeheight")) == 0 )
	{
		SetNodeHeight(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("multiselect")) == 0 )
	{
		SetMultiSelect(_tcsicmp(pstrValue,_T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("horizattr")) == 0 )
	{
		SetStyleNode(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("folderattr")) == 0 )
	{
		SetStyleFolderBtn(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("checkboxattr")) == 0 )
	{
		SetStyleCheckBoxBtn(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("iconattr")) == 0 )
	{
		SetStyleIconBtn(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("textattr")) == 0 )
	{
		SetStyleTextBtn(pstrValue);
	}
	else
		__super::SetAttribute(pstrName, pstrValue);
}


}