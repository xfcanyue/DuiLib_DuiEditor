#include "StdAfx.h"
#include "UITrackerMuliti.h"
#include "DuiEditor.h"

#include "MainFrm.h"
#include "UIManager.h"

CUITrackerMuliti::CUITrackerMuliti(void):m_pFocused(NULL)
{
	HINSTANCE hInst = AfxFindResourceHandle(
		ATL_MAKEINTRESOURCE(AFX_IDC_TRACK4WAY), ATL_RT_GROUP_CURSOR);
	m_hNoDropCursor=::LoadCursor(hInst, ATL_MAKEINTRESOURCE(AFX_IDC_NODROPCRSR));

	m_nHitTest = -1;
}


CUITrackerMuliti::~CUITrackerMuliti(void)
{
	m_nHitTest = -1;
	RemoveAll();
}

void CUITrackerMuliti::Draw(CDC* pDC, CSize *szOffset)
{
	int size = m_arrTracker.GetSize();
	for (int i=0; i<size; i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		if(szOffset)
		{
			m_rect.OffsetRect(*szOffset);
		}

		if(pArrTracker==m_pFocused)
			m_bFocus = TRUE;
		else
			m_bFocus = FALSE;

		if(GetUIManager()->GetDesignView()->m_bShowUiPreview)
		{
			m_bDrawHandle = FALSE;
			m_bDrawMoveHandle = FALSE;
		}

		m_bMuiltiSelected = size > 1;

		m_node = pArrTracker->m_node;
		CUITracker::Draw(pDC);
	}
}

int CUITrackerMuliti::HitTest(CPoint point)
{
	int nHit=hitNothing;
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		nHit=CUITracker::HitTest(point);
		if (nHit!=hitNothing)
		{
			return nHit;
		}
	}

	return hitNothing;
}

int CUITrackerMuliti::HitTestHandles(CPoint point)
{
	int nHit=hitNothing;
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		m_rect=pArrTracker->GetPos();
		nHit=CUITracker::HitTestHandles(point);
		if (nHit!=hitNothing)
		{
			return nHit;
		}
	}
	return hitNothing;
}

BOOL CUITrackerMuliti::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert/* = FALSE*/, CDC* pDCClipTo/* = NULL*/)
{
	// perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;

	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= 8);   // handle 8 is inside the rect

	if(nHandle != hitMiddle && GetSize()==1)
	{
		return TrackHandle(nHandle, pWnd, pDCClipTo);
	}

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();

	// convert cursor position to client co-ordinates
	CPoint oldPoint;
	GetCursorPos(&oldPoint);
	pWnd->ScreenToClient(&oldPoint);

	CopyTrackerRect();

	// get DC for drawing
	CDC* pDrawDC;
	if (pDCClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pDCClipTo;
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ENSURE_VALID(pDrawDC);

	CRect rectOld;
//	CPoint point;
	HCURSOR hOldCursor=::GetCursor();
	BOOL bBeyond=FALSE;
	BOOL bMoved = FALSE;

	CRect rcWindow;
	pWnd->GetClientRect(rcWindow);
	CSize szForm(rcWindow.Width(), rcWindow.Height());

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
			// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);

			point=msg.lParam;

			pDrawDC->DPtoLP(&point);
			if(point.x<0||point.x>szForm.cx||
				point.y<0||point.y>szForm.cy)
			{
				bBeyond=TRUE;
				::SetCursor(m_hNoDropCursor);
			}
			else
			{
				bBeyond=FALSE;
				::SetCursor(hOldCursor);
			}

			//如果焦点控件不是float，不允许拖动。
			if(!CanbeMoveRect())
				::SetCursor(m_hNoDropCursor);

			point=msg.lParam;


			for (int i=0;i<m_arrCloneRect.GetSize();i++)
			{
				m_rect=m_arrCloneRect.GetAt(i);
				rectOld = m_rect;

				m_rect.left += point.x - oldPoint.x;
				m_rect.top += point.y - oldPoint.y;
				m_rect.right = m_rect.left + rectOld.Width();
				m_rect.bottom = m_rect.top + rectOld.Height();

				if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
				{
					if (bMoved)
					{
						m_bErase = TRUE;
						DrawTrackerRect(&rectOld, pDrawDC);
					}
					if (!m_bFinalErase)
						bMoved = TRUE;
				}
				if (!rectOld.EqualRect(&m_rect) && !m_bFinalErase)
				{
					m_bErase = FALSE;
					DrawTrackerRect(&m_rect, pDrawDC);
				}
				m_arrCloneRect.SetAt(i,m_rect);
			}

			oldPoint = msg.lParam;
			if (m_bFinalErase)
				goto ExitLoop;

			break;

			// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;

			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pDCClipTo == NULL)
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (bMoved)
	{
		OnChangedRect();
	}
	ClearCopy();

	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return bMoved;
}

BOOL CUITrackerMuliti::SetCursor(CWnd *pWnd, UINT nHitTest, const CPoint &ptDPtoLP, const CSize &szOffset)
{
	//这里只判断焦点控件的鼠标
	if(m_pFocused == NULL)
		return FALSE;

	CRect rectSave = m_rect;
	m_rect = m_pFocused->GetPos();
	if (CUITracker::SetCursor(pWnd, nHitTest, ptDPtoLP, szOffset))
	{
		m_rect = rectSave;
		return TRUE;
	}

	return FALSE;
}

BOOL CUITrackerMuliti::Add(xml_node node, CRect rect)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if (pArrTracker->m_node == node)
		{
			m_pFocused=pArrTracker;
			return FALSE;
		}
	}

	CTrackerElement *pTracker = new CTrackerElement;
	pTracker->m_node = node;
	pTracker->m_pControl = (CControlUI *)node.get_tag();
	pTracker->m_rect = rect;

	m_pFocused=pTracker;
	m_arrTracker.Add(pTracker);

	return TRUE;
}

BOOL CUITrackerMuliti::Add(CTrackerElement * pTracker)
{
	ASSERT(pTracker);

	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if (pArrTracker->m_node == pTracker->m_node)
		{
			m_pFocused=pArrTracker;
			return FALSE;
		}
	}
	m_pFocused=pTracker;
	m_arrTracker.Add(pTracker);

	return TRUE;
}

BOOL CUITrackerMuliti::Remove(xml_node node)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pTracker = m_arrTracker.GetAt(i);

		if (pTracker->m_node == node)
		{
			m_arrTracker.RemoveAt(i);
			delete pTracker;
			return TRUE;
		}
	}

	return FALSE;
}

void CUITrackerMuliti::RemoveAll()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pTracker = m_arrTracker.GetAt(i);
		delete pTracker;
	}
	m_arrTracker.RemoveAll();
}

BOOL CUITrackerMuliti::IsSelected(xml_node node)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if (pArrTracker->m_node == node)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUITrackerMuliti::IsSelected(CControlUI *pControl)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		if (pArrTracker->m_pControl == pControl)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CUITrackerMuliti::CopyTrackerRect()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		m_arrCloneRect.Add(m_arrTracker.GetAt(i)->GetPos());
	}
}

void CUITrackerMuliti::ClearCopy()
{
	m_arrCloneRect.RemoveAll();
}

void CUITrackerMuliti::UpdateRect(xml_node node)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		CContainerUI *pControl = (CContainerUI *)pArrTracker->m_node.get_tag();

		pArrTracker->m_rect = pControl->GetPos();
		break;
	}

}

void CUITrackerMuliti::SetFocus(xml_node node)
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);

		CContainerUI *pControl = (CContainerUI *)pArrTracker->m_node.get_tag();

		if(pArrTracker->m_node == node)
		{
			m_pFocused = pArrTracker;
			break;
		}
	}
}

void CUITrackerMuliti::OnChangedRect()
{
	if(!CanbeMoveRect()) return;

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());

	BOOL bCopyControls = FALSE;
	CArray<CControlUI *, CControlUI *> arrNewControl;

	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		CRect rc = m_arrCloneRect[i];
		rc.NormalizeRect();

		//没有按住Ctrl键, 移动控件
		if(::GetKeyState(VK_CONTROL) >= 0)
		{
			xml_node nodeCurrentControl = pArrTracker->m_node;
			CControlUI *pCurrentControl = (CControlUI *)nodeCurrentControl.get_tag();
			if(pCurrentControl->IsFloat())
			{
				pArrTracker->m_rect = rc;
				GetUIManager()->UpdateControlPos(pArrTracker->m_node, rc);
			}
			else
			{
				InsertMsg(_T("非float控件不能通过拖动鼠标改变其位置"));
			}
			continue;
		}

		//按住Ctrl键, 开始复制控件
		bCopyControls = TRUE;

		xml_node nodeCurrentControl = pArrTracker->m_node;
		CControlUI *pCurrentControl = (CControlUI *)nodeCurrentControl.get_tag();
		CContainerUI *pParent = (CContainerUI *)pCurrentControl->GetParent();
		xml_node nodeParent =  nodeCurrentControl.parent();
		CControlUI *pNewControl = NULL;

		//创建控件
		CString pstrClass = XML2T(nodeCurrentControl.name());
		pNewControl = CUIBuilder::CreateControl(pstrClass);
		if(!pNewControl) continue;

		//当前容器下方插入控件
		if(!pParent->Add(pNewControl)) { CUIBuilder::DeleteControl(pNewControl); continue; }	

		//写入文档
		xml_node nodeNewControl = nodeParent.append_child(nodeCurrentControl.name());

		//保存文档和控件的双向指针
		pNewControl->SetTag((UINT_PTR)nodeNewControl.internal_object());
		nodeNewControl.set_tag((UINT_PTR)pNewControl);

		GetUIManager()->GetCodeView()->AddNode(nodeNewControl);

		//复制属性
		for (xml_attribute attr=nodeCurrentControl.first_attribute(); attr; attr=attr.next_attribute())
		{
			if(CompareString(attr.name(), XTEXT("name"))) //不能复制控件的name
				continue;

			g_duiProp.AddAttribute(nodeNewControl, XML2T(attr.name()), XML2T(attr.value()), GetUIManager());
		}

		if(pCurrentControl->IsFloat()) //绝对定位
		{
			CRect rcParent = pParent->GetPos();
			CRect rcNew;
			rcNew.left = rc.left - rcParent.left;
			rcNew.right = rcNew.left + rc.right - rc.left;
			rcNew.top = rc.top - rcParent.top;
			rcNew.bottom = rcNew.top + rc.bottom - rc.top;

			pNewControl->SetAttribute(_T("pos"), RectToString(&rcNew));

			g_duiProp.AddAttribute(nodeNewControl, _T("float"), _T("true"),					GetUIManager());
			g_duiProp.AddAttribute(nodeNewControl, _T("pos"), RectToString(&rcNew),			GetUIManager());
			g_duiProp.AddAttribute(nodeNewControl, _T("width"), rcNew.right-rcNew.left,		GetUIManager());
			g_duiProp.AddAttribute(nodeNewControl, _T("height"), rcNew.bottom-rcNew.top,	GetUIManager());
		}
		else
		{

		}

		//载入控件默认属性
		LPCTSTR pDefaultAttributes = GetUIManager()->GetManager()->GetDefaultAttributeList(pstrClass);
		if( pDefaultAttributes ) 
		{
			pNewControl->ApplyAttributeList(pDefaultAttributes);
		}

		//载入控件当前属性
		for (xml_attribute attr = nodeNewControl.first_attribute(); attr; attr=attr.next_attribute())
		{
			pNewControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));
		}

		//插入左边控件树
		HTREEITEM hTreeSibling = GetUIManager()->GetTreeView()->FindXmlNode(nodeCurrentControl); 
		if(!hTreeSibling)	hTreeSibling = TVI_FIRST;
		HTREEITEM hNewControl = GetUIManager()->GetTreeView()->AddNewControl(nodeNewControl, hTreeSibling);
	
		//刷新属性窗口
		GetUIManager()->GetPropList()->InitProp(nodeNewControl);

		//刷新控件
		GetUIManager()->UpdateControlUI(nodeNewControl);

		//插入command history
		//GetUIManager()->GetCmdHistory()->AddNewControl(nodeNewControl);

		//保存文档修改标志
		//GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);

		//缓存当前复制的控件
		arrNewControl.Add(pNewControl);
	}

	//只选中新添加的控件
	if(bCopyControls)
	{
		RemoveAll();
		for (int i=0; i<arrNewControl.GetSize(); i++)
		{
			xml_node node((xml_node_struct *)arrNewControl[i]->GetTag());
			Add(node, arrNewControl[i]->GetPos());
		}
	}
}

void CUITrackerMuliti::OnChangedRect(const CRect& rectOld)
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
//	InsertMsg(_T("OnChangedRect(const CRect& rectOld)"));

	switch (m_nHitTest)
	{
	case CRectTracker::hitTopLeft:
		//break;
	case CRectTracker::hitTopRight:
		//break;
	case CRectTracker::hitBottomRight:
		//break; 
	case CRectTracker::hitBottomLeft:
		//break;
	case CRectTracker::hitMiddle: 
		{
			//change width and height
			for (int i=0;i<m_arrTracker.GetSize();i++)
			{
				CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
				pArrTracker->m_rect = m_rect;

				CRect rc = m_rect;
				rc.NormalizeRect();
				GetUIManager()->UpdateControlPos(pArrTracker->m_node, rc);
				break;
			}
		}
		break;
	case CRectTracker::hitTop:
		//break;
	case CRectTracker::hitBottom:
		{
			//change height
			for (int i=0;i<m_arrTracker.GetSize();i++)
			{
				CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
				pArrTracker->m_rect = m_rect;

				CRect rc = m_rect;
				rc.NormalizeRect();
				GetUIManager()->UpdateControlHeight(pArrTracker->m_node, rc.Height());
				break;
			}
		}
		break;
	case CRectTracker::hitRight:
		//break; 
	case CRectTracker::hitLeft:
		{
			//change width
			for (int i=0;i<m_arrTracker.GetSize();i++)
			{
				CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
				pArrTracker->m_rect = m_rect;

				CRect rc = m_rect;
				rc.NormalizeRect();
				GetUIManager()->UpdateControlWidth(pArrTracker->m_node, rc.Width());
				break;
			}
		}
		break; 
	}
}



void CUITrackerMuliti::OnkeyUp()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		pArrTracker->m_rect.OffsetRect(0, -1);
		GetUIManager()->UpdateControlPos(pArrTracker->m_node, pArrTracker->m_rect);
	}
}

void CUITrackerMuliti::OnkeyDown()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		pArrTracker->m_rect.OffsetRect(0, 1);
		GetUIManager()->UpdateControlPos(pArrTracker->m_node, pArrTracker->m_rect);
	}
}

void CUITrackerMuliti::OnkeyLeft()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		pArrTracker->m_rect.OffsetRect(-1, 0);
		GetUIManager()->UpdateControlPos(pArrTracker->m_node, pArrTracker->m_rect);
	}
}

void CUITrackerMuliti::OnkeyRight()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		pArrTracker->m_rect.OffsetRect(1, 0);
		GetUIManager()->UpdateControlPos(pArrTracker->m_node, pArrTracker->m_rect);
	}
}

void CUITrackerMuliti::RefreshRect()
{
	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		pArrTracker->m_rect = pArrTracker->m_pControl->GetPos();
	}
	if(GetUIManager())
	{
		GetUIManager()->GetUiWindow()->Invalidate();
	}
}

BOOL CUITrackerMuliti::CanbeMoveRect()
{
	//如果焦点控件不是float，不允许拖动。
	if(m_pFocused == NULL || m_pFocused->m_pControl == NULL || !m_pFocused->m_pControl->IsFloat())
		return FALSE;

	for (int i=0;i<m_arrTracker.GetSize();i++)
	{
		CTrackerElement* pArrTracker = m_arrTracker.GetAt(i);
		if(!pArrTracker->m_pControl->IsFloat())
			return FALSE;
	}
	return TRUE;
}