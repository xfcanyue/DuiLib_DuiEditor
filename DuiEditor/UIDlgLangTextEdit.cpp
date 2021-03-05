#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIDlgLangTextEdit.h"

#include "UIBuilder.h"
#include "UIManager.h"

CDlgLangTextEditUI::CDlgLangTextEditUI(void)
{
	m_currentid = 0;
}

CDlgLangTextEditUI::~CDlgLangTextEditUI(void)
{
}

LRESULT CDlgLangTextEditUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 调整窗口样式
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	// 关联UI管理器
	m_pm.Init(m_hWnd, GetManagerName());
	// 注册PreMessage回调
	m_pm.AddPreMessageFilter(this);

	//从资源中读取xml布局文件
	xml_document xml;
	xml_parse_result ret;
	HRSRC hResource = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_XML_DLG_LANG_TEXT_EDIT), _T("xml"));
	if( hResource != NULL ) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(AfxGetResourceHandle(), hResource);
		if( hGlobal != NULL ) {
			dwSize = ::SizeofResource(AfxGetResourceHandle(), hResource);
			if( dwSize > 0 ) 
			{
				CStringA strXml = CStringA((const char *)::LockResource(hGlobal), dwSize);
				ret = xml.load_string(strXml);
			}
		}
		::FreeResource(hResource);
	}

	// 创建主窗口
	CUIBuilder builder;
	CControlUI* pRoot = builder.Create(xml, NULL, &m_pm);
	if (pRoot==NULL)
	{
		MessageBox(m_hWnd, _T("Loading resource files error."), _T("Duilib"), MB_OK|MB_ICONERROR);
		return 0;
	}

	m_pm.AttachDialog(pRoot);
	// 添加Notify事件接口
	m_pm.AddNotifier(this);

	// 窗口初始化完毕
	__InitWindow();
	return 0;
}

void CDlgLangTextEditUI::InitWindow()
{
	UI_BINDCONTROL(CGridListUI, m_pGrid, _T("grid_lang_text"));
	m_pGrid->SetColumnWidth(1,100);

	CStdStringPtrMap *map = &GetUIManager()->GetDocument()->m_mLangPackage;
	CVerticalLayoutUI *pVerti = (CVerticalLayoutUI *)FindControl(_T("verti_lang_list"));
	for (int i=0; i<map->GetSize(); i++)
	{
		LPCTSTR key = map->GetAt(i);
		COptionUI *pOpt = new COptionUI;
		pVerti->Add(pOpt);
		pOpt->SetText(key);
		pOpt->SetName(key);
		pOpt->ApplyAttributeList(_T("opt_lang"));
	}

	COptionUI *pOpt = (COptionUI *)pVerti->GetItemAt(0);
	if(pOpt)
	{
		pOpt->Selected(true);
	}
}

void CDlgLangTextEditUI::OnNotifySelectChanged(TNotifyUI& msg)
{
	if(msg.pSender->GetParent() && msg.pSender->GetParent()->GetName() == _T("verti_lang_list"))
	{
		LoadText(msg.pSender->GetText());
	}
}

void CDlgLangTextEditUI::OnNotifyEndEdit(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_lang_text")))
	{
		if(m_currentid == 0) return;

		int row = (int)msg.wParam;
		int col = (int)msg.lParam;
		CString attrName = m_pGrid->Cell(row,1).GetText();
		CString attrValue = m_pGrid->Cell(row,col).GetText();

		xml_document *xml = (xml_document *)GetUIManager()->GetDocument()->m_mLangPackage.Find(m_strCurLangType);
		if(!xml) return;
		xml_node root = xml->child("Language");
		if(!root) return;

		xml_node findNode;
		for (xml_node node=root.first_child(); node; node=node.next_sibling())
		{
			if(node.attribute("id").as_int() != m_currentid)
				continue;

			findNode = node;
			break;
		}

		//如果语言包中不存在，创建新的Node
		if(!findNode)
		{
			findNode = root.append_child("Text");
			findNode.attribute_auto("id").set_value(m_currentid);
		}

		findNode.attribute_auto(T2XML(attrName)).set_value(T2XML(attrValue));
		GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
	}
}

void CDlgLangTextEditUI::LoadText(LPCTSTR langtype)
{
	m_strCurLangType = langtype;
	m_pGrid->SetRowCount(1);

	m_currentid = 0;
	xml_document *xml = (xml_document *)GetUIManager()->GetDocument()->m_mLangPackage.Find(langtype);
	if(!xml) return;
	xml_node root = xml->child("Language");
	if(!root) return;

	CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_pFocused;
	if(!pTrackElem) return;	 
	CControlUI *pFocus = pTrackElem->m_pControl;
	if(!pFocus) return;

	CString text = XML2T(pTrackElem->m_node.attribute(XTEXT("text")).as_string());
	CString tooltip = XML2T(pTrackElem->m_node.attribute(XTEXT("tooltip")).as_string());
	CString tipvalue = XML2T(pTrackElem->m_node.attribute(XTEXT("tipvalue")).as_string());

	BOOL bFind = FALSE;
	m_currentid = pFocus->GetResourceID();
	for (xml_node node=root.first_child(); node; node=node.next_sibling())
	{
		if(node.attribute("id").as_int() != m_currentid)
			continue;

		xml_attribute attrText		= node.attribute("text");
		xml_attribute attrToolTip	= node.attribute("tooltip");
		xml_attribute attrTipValue	= node.attribute("tipvalue");

		InsertGridRow(_T("id"), XML2T(node.attribute("id").value()));

		if(attrText)
			InsertGridRow(XML2T(attrText.name()), XML2T(attrText.value()));
		else if(!text.IsEmpty())
		{
			InsertGridRow(_T("text"), text);
			attrText = node.attribute_auto("text").set_value(T2XML(text));
			GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
		}

		if(attrToolTip)
			InsertGridRow(XML2T(attrToolTip.name()), XML2T(attrToolTip.value()));
		else if(!tooltip.IsEmpty())
		{
			InsertGridRow(_T("tooltip"), tooltip);
			attrToolTip = node.attribute_auto("tooltip").set_value(T2XML(tooltip));
			GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
		}

		if(attrTipValue)
			InsertGridRow(XML2T(attrTipValue.name()), XML2T(attrTipValue.value()));
		else if(!tipvalue.IsEmpty())
		{
			InsertGridRow(_T("tipvalue"), tipvalue);
			attrTipValue = node.attribute_auto("tipvalue").set_value(T2XML(tipvalue));
			GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
		}

		bFind = TRUE;
		break;
	}

	//语言包中没有找到，载入默认的，并且创建新的节点
	if(!bFind)
	{
		xml_node node = root.append_child("Text");
		node.attribute_auto("id").set_value(m_currentid);

		CString strID;
		strID.Format(_T("%d"), m_currentid);
		InsertGridRow(_T("id"), strID);

		if(!text.IsEmpty())
		{
			InsertGridRow(_T("text"), text);
			node.attribute_auto("text").set_value(T2XML(text));
		}

		if(!tooltip.IsEmpty())
		{
			InsertGridRow(_T("tooltip"), tooltip);
			node.attribute_auto("tooltip").set_value(T2XML(tooltip));
		}

		if(!tipvalue.IsEmpty())
		{
			InsertGridRow(_T("tipvalue"), tipvalue);
			node.attribute_auto("tipvalue").set_value(T2XML(tipvalue));
		}

		GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
	}

	m_pGrid->Refresh();
}

int CDlgLangTextEditUI::InsertGridRow(LPCTSTR attrName, LPCTSTR attrValue)
{
	int row = m_pGrid->InsertRow();
	m_pGrid->SetRowHeight(row, 40);
	m_pGrid->Cell(row,1).SetText(attrName);
	m_pGrid->Cell(row,2).SetText(attrValue);
	if(!CompareString(attrName, _T("id")))
	{
		m_pGrid->Cell(row,2).SetCellType(celltypeEdit);
	}
	return row;
}