// ImageEditorFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorPaneFile.h"

#include "UIManager.h"
#include "ImageEditor.h"
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
	ClearThumbnail();
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
// 	*pResult = CDRF_DODEFAULT; //控件自己绘制
// 	return;
	if((GetStyle () & LVS_TYPEMASK) != LVS_ICON)
		return;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;

	// Request item-specific notifications if this is the
	// beginning of the paint cycle.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )	//控件开始绘制前
	{
		*pResult = CDRF_NOTIFYITEMDRAW; //返回给控件，真的要自己画
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage ) //控件开始绘制Item前
	{
		// This is the beginning of an item's paint cycle.
		LVITEM   rItem;
		int      nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		CDC*     pDC   = CDC::FromHandle ( pLVCD->nmcd.hdc );
		BOOL     bListHasFocus;
		CRect    rcItem;
		CRect    rcText;
		CString  sText;
		UINT     uFormat;

		//bListHasFocus = ( m_agentList.GetSafeHwnd() == ::GetFocus() );
		//bListHasFocus = ( GetSafeHwnd() == ::GetFocus() );
		bListHasFocus = TRUE;

		// Get the image index and selected/focused state of the
		// item being drawn.
		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		//m_agentList.GetItem ( &rItem );
		GetItem ( &rItem );

		CRect rcBounds;
		GetItemRect ( nItem, rcBounds, LVIR_BOUNDS );
		if(rItem.state & LVIS_SELECTED)
		{
			pDC->FillSolidRect(rcBounds, RGB(0xe6,0xe6,0xe6));
		}

		// Get the rect that holds the item's icon.
		GetItemRect ( nItem, &rcItem, LVIR_ICON );

		// Draw the icon.
		uFormat = ILD_TRANSPARENT;

		if ( ( rItem.state & LVIS_SELECTED ) && bListHasFocus )
			uFormat |= ILD_FOCUS;

		//画图标，如果不能加载的，就用系统默认图标显示
		CRect rcIcon = rcItem;
		rcIcon.left += 5;
		rcIcon.right -= 5;
		TImageInfo *pImageInfo = GetThumbnail(nItem);
		if(pImageInfo)
		{
			CRect rcImage;
			CRect rcTemp(0, 0, pImageInfo->nX, pImageInfo->nY);
			if( rcTemp.Width() < rcIcon.Width())
			{
				CPoint pt = rcItem.CenterPoint();
				rcImage.left = pt.x - rcTemp.Width()/2;
				rcImage.right = pt.x + rcTemp.Width()/2;
			}
			else
			{
				rcImage.left = rcIcon.left;
				rcImage.right = rcIcon.right;
			}
			if(rcTemp.Height() < rcIcon.Height())
			{
				CPoint pt = rcItem.CenterPoint();
				rcImage.top = pt.y - rcTemp.Height()/2;
				rcImage.bottom = pt.y + rcTemp.Height()/2;
			}
			else
			{
				rcImage.top = rcIcon.top;
				rcImage.bottom = rcIcon.bottom;
			}

			RECT rcSource = {0};
			if( rcSource.left == 0 && rcSource.right == 0 && rcSource.top == 0 && rcSource.bottom == 0 ) {
				rcSource.right = pImageInfo->nX;
				rcSource.bottom = pImageInfo->nY;
			}
			if (rcSource.right > pImageInfo->nX) rcSource.right = pImageInfo->nX;
			if (rcSource.bottom > pImageInfo->nY) rcSource.bottom = pImageInfo->nY;

			RECT rcCorner = {0};
			CRenderEngine::DrawImage(pDC->GetSafeHdc(), pImageInfo->hBitmap, rcImage, rcImage, rcSource, rcCorner, pImageInfo->bAlpha, 255, false, false, false);
		}
		else
		{
			IMAGEINFO info;
			if(m_imageShell.GetImageInfo(nItem, &info))
			{
				CRect rcImage = info.rcImage;
				CRect rcTemp = rcImage;
				CPoint pt = rcItem.CenterPoint();
				rcImage.left = pt.x - rcTemp.Width()/2;
				rcImage.right = pt.x + rcTemp.Width()/2;
				rcImage.top = pt.y - rcTemp.Height()/2;
				rcImage.bottom = pt.y + rcTemp.Height()/2;

				m_imageShell.Draw ( pDC, rItem.iImage, rcImage.TopLeft(), uFormat );
			}
			else
				m_imageShell.Draw ( pDC, rItem.iImage, rcItem.TopLeft(), uFormat );
		}


		// Get the rect that bounds the text label.
		//m_agentList.GetItemRect ( nItem, rcItem, LVIR_LABEL );
		GetItemRect ( nItem, rcItem, LVIR_LABEL );

		pDC->SetBkMode ( TRANSPARENT );

		// Tweak the rect a bit for nicer-looking text alignment.
		rcText = rcItem;
		// Draw the text.
		//sText = m_agentList.GetItemText ( nItem, 0 );
		sText = GetItemText ( nItem, 0 );

		pDC->DrawText ( sText, CRect::CRect(rcText.left+3,rcText.top,rcText.right,rcText.bottom+60), DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL );



		// Draw a focus rect around the item if necessary.
		if ( bListHasFocus && ( rItem.state & LVIS_FOCUSED ))
		{
			pDC->DrawFocusRect ( rcIcon );
		}

		*pResult = CDRF_SKIPDEFAULT;    // We've painted everything. 返回给控件，我自己绘制好了，控件别画了
	}
	
}


HRESULT CImageEditorListCtrl::DisplayFolder(LPCTSTR lpszPath)
{
	return __super::DisplayFolder(lpszPath);
}

HRESULT CImageEditorListCtrl::DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo)
{
	HRESULT hr = __super::DisplayFolder(lpItemInfo);
	if(!SUCCEEDED(hr))	return hr;

	ClearThumbnail();
	Sort(0);
	return hr;
}

BOOL CImageEditorListCtrl::InitList()
{
	//弄个假的ImageList给ListCtrl，从而把图标区域变大了。
	m_imageEmpty.Create(60, 60, ILC_COLOR32 | ILC_MASK, 1, 1);
	SetImageList(&m_imageEmpty, LVSIL_NORMAL);

	m_imageShell.Attach(GetShellImageList(TRUE));
	return TRUE;
}

TImageInfo *CImageEditorListCtrl::GetThumbnail(int nItem)
{
	std::map<int, TImageInfo *>::iterator it = m_map.find(nItem);
	if(it != m_map.end())
	{
		return it->second;
	}
	else
	{
		CString strPath;
		GetItemPath(strPath, nItem);
		TImageInfo *pImage = CRenderEngine::LoadImage(strPath);
		if(pImage)
		{
			m_map[nItem] = pImage;
			return pImage;
		}
	}
	return NULL;
}

void CImageEditorListCtrl::ClearThumbnail()
{
	std::map<int, TImageInfo *>::iterator it;
	for(it=m_map.begin(); it!=m_map.end(); it++)
	{
		CRenderEngine::FreeImage(it->second);
	}
	m_map.clear();
}

//////////////////////////////////////////////////////////////////////////
/*
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
*/
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CImageEditorPaneFile, CDockablePane)

CImageEditorPaneFile::CImageEditorPaneFile()
{

}

CImageEditorPaneFile::~CImageEditorPaneFile()
{
}


BEGIN_MESSAGE_MAP(CImageEditorPaneFile, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EXPLORER_UP, OnExplorerUp)
	ON_COMMAND(ID_EXPLORER_BACK, &CImageEditorPaneFile::OnExplorerBack)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_BACK, &CImageEditorPaneFile::OnUpdateExplorerBack)
	ON_COMMAND(ID_EXPLORER_FORWARD, &CImageEditorPaneFile::OnExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_FORWARD, &CImageEditorPaneFile::OnUpdateExplorerForward)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_UP, &CImageEditorPaneFile::OnUpdateExplorerUp)
	ON_COMMAND(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorPaneFile::OnExplorerViewsLargeIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LARGE_ICON, &CImageEditorPaneFile::OnUpdateExplorerViewsLargeIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorPaneFile::OnExplorerViewsSmallIcon)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_SMALL_ICON, &CImageEditorPaneFile::OnUpdateExplorerViewsSmallIcon)
	ON_COMMAND(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorPaneFile::OnExplorerViewsDetails)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_DETAILS, &CImageEditorPaneFile::OnUpdateExplorerViewsDetails)
	ON_COMMAND(ID_EXPLORER_VIEWS_LIST, &CImageEditorPaneFile::OnExplorerViewsList)
	ON_UPDATE_COMMAND_UI(ID_EXPLORER_VIEWS_LIST, &CImageEditorPaneFile::OnUpdateExplorerViewsList)
END_MESSAGE_MAP()

// CViewControlWnd 消息处理程序
int CImageEditorPaneFile::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	m_wndList.Create (WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_REPORT, rectDummy, this, IDC_SHELL_LIST_CTRL);
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

	m_wndList.InitList();


	CString strSkinDir = g_pEditorImage->GetUIManager()->GetDocument()->GetSkinPath();
	CString strPathName = strSkinDir + XML2T(g_pEditorImage->m_nodedata.attribute(XTEXT("file")).value());

	BOOL bGoToPath = FALSE;
	TCHAR szPath[256];
	_tcscpy_s(szPath, 256, strPathName);
	if(PathRemoveFileSpec(szPath))
	{
		HRESULT hr = m_wndList.DisplayFolder(szPath);
		if(SUCCEEDED(hr))
		{
			for (int i=0; i<m_wndList.GetItemCount(); i++)
			{
				CString temp;
				if(m_wndList.GetItemPath(temp, i))
				{
					if(temp == strPathName)
					{
						//.....没效果啊
						m_wndList.SetItemState(i, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
						bGoToPath = TRUE;
						break;
					}
				}
			}
		}
	}
	
	if(!bGoToPath)
		m_wndList.DisplayFolder(strSkinDir);
	return 0;
}

void CImageEditorPaneFile::OnSize(UINT nType, int cx, int cy)
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
	//m_wndList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndList.Refresh();
}


void CImageEditorPaneFile::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_wndList.SetFocus();
}


void CImageEditorPaneFile::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_EXPLORER);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}

void CImageEditorPaneFile::OnExplorerBack()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageEditorPaneFile::OnUpdateExplorerBack(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CImageEditorPaneFile::OnExplorerForward()
{
	// TODO: 在此添加命令处理程序代码
}


void CImageEditorPaneFile::OnUpdateExplorerForward(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CImageEditorPaneFile::OnExplorerUp()
{
	m_wndList.DisplayParentFolder();
}

void CImageEditorPaneFile::OnUpdateExplorerUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_wndList.IsDesktop());
}


void CImageEditorPaneFile::OnExplorerViewsLargeIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsLargeIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_ICON);
}


void CImageEditorPaneFile::OnExplorerViewsSmallIcon()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsSmallIcon(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_SMALLICON);
}


void CImageEditorPaneFile::OnExplorerViewsDetails()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsDetails(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_REPORT);
}


BOOL CImageEditorPaneFile::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
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


void CImageEditorPaneFile::OnExplorerViewsList()
{
	m_wndList.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}


void CImageEditorPaneFile::OnUpdateExplorerViewsList(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio ((m_wndList.GetStyle () & LVS_TYPEMASK) == LVS_LIST);
}

