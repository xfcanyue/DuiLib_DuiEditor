#include "StdAfx.h"
#include "UITreeItem.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CTreeItemUI)

	CTreeItemUI::CTreeItemUI() : m_pOwner(NULL), m_pParentNode(NULL), m_nLevel(0), 
		m_pHoriz(NULL), m_pFolderButton(NULL), m_pCheckBox(NULL), m_pIcon(NULL), m_pText(NULL), m_pChildContainer(NULL),
		m_bSelected(false)
	{
		SetAutoCalcHeight(true);

		Add(m_pHoriz = new CHorizontalLayoutUI);
		Add(m_pChildContainer = new CVerticalLayoutUI);

		//m_pHoriz->SetBkColor(UIRGB(255,0,0));
		m_pHoriz->SetFixedHeight(25);
		m_pHoriz->SetChildVAlign(DT_VCENTER);
		m_pHoriz->SetChildPadding(2);

		//m_pChildContainer->SetBkColor(UIRGB(0,255,0));
		m_pChildContainer->SetAutoCalcHeight(true);
		//m_pChildContainer->SetAnimation(DuiAnim_ScaleVertical);
	}

	CTreeItemUI::~CTreeItemUI( void )
	{

	}

	LPCTSTR CTreeItemUI::GetClass() const
	{
		return _T("TreeItemUI");
	}

	LPVOID CTreeItemUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_TREEITEM) == 0 )
			return static_cast<CTreeItemUI*>(this);
		return __super::GetInterface(pstrName);
	}

	UINT CTreeItemUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	int CTreeItemUI::GetChildNodeCount()
	{
		return m_pChildContainer->GetCount();
	}

	CTreeItemUI *CTreeItemUI::GetChildNodeAt(int iIndex)
	{
		return (CTreeItemUI *)m_pChildContainer->GetItemAt(iIndex);
	}

	int CTreeItemUI::GetChildNodeIndex(CTreeItemUI *pNode)
	{
		return m_pChildContainer->GetItemIndex(pNode);
	}

	BOOL CTreeItemUI::DeleteChildNode(CTreeItemUI *pNode)
	{
		if(GetOwner() && GetOwner()->GetFocusNode() == pNode) GetOwner()->SetFocusNode(NULL);
		return m_pChildContainer->Remove(pNode);
	}

	bool CTreeItemUI::Add(CControlUI* pControl)
	{
		if(pControl->GetInterface(DUI_CTR_TREEITEM))
		{
			CTreeItemUI *pNode = static_cast<CTreeItemUI *>(pControl);
			pNode->SetParentNode(this);
			if(m_pFolderButton && !m_pFolderButton->IsVisible()) m_pFolderButton->SetVisible(true);
			return m_pChildContainer->Add(pNode);
		}
		return __super::Add(pControl);
	}

	bool CTreeItemUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if(pControl->GetInterface(DUI_CTR_TREEITEM))
		{
			CTreeItemUI *pNode = static_cast<CTreeItemUI *>(pControl);
			pNode->SetParentNode(this);
			if(m_pFolderButton && !m_pFolderButton->IsVisible()) m_pFolderButton->SetVisible(true);
			return m_pChildContainer->AddAt(pNode, iIndex);
		}
		return __super::AddAt(pControl, iIndex);
	}

	bool CTreeItemUI::Remove(CControlUI* pControl, bool bDoNotDestroy )
	{
		if(GetOwner() && GetOwner()->GetFocusNode() == pControl)
			GetOwner()->SetFocusNode(NULL);
		if(pControl->GetInterface(DUI_CTR_TREEITEM))
		{
			return m_pChildContainer->Remove(pControl);
		}
		return __super::Remove(pControl);
	}

	bool CTreeItemUI::RemoveAt(int iIndex, bool bDoNotDestroy )
	{
		return m_pChildContainer->RemoveAt(iIndex);
	}

	void CTreeItemUI::RemoveAll()
	{
		m_pChildContainer->RemoveAll();
	}

	CDuiString CTreeItemUI::GetText() const
	{
		return __super::GetText();
	}

	void CTreeItemUI::SetText(LPCTSTR pstrText)
	{
		__super::SetText(pstrText);
		if(m_pText)
		{
			m_pText->SetText(pstrText);
		}
	}

	void CTreeItemUI::ExpandItem(bool bExpand)
	{
		if(IsExpandItem() == bExpand) return;

		//如果是展开，那么要先visible
		if(bExpand)
		{
			if(m_pChildContainer) m_pChildContainer->SetVisible(true);
			if(m_pFolderButton) m_pFolderButton->Selected(false, false);
		}
		else
		{
			if(m_pChildContainer) m_pChildContainer->SetVisible(false);
			if(m_pFolderButton) m_pFolderButton->Selected(true, false);
		}

		if(GetOwner()) GetOwner()->NeedUpdate();
	}

	bool CTreeItemUI::IsExpandItem()
	{
		if(m_pChildContainer && m_pChildContainer->GetCount() > 0 && m_pChildContainer->IsVisible())
			return true;
		return false;
	}

	bool CTreeItemUI::Select(bool bSelect)
	{
		if(m_bSelected == bSelect)	return false;
		m_bSelected = bSelect;
		if(m_pCheckBox) m_pCheckBox->Selected(m_bSelected, false);
		if(m_pText)	m_pText->Selected(m_bSelected, false);
		Invalidate();
		return true;
	}

	bool CTreeItemUI::IsSelected() const
	{
		return m_bSelected;
	}

	void CTreeItemUI::DoInit()
	{
		if(!GetOwner())
		{
			m_nLevel = 0;
			CControlUI *pParent = GetParent();
			while (pParent)
			{
				if(pParent->GetInterface(DUI_CTR_TREECTRL))
				{
					SetOwner((CTreeCtrlUI *)pParent);
					break;
				}
				if(pParent->GetInterface(DUI_CTR_TREEITEM))
				{
					m_nLevel++;
					pParent = pParent->GetParent();
					continue;
				}
				pParent = pParent->GetParent();
			}
		}

		m_pHoriz->Add(m_pFolderButton = new CCheckBoxUI);
		m_pHoriz->Add(m_pCheckBox = new CCheckBoxUI);
		m_pHoriz->Add(m_pIcon = new COptionUI);
		m_pHoriz->Add(m_pText = new COptionUI);
		if(GetOwner())
		{
			m_pHoriz->ApplyAttributeList(GetOwner()->GetStyleHoriz());			
			m_pHoriz->OnPaint += MakeDelegate(this,&CTreeItemUI::OnPaintHoriz);

			m_pFolderButton->ApplyAttributeList(GetOwner()->GetStyleFolder());
			m_pFolderButton->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pCheckBox->ApplyAttributeList(GetOwner()->GetStyleCheckBox());
			m_pCheckBox->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pIcon->ApplyAttributeList(GetOwner()->GetStyleIcon());
			m_pIcon->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pText->ApplyAttributeList(GetOwner()->GetStyleText());
			m_pText->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);
			m_pText->SetText(GetText());
			m_pText->SetAutoCalcWidth(true);
		}
	}

	SIZE CTreeItemUI::EstimateSize(SIZE szAvailable)
	{
		if(!GetOwner()) return __super::EstimateSize(szAvailable);

		if(m_pChildContainer->GetCount() == 0){
			if(m_pFolderButton) m_pFolderButton->SetVisible(false);
			else m_pFolderButton->SetVisible(true);
		}

		int indent = m_nLevel * GetOwner()->GetIndent();
		if(!m_pFolderButton->IsVisible()) 
			indent += m_pFolderButton->GetFixedWidth() + GetChildPadding()*4;
		m_pHoriz->SetPadding(CDuiRect(indent, 0, 0, 0));

		return __super::EstimateSize(szAvailable);
	}

	void CTreeItemUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_BUTTONDOWN ) 
		{
			if( IsEnabled() ) 
			{
				GetOwner()->SetFocusNode(this);
				Select(!IsSelected());
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT);
			}
			return;
		}

		if( event.Type == UIEVENT_DBLCLICK ) 
		{
			if( IsEnabled() ) 
			{
				ExpandItem(!IsExpandItem());
				m_pManager->SendNotify(GetOwner(), DUI_MSGTYPE_DBCLICK, (WPARAM)this);
			}
			return;
		}

		if( event.Type == UIEVENT_MOUSEENTER ) 
		{
			if( IsEnabled() ) {
				SetHot(true);
				Invalidate();
			}
			return;
		}

		if( event.Type == UIEVENT_MOUSELEAVE ) 
		{
			if( IsEnabled() ) {
				SetHot(false);
				Invalidate();
			}
			return;
		}

		__super::DoEvent(event);
	}

	bool CTreeItemUI::OnEventInnerControl(void* param)
	{
		TEventUI &event = *(TEventUI *)param;

		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return true;
		}

		if( event.Type == UIEVENT_BUTTONDOWN ) 
		{
			if( IsEnabled() ) 
			{
				GetOwner()->SetFocusNode(this);
				if(event.pSender == m_pFolderButton)
				{
					ExpandItem(!IsExpandItem());
					m_pManager->SendNotify(GetOwner(), DUI_MSGTYPE_DBCLICK);
					return false;
				}

				Select(!IsSelected());
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT);
			}

			//folder按钮和checkbox事件，不让它自己处理
			if(event.pSender == m_pFolderButton || event.pSender == m_pCheckBox)
				return false;
			return true;
		}

		if( event.Type == UIEVENT_DBLCLICK ) 
		{
			if( IsEnabled() ) 
			{
				ExpandItem(!IsExpandItem());
				m_pManager->SendNotify(GetOwner(), DUI_MSGTYPE_DBCLICK);
			}

			//folder按钮和checkbox事件，不让它自己处理
			if(event.pSender == m_pFolderButton || event.pSender == m_pCheckBox)
				return false;
			return true;
		}

		if( event.Type == UIEVENT_MOUSEENTER ) 
		{
			if( IsEnabled() ) {
				SetHot(true);
				Invalidate();
			}
			return true;
		}

		if( event.Type == UIEVENT_MOUSELEAVE ) 
		{
			if( IsHot() ) {
				SetHot(false);
				Invalidate();
			}
			return true;
		}


		return true;
	}

	bool CTreeItemUI::OnPaintHoriz(void * param)
	{	
		HDC hDC = (HDC)param;
		DWORD dwBkColor = 0;
		if(GetOwner()->GetFocusNode() == this) {
			dwBkColor = GetOwner()->GetItemFocusBkColor();
		}
		else if(dwBkColor == 0 && IsHot()){
			dwBkColor = GetOwner()->GetItemHotBkColor();
		}
		else if(dwBkColor == 0 && IsSelected()) {
			dwBkColor = GetOwner()->GetItemSelectedBkColor();
		}
		else if(dwBkColor == 0) {
			dwBkColor = GetOwner()->GetItemBkColor();
		}

		if(dwBkColor != 0)
			CRenderEngine::DrawColor(hDC, m_pHoriz->GetPos(), GetOwner()->GetItemFocusBkColor());
		return true;
	}

	void CTreeItemUI::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
	{
		CControlUI::OnAnimationStep(nTotalFrame, nCurFrame, nAnimationID);
		if(GetOwner()) GetOwner()->NeedParentUpdate();
	}

	void CTreeItemUI::OnAnimationStop(int nAnimationID)
	{
		CControlUI::OnAnimationStop(nAnimationID);
		if(GetOwner()) GetOwner()->NeedParentUpdate();
	}

	void CTreeItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		__super::SetAttribute(pstrName, pstrValue);
	}
}