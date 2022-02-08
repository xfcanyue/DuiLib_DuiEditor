#include "StdAfx.h"
#include "UITreeItem.h"

namespace DuiLib
{

	//////////////////////////////////////////////////////////////////////////
	CTreeInnerCheckBoxUI::CTreeInnerCheckBoxUI()
	{
		m_pOwner = NULL;
	}

	bool CTreeInnerCheckBoxUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		Selected(!IsSelected());
		return true;
	}

	bool CTreeInnerCheckBoxUI::IsSelected() const
	{
		if(!m_pOwner) return false;
		CTreeItemUI *pItemUI = (CTreeItemUI *)GetOwner();
		CTreeUI *pTree = (CTreeUI *)pItemUI->GetOwner();
		if(!pItemUI || !pTree || !pItemUI->GetNodeData())
			return false;

		switch (m_type)
		{
		case BTN_FOLDER:
			if(!pItemUI->GetNodeData()->NodeHasChildren()) return true;
			return pItemUI->GetNodeData()->IsExpand();
			break;
		case BTN_CHECKBOX:
			return pItemUI->GetNodeData()->IsCheckBoxCheck();
			break;
		case BTN_ICON:
			return pItemUI->GetNodeData()->IsSelected();
			break;
		case BTN_TEXT:
			return pItemUI->GetNodeData()->IsSelected();
			break;
		}

		return false;
	}

	void CTreeInnerCheckBoxUI::Selected(bool bSelected, bool bTriggerEvent)
	{
		if(IsSelected() == bSelected) return;

		SetSelectedState(bSelected);

		if(!m_pOwner) return;
		CTreeItemUI *pItemUI = (CTreeItemUI *)GetOwner();
		CTreeUI *pTree = (CTreeUI *)pItemUI->GetOwner();
		if(!pItemUI || !pTree || !pItemUI->GetNodeData())
			return;

		switch (m_type)
		{
		case BTN_FOLDER:
			if(pItemUI->GetNodeData()->NodeHasChildren())
			{
				pItemUI->GetNodeData()->Expand(bSelected);
				pTree->NeedUpdate();
			}
			else pItemUI->GetNodeData()->Expand(true);
			break;
		case BTN_CHECKBOX:
			pItemUI->GetNodeData()->SetCheckBoxCheck(bSelected);
			pTree->Invalidate();
			break;
		case BTN_ICON:
			if(bSelected)
			{
				if(!pTree->IsMultiSelect() || ::GetKeyState(VK_CONTROL)>=0) pTree->ClearSeletedNodes();

				pTree->SelectNode(pItemUI->GetNodeData(), bSelected);
				pTree->SetFocusNode(pItemUI->GetNodeData());
				pTree->SendNotify(DUI_MSGTYPE_SELECTCHANGED, 0, 0);
			}
			else
			{
				pTree->SelectNode(pItemUI->GetNodeData(), bSelected);
				pTree->SetFocusNode(NULL);
				pTree->SendNotify(DUI_MSGTYPE_SELECTCHANGED, 0, 0);
			}
			pTree->Invalidate();
			break;
		case BTN_TEXT:
			if(bSelected)
			{
				if(!pTree->IsMultiSelect() || ::GetKeyState(VK_CONTROL)>=0) pTree->ClearSeletedNodes();

				pTree->SelectNode(pItemUI->GetNodeData(), bSelected);
				pTree->SetFocusNode(pItemUI->GetNodeData());
				pTree->SendNotify(DUI_MSGTYPE_SELECTCHANGED, 0, 0);
			}
			else
			{
				pTree->SelectNode(pItemUI->GetNodeData(), bSelected);
				pTree->SetFocusNode(NULL);
				pTree->SendNotify(DUI_MSGTYPE_SELECTCHANGED, 0, 0);
			}
			pTree->Invalidate();
			break;
		}

	}

	bool CTreeInnerCheckBoxUI::IsSelectedState() const
	{
		return IsSelected();
	}

	CDuiString CTreeInnerCheckBoxUI::GetText() const
	{
		if(!m_pOwner) return _T("");
		CTreeItemUI *pItemUI = (CTreeItemUI *)GetOwner();
		CTreeUI *pTree = (CTreeUI *)pItemUI->GetOwner();
		if(!pItemUI || !pTree || !pItemUI->GetNodeData())
			return _T("");

		switch (m_type)
		{
		case BTN_FOLDER:
			break;
		case BTN_CHECKBOX:
			break;
		case BTN_ICON:
			break;
		case BTN_TEXT:
			return pItemUI->GetNodeData()->GetText();
			break;
		}

		return _T("");
	}

	void CTreeInnerCheckBoxUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else COptionUI::DoEvent(event);
			return;
		}

		if(!m_pOwner) return COptionUI::DoEvent(event);
		CTreeItemUI *pItemUI = (CTreeItemUI *)GetOwner();
		CTreeUI *pTree = (CTreeUI *)pItemUI->GetOwner();
		if(!pItemUI || !pTree || !pItemUI->GetNodeData())
			return COptionUI::DoEvent(event);

		if( event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() )
			{
				pItemUI->GetNodeData()->Expand(!pItemUI->GetNodeData()->IsExpand());
				pTree->NeedUpdate();
			}
			return;
		}

		COptionUI::DoEvent(event);
	}
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CTreeItemUI)
	CTreeItemUI::CTreeItemUI()
	{
		//SetBkColor((UIRGB(0xa4,0xa4,0xa4)));
		m_pOwner = NULL;
		m_pNodeData = NULL;
		SetChildVAlign(DT_VCENTER);
		SetChildPadding(5);
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

	void CTreeItemUI::SetNodeData(TNodeData *pNodeData) 
	{ 
		m_pNodeData = pNodeData; 
		if(m_pNodeData)
		{
			RECT inSet = {m_pNodeData->GetLevel() * GetOwner()->GetIndentWidth(),0,0,0};
			SetInset(inSet);
// 			if(m_pNodeData->GetChild(0))
// 			{
// 				m_pFolderButton->SetVisible(true);
// 			}
// 			else
// 			{
// 				m_pFolderButton->SetVisible(false);
// 			}
		}	
	}

	void CTreeItemUI::DoInit()
	{
		Add(m_pFolderButton = new CTreeInnerCheckBoxUI);
		Add(m_pCheckBox = new CTreeInnerCheckBoxUI);
		Add(m_pIcon = new CTreeInnerCheckBoxUI);
		Add(m_pText = new CTreeInnerCheckBoxUI);

		m_pFolderButton->SetOwner(this);
		m_pFolderButton->m_type = CTreeInnerCheckBoxUI::BTN_FOLDER;

		m_pCheckBox->SetOwner(this);
		m_pCheckBox->m_type = CTreeInnerCheckBoxUI::BTN_CHECKBOX;

		m_pIcon->SetOwner(this);
		m_pIcon->m_type = CTreeInnerCheckBoxUI::BTN_ICON;

		m_pText->SetOwner(this);
		m_pText->m_type = CTreeInnerCheckBoxUI::BTN_TEXT;

		if(GetOwner())
		{
			m_pFolderButton->ApplyAttributeList(GetOwner()->GetStyleFolderBtn());
			//m_pFolderButton->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pCheckBox->ApplyAttributeList(GetOwner()->GetStyleCheckBoxBtn());
			//m_pCheckBox->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pIcon->ApplyAttributeList(GetOwner()->GetStyleIconBtn());
			//m_pIcon->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);

			m_pText->ApplyAttributeList(GetOwner()->GetStyleTextBtn());
			//m_pText->OnEvent += MakeDelegate(this,&CTreeItemUI::OnEventInnerControl);
			//m_pText->SetAutoCalcWidth(true);
			m_pText->SetTextStyle(DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
	}

	int CTreeItemUI::GetCxNeeded(SIZE szAvailable)
	{
		if(!GetOwner() | !m_pNodeData) return 30;

		int nWidth = 0;
		nWidth += m_pFolderButton->GetFixedWidth() + m_pCheckBox->GetFixedWidth() + m_pIcon->GetFixedWidth();

		RECT rcText = {0, 0, szAvailable.cx, szAvailable.cy};
		GetManager()->Render()->DrawText(rcText, m_pText->GetTextPadding(), m_pNodeData->GetText(), 0, m_pText->GetFont(), DT_CALCRECT | m_pText->GetTextStyle());
		nWidth += rcText.right - rcText.left;

		RECT rcInset = GetInset();
		nWidth += rcInset.left;
		nWidth += rcInset.right;
		
		return nWidth;
	}

	void CTreeItemUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else __super::DoEvent(event);
			return;
		}

		CTreeUI *pTree = (CTreeUI *)GetOwner();
		if(!pTree || !m_pNodeData) return __super::DoEvent(event);

		if( event.Type == UIEVENT_BUTTONDOWN ) 
		{
			if( IsEnabled() ) 
			{
				if(!pTree->IsMultiSelect() || ::GetKeyState(VK_CONTROL)>=0) pTree->ClearSeletedNodes();
				GetOwner()->SelectNode(m_pNodeData);
				GetOwner()->SetFocusNode(m_pNodeData);
				GetOwner()->Refresh(false);
				pTree->SendNotify(DUI_MSGTYPE_SELECTCHANGED, 0, 0);
			}
			return;
		}

		if( event.Type == UIEVENT_DBLCLICK ) 
		{
			if( IsEnabled() ) 
			{
				m_pNodeData->Expand(!m_pNodeData->IsExpand());
				GetOwner()->Refresh(true);
				m_pManager->SendNotify(pTree, DUI_MSGTYPE_DBCLICK, (WPARAM)this);			
			}
			return;
		}

		if( event.Type == UIEVENT_MOUSEENTER ) 
		{
			if( IsEnabled() ) {
				SetHotState(true);
				Invalidate();
			}
			return;
		}

		if( event.Type == UIEVENT_MOUSELEAVE ) 
		{
			if( IsEnabled() ) {
				SetHotState(false);
				Invalidate();
			}
			return;
		}

		__super::DoEvent(event);
	}
}