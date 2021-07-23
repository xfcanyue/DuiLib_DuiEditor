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
	UI_BINDCONTROL(CGridUI, m_pGrid, _T("grid_lang_text"));
	m_pGrid->SetColumnWidth(1,100);
	m_pGrid->SetColumnWidth(2,100);

	CStdStringPtrMap *map = &GetUIManager()->GetDocument()->m_mLangPackage;
	for (int i=0; i<map->GetSize(); i++)
	{
		LPCTSTR key = map->GetAt(i);

		int row = m_pGrid->InsertRow();
		m_pGrid->Cell(row,1).SetText(key);

		LoadText(row, key);
	}
}

void CDlgLangTextEditUI::OnNotifyClick(TNotifyUI& msg)
{
	if(IsControl(msg, _T("btn_ok")))
	{
		CString s1, s2;
		BOOL bDirty = FALSE;
		for (int row=1; row<m_pGrid->GetRowCount(); row++)
		{
			xml_node node((xml_node_struct *)m_pGrid->GetRowTag(row));

			if(node.attribute("id").as_int() != m_currentid)
			{
				node.attribute_auto("id").set_value(m_currentid);
				bDirty = TRUE;
			}

			s1 = XML2T(node.attribute("text").value());
			s2 = m_pGrid->Cell(row,3).GetText();
			if(s1 != s2 )
			{
				node.attribute_auto("text").set_value(T2XML(s2));
				bDirty = TRUE;
			}

			s1 = XML2T(node.attribute("tooltip").value());
			s2 = m_pGrid->Cell(row,4).GetText();
			if(s1 != s2 )
			{
				node.attribute_auto("tooltip").set_value(T2XML(s2));
				bDirty = TRUE;
			}

			s1 = XML2T(node.attribute("tipvalue").value());
			s2 = m_pGrid->Cell(row,5).GetText();
			if(s1 != s2 )
			{
				node.attribute_auto("tipvalue").set_value(T2XML(s2));
				bDirty = TRUE;
			}
		}

		if(bDirty)
		{
			GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
		}

		Close(IDOK);
	}
	else if(IsControl(msg, _T("btn_cancel")))
	{
		Close(IDCANCEL);
	}
}

void CDlgLangTextEditUI::LoadText(int row, LPCTSTR langtype)
{
	xml_document *xml = (xml_document *)GetUIManager()->GetDocument()->m_mLangPackage.Find(langtype);
	if(!xml) return;
	xml_node root = xml->child("Language");
	if(!root) return;

	CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_pFocused;
	if(!pTrackElem) return;	 
	CControlUI *pFocus = pTrackElem->m_pControl;
	xml_node nodeFocus = xml_node((xml_node_struct *)pFocus->GetTag());
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

		m_pGrid->SetRowTag(row, (UINT_PTR)node.internal_object());

		xml_attribute attrText		= node.attribute("text");
		xml_attribute attrToolTip	= node.attribute("tooltip");
		xml_attribute attrTipValue	= node.attribute("tipvalue");

		m_pGrid->Cell(row,2).SetTextN(m_currentid);

		m_pGrid->SetCellType(row, 3, celltypeEdit);
		m_pGrid->SetCellType(row, 4, celltypeEdit);
		m_pGrid->SetCellType(row, 5, celltypeEdit);

		if(attrText)
			m_pGrid->Cell(row,3).SetText(XML2T(attrText.value()));
		else
			m_pGrid->Cell(row,3).SetText(XML2T(nodeFocus.attribute("text").value()));

		if(attrToolTip)
			m_pGrid->Cell(row,4).SetText(XML2T(attrToolTip.value()));
		else
			m_pGrid->Cell(row,4).SetText(XML2T(nodeFocus.attribute("tooltip").value()));

		if(attrTipValue)
			m_pGrid->Cell(row,5).SetText(XML2T(attrTipValue.value()));
		else
			m_pGrid->Cell(row,5).SetText(XML2T(nodeFocus.attribute("tipvalue").value()));

		bFind = TRUE;
		break;
	}

	m_pGrid->Refresh();
}