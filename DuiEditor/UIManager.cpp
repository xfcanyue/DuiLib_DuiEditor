#include "StdAfx.h"
#include "UIManager.h"

#include "DuiEditor.h"

CUIManager::CUIManager(void)
{
	m_nSplitterMode = SPLIT_UPDOWN;

	m_pDoc			= NULL;
	m_pDesignView	= NULL;
	m_pManager		= NULL;
	m_pUiWindow		= NULL;
	m_pUiTracker	= NULL;
	m_pTreeView		= NULL;
}


CUIManager::~CUIManager(void)
{
	
}

BOOL CUIManager::IsUiWindow(xml_node node)
{
	if(CompareString(node.name(), _T("Window")))
		return TRUE;
	return FALSE;
}

xml_node CUIManager::FindContainer(xml_node node) //获取控件对应的容器
{
	xml_node nodeContainner = node.parent();
	while (nodeContainner)
	{
		if(g_duiProp.IsContainerUI(nodeContainner))
		{
			return nodeContainner;
		}
		nodeContainner = nodeContainner.parent();
	}
	
	return xml_node((xml_node_struct *)GetUiFrom()->GetTag());
}

BOOL CUIManager::SelectItem(xml_node node)
{
	CControlUI *pControlUI = (CControlUI *)node.get_tag();
//	if(!pControlUI)	return FALSE;
	return SelectItem(pControlUI);
}

BOOL CUIManager::SelectItem(CControlUI *pControl)
{
	if(!pControl)	
	{
		GetUiTracker()->RemoveAll();
		GetUiWindow()->Invalidate();
		return FALSE;
	}
	
	//往上回溯，碰到TabLayout都切换过来
	CControlUI *pControlTemp = pControl;
	CControlUI *pParent = pControl->GetParent();
	while(pParent)
	{
		CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(pParent->GetInterface(DUI_CTR_TABLAYOUT));
		if(pTabLayout)
		{
			pTabLayout->SelectItem(pControlTemp);
		}
		pControlTemp = pParent;
		pParent = pParent->GetParent();
	}

	//判断当前按钮是否绑定了TabLayout 和 BindTabIndex
	if(pControl->GetInterface(DUI_CTR_BUTTON))
	{
		CButtonUI *pButton = static_cast<CButtonUI*>(pControl->GetInterface(DUI_CTR_BUTTON));
		CString strTabLayoutName = pButton->GetBindTabLayoutName();
		if(!strTabLayoutName.IsEmpty())
		{
			pButton->BindTriggerTabSel();
		}
// 		int nBindIndex = pButton->GetBindTabLayoutIndex();
// 		if(!strTabLayoutName.IsEmpty() && nBindIndex >= 0)
// 		{
// 			CTabLayoutUI *pTabLayout = dynamic_cast<CTabLayoutUI *>(GetManager()->FindControl(strTabLayoutName));
// 			if(pTabLayout)
// 			{
// 				pTabLayout->SelectItem(nBindIndex);
// 			}
// 		}
	}

	if(::GetKeyState(VK_CONTROL)>=0) //没有按下Ctrl键
	{	
		if(!GetUiTracker()->IsSelected(pControl))//当前点击的控件是未选中的
			GetUiTracker()->RemoveAll();
		else if(GetUiTracker()->m_pFocused->m_pControl != pControl) //当前点击的控件不是焦点控件
			GetUiTracker()->RemoveAll();
	}
	
	GetUiTracker()->Add(xml_node((xml_node_struct *)pControl->GetTag()), pControl->GetPos());
	
	GetUiWindow()->Invalidate();
	return TRUE;
}

void CUIManager::EnsureVisible(xml_node node)
{
	CControlUI *pControlUI = (CControlUI *)node.get_tag();
	return EnsureVisible(pControlUI);
}

void CUIManager::EnsureVisible(CControlUI *pControl)
{
	if(!pControl) return;
	CDuiEditorViewDesign *pDesign = GetDesignView();
	if(!pDesign) return;

	CSize czSize = pDesign->GetTotalSize();
	CPoint ptScroll = pDesign->GetScrollPosition();
	CRect rcControl = pControl->GetPos();
	CRect rcClient;
	pDesign->GetClientRect(rcClient);
	rcClient += ptScroll;

//	InsertMsgV(_T("ScrollPosition.x=%d, ScrollPosition.y=%d, rcControl.left=%d, rcControl.top=%d, rcControl.right=%d, rcControl.bottom=%d, rcClient.left=%d, rcClient.top=%d, rcClient.right=%d, rcClient.bottom=%d"), 
//		ptScroll.x, ptScroll.y, rcControl.left, rcControl.top, rcControl.right, rcControl.bottom, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);

	CRect rcx;
	if(rcx.IntersectRect(rcControl, rcClient))
		return;

	if(rcControl.bottom <= rcClient.top)
	{
		int offset = rcClient.top - rcControl.bottom;
		ptScroll.y -= offset;
		ptScroll.y -= rcControl.Height();
		if(pDesign->IsViewRuleBar()) ptScroll.y -= RULEBAR_SIZE_Y;
		if(ptScroll.y < 0) ptScroll.y = 0;
	}
	else if(rcControl.top >= rcClient.bottom)
	{
		int offset = rcControl.top - rcClient.bottom;
		ptScroll.y += offset;
		ptScroll.y += rcControl.Height();
		if(pDesign->IsViewRuleBar()) ptScroll.y += RULEBAR_SIZE_Y;
		if(ptScroll.y > czSize.cy) ptScroll.y = czSize.cy;
	}

	if(rcControl.right <= rcClient.left)
	{
		int offset = rcClient.left - rcControl.right;
		ptScroll.x -= offset;
		ptScroll.x -= rcControl.Width();
		if(pDesign->IsViewRuleBar()) ptScroll.x -= RULEBAR_SIZE_X;
		if(ptScroll.x < 0) ptScroll.x = 0;
	}
	else if(rcControl.left >= rcClient.right)
	{
		int offset = rcControl.left - rcClient.right;
		ptScroll.x += offset;
		ptScroll.x += rcControl.Width();
		if(pDesign->IsViewRuleBar()) ptScroll.x += RULEBAR_SIZE_X;
		if(ptScroll.x > czSize.cy) ptScroll.x = czSize.cx;
	}

	pDesign->ScrollToPosition(ptScroll);
}

BOOL CUIManager::UpdateControlUI(CControlUI *pControl)
{
	xml_node nodeControl((xml_node_struct *)pControl->GetTag());

	xml_node nodeContainer = FindContainer(nodeControl);
	CContainerUI *pContainerUI = (CContainerUI *)nodeContainer.get_tag();
	if(pContainerUI != NULL)
	{
		pContainerUI->SetPos(pContainerUI->GetPos());
		pContainerUI->NeedUpdate();
	}
	else
	{
		pControl->SetPos(pControl->GetPos());
		pControl->NeedUpdate();
	}
	GetUiTracker()->UpdateRect(nodeControl);
	GetUiWindow()->Invalidate();

	//改动属性之后，即时刷新xml
	/*
	if(GetCodeView())
	{
		CLockWindowUpdate lock(GetCodeView());
		GetCodeView()->LoadDocument();
		GetCodeView()->SelectXmlNode(pControl);
	}
	*/
	return TRUE;
}

BOOL CUIManager::UpdateControlPos(xml_node node, CRect rect, BOOL bUpdateWithHeight)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		SetUIFormWindowSize(rect.Width(), rect.Height());

		CString temp;
		temp.Format(_T("%d,%d"), rect.Width(), rect.Height());
		g_duiProp.AddAttribute(node, _T("size"), temp, this);
		return TRUE;
	}

	//float控件
	if(pControl->IsFloat())
	{
		if(!pControl->GetParent())	return FALSE;

		CRect rcParent = pControl->GetParent()->GetPos();
		CRect rcControl;
		rcControl.left = rect.left - rcParent.left;
		rcControl.right = rcControl.left + rect.Width();
		rcControl.top = rect.top - rcParent.top;//rcParent.top - rect.top;//rect.top - rcParent.top;
		rcControl.bottom = rcControl.top + rect.Height();

		pControl->SetAttribute(_T("pos"), RectToString(rcControl)); //不能调用pControl->SetPos();
		UpdateControlUI(pControl);

		CRect rcNewPos = rcControl; //pControl->GetPos();
		g_duiProp.AddAttribute(node, _T("pos"), rcNewPos, this);
		if(bUpdateWithHeight)
		{
			g_duiProp.AddAttribute(node, _T("width"), rcNewPos.Width(), this);
			g_duiProp.AddAttribute(node, _T("height"), rcNewPos.Height(), this);
		}

		return TRUE;
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc(XML2T(node.attribute(XTEXT("pos")).as_string(XTEXT("0,0,0,0"))));
	rcDoc.left = 0;
	rcDoc.right = rect.Width();
	rcDoc.top = 0;
	rcDoc.bottom = rect.Height();

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	if(bUpdateWithHeight)
	{
		g_duiProp.AddAttribute(node, _T("pos"), rcDoc, this);
		g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(), this);
		g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(), this);
	}
	return TRUE;
}


BOOL CUIManager::UpdateControlWidth(xml_node node, int width)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		SIZE szForm = GetUiFrom()->GetInitSize();
		szForm.cx = width;
		SetUIFormWindowSize(szForm.cx, szForm.cy);

		CString temp;
		temp.Format(_T("%d,%d"), szForm.cx, szForm.cy);
		g_duiProp.AddAttribute(node, _T("size"), temp, this);
		return TRUE;
	}

	if(pControl->IsFloat())
	{
		CRect rc = pControl->GetPos();
		rc.right = rc.left + width;
		return UpdateControlPos(node, rc);
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc;
	xml_attribute attrPos = node.attribute(XTEXT("pos"));
	if(attrPos)
	{
		rcDoc = CDuiRect(XML2T(attrPos.as_string(XTEXT("0,0,0,0"))));
		rcDoc.right = width;
	}
	else
	{
		rcDoc.left = 0; rcDoc.right = width;
		rcDoc.top = 0;	rcDoc.bottom = pControl->GetFixedHeight();
	}

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	g_duiProp.AddAttribute(node, _T("pos"), rcDoc, this);
	g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(), this);
	g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(), this);
	return TRUE;
}

BOOL CUIManager::UpdateControlHeight(xml_node node, int height)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		SIZE szForm = GetUiFrom()->GetInitSize();
		szForm.cy = height;
		SetUIFormWindowSize(szForm.cx, szForm.cy);

		CString temp;
		temp.Format(_T("%d,%d"), szForm.cx, szForm.cy);
		g_duiProp.AddAttribute(node, _T("size"), temp, this);
		return TRUE;
	}

	if(pControl->IsFloat())
	{
		CRect rc = pControl->GetPos();
		rc.bottom = rc.top + height;
		return UpdateControlPos(node, rc);
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc;
	xml_attribute attrPos = node.attribute(XTEXT("pos"));
	if(attrPos)
	{
		rcDoc = CDuiRect(XML2T(attrPos.as_string(XTEXT("0,0,0,0"))));
		rcDoc.bottom = height;
	}
	else
	{
		rcDoc.left = 0; rcDoc.right = pControl->GetFixedWidth();
		rcDoc.top = 0;	rcDoc.bottom = height;
	}

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	g_duiProp.AddAttribute(node, _T("pos"), rcDoc, this);
	g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(),	this);
	g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(),	this);
	return TRUE;
}

BOOL CUIManager::UpdateControlUI(xml_node node)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(pControl)
	{
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			pControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));
		}

		return UpdateControlUI(pControl);
	}
	return FALSE;
}

BOOL CUIManager::UpdateControlUI(xml_node node, xml_attribute attr)
{
	//更新Style属性
	if(CompareString(node.name(), _T("Style")))
	{
		CString strStyleName = XML2T(node.attribute(XTEXT("name")).as_string());
		CString strStyleValue = XML2T(node.attribute(XTEXT("value")).as_string());
		bool bShared = node.attribute(XTEXT("shared")).as_bool();
		GetManager()->AddStyle(strStyleName, strStyleValue, bShared);
		return TRUE;
	}

	//更新Font属性
	if( CompareString(node.name(), _T("Font")) )
	{
		int id			= node.attribute(XTEXT("id")).as_int(-1);
		CString FontName = XML2T(node.attribute(XTEXT("name")).value());
		int size		= node.attribute(XTEXT("size")).as_int(0);
		bool bold		= node.attribute(XTEXT("bold")).as_bool();
		bool underline	= node.attribute(XTEXT("underline")).as_bool();
		bool italic		= node.attribute(XTEXT("italic")).as_bool();
		bool defaultfont = node.attribute(XTEXT("default")).as_bool();
		bool shared		= node.attribute(XTEXT("shared")).as_bool();

		if( id >= 0 ) {
			GetManager()->AddFont(id, FontName, size, bold, underline, italic, shared);
			if( defaultfont ) GetManager()->SetDefaultFont(FontName, GetManager()->GetDPIObj()->Scale(size), bold, underline, italic, shared);
		}
	}


	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(pControl)
	{
		if(pControl == GetUiFrom()) //改变窗口大小
		{
			if(CompareString(attr.name(), _T("size")))
			{
				CDuiSize sz(XML2T(attr.value()));
				SetUIFormWindowSize(sz.cx, sz.cy);
			}
			else if(CompareString(attr.name(), _T("renderengine")) ) 
			{
				if( CompareString(attr.as_string(), _T("gdi")) )
					CPaintManagerUI::SetRenderEngineType(DuiLib_Render_Default);
				else if( CompareString(attr.as_string(), _T("gdiplus")) )
					CPaintManagerUI::SetRenderEngineType(DuiLib_Render_GdiPlus);
			} 
			else if(CompareString(attr.name(), _T("forcehsl")))
			{
				GetManager()->SetForceHSL(attr.as_bool());
			}
			else if(CompareString(attr.name(), _T("hsl")))
			{
				CString strValue = XML2T(attr.value());
				LPCTSTR pstrValue = (LPCTSTR)strValue;

				int h = 180, s=100, l=100;
				LPTSTR pstr = NULL;
				h = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				s = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				l = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr); 
				GetManager()->SetHSL(h, s, l);
			}

		}
		else if(CompareString(attr.name(), _T("pos")))
		{
			CDuiRect rc(XML2T(attr.value()));
			return UpdateControlPos(node, rc, TRUE);
		}
		else if(CompareString(attr.name(), _T("width")))
		{
			return UpdateControlWidth(node, attr.as_int());
		}
		else if(CompareString(attr.name(), _T("height")))
		{
			return UpdateControlHeight(node, attr.as_int());
		}
		else
			pControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));

		return UpdateControlUI(pControl);
	}

	return FALSE;
}

BOOL CUIManager::DeleteControl(xml_node node)
{
	if(g_duiProp.IsWindowForm(node))
		return FALSE;

	CControlUI *pControl = (CControlUI *)node.get_tag();
	xml_node nodeContainer = node.parent();

	//有没有语言包要删除的，如果有，做出提示
	BOOL bDeleteLangText = FALSE;
	if(IsNeedDeleteLangText(node))
	{
		if(AfxMessageBox(_T("是否删除相关的语言包内容？\r\n此内容删除无法撤销。"), MB_OKCANCEL) == IDOK)
			bDeleteLangText = TRUE;
	}

	GetCodeView()->DeleteNode(node);
	GetTreeView()->DeleteXmlNode(node);

	if(bDeleteLangText)
	{
		DeleteLangText(node);
	}

	if(nodeContainer)
	{
		nodeContainer.remove_child(node);
	}

	if(g_duiProp.IsContainerUI(nodeContainer) || g_duiProp.IsWindowForm(nodeContainer))
	{
		CContainerUI *pContainer = (CContainerUI *)nodeContainer.get_tag();
		pContainer->Remove(pControl);
		pContainer->NeedUpdate();
	}

	
	return TRUE;
}

BOOL CUIManager::IsNeedDeleteLangText(xml_node node)
{
	if(node.attribute("resourceid").as_int() > 0)
		return TRUE;
	
	for (xml_node node1=node.first_child(); node1; node1=node1.next_sibling())
	{
		if(IsNeedDeleteLangText(node1))
			return TRUE;
	}
	return FALSE;
}

void CUIManager::DeleteLangText(xml_node node)
{
	for (xml_node node1=node.first_child(); node1; node1=node1.next_sibling())
	{
		DeleteLangText(node1);
	}

	int resID = node.attribute("resourceid").as_int();
	if(resID > 0)
	{
		for (int i=0; i<GetDocument()->m_mLangPackage.GetSize(); i++)
		{
			LPCTSTR key = GetDocument()->m_mLangPackage.GetAt(i);
			xml_document *xml = (xml_document *)GetDocument()->m_mLangPackage.Find(key);
			if(!xml) continue;
			for (xml_node xmlText=xml->child("Language").first_child(); xmlText; xmlText=xmlText.next_sibling())
			{
				if(xmlText.attribute("id").as_int() == resID)
				{
					xml->child("Language").remove_child(xmlText);
					break;
				}
			}
		}
	}

}

void CUIManager::SetScrollSize()
{
	CSize szForm = GetManager()->GetInitSize();

	if(GetDesignView()->m_bViewRuleBar)
	{
		szForm.cx += RULEBAR_SIZE_X;
		szForm.cy += RULEBAR_SIZE_Y;
	}
	if(szForm.cx < 0) szForm.cx = 0;
	if(szForm.cy < 0) szForm.cy = 0;
	m_pDesignView->SetScrollSizes(MM_TEXT, szForm);
	m_pDesignView->GetParent()->SendMessage(WM_SIZE);
}

void CUIManager::SetZoom(int zoom)
{
	GetManager()->SetDPI(zoom);
	GetManager()->ResetDPIAssets();

	CRect rc;
	GetUiWindow()->GetWindowRect(rc);
	CPoint point = GetDesignView()->GetScrollPosition();

	if(GetDesignView()->m_bViewRuleBar)
		GetUiWindow()->MoveWindow(RULEBAR_SIZE_X-point.x, RULEBAR_SIZE_Y-point.y, rc.Width(), rc.Height(), TRUE);
	else
		GetUiWindow()->MoveWindow(-point.x, -point.y, rc.Width(), rc.Height(), TRUE);

	SetUIFormWindowSize(rc.Width(), rc.Height());

	SetScrollSize();
}

void CUIManager::SetSplitterMode(int nMode)
{
	m_nSplitterMode = nMode;
}

int  CUIManager::GetSplitterMode() const
{
	return m_nSplitterMode;
}

void CUIManager::SetUIFormWindowSize(int cx, int cy)
{
	CDuiSize szForm(cx, cy);
	if(szForm.cx <= 0 || szForm.cy <= 0)
	{
		CRect rcClient;
		GetDesignView()->GetClientRect(rcClient);
		if(szForm.cx <= 0) 
		{
			szForm.cx = rcClient.Width();
			if(GetDesignView()->IsViewRuleBar()) szForm.cx -= RULEBAR_SIZE_X;
		}
		if(szForm.cy <= 0) 
		{
			szForm.cy = rcClient.Height();
			if(GetDesignView()->IsViewRuleBar()) szForm.cy -= RULEBAR_SIZE_Y;
		}
	}
	GetUiFrom()->SetInitSize(szForm.cx, szForm.cy);
	SetScrollSize();
}