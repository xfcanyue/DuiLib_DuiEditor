#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIWindowMenu.h"

#include "MainFrm.h"
#include "DuiEditorViewDesign.h"
#include "DlgInsertControl.h"
#include "DlgCustomControl.h"
#include "MenuExUI.h"
#include "MenuElementExUI.h"

CUIWindowMenu::CUIWindowMenu(void)
{
}


CUIWindowMenu::~CUIWindowMenu(void)
{
}

HWND CUIWindowMenu::CreateUiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle, DWORD dwExStyle) 
{ 
	return CreateDuiWindow(hwndParent, pstrWindowName, dwStyle, dwExStyle); 
}

CControlUI* CUIWindowMenu::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("Menu")) == 0)
	{
		return new CMenuExUI();
	}
	else if (_tcsicmp(pstrClass, _T("MenuElement")) == 0)
	{
		return new CMenuElementExUI();
	}
	return NULL;
}

void CUIWindowMenu::AddNewControlFromToolBox(xml_node nodeToolBox, CPoint point)
{
	//当前节点，从控件树获取
	xml_node nodeCurrent = m_pManager->GetTreeView()->GetSelectXmlNode();					//树选中的控件

	CRect rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	if(!rcClient.PtInRect(point))
	{
		return;
	}

	CControlUI *pNewControl = NULL;
	xml_node nodeContainer;

	CString strClass = nodeToolBox.name();

	//当前节点是容器
	if(g_duiProp.IsBaseFromContainer(nodeCurrent.name()))
	{
		nodeContainer = nodeCurrent;
	}
	else
	{
		//当前节点不是容器, 往上寻找
		nodeContainer = m_pManager->FindContainer(nodeCurrent); //pControl;
	}

	CContainerUI *pParent = (CContainerUI *)nodeContainer.get_tag();
	CControlUI   *pCurControl = (CControlUI *)nodeCurrent.get_tag();

	//找到FORM那里了, 如果即将插入的控件不是容器, 报错
	if(m_pManager->GetUiFrom() == pParent)
	{
		if(!g_duiProp.IsBaseFromContainer(strClass))
		{
			AfxMessageBox(_T("控件需要放置于容器内."));
			return;
		}
	}


	CUITracker tracker;
	CRect rc;
	CWnd *pWnd = CWnd::FromHandle(m_hWnd);
	if(tracker.TrackRubberBand(pWnd, point, TRUE))
	{
		rc.left = point.x;
		rc.top = point.y;
		rc.right = rc.left + tracker.m_rect.Width();
		rc.bottom = rc.top + tracker.m_rect.Height();
	}

	pNewControl = CUIBuilder::CreateControl(strClass);
	BOOL bCustomControl = FALSE;
	CString strCuscomControlParent;
	if(!pNewControl) 
	{
		//如果创建不成功, 认为是自定义控件.
		CDlgCustomControl dlg;
		if(dlg.DoModal() != IDOK)
			return;

		pNewControl = CUIBuilder::CreateControl(dlg.m_strParentControl);
		if(!pNewControl)	return;

		strClass = dlg.m_strClassName;
		strCuscomControlParent = dlg.m_strParentControl;
		bCustomControl = TRUE;
	}

	CDlgInsertControl dlg;
	dlg.nodeControl = nodeCurrent;
	if(dlg.DoModal() != IDOK)	return;

	xml_node nodeNewControl;
	switch (dlg.m_nPosition)
	{
	case 0: //当前容器下方插入控件
		{
			if(!pParent->Add(pNewControl)) { delete pNewControl; return; }		
			nodeNewControl = nodeContainer.append_child(strClass);	//写入文档
		}
		break;
	case 1:	//当前控件下方插入兄弟控件
		{
			if(! pParent->AddAt(pNewControl, pParent->GetItemIndex(pCurControl)) ) { delete pNewControl; return; }
			nodeNewControl = nodeContainer.append_child(strClass);	//写入文档
		}
		break;
	case 2:	//当前控件上方插入兄弟控件
		{
			int nIndex = pParent->GetItemIndex(pCurControl);
			if(nIndex == 0) //第一个
			{
				if(! pParent->AddAt(pNewControl, nIndex) ) { delete pNewControl; return; }
				pParent->SetItemIndex(pNewControl, 0);
			}
			else
			{
				if(! pParent->AddAt(pNewControl, nIndex-1) ) { delete pNewControl; return; }
			}
			nodeNewControl = nodeContainer.insert_child_before(strClass, nodeCurrent);	//写入文档
		}
		break;
	}

	if(bCustomControl)
	{
		nodeNewControl.attribute_auto(_T("custombasedfrom")).set_value(strCuscomControlParent);
	}

	//保存文档和控件的双向指针
	pNewControl->SetTag((UINT_PTR)nodeNewControl.internal_object());
	nodeNewControl.set_tag((UINT_PTR)pNewControl);

	if(m_pManager->GetView()->m_nFormatInsert == 0)	//自动定位
	{
		RECT rcNew;
		rcNew.left = 0;
		rcNew.top = 0;
		rcNew.right = rc.right - rc.left;
		rcNew.bottom = rc.bottom - rc.top;

		pNewControl->SetAttribute(_T("pos"), RectToString(&rcNew));

		g_duiProp.AddAttribute(nodeNewControl, _T("pos"), RectToString(&rcNew),			NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("width"), rcNew.right-rcNew.left,		NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("height"), rcNew.bottom-rcNew.top,	NULL);
	}
	else if(m_pManager->GetView()->m_nFormatInsert == 1)	//绝对定位
	{
		CRect rcParent = pParent->GetPos();
		CRect rcNew;
		rcNew.left = rc.left - rcParent.left;
		rcNew.right = rcNew.left + rc.right - rc.left;
		rcNew.top = rc.top - rcParent.top;
		rcNew.bottom = rcNew.top + rc.bottom - rc.top;

		pNewControl->SetAttribute(_T("pos"), RectToString(&rcNew));

		g_duiProp.AddAttribute(nodeNewControl, _T("float"), _T("true"),					NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("pos"), RectToString(&rcNew),			NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("width"), rcNew.right-rcNew.left,		NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("height"), rcNew.bottom-rcNew.top,	NULL);
	}
	else if(m_pManager->GetView()->m_nFormatInsert == 2)	//相对定位
	{
		CRect rcParent = pNewControl->GetParent()->GetPos();
		CRect rcNew;
		rcNew.left = rc.left - rcParent.left;
		rcNew.right = rcNew.left + rc.right - rc.left;
		rcNew.top = rc.top - rcParent.top;
		rcNew.bottom = rcNew.top + rc.bottom - rc.top;

		pNewControl->SetAttribute(_T("pos"), RectToString(&rcNew));

		g_duiProp.AddAttribute(nodeNewControl, _T("float"),			_T("true"),				NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("relativepos"),	_T("50,50,0,0"),		NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("pos"),			RectToString(&rcNew),	NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("width"),			rcNew.right-rcNew.left, NULL);
		g_duiProp.AddAttribute(nodeNewControl, _T("height"),		rcNew.bottom-rcNew.top, NULL);
	}

	//载入控件默认属性
	LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(strClass);
	if( pDefaultAttributes ) 
	{
		pNewControl->ApplyAttributeList(pDefaultAttributes);
	}

	//载入控件当前属性

	//插入控件树
	if(dlg.m_nPosition == 2)
		m_pManager->GetTreeView()->AddNewControl(nodeNewControl, nodeCurrent, TVI_BEFORE);
	else
		m_pManager->GetTreeView()->AddNewControl(nodeNewControl, nodeCurrent, TVI_NEXT);

	//刷新属性窗口
	g_pPropWnd->InitProp(nodeNewControl);

	//刷新控件
	m_pManager->UpdateControlUI(nodeNewControl);

	//只选中新添加的控件
	m_tracker.RemoveAll();
	m_tracker.Add(nodeNewControl, pNewControl->GetPos());

	//插入command history
	m_pManager->GetCmdHistory()->AddNewControl(nodeNewControl);

	m_pManager->GetDocument()->SetModifiedFlag(TRUE);
}

void CUIWindowMenu::OnSelectingControl(CControlUI *pControl, const CRect &rcTracker)
{
	xml_node nodeControl((xml_node_struct *)pControl->GetTag());
	if(!nodeControl)
		return;
	for (xml_node node=nodeControl.first_child(); node; node=node.next_sibling())
	{
		CControlUI *p = (CControlUI *)node.get_tag();
		if(!p)	return;
		CRect rc = p->GetPos();
		CRect rc1;
		if(rc1.IntersectRect(rc, &rcTracker))
		{
			m_listTrackerSeleted.push_back(p);
		}

		OnSelectingControl(p, rcTracker);
	}
}

LRESULT CUIWindowMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg )
	{
	case WM_CREATE:       
		lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_KILLFOCUS:       
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
		break;
// 	case WM_KEYDOWN: //不能关闭窗口
// 		if( wParam == VK_ESCAPE || wParam == VK_LEFT)
// 			Close();
// 		break;
	case WM_SIZE:
		lRes = OnSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_CLOSE:
		if( m_pOwner != NULL )
		{
			m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
			m_pOwner->SetPos(m_pOwner->GetPos());
			m_pOwner->SetFocus();
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return 0L;
		break;
	case WM_PAINT:			lRes = OnPaint(uMsg, wParam, lParam, bHandled);	break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
		break;
	}

	if(uMsg == WM_LBUTTONDOWN)
	CString strMessage = GetMessageText(uMsg);

// 	if(m_pManager->GetView()->m_bShowUiPreview)
// 	{
// 		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
// 			return lRes;
// 	}
// 	else
// 	{
// 		if(uMsg == WM_PAINT)
// 		{
// 			if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
// 				return lRes;
// 		}
// 	}

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIWindowMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_tracker.m_nHandleSize = 6;
	m_tracker.m_nStyle = CUITracker::dottedLine|CUITracker::resizeInside;
	m_tracker.m_pManager = m_pManager;

	bool bShowShadow = false;
	if( m_pOwner != NULL) {
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		m_pm.Init(m_hWnd);
		m_pm.GetDPIObj()->SetScale(m_pOwner->GetManager()->GetDPIObj()->GetDPI());
		// The trick is to add the items to the new container. Their owner gets
		// reassigned by this operation - which is why it is important to reassign
		// the items back to the righfull owner/manager when the window closes.
		m_pLayout = new CMenuUI();
		m_pm.SetForceUseSharedRes(true);
		m_pLayout->SetManager(&m_pm, NULL, true);
		LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(_T("Menu"));
		if( pDefaultAttributes ) {
			m_pLayout->ApplyAttributeList(pDefaultAttributes);
		}
		m_pLayout->GetList()->SetAutoDestroy(false);

		for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
			if(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL ){
				(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
				m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
			}
		}

		CShadowUI *pShadow = m_pOwner->GetManager()->GetShadow();
		pShadow->CopyShadow(m_pm.GetShadow());
		bShowShadow = m_pm.GetShadow()->IsShowShadow();
		m_pm.GetShadow()->ShowShadow(false);
		m_pm.SetLayered(m_pOwner->GetManager()->IsLayered());
		m_pm.AttachDialog(m_pLayout);
		m_pm.AddNotifier(this);

		ResizeSubMenu();
	}
	else {

		CMenuWnd::GetGlobalContextMenuObserver().SetManger(&m_pm);

		m_pm.Init(m_hWnd);
		m_pm.GetDPIObj()->SetScale(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetDPIObj()->GetDPI());
		CUIBuilder builder;

		CDuiEditorDoc *pDoc = m_pManager->GetDocument();

		CControlUI* pRoot = builder.Create(pDoc->m_doc, this, &m_pm, NULL);
		bShowShadow = m_pm.GetShadow()->IsShowShadow();
		m_pm.GetShadow()->ShowShadow(false);
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		ResizeMenu();
	}
	GetMenuUI()->m_pWindow = this;
	m_pm.GetShadow()->ShowShadow(bShowShadow);
	m_pm.GetShadow()->Create(&m_pm);
	return 0;
}


LRESULT CUIWindowMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HWND hFocusWnd = (HWND)wParam;

	BOOL bInMenuWindowList = FALSE;
	ContextMenuParam param;
	param.hWnd = GetHWND();

	MenuObserverImpl::Iterator iterator(CMenuWnd::GetGlobalContextMenuObserver());
	MenuMenuReceiverImplBase* pReceiver = iterator.next();
	while( pReceiver != NULL ) {
		CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
		if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
			bInMenuWindowList = TRUE;
			break;
		}
		pReceiver = iterator.next();
	}

	if( !bInMenuWindowList ) {
		param.wParam = 1;
		CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
		return 0;
	}
	return 0;
}


LRESULT CUIWindowMenu::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	m_pm.MessageHandler(uMsg, wParam, lParam, lRes);

	HDC hDC = ::GetDC(m_hWnd);
	CDC *pDC = CDC::FromHandle(hDC);

	m_tracker.Draw(pDC, NULL);

	CRect rectClient;
	::GetClientRect(m_hWnd, rectClient);
	if(m_pManager->GetView()->m_bViewGrid)
	{
		for(int i=rectClient.left; i<rectClient.right; i+=10)
		{
			for(int j=rectClient.top; j<rectClient.bottom; j+=10)
				pDC->SetPixel(i, j, RGB(0,0,0));
		}
	}


	if(m_pManager->GetView()->m_bViewMouse)
	{
		CPoint point;
		GetCursorPos(&point);
		::ScreenToClient(m_hWnd, &point);
		if(rectClient.PtInRect(point))
		{
			CPen pen(PS_DOT, 1, RGB(0,0,0));
			CPen *pOldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(0, point.y);
			pDC->LineTo(999, point.y);
			pDC->MoveTo(point.x, 0);
			pDC->LineTo(point.x, 999);

			CRect rcMouseText;
			rcMouseText.left	= point.x;
			rcMouseText.right	= point.x + 100;
			rcMouseText.top		= point.y - 30;
			rcMouseText.bottom	= point.y;

			pDC->SetBkMode(TRANSPARENT);
			CString strText;
			CPoint pt2 = point;
			strText.Format(_T("( %d, %d )"), pt2.x, pt2.y);
			pDC->DrawText(strText, rcMouseText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			pDC->SelectObject(pOldPen);
		}
	}

	::ReleaseDC(m_hWnd, hDC);
	return 0;
}

LRESULT CUIWindowMenu::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pManager->GetView()->m_bShowUiPreview)
	{
		LRESULT lRes = 0;
		m_pm.MessageHandler(uMsg, wParam, lParam, lRes);
	}

	CWnd *pWnd = CWnd::FromHandle(m_hWnd);

	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hWnd, &pt);

	xml_node nodeToolBox((xml_node_struct *)g_pToolBox->GetCurSelTag());
	if(nodeToolBox) //添加控件
	{ 
		m_tracker.RemoveAll();
		AddNewControlFromToolBox(nodeToolBox, pt);	
		g_pToolBox->SetCurSel(TREENODETYPE_POINTER);
		return 0;
	}	

	CControlUI* pControl = GetManager()->FindControl(pt);
	if(pControl==NULL) 
	{
		m_tracker.RemoveAll();
		return 0;
	}

	xml_node nodeClick;
	//DWORD dt = GetTickCount();
	while (TRUE)
	{
		if(!pControl)	break;
		//if(GetTickCount() - dt > 1000) break; //搜索控件有超时时间

		nodeClick = xml_node((xml_node_struct *)pControl->GetTag());
		if(nodeClick)	break;
		else
		{
			pControl = pControl->GetParent();
		}
	}

	if(!nodeClick)
	{
		m_tracker.RemoveAll();
		return 0;
	}

	pControl = (CControlUI*)nodeClick.get_tag();
	if(pControl==NULL) 
	{
		m_tracker.RemoveAll();
		return 0;
	}

	int nHit = m_tracker.HitTest(pt);
	m_tracker.m_nHitTest = nHit;
	if(::GetKeyState(VK_CONTROL)>=0  && nHit==CUITracker::hitNothing)
		m_tracker.RemoveAll();
	if(nHit==CUITracker::hitNothing)
	{
		m_tracker.Add(nodeClick, pControl->GetPos());
		m_pManager->GetTreeView()->SelectXmlNode(nodeClick);
	}
	else
		m_tracker.SetFocus(nodeClick);

	//设置键盘输入光标
	// 	CControlUI *pFocusControl = (CControlUI *)m_tracker.m_pFocused->m_node.get_tag();
	// 	if(pFocusControl)
	// 	{
	// 		CRect rcFocusControl = pFocusControl->GetPos();
	// 		::SetCaretPos(rcFocusControl.left, rcFocusControl.bottom);
	// 	}

	if(nHit > 0 && (g_duiProp.IsBaseFromControlUI(nodeClick.name()) || g_duiProp.IsWindowForm(nodeClick.name())))
	{
		m_tracker.Track(pWnd, pt, TRUE);
	}
	else
	{
		CUITracker tracker;
		tracker.TrackRubberBand(pWnd, pt, TRUE);
		CRect rc = tracker.m_rect;
		rc.NormalizeRect();

		//鼠标拉动框选控件, 必须是一个容器里面的
		if(g_duiProp.IsBaseFromContainer(nodeClick.name()))
		{
			m_listTrackerSeleted.clear();
			OnSelectingControl(pControl, rc);
			if(m_listTrackerSeleted.size() > 0)
			{
				m_tracker.RemoveAll();
				std::list<CControlUI *>::iterator it;
				for (it=m_listTrackerSeleted.begin(); it!=m_listTrackerSeleted.end(); it++)
				{
					CControlUI *p = (CControlUI *)(*it);
					xml_node n((xml_node_struct *)p->GetTag());
					m_tracker.Add(n, p->GetPos());
				}
			}
		}
	}

	Invalidate();
	return 0;
}

LRESULT CUIWindowMenu::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pManager->GetView()->m_bShowUiPreview)
	{
		bHandled = FALSE;
		return 0;
	}

	g_pToolBox->SetDefaultPoint();
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	return 0;
}

LRESULT CUIWindowMenu::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pManager->GetView()->m_bShowUiPreview)
	{
		bHandled = FALSE;
		return 0;
	}

	if(m_pManager->GetView()->m_bViewMouse)
	{
		Invalidate();
	}

	return 0;
}

LRESULT CUIWindowMenu::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pManager->GetView()->m_bShowUiPreview)
	{
		bHandled = FALSE;
		return 0;
	}

	UINT nHittest = LOWORD(lParam); 

	if(m_pManager->GetView()->m_bShowUiPreview)
		return 0;

	xml_node nodeToolBox((xml_node_struct *)g_pToolBox->GetCurSelTag());
	if(nodeToolBox)
	{
		::SetCursor(LoadCursor(NULL, IDC_CROSS));
		return 0;
	}

	CWnd *pWnd = CWnd::FromHandle(m_hWnd);
	if(m_tracker.SetCursor(pWnd, nHittest, CPoint(0,0), CSize(0,0)))
		return 0;

	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	return 0;
}