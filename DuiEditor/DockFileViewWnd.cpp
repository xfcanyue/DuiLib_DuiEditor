// DockFileViewWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockFileViewWnd.h"


// CDockFileViewWnd

IMPLEMENT_DYNAMIC(CDockFileViewWnd, CDockablePane)

CDockFileViewWnd::CDockFileViewWnd()
{

}

CDockFileViewWnd::~CDockFileViewWnd()
{
}


BEGIN_MESSAGE_MAP(CDockFileViewWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CDockFileViewWnd 消息处理程序
int CDockFileViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		return -1;      // 未能创建
	}

	return 0;
}


void CDockFileViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	m_wndFileView.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}


void CDockFileViewWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_wndFileView.SetFocus();
}


void CDockFileViewWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{

}
