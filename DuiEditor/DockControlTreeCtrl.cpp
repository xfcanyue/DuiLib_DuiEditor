// ViewControlTree.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockControlTreeCtrl.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DuiEditorDoc.h"
#include "DuiEditorViewDesign.h"
#include "DockPropertyWnd.h"
#include "UIManager.h"

// CViewControlTree

IMPLEMENT_DYNAMIC(CDockControlTreeCtrl, CTreeCtrl)

CDockControlTreeCtrl::CDockControlTreeCtrl()
{
	m_pUIManager = NULL;
// 	m_hItemDragSrc = NULL;
// 	m_hItemDragDest = NULL;
// 
 	m_bIsDragging = FALSE;
 	m_bValidDrop = FALSE;
// 	m_pDragImage = NULL; 

	m_nMaxDeep = 3;
	m_nCurrentDeep = 0;

}

CDockControlTreeCtrl::~CDockControlTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDockControlTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CDockControlTreeCtrl::OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CDockControlTreeCtrl::OnTvnBeginrdrag)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CDockControlTreeCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CDockControlTreeCtrl::OnNMCustomdraw)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()



// CViewControlTree 消息处理程序
BOOL CDockControlTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CDockControlTreeCtrl::InitTreeContent()
{
	LockWindowUpdate();

	DeleteAllItems();

	pugi::xml_node root = GetUIManager()->GetDocument()->m_doc.root().child(XTEXT("Window"));

	HTREEITEM hRootItem = InsertItem(XML2T(root.name()), 0, 0, TVI_ROOT, TVI_LAST);
	SetItemData(hRootItem, (DWORD)root.internal_object());

	int type = CDuiPropertyFile::GetControlIconIndex(root);
	SetItemImage(hRootItem, type, type);

	m_nMaxDeep = g_cfg.nControlTreeDeep;
	m_nCurrentDeep = 0;
	OpenXmlDocument(root, hRootItem);

	SelectItem(hRootItem);
	GetUIManager()->GetPropList()->InitProp(root);

	UnlockWindowUpdate();
}

void CDockControlTreeCtrl::OpenXmlDocument(pugi::xml_node elem, HTREEITEM hTreeParent)
{
	if(elem.first_child())
	{
		m_nCurrentDeep++;
	}

	for (pugi::xml_node node = elem.first_child(); node; node = node.next_sibling())
	{
		if(node.type() != node_element)
			continue;

		if(!node.first_child())
		{
			HTREEITEM hTreeItem = AddNewControl(node, hTreeParent);
			//Expand(GetParentItem(hTreeItem), TVE_EXPAND);
			if(m_nCurrentDeep >= m_nMaxDeep)
				Expand(GetParentItem(hTreeItem), TVE_COLLAPSE);
		}
		else
		{
			HTREEITEM hTreeItem = AddNewControl(node, hTreeParent);
			OpenXmlDocument(node, hTreeItem);
		}
	}

	if(elem.first_child())
	{
		m_nCurrentDeep--;
	}

	//Expand(hTreeParent, TVE_EXPAND);
	if(m_nCurrentDeep >= m_nMaxDeep)
		Expand(hTreeParent, TVE_COLLAPSE);
}

HTREEITEM CDockControlTreeCtrl::AddNewControl(xml_node node, HTREEITEM hParent, HTREEITEM hInertAfter)
{
	CString text = XML2T(node.name());

	xml_attribute attr = node.attribute(XTEXT("sname"));
	if(!attr)	attr = node.attribute(XTEXT("name"));
	if(attr)
	{
		CString strControlName = XML2T(attr.as_string());
		if(!strControlName.IsEmpty())
			text.Format(_T("%s %s=\"%s\""), XML2T(node.name()), XML2T(attr.name()), XML2T(attr.as_string()));
	}

	if(node.type() == node_comment)
	{
		text.Format(_T("<!-- %s -->"), XML2T(node.value()));
	}

	HTREEITEM hItem = InsertItem(text, 0, 0, hParent, hInertAfter);
	SetItemData(hItem, (DWORD)node.internal_object());

	int type = CDuiPropertyFile::GetControlIconIndex(node);
	SetItemImage(hItem, type, type);

	SelectItem(hItem);
	return hItem;
}

HTREEITEM CDockControlTreeCtrl::AddNewControl(xml_node node, HTREEITEM hInertAfter)
{
	HTREEITEM hItemParent = FindXmlNode(node.parent());
	if(hItemParent == NULL)	return NULL;

	return AddNewControl(node, hItemParent, hInertAfter);
}

HTREEITEM CDockControlTreeCtrl::AddNewControl(xml_node nodeNewContrl, xml_node nodeCurrentControl, HTREEITEM htiAfter)
{
	HTREEITEM hItemNew = NULL;

	HTREEITEM hItemCurrent = FindXmlNode(nodeCurrentControl);
	if(!hItemCurrent)	return NULL;

	if(htiAfter == TVI_BEFORE)
	{
		HTREEITEM hItemParent = GetParentItem(hItemCurrent);
		if(hItemParent == NULL)	
		{
			AfxMessageBox(_T("插入到控件树中出错了。"));
			return NULL;
		}

		HTREEITEM hPrev = GetNextItem(hItemCurrent, TVGN_PREVIOUS);
		if(hPrev) //插入到前一个兄弟项的后面
		{
			hItemNew = AddNewControl(nodeNewContrl, hItemParent, hPrev);
		}
		else //插入到第一个
		{
			hItemNew = AddNewControl(nodeNewContrl, hItemParent, TVI_FIRST);
		}
		return hItemNew;
	}
	else if(htiAfter == TVI_NEXT)
	{
		HTREEITEM hItemParent = GetParentItem(hItemCurrent);
		if(hItemParent == NULL)	
		{
			AfxMessageBox(_T("插入到控件树中出错了。"));
			return NULL;
		}
		return AddNewControl(nodeNewContrl, hItemParent, hItemCurrent);
	}

	return AddNewControl(nodeNewContrl, hItemCurrent, TVI_LAST);
}

void CDockControlTreeCtrl::UpdateXmlNode(xml_node node)
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)	return;

	if(hItem == GetRootItem())
		return;

	if(GetItemData(hItem) != (DWORD_PTR)node.internal_object())
		return;

	CString text = XML2T(node.name());

	CString strName = XML2T(node.attribute(XTEXT("name")).as_string());
	if(!strName.IsEmpty())
	{
		CString temp;
		temp.Format(_T(" %s=\"%s\""), XML2T(node.attribute(XTEXT("name")).name()), XML2T(node.attribute(XTEXT("name")).as_string()));
		text += temp;
	}

	CString strSName = XML2T(node.attribute(XTEXT("sname")).as_string());
	if(!strSName.IsEmpty())
	{
		CString temp;
		temp.Format(_T(" %s=\"%s\""), XML2T(node.attribute(XTEXT("sname")).name()), XML2T(node.attribute(XTEXT("sname")).as_string()));
		text += temp;
	}

	SetItemText(hItem, text);
}

void CDockControlTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CPoint point;
	if(!GetCursorPos(&point))	return;

	ScreenToClient(&point); 
	HTREEITEM hItem = HitTest(point); 
	if(hItem)
	{
		SelectItem(hItem);

		xml_node node((xml_node_struct *)GetItemData(hItem));
		GetUIManager()->GetPropList()->InitProp(node);

		GetUIManager()->SelectItem(node);
		GetUIManager()->EnsureVisible(node);
		GetUIManager()->GetCodeView()->SelectXmlNode(node);
	}
}

void CDockControlTreeCtrl::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	if(!GetCursorPos(&point))	return;

	ScreenToClient(&point); 
	HTREEITEM hItem = HitTest(point); 
	if(hItem)
	{
		SelectItem(hItem);

		xml_node node((xml_node_struct *)GetItemData(hItem));
		GetUIManager()->GetPropList()->InitProp(node);

		GetUIManager()->SelectItem(node);
		GetUIManager()->EnsureVisible(node);
		GetUIManager()->GetCodeView()->SelectXmlNode(node);
	}

//////////////////////////////////////////////////////////////////////////
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_EDIT);

	CMenu menutab;
	menutab.LoadMenu(IDR_MENU_TABLAYOUT_SETSEL);

	//TabLayout列出切换菜单项
	hItem = GetSelectedItem();
	if(hItem)
	{
		xml_node node((xml_node_struct *)GetItemData(hItem));
		if(node)
		{
			CControlUI *pControl = (CControlUI *)node.get_tag();
			//if(g_duiProp.IsTabLayoutNode(node))
			if(pControl && pControl->GetInterface(DUI_CTR_TABLAYOUT))
			{
				if(node.first_child())
				{
					::DeleteMenu(menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, 0, MF_BYPOSITION);

					UINT nCmd = ID_TABLAYOUT_SETSEL_00;
					for (xml_node nd = node.first_child(); nd; nd=nd.next_sibling())
					{
						CString temp;
						temp.Format(_T("页面%d %s name=%s"), nCmd-ID_TABLAYOUT_SETSEL_00+1, XML2T(nd.name()), XML2T(nd.attribute(XTEXT("name")).value()));
						::InsertMenu(menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, -1, MF_BYPOSITION, nCmd, temp);
						nCmd++;
					}

					::InsertMenu(menu.GetSubMenu(0)->m_hMenu, -1, MF_BYPOSITION | MF_POPUP, 
						(UINT_PTR)menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, _T("切换TabLayout页面"));
				}
			}
		}
	}
	CPoint curpoint;
	if(!GetCursorPos(&curpoint))	return;
	theApp.GetContextMenuManager()->ShowPopupMenu(menu.GetSubMenu(0)->m_hMenu, curpoint.x, curpoint.y, AfxGetMainWnd(), TRUE);
//////////////////////////////////////////////////////////////////////////
	*pResult = 0;
}

HTREEITEM CDockControlTreeCtrl::FindXmlNode(xml_node node)
{
	HTREEITEM ht = GetRootItem();
	while (ht!=NULL)
	{
		if(GetItemData(ht) == UINT_PTR(node.internal_object()))
			return ht;
		ht = GetNextItemEx(ht);
	}
	return NULL;
}

HTREEITEM CDockControlTreeCtrl::SelectXmlNode(xml_node node)
{
	CLockWindowUpdate lockUpdate(this);

	HTREEITEM hItem = FindXmlNode(node);
	if(hItem)
	{
		SelectItem(hItem);
	}
	else
	{
		hItem = GetRootItem();
		SelectItem(hItem);
	}

	GetUIManager()->GetPropList()->InitProp(node);

	return NULL;
}

xml_node CDockControlTreeCtrl::GetSelectXmlNode()
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem != NULL)
	{
		xml_node node((xml_node_struct *)GetItemData(hItem));
		return node;
	}
	
	xml_node node;
	return node;
}

BOOL CDockControlTreeCtrl::DeleteXmlNode(xml_node node)
{
	HTREEITEM hItem = FindXmlNode(node);
	if(hItem)
	{
		return DeleteItem(hItem);
	}
	return FALSE;
}

BOOL CDockControlTreeCtrl::OnDragXmlNode(HTREEITEM src, HTREEITEM dest)
{
	xml_node nodesrc = xml_node((xml_node_struct *)GetItemData(src));
	xml_node nodedest = xml_node((xml_node_struct *)GetItemData(dest));

	//

	//判断目标位置是否容器
	if(!g_duiProp.IsContainerUI(nodedest))
		return FALSE;

	xml_node nodeNew;
	if(nodedest.first_child())
		nodeNew = nodedest.prepend_move(nodesrc);
	else
		nodeNew = nodedest.append_move(nodesrc);

	//拖动节点之后, 刷新视图
	if(nodeNew)
	{
		CDuiEditorViewDesign *pView = GetUIManager()->GetDesignView();
		if(pView)
		{
//			pView->InitView();
		}
	}

	return nodeNew != NULL;
}

HTREEITEM CDockControlTreeCtrl::GetNextItemEx(HTREEITEM hItem)
{
	HTREEITEM ht = NULL;
	ht = GetChildItem(hItem); //先查找子项
	if(ht==NULL) //没有子项，
	{
		ht = GetNextSiblingItem(hItem);//再查找兄弟项
		if(ht==NULL) //没有兄弟项，就开始回滚查找父项的兄弟
		{
			HTREEITEM hPare = GetParentItem(hItem); //获取父项
			while (hPare!=NULL)
			{
				ht = GetNextSiblingItem(hPare); //查找父项的兄弟项
				if(ht==NULL) //父项没有兄弟项, 
				{
					hPare = GetParentItem(hPare); //就继续查找父项的父项，一直往上回滚。
				}
				else
				{
					return ht; //父项有兄弟项，就返回。
				}
			}
		}
	}

	return ht;
}


void CDockControlTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
	m_dwDragStart = GetTickCount();

	CPoint pt;
	GetCursorPos(&pt);

	m_dwDragStart = GetTickCount();

	HTREEITEM hTreeItem =  NULL;
	if (pt != CPoint(-1, -1))
	{
		CPoint ptTree = pt;
		ScreenToClient(&ptTree);

		UINT flags = 0;
		hTreeItem = HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			SelectItem(hTreeItem);
		}
		else
		{
			SelectItem(NULL);
			return;
		}
	}
	SetFocus();
}

void CDockControlTreeCtrl::OnTvnBeginrdrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	//如果是无意拖动，则放弃操作 
	if( (GetTickCount() - m_dwDragStart) < 200 )       
		return; 


	m_hItemDragSrc = pNMTreeView->itemNew.hItem;
	if(m_hItemDragSrc == NULL)	return;

	SelectItem(m_hItemDragSrc);

	m_pDragImage = CreateDragImage(m_hItemDragSrc);
	m_bIsDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(0, 0));

	POINT pt = pNMTreeView->ptDrag;
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}

void CDockControlTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsDragging)
	{
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		CTreeCtrl::OnMouseMove(nFlags, point);
		return;
	}

	POINT pt = point;
	ClientToScreen( &pt );
	CImageList::DragMove(pt);

	if(!OnDraging())
	{
		::SetCursor(LoadCursor(NULL, IDC_NO));
		m_bValidDrop = FALSE;
	}
	else
	{
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_bValidDrop = TRUE;
	}	

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CDockControlTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if(m_bIsDragging)
	{
		m_bIsDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		if(m_pDragImage != NULL) 
		{ 
			delete m_pDragImage; 
			m_pDragImage = NULL; 
			SelectDropTarget(NULL);
		}

		if(m_bValidDrop)
		{
			if(!OnDragXmlNode(m_hItemDragSrc, m_hItemDragDest))
				return;

			HTREEITEM  htiParent = m_hItemDragDest; 
			while( (htiParent = GetParentItem(htiParent)) != NULL )      
			{      
				if( htiParent == m_hItemDragSrc )   
				{                 
					//建立一个临时节点以完成操作             
					HTREEITEM  htiNewTemp = CopyBranch( m_hItemDragSrc,NULL,TVI_LAST );               
					HTREEITEM  htiNew = CopyBranch( htiNewTemp,m_hItemDragDest, TVI_FIRST );               
					DeleteItem( htiNewTemp );               
					SelectItem( htiNew );   
					return;           
				}      
			} 

			HTREEITEM  htiNew = CopyBranch(m_hItemDragSrc,m_hItemDragDest, TVI_FIRST);       
			DeleteItem( m_hItemDragSrc );       
			SelectItem( htiNew ); 


			m_hItemDragSrc = NULL;
			m_hItemDragDest = NULL;
		}

	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

BOOL CDockControlTreeCtrl::OnDraging()
{
	CPoint pt;
	GetCursorPos(&pt);

	HTREEITEM hTreeItem =  NULL;
	if (pt == CPoint(-1, -1))	return FALSE;

	CPoint ptTree = pt;
	ScreenToClient(&ptTree);

	UINT flags = 0;
	hTreeItem = HitTest(ptTree, &flags);
	if(hTreeItem == NULL)
		return FALSE;

	//不允许拖拽到自己
	if(m_hItemDragSrc == hTreeItem)
		return FALSE;

	//不允许父亲拖到儿子
	HTREEITEM hParent = GetParentItem(hTreeItem);
	while (hParent != NULL)
	{
		if(m_hItemDragSrc == hParent)
			return FALSE;

		hParent = GetParentItem(hParent);
	}

	xml_node nodedest = xml_node((xml_node_struct *)GetItemData(hTreeItem));
	if(!g_duiProp.IsContainerUI(nodedest))
		return FALSE;

	CImageList::DragShowNolock(FALSE);
	SelectDropTarget(hTreeItem);
	m_hItemDragDest = hTreeItem;
	CImageList::DragShowNolock(TRUE);

	return TRUE;
}

HTREEITEM CDockControlTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter) 
{    
	TV_INSERTSTRUCT  tvstruct;  
	HTREEITEM        hNewItem;    
	CString          sText; 

	tvstruct.item.hItem = hItem; 
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;    
	GetItem( &tvstruct.item );   
	sText = GetItemText( hItem ); 
	tvstruct.item.cchTextMax = sText.GetLength ();   
	tvstruct.item.pszText    = sText.LockBuffer (); 
	tvstruct.hParent         = htiNewParent;    
	tvstruct.hInsertAfter    = htiAfter;  
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT; 
	hNewItem = InsertItem( &tvstruct );   
	sText.ReleaseBuffer(); 

	//限制拷贝条目数据和条目状态 
	SetItemData( hNewItem,GetItemData(hItem) );  
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK); 

	return hNewItem; 
} 

HTREEITEM CDockControlTreeCtrl::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter) 
{   
	HTREEITEM  hChild;   
	HTREEITEM  hNewItem = CopyItem( htiBranch,htiNewParent,htiAfter );    
	hChild = GetChildItem( htiBranch ); 

	while( hChild != NULL ) 
	{       
		CopyBranch( hChild,hNewItem,htiAfter );      
		hChild = GetNextSiblingItem( hChild );  
	} 

	return  hNewItem; 
} 

void CDockControlTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	*pResult = CDRF_NOTIFYITEMDRAW;

	LPNMTVCUSTOMDRAW pDraw=(LPNMTVCUSTOMDRAW)pNMHDR;
	DWORD dwDrawStage=pDraw->nmcd.dwDrawStage;
	UINT uItemState =pDraw->nmcd.uItemState;
	if((dwDrawStage & CDDS_ITEM) && (uItemState & CDIS_SELECTED))
	{
		pDraw->clrText = RGB(255,255,255);
		pDraw->clrTextBk = GetSysColor(COLOR_HIGHLIGHT);
	}
}



INT_PTR CDockControlTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	RECT rect;
	UINT nFlags;
	HTREEITEM hitem = HitTest( point, &nFlags );
	if( nFlags & TVHT_ONITEMLABEL )
	{
		GetItemRect( hitem, &rect, TRUE );
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT)hitem;
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rect;
		return pTI->uId;
	}
	return -1;
}

BOOL CDockControlTreeCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXT* pToolTipText = (TOOLTIPTEXT*)pNMHDR;

	CString strTipText;
	UINT nID = (UINT)pNMHDR->idFrom;

	// Do not process the message from built in tooltip 
	if( nID == (UINT)m_hWnd && pNMHDR->code == TTN_NEEDTEXT && pToolTipText->uFlags & TTF_IDISHWND )
		return FALSE;

	// Get the mouse position
	const MSG* pMessage;
	CPoint pt;
	pMessage = GetCurrentMessage(); // get mouse pos 
	ASSERT ( pMessage );
	pt = pMessage->pt;
	ScreenToClient( &pt );

 	UINT nFlags;
 	HTREEITEM hitem = HitTest( pt, &nFlags ); //Get item pointed by mouse

	xml_node node((xml_node_struct *)GetItemData(hitem));;

	CString temp;
	temp.Format(_T("<%s "), XML2T(node.name()));
	strTipText += temp;
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		temp.Format(_T("%s=\"%s\""), XML2T(attr.name()), XML2T(attr.value()));
		strTipText += temp;
		strTipText += _T(" ");
	}
	strTipText += _T(">");

	_tcscpy_s(pToolTipText->szText, 80, strTipText.Left(79));
	*pResult = 0;
	return TRUE;    // message was handled
}
