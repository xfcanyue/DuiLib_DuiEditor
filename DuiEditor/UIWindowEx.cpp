// UIWindowEx.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "UIWindowEx.h"

#include "MainFrm.h"
#include "DuiEditorViewDesign.h"
#include "DlgInsertControl.h"
#include "DlgCustomControl.h"

#include "wm.c"
// CUIWindowEx

CUIWindowEx::CUIWindowEx()
{
	m_bOpenConfigFile = false;
}

CUIWindowEx::~CUIWindowEx()
{
}

// CUIWindowEx 消息处理程序
void CUIWindowEx::AddNewControlFromToolBox(xml_node nodeToolBox, CPoint point)
{
	//当前节点，从控件树获取
	xml_node nodeCurrent = m_pManager->GetTreeView()->GetSelectXmlNode();					//树选中的控件
	CControlUI   *pCurControl = (CControlUI *)nodeCurrent.get_tag();
	if(!pCurControl)
	{
		pCurControl = m_pManager->GetUiFrom();
	}

	CRect rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	if(!rcClient.PtInRect(point))
	{
		return;
	}

	//拖动鼠标 设置控件的大小
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

	CString strNewControlClass = nodeToolBox.name(); //准备插入控件的类名

	//插入位置, 知道了插入位置，才能确定container
	CDlgInsertControl dlg;
	dlg.nodeControl = nodeCurrent;
	if(dlg.DoModal() != IDOK)	return;

	xml_node nodeContainer;
	CContainerUI *pParentContainer = NULL;
	switch(dlg.m_nPosition)
	{
	case INSERT_CHILD: //当前容器下方插入控件
		{
			pParentContainer = (CContainerUI *)pCurControl->GetInterface(DUI_CTR_CONTAINER);
			if(!pParentContainer)
			{
				AfxMessageBox(_T("控件需要放置于容器内."));
				return;
			}

			//选中控件树Window节点添加子控件
			if(m_pManager->GetUiFrom() == pParentContainer) 
			{ 
				if(g_duiProp.IsBaseFromContainer(strNewControlClass)) //必须放置于容器内
				{
					if(pParentContainer->GetCount() > 0)
					{
						//不允许在Window下面插入第2个容器, 强制放到第一个容器里
						pParentContainer = (CContainerUI *)m_pManager->GetUiFrom()->GetItemAt(0);
						nodeCurrent = xml_node((xml_node_struct *)pParentContainer->GetTag());
						pCurControl = pParentContainer;
					}
				}				
				else
				{
					AfxMessageBox(_T("控件需要放置于容器内."));  return; 
				}
			}

			nodeContainer = xml_node((xml_node_struct *)pParentContainer->GetTag());
		}
		break;
	case INSERT_NEXT:	//当前控件下方插入兄弟控件
		{
			//企图给Form插入兄弟控件
			if(m_pManager->GetUiFrom() == pCurControl)
			{
				AfxMessageBox(_T("控件需要放置于容器内."));  return;
			}
			pParentContainer = (CContainerUI *)pCurControl->GetParent()->GetInterface(DUI_CTR_CONTAINER);
			if(!pParentContainer)
			{
				AfxMessageBox(_T("无法插入兄弟控件, 当前控件的父控件不是容器."));
				return;
			}
			nodeContainer = xml_node((xml_node_struct *)pParentContainer->GetTag());
		}
		break;
	case INSERT_BEFORE:	//当前控件上方插入兄弟控件
		{
			//企图给Form插入兄弟控件
			if(m_pManager->GetUiFrom() == pCurControl)
			{
				AfxMessageBox(_T("控件需要放置于容器内."));  return;
			}
			pParentContainer = (CContainerUI *)pCurControl->GetParent()->GetInterface(DUI_CTR_CONTAINER);
			if(!pParentContainer)
			{
				AfxMessageBox(_T("无法插入兄弟控件, 当前控件的父控件不是容器."));
				return;
			}
			nodeContainer = xml_node((xml_node_struct *)pParentContainer->GetTag());
		}
		break;	
	}

	CControlUI *pNewControl = pNewControl = CUIBuilder::CreateControl(strNewControlClass);
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

		strNewControlClass = dlg.m_strClassName;
		strCuscomControlParent = dlg.m_strParentControl;
		bCustomControl = TRUE;
	}


	xml_node nodeNewControl;
	switch (dlg.m_nPosition)
	{
	case INSERT_CHILD: //当前容器下方插入控件
		{
			if(!pParentContainer->Add(pNewControl)) { delete pNewControl; return; }		
			nodeNewControl = nodeContainer.append_child(strNewControlClass);	//写入文档
		}
		break;
	case INSERT_NEXT:	//当前控件下方插入兄弟控件
		{
			if(! pParentContainer->AddAt(pNewControl, pParentContainer->GetItemIndex(pCurControl) + 1) ) 
			{ 
				delete pNewControl; return; 
			}
			nodeNewControl = nodeContainer.append_child(strNewControlClass);	//写入文档
		}
		break;
	case INSERT_BEFORE:	//当前控件上方插入兄弟控件
		{
			if(! pParentContainer->AddAt(pNewControl, pParentContainer->GetItemIndex(pCurControl)) ) 
			{ 
				delete pNewControl; return; 
			}
			nodeNewControl = nodeContainer.insert_child_before(strNewControlClass, nodeCurrent);	//写入文档
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
		CRect rcParent = pParentContainer->GetPos();
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

	g_duiProp.FilterDefaultValue(nodeNewControl);

	//载入控件默认属性
	LPCTSTR pDefaultAttributes = GetManager()->GetDefaultAttributeList(strNewControlClass);
	if( pDefaultAttributes ) 
	{
		pNewControl->ApplyAttributeList(pDefaultAttributes);
	}

	//载入控件当前属性

	//插入控件树
	if(dlg.m_nPosition == INSERT_BEFORE)
		m_pManager->GetTreeView()->AddNewControl(nodeNewControl, nodeCurrent, TVI_BEFORE);
	else if(dlg.m_nPosition == INSERT_NEXT)
		m_pManager->GetTreeView()->AddNewControl(nodeNewControl, nodeCurrent, TVI_NEXT);
	else
		m_pManager->GetTreeView()->AddNewControl(nodeNewControl, nodeCurrent, TVI_CHILD);

	//刷新属性窗口
	g_pPropWnd->InitProp(nodeNewControl);

	//刷新控件
	m_pManager->UpdateControlUI(nodeNewControl);

	//只选中新添加的控件
	m_tracker.RemoveAll();
	m_tracker.Add(nodeNewControl, pNewControl->GetPos());

	//插入command history
	m_pManager->GetCmdHistory()->AddNewControl(nodeNewControl);

	//保存文档修改标志
	m_pManager->GetDocument()->SetModifiedFlag(TRUE);
}

void CUIWindowEx::OnSelectingControl(CControlUI *pControl, const CRect &rcTracker)
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

void CUIWindowEx::ResizeWindow()
{
	if(!m_pManager->GetDocument()->m_bMenuWnd)	return;

	CUIFormView *pView = (CUIFormView *)m_pm.GetRoot();
	CControlUI* pRoot = pView->GetItemAt(0);

#if defined(WIN32) && !defined(UNDER_CE)
	MONITORINFO oMonitor = {}; 
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
#else
	CDuiRect rcWork;
	GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
	SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);
	m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

	SIZE szInit = m_pm.GetInitSize();
	CDuiRect rc;
	CDuiPoint point;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	int nWidth = rc.GetWidth();
	int nHeight = rc.GetHeight();

// 	if (m_dwAlignment & eMenuAlignment_Right)
// 	{
// 		rc.right = point.x;
// 		rc.left = rc.right - nWidth;
// 	}
// 
// 	if (m_dwAlignment & eMenuAlignment_Bottom)
// 	{
// 		rc.bottom = point.y;
// 		rc.top = rc.bottom - nHeight;
// 	}

	MoveWindow(rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
}

LRESULT CUIWindowEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
	case WM_SIZE:			
		lRes = OnSize(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_CHAR:		lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEHOVER:		lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	case WM_PAINT:			lRes = OnPaint(uMsg, wParam, lParam, bHandled);	break;
	case WM_RBUTTONDOWN:	lRes = OnRButtonDown(uMsg, wParam, lParam, bHandled);	break;
	case WM_SETCURSOR:		lRes = OnSetCursor(uMsg, wParam, lParam, bHandled);	break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	if(m_pManager->GetView()->m_bShowUiPreview) //这个非常容易崩溃的
	{
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
			return lRes;
	}
	else
	{
		if(uMsg == WM_PAINT)
		{
			if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
				return lRes;
		}
	}

// 	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
// 		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIWindowEx::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_tracker.m_nHandleSize = 6;
	m_tracker.m_nStyle = CUITracker::dottedLine|CUITracker::resizeInside;
	m_tracker.m_pManager = m_pManager;

	//////////////////////////////////////////////////////////////////////////
	// 调整窗口样式
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	CDuiEditorDoc *pDoc = m_pManager->GetDocument();
	GetManager()->Init(m_hWnd);
	GetManager()->AddPreMessageFilter(this);
	GetManager()->SetResourcePath(pDoc->GetSkinPath());

	CUIBuilder builder;
	CString skinFile = pDoc->GetSkinFileName();
	if(skinFile.CompareNoCase(_T("config.xml")) == 0)
	{
		m_bOpenConfigFile = true;
	}

	CControlUI* pRoot=NULL;
	pRoot = builder.Create(pDoc->m_doc, this, GetManager());
	if (pRoot==NULL && !m_bOpenConfigFile)
	{
		//MessageBox(m_hWnd, _T("Loading resource files error."), _T("Duilib"), MB_OK|MB_ICONERROR);
		//return 0;
	}

	//先在根部放一个containner, 设计器内部把它当成窗口对象。
	CUIFormView *p = new CUIFormView;
	xml_node nodeWindow = pDoc->m_doc.child(_T("Window"));
	p->SetTag((UINT_PTR)nodeWindow.internal_object());
	nodeWindow.set_tag((UINT_PTR)p);
	p->Add(pRoot);

	GetManager()->AttachDialog(p);
	GetManager()->AddNotifier(this);

	ResizeWindow();

	GetManager()->GetDPIObj()->SetDPIAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	GetManager()->SetDPI(CDPI::GetMainMonitorDPI());
	return 0;
}

LRESULT CUIWindowEx::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIWindowEx::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;

	::DestroyCaret();

	//InsertMsg(_T("CUIWindowEx::OnKillFocus"));
	return 0;
}

LRESULT CUIWindowEx::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;

	//创建一个隐藏光标, 此光标用于在控件上直接输入文字
	::CreateCaret(m_hWnd,(HBITMAP) NULL, 0, 0);
	//InsertMsg(_T("CUIWindowEx::OnSetFocus"));
	return 0;
}

LRESULT CUIWindowEx::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	if( (::GetKeyState(VK_CONTROL) < 0) || (::GetKeyState(VK_MENU) < 0) )
	{
		return 0;
	}

	if(!m_tracker.m_pFocused)
		return 0;

	UINT nChar = (UINT)wParam;

	xml_node node = m_tracker.m_pFocused->m_node;
	if(!g_duiProp.IsNeedInputText(node))
		return 0;


	CString strText = node.attribute(_T("text")).value();

	if(nChar == 0x08) //Backspace, 退格键
	{
		strText.Delete(strText.GetLength()-1);
	}
	else
	{	
		if(!_istprint(nChar))
			return 0;

		CString temp;
		temp.Format(_T("%c"), nChar);
		strText += temp;
	}
	g_duiProp.AddAttribute(node, _T("text"), strText,	m_pManager->GetView());
	CControlUI *pFocusControl = (CControlUI *)node.get_tag();
	pFocusControl->SetText(strText);
	pFocusControl->NeedParentUpdate();

	return 0;
}

LRESULT CUIWindowEx::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUIWindowEx::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetFocus(GetSafeHwnd());

	//切换左边控件树
//	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
//	pMain->m_wndControl.SetActiveTreeView(m_pManager->GetTreeView());

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
	while (TRUE)
	{
		if(!pControl)	break;

		nodeClick = xml_node((xml_node_struct *)pControl->GetTag());
		if(nodeClick)	break;
		pControl = pControl->GetParent();
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
		m_pManager->GetXmlPane()->SelectXmlNode(nodeClick);
	}
	else
		m_tracker.SetFocus(nodeClick);

	//设置键盘输入光标位置
 	CControlUI *pFocusControl = (CControlUI *)m_tracker.m_pFocused->m_node.get_tag();
 	if(pFocusControl)
 	{
 		CRect rcFocusControl = pFocusControl->GetPos();
 		BOOL b = ::SetCaretPos(rcFocusControl.left, rcFocusControl.bottom);
		if(!b) InsertMsg(_T("::SetCaretPos failed."));
 	}

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

LRESULT CUIWindowEx::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUIWindowEx::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pManager && m_pManager->GetView()->m_bShowUiPreview)
	{
		bHandled = FALSE;
		return 0;
	}

	if(m_pManager && m_pManager->GetView()->m_bViewMouse)
	{
		Invalidate();
	}

	return 0;
}

LRESULT CUIWindowEx::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUIWindowEx::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnKeyDown(uMsg, wParam, lParam, bHandled);

	switch(wParam)
	{
	case VK_RETURN:
		break;
	case VK_DELETE:
		break;
	case VK_UP:
		m_tracker.OnkeyUp();
		Invalidate();
		break;
	case VK_DOWN:
		m_tracker.OnkeyDown();
		break;
	case VK_LEFT:
		m_tracker.OnkeyLeft();
		break;
	case VK_RIGHT:
		m_tracker.OnkeyRight();
		break;
	}

	return 0;
}