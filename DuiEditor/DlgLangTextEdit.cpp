// DlgLangTextEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgLangTextEdit.h"
#include "afxdialogex.h"
#include "UIManager.h"

// CDlgLangTextEdit 对话框

IMPLEMENT_DYNAMIC(CDlgLangTextEdit, CDialogEx)

CDlgLangTextEdit::CDlgLangTextEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLangTextEdit::IDD, pParent)
{
	m_currentid = 0;
}

CDlgLangTextEdit::~CDlgLangTextEdit()
{
}

void CDlgLangTextEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CDlgLangTextEdit, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLangTextEdit::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDlgLangTextEdit::OnNMClickTree)
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CDlgLangTextEdit::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgLangTextEdit 消息处理程序


BOOL CDlgLangTextEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CStdStringPtrMap *map = &GetUIManager()->GetDocument()->m_mLangPackage;
	for (int i=0; i<map->GetSize(); i++)
	{
		LPCTSTR key = map->GetAt(i);
		m_Tree.InsertItem(key, 0, 0, TVI_ROOT);
	}

	//////////////////////////////////////////////////////////////////////////
	m_Grid.FormatReport();
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetHighLightBkColor(RGB(164,195,235));
	m_Grid.SetHighLightTextClr(RGB(0,0,0));
	m_Grid.SetFrameFocusCell(TRUE);
	m_Grid.CellDef().SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_Grid.CellFixedColDef().SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_Grid.SetFont(GetFont());
	m_Grid.SetEditable(TRUE);

	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.Cell(0,1).SetText(_T("Attributes"));
	m_Grid.Cell(0,2).SetText(_T("Values"));
	m_Grid.SetColumnWidth(1, 150);
	m_Grid.ExpandColumnsToFit(TRUE);

	m_Tree.SelectItem(m_Tree.GetRootItem());
	LoadText();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgLangTextEdit::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CDlgLangTextEdit::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	if(!GetCursorPos(&point))	return;

	m_Tree.ScreenToClient(&point); 
	HTREEITEM hItem = m_Tree.HitTest(point); 
	if(hItem)
	{
		m_Tree.SelectItem(hItem);
		LoadText();
	}

	*pResult = 0;
}

void CDlgLangTextEdit::LoadText()
{
	m_Grid.SetRowCount(1);

	m_currentid = 0;
	CString lang = m_Tree.GetItemText(m_Tree.GetSelectedItem());
	xml_document *xml = (xml_document *)GetUIManager()->GetDocument()->m_mLangPackage.Find(lang);
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

	m_Grid.Refresh();
}

void CDlgLangTextEdit::InsertGridRow(LPCTSTR attrName, LPCTSTR attrValue)
{
	int row = m_Grid.InsertRow(_T(""));
	m_Grid.SetRowHeight(row,40);
	m_Grid.Cell(row,1).SetText(attrName);
	m_Grid.Cell(row,2).SetText(attrValue);
	if(_tcscmp(attrName, _T("id")) == 0)
	{
		m_Grid.Cell(row,2).SetEditable(FALSE);
	}
}

void CDlgLangTextEdit::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	int nRow = pItem->iRow;
	int nCol = pItem->iColumn;
	if(nCol < 2) return;

	if(m_currentid == 0) return;

	CString attrName = m_Grid.Cell(nRow,1).GetText();
	CString attrValue = m_Grid.Cell(nRow,nCol).GetText();

	CString lang = m_Tree.GetItemText(m_Tree.GetSelectedItem());
	xml_document *xml = (xml_document *)GetUIManager()->GetDocument()->m_mLangPackage.Find(lang);
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

void CDlgLangTextEdit::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CDlgLangTextEdit::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgLangTextEdit::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!m_Grid.GetSafeHwnd())	return;
	if(!m_Tree.GetSafeHwnd())	return;

	CRect rcClient, rcTree, rcGrid;
	GetClientRect(rcClient);
	m_Tree.GetWindowRect(rcTree);

	m_Tree.MoveWindow(0,0, rcTree.Width(), rcClient.Height());
	m_Grid.MoveWindow(rcTree.Width(), 0, rcClient.Width()-rcTree.Width(), rcClient.Height());
	m_Grid.ExpandColumnsToFit(TRUE);
}
