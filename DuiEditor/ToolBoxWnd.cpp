// ToolBoxWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ToolBoxWnd.h"

CDockToolBoxWnd *g_pToolBox;
//////////////////////////////////////////////////////////////////////////
// CToolBoxWnd

IMPLEMENT_DYNAMIC(CDockToolBoxWnd, CDockablePane)

CDockToolBoxWnd::CDockToolBoxWnd()
{
	
}

CDockToolBoxWnd::~CDockToolBoxWnd()
{
}


BEGIN_MESSAGE_MAP(CDockToolBoxWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CToolBoxWnd 消息处理程序

int CDockToolBoxWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_ctlToolList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 12))
	{
		TRACE0("Failed to create ToolBox\n");
		return -1;      // fail to create
	}

	InitToolList();

	g_pToolBox = this;

	return 0;
}

void CDockToolBoxWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

void CDockToolBoxWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_ctlToolList.SetWindowPos(NULL, rectClient.left,rectClient.top,rectClient.Width(),rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDockToolBoxWnd::InitToolList()
{
	CDockToolBoxElement *pTooElemHeader = new CDockToolBoxElement(_T("指针"), TREENODETYPE_POINTER,	IDI_TOOLBOX_POINTER);
	pTooElemHeader->SetItemImage(TREENODETYPE_POINTER);
	m_ctlToolList.AddToolTab(pTooElemHeader);


	pugi::xml_node root = g_duiProp.GetRoot();
	for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		if(g_duiProp.IsControlUI(node))
		{
			CString nodeName = XML2T(node.name());
			UINT classID = CDuiPropertyFile::GetControlIconIndex(node);
			CDockToolBoxElement *pTooElem = new CDockToolBoxElement(XML2T(node.name()), classID, 0);

			pTooElem->SetItemImage(classID);
			pTooElem->SetTag((UINT_PTR)node.internal_object());
			m_ctlToolList.AddToolTab(pTooElem);
		}
	}

	m_ctlToolList.SortByItemName();
	m_ctlToolList.SetCurSel(pTooElemHeader, TRUE);
}