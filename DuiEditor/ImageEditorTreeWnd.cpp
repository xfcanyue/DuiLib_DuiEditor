// ImageEditorFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorTreeWnd.h"

#include "ImageEditorFrame.h"

BEGIN_MESSAGE_MAP(CImageEditorListCtrl, CMFCShellListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CImageEditorListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CImageEditorListCtrl::OnNMClick)
END_MESSAGE_MAP()

CImageEditorListCtrl::CImageEditorListCtrl()
{
	
}

CImageEditorListCtrl::~CImageEditorListCtrl()
{
}

void CImageEditorListCtrl::OnSetColumns()
{
	int nColumnCount = GetHeaderCtrl().GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
	{
		DeleteColumn(0);
	}

	const TCHAR* szName [] = {
		_T("文件"), _T("大小"), _T("类型"), _T("修改时间"), };

		for (int iColumn = 0; iColumn < 4; iColumn++)
		{
			int nFormat = (iColumn == AFX_ShellList_ColumnSize) ? LVCFMT_RIGHT : LVCFMT_LEFT;

			InsertColumn(iColumn, szName [iColumn], nFormat, 100, iColumn);
		}
}

void CImageEditorListCtrl::DoDefault(int iItem)
{
	LVITEM lvItem;

	ZeroMemory(&lvItem, sizeof(lvItem));
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = iItem;

	if (!GetItem(&lvItem))
	{
		return;
	}

	LPAFX_SHELLITEMINFO pInfo = (LPAFX_SHELLITEMINFO) lvItem.lParam;
	if (pInfo == NULL || pInfo->pParentFolder == NULL || pInfo->pidlRel == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	IShellFolder *psfFolder = pInfo->pParentFolder;
	if (psfFolder == NULL)
	{
		HRESULT hr = SHGetDesktopFolder(&psfFolder);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
			return;
		}
	}
	else
	{
		psfFolder->AddRef();
	}

	if (psfFolder == NULL)
	{
		return;
	}

	// If specified element is a folder, try to display it:
	ULONG ulAttrs = SFGAO_FOLDER;
	psfFolder->GetAttributesOf(1, (const struct _ITEMIDLIST **) &pInfo->pidlRel, &ulAttrs);

	if (ulAttrs & SFGAO_FOLDER)
	{
		DisplayFolder(pInfo);
	}
	else
	{
		CString strPath;
		if(GetItemPath(strPath, iItem))
		{
			SHFILEINFO sfi;
			SHGetFileInfo((LPCTSTR)pInfo->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME);
			int x = 0;
			CImageEditorFrame *pFrame = (CImageEditorFrame *)GetParent()->GetParent();
			pFrame->OnSelectedFile(strPath);
		}
	}

	psfFolder->Release();
}

void CImageEditorListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = GetNextItem(-1, LVNI_FOCUSED);
	if (nItem != -1)
	{
		DoDefault(nItem);
	}

	*pResult = 0;
}

void CImageEditorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = CDRF_DODEFAULT;

	if(!((GetStyle () & LVS_TYPEMASK) == LVS_REPORT))
		return;


	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{

		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		BOOL bSelect = FALSE;
		POSITION pos =GetFirstSelectedItemPosition(); 
		while(pos) 
		{ 
			int index = GetNextSelectedItem(pos); 
			if(index==nItem) 
			{ 
				bSelect = TRUE; 
				break; 
			} 
		}     
		if(bSelect)
		{
			clrNewTextColor = RGB(0,0,0);     //Set the text to red
			//clrNewTextColor = RGB(255,255,255);     //Set the text to red
			clrNewBkColor = RGB(0x57, 0x80, 0xF0);     //Set the bkgrnd color to blue
		}
		else
		{    
			clrNewTextColor = RGB(0,0,0);     //Leave the text black
			clrNewBkColor = RGB(255,255,255);    //leave the bkgrnd color white
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;


		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;

	}
}


//////////////////////////////////////////////////////////////////////////
//CImageEditorListToolBarMenuButton
IMPLEMENT_SERIAL(CImageEditorListToolBarMenuButton, CMFCToolBarMenuButton, 1)
CImageEditorListToolBarMenuButton::CImageEditorListToolBarMenuButton(HMENU hMenu) 
	: CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
{
}
CImageEditorListToolBarMenuButton::CImageEditorListToolBarMenuButton(UINT uiID, HMENU hMenu, int iImage, LPCTSTR lpszText, BOOL bUserButton)
	: CMFCToolBarMenuButton(uiID, hMenu, iImage, lpszText, bUserButton)
{
}

BOOL CImageEditorListToolBarMenuButton::OpenPopupMenu(CWnd* pWnd)
{
	//pWnd->SendMessage(WM_CONTEXTMENU, 0, 0);
	CPoint point;
	if(!GetCursorPos(&point))	return TRUE;
	//ScreenToClient(&point); 

	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_EXPLORER_VIEWS);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, pWnd, TRUE);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CImageEditorListWnd, CDockablePane)

CImageEditorListWnd::CImageEditorListWnd()
{

}

CImageEditorListWnd::~CImageEditorListWnd()
{
}


BEGIN_MESSAGE_MAP(CImageEditorListWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EXPLORER_UP, OnExplorerUp)
	ON_COMMAND(ID_EXPLORER_BACK, &CImageEditorListWnd::OnExplorerBack)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_BACK, &CImageEditorListWnd::OnUpdateExplorerBack)
	ON_COMMAND(ID_EXPLORER_FORWARD, &CImageEditorListWnd::OnExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_FORWARD, &CImageEditorListWnd::OnUpdateExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_UP, &CImageEditorListWnd::OnUpdateExplorerUp)
	ON_COMMAND(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorListWnd::OnExplorerViewsLargeIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorListWnd::OnUpdateExplorerViewsLargeIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorListWnd::OnExplorerViewsSmallIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorListWnd::OnUpdateExplorerViewsSmallIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorListWnd::OnExplorerViewsDetails)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorListWnd::OnUpdateExplorerViewsDetails)
	ON_COMMAND(ID_EXPLORER_VIEWS_LIST, &CImageEditorListWnd::OnExplorerViewsList)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LIST, &CImageEditorListWnd::OnUpdateExplorerViewsList)
END_MESSAGE_MAP()

// CViewControlWnd 消息处理程序
int CImageEditorListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	m_wndList.Create (WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT, rectDummy, this, IDC_SHELL_LIST_CTRL);
	m_wndList.EnableShellContextMenu(FALSE);
	m_wndList.SetColumnWidth(0, 200);

	DWORD dwStyle = m_wndList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_wndList.SetExtendedStyle(dwStyle); //设置扩展风格

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TB_IMAGE_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_TB_IMAGE_EXPLORER, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);
	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_BACK));
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_FORWARD));
	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_EXPLORER_UP));
	return 0;
}

void CImageEditorListWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER | SWP_NOACTIVATE);
}


void CImageEditorListWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_wndList.SetFocus();
}


void CImageEditorListWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_EXPLORER);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}

void CImageEditorListWnd::OnExplorerBack()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageEditorListWnd::OnUpdateExplorerBack(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CImageEditorListWnd::OnExplorerForward()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageEditorListWnd::OnUpdateExplorerForward(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CImageEditorListWnd::OnExplorerUp()
{
	m_wndList.DisplayParentFolder();
}

void CImageEditorListWnd::OnUpdateExplorerUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_wndList.IsDesktop());
}


void CImageEditorListWnd::OnExplorerViewsLargeIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}


void CImageEditorListWnd::OnUpdateExplorerViewsLargeIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}


void CImageEditorListWnd::OnExplorerViewsSmallIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}


void CImageEditorListWnd::OnUpdateExplorerViewsSmallIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}


void CImageEditorListWnd::OnExplorerViewsDetails()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}


void CImageEditorListWnd::OnUpdateExplorerViewsDetails(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}


BOOL CImageEditorListWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if(nCode == CN_UPDATE_COMMAND_UI)
// 	{
// 		CString str;
// 		str.Format(_T("id=%d"), nID);
// 		InsertMsg(str);
// 	}
	return CDockablePane::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CImageEditorListWnd::OnExplorerViewsList()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}


void CImageEditorListWnd::OnUpdateExplorerViewsList(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}
