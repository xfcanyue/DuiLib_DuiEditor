// DockFileViewCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockFileViewCtrl.h"

#include "ThreadTest.h"
#include "ThreadPipe.h"

// CDockFileViewCtrl

IMPLEMENT_DYNAMIC(CDockFileViewCtrl, CTreeCtrl)

CDockFileViewCtrl::CDockFileViewCtrl()
{
	m_psfRootFolder = NULL;
	m_hStartupItem = NULL;
}

CDockFileViewCtrl::~CDockFileViewCtrl()
{
	if(m_psfRootFolder) m_psfRootFolder->Release();
}


BEGIN_MESSAGE_MAP(CDockFileViewCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CDockFileViewCtrl::OnItemexpanding)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_FILE_OPEN, &CDockFileViewCtrl::OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILE_OPEN, &CDockFileViewCtrl::OnUpdateFileOpen)
	ON_COMMAND(ID_EDIT_DEBUG_FILE, &CDockFileViewCtrl::OnEditDebugFile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEBUG_FILE, &CDockFileViewCtrl::OnUpdateEditDebugFile)
	ON_COMMAND(ID_EDIT_REFRESH, &CDockFileViewCtrl::OnEditRefresh)
	ON_COMMAND(ID_EDIT_DEBUG_RUN, &CDockFileViewCtrl::OnEditDebugRun)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEBUG_RUN, &CDockFileViewCtrl::OnUpdateEditDebugRun)
	ON_COMMAND(ID_EDIT_COPY_FILE_NAME, &CDockFileViewCtrl::OnEditCopyFileName)
	ON_COMMAND(ID_EDIT_DEBUG_NO_FILE, &CDockFileViewCtrl::OnEditDebugNoFile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEBUG_NO_FILE, &CDockFileViewCtrl::OnUpdateEditDebugNoFile)
END_MESSAGE_MAP()



// CDockFileViewCtrl 消息处理程序
int CDockFileViewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	TCHAR szWinDir [MAX_PATH + 1];
	if (GetWindowsDirectory(szWinDir, MAX_PATH) > 0)
	{
		SHFILEINFO sfi;

		CImageList *pImgList = CImageList::FromHandle((HIMAGELIST) SHGetFileInfo(szWinDir, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON));
		HIMAGELIST hImgList = (HIMAGELIST) SHGetFileInfo(szWinDir, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
		m_ImageList.Attach(hImgList);

		HINSTANCE hInstance = AfxGetApp()->m_hInstance;
		m_nIconRoot = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_PROJECT)));

		SetImageList(&m_ImageList, 0);
	}

	return 0;
}

BOOL CDockFileViewCtrl::InitFolder(LPCTSTR szFolderPath)
{
	CLockWindowUpdate lock(this);

	m_strCurrentPath = szFolderPath;

	//载入项目配置
	g_proj.InitProject(szFolderPath);

	DeleteAllItems();

	// Get the desktop's IShellFolder:
	LPSHELLFOLDER pDesktop;
	if (FAILED(SHGetDesktopFolder(&pDesktop)))
	{
		return FALSE;
	}

	LSSTRING_CONVERSION;
	LPSHELLFOLDER psfRootFolder = NULL;
	LPITEMIDLIST pidlRootFolder = NULL;
	if (FAILED(pDesktop->ParseDisplayName(NULL, NULL, (LPWSTR)(const wchar_t *)LST2W(szFolderPath), NULL, &pidlRootFolder, NULL)))
	{
		return FALSE;
	}
	if (FAILED(pDesktop->BindToObject(pidlRootFolder, NULL, IID_PPV_ARGS(&psfRootFolder))))
	{
		return FALSE;
	}

	m_psfRootFolder = psfRootFolder;

	// Fill in the TVITEM structure for this item:
	TV_ITEM tvItem;
	tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

	// Put the private information in the lParam:
	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO) GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
	ENSURE(pItem != NULL);

	pItem->pidlRel = pidlRootFolder;
	pItem->pidlFQ = ((CDuiEditorApp *)AfxGetApp())->GetShellManager()->CopyItem(pidlRootFolder); 
	
	// The desktop doesn't have a parent folder, so make this NULL:
	pItem->pParentFolder = NULL;//pWorkDir;
	tvItem.lParam = (LPARAM)pItem;

	CString strItem = (_T("项目文件"));//OnGetItemText(pItem);
	tvItem.pszText = strItem.GetBuffer(strItem.GetLength());
	tvItem.iImage = m_nIconRoot;//OnGetItemIcon(pItem, FALSE);
	tvItem.iSelectedImage =  m_nIconRoot;//OnGetItemIcon(pItem, TRUE);

	// Assume the desktop has children:
	tvItem.cChildren = TRUE;

	// Fill in the TV_INSERTSTRUCT structure for this item:
	TV_INSERTSTRUCT tvInsert;

	tvInsert.item = tvItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.hParent = TVI_ROOT;

	// Add the item:
	HTREEITEM hParentItem = InsertItem(&tvInsert);

	// Go ahead and expand this item:
	Expand(hParentItem, TVE_EXPAND);

	pDesktop->Release();
	return TRUE;
}

BOOL CDockFileViewCtrl::GetChildItems(HTREEITEM hParentItem)
{
	ASSERT_VALID(this);

#ifdef DUILIB_VERSION_ORIGINAL
	DuiLib::CWaitCursor wait;
#else
	CWaitCursor wait;
#endif

	// Get the parent item's pidl:
	TVITEM tvItem;
	ZeroMemory(&tvItem, sizeof(tvItem));

	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hParentItem;

	if (!GetItem(&tvItem))
	{
		return FALSE;
	}

	SetRedraw(FALSE);

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO) tvItem.lParam;
	ENSURE(pItem != NULL);

	LPSHELLFOLDER pParentFolder = NULL;
	HRESULT hr;

	// If the parent folder is NULL, then we are at the root
	// of the namespace, so the parent of this item is the desktop folder
	if (pItem->pParentFolder == NULL)
	{
		//hr = SHGetDesktopFolder(&pParentFolder);
		hr = S_OK;
		pParentFolder = m_psfRootFolder;
	}
	else
	{
		// Otherwise we need to get the IShellFolder for this item:
		hr = pItem->pParentFolder->BindToObject(pItem->pidlRel, NULL, IID_IShellFolder, (LPVOID*) &pParentFolder);
	}

	if (FAILED(hr))
	{
		SetRedraw();
		return FALSE;
	}

	EnumObjects(hParentItem, pParentFolder, pItem->pidlFQ);

	// Sort the new items:
	TV_SORTCB tvSort;

	tvSort.hParent = hParentItem;
	tvSort.lpfnCompare = CompareProc;
	tvSort.lParam = 0;

	SortChildrenCB(&tvSort);

	SetRedraw();
	RedrawWindow();

	pParentFolder->Release();
	return TRUE;
}

HRESULT CDockFileViewCtrl::EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(this);
	//ASSERT_VALID(afxShellManager);

	LPENUMIDLIST pEnum = NULL;

	SHCONTF dwFlags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN;
	HRESULT hr = pParentFolder->EnumObjects(NULL, dwFlags, &pEnum);
	if (FAILED(hr) || pEnum == NULL)
	{
		return hr;
	}

	LPITEMIDLIST pidlTemp;
	DWORD dwFetched = 1;

	// Enumerate the item's PIDLs:
	while (SUCCEEDED(pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)
	{
		TVITEM tvItem;
		ZeroMemory(&tvItem, sizeof(tvItem));

		// Fill in the TV_ITEM structure for this item:
		tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

		// AddRef the parent folder so it's pointer stays valid:
		pParentFolder->AddRef();

		// Put the private information in the lParam:
		LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFO));
		ENSURE(pItem != NULL);

		pItem->pidlRel = pidlTemp;
		pItem->pidlFQ = ((CDuiEditorApp *)AfxGetApp())->GetShellManager()->ConcatenateItem(pidlParent, pidlTemp);

		pItem->pParentFolder = pParentFolder;
		tvItem.lParam = (LPARAM)pItem;

		CString strItem = OnGetItemText(pItem);
		if(strItem.CompareNoCase(_T("project.dui")) == 0) //项目文件就不要载入了
			continue;

		tvItem.pszText = strItem.GetBuffer(strItem.GetLength());
		tvItem.iImage = OnGetItemIcon(pItem, FALSE);
		tvItem.iSelectedImage = OnGetItemIcon(pItem, TRUE);

		// Determine if the item has children:
		DWORD dwAttribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME | SFGAO_FILESYSANCESTOR;

		pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*) &pidlTemp, &dwAttribs);
		tvItem.cChildren = (dwAttribs & (SFGAO_HASSUBFOLDER | SFGAO_FILESYSANCESTOR));

		// Determine if the item is shared:
		if (dwAttribs & SFGAO_SHARE)
		{
			tvItem.mask |= TVIF_STATE;
			tvItem.stateMask |= TVIS_OVERLAYMASK;
			tvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
		}

		// Fill in the TV_INSERTSTRUCT structure for this item:
		TVINSERTSTRUCT tvInsert;

		tvInsert.item = tvItem;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.hParent = hParentItem;

		HTREEITEM htNew = InsertItem(&tvInsert);
		
		CString strStartupFile = g_proj.GetStartupFile();
		if(strStartupFile.CompareNoCase(tvItem.pszText) == 0)
		{
			m_hStartupItem = htNew;
			SetItemState(htNew, TVIS_BOLD, TVIS_BOLD);
		}
		dwFetched = 0;
	}

	pEnum->Release();
	return S_OK;
}

int CALLBACK CDockFileViewCtrl::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNREFERENCED_PARAMETER(lParamSort);

	LPAFX_SHELLITEMINFO pItem1 = (LPAFX_SHELLITEMINFO)lParam1;
	LPAFX_SHELLITEMINFO pItem2 = (LPAFX_SHELLITEMINFO)lParam2;

	HRESULT hr = pItem1->pParentFolder->CompareIDs(0, pItem1->pidlRel, pItem2->pidlRel);

	if (FAILED(hr))
	{
		return 0; 
	}

	return(short)SCODE_CODE(GetScode(hr));
}

CString CDockFileViewCtrl::OnGetItemText(LPAFX_SHELLITEMINFO pItem)
{
	ENSURE(pItem != NULL);

	SHFILEINFO sfi;

	if (SHGetFileInfo((LPCTSTR) pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
	{
		return sfi.szDisplayName;
	}

	return _T("???");
}

int CDockFileViewCtrl::OnGetItemIcon(LPAFX_SHELLITEMINFO pItem, BOOL bSelected)
{
	ENSURE(pItem != NULL);

	SHFILEINFO sfi;

	UINT uiFlags = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;

	if (bSelected)
	{
		uiFlags |= SHGFI_OPENICON;
	}
	else
	{
		uiFlags |= SHGFI_LINKOVERLAY;
	}

	if (SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), uiFlags))
	{
		return sfi.iIcon;
	}

	return -1;
}

BOOL CDockFileViewCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	if (message == WM_NOTIFY)
	{
		LPNMHDR lpnmh = (LPNMHDR) lParam;
		ENSURE(lpnmh != NULL);

		if (lpnmh->code == TVN_SELCHANGED)
		{
		}
	}

	return CTreeCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

void CDockFileViewCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	ENSURE(pNMTreeView != NULL);

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	ENSURE(hItem != NULL);

	switch (pNMTreeView->action)
	{
	case TVE_EXPAND:
		GetChildItems(hItem);

		if (GetChildItem(hItem) == NULL)
		{
			// Remove '+':
			TV_ITEM tvItem;
			ZeroMemory(&tvItem, sizeof(tvItem));

			tvItem.hItem = hItem;
			tvItem.mask = TVIF_CHILDREN;

			SetItem(&tvItem);
		}
		break;

	case TVE_COLLAPSE:
		{
			for (HTREEITEM hItemSel = GetSelectedItem(); hItemSel != NULL;)
			{
				HTREEITEM hParentItem = GetParentItem(hItemSel);

				if (hParentItem == hItem)
				{
					SelectItem(hItem);
					break;
				}

				hItemSel = hParentItem;
			}

			//remove all of the items from this node
			Expand(hItem, TVE_COLLAPSE | TVE_COLLAPSERESET);
		}
		break;
	}

	*pResult = 0;
}

void CDockFileViewCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_FILE_VIEW);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}

void CDockFileViewCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	SetFocus();
	UINT nFlags = 0;
	HTREEITEM ht = HitTest(point, &nFlags);

	if(nFlags & TVHT_ONITEMBUTTON)
	{
		if(TVIS_EXPANDED & GetItemState(ht, TVIS_EXPANDED))
		{
			Expand(ht, TVE_COLLAPSE);
		}
		else
		{
			Expand(ht, TVE_EXPAND);
		}
	}


	SelectItem(ht);
}

void CDockFileViewCtrl::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	SetFocus();
	UINT nFlags = 0;
	SelectItem(HitTest(point, &nFlags));
}


void CDockFileViewCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnFileOpen();
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

CString CDockFileViewCtrl::GetItemPath(HTREEITEM htreeItem)
{
	ASSERT_VALID(this);

	BOOL bRes = FALSE;
	CString strPath;

	if (htreeItem == NULL)
	{
		htreeItem = GetSelectedItem();
	}

	if (htreeItem == NULL)
	{
		return FALSE;
	}

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GetItemData(htreeItem);
	TCHAR path[MAX_PATH];
	if(SHGetPathFromIDList(pItem->pidlFQ, path))
	{
		strPath = path;
	}
	return strPath;
}
//////////////////////////////////////////////////////////////////////////
void CDockFileViewCtrl::OnFileOpen()
{
	HTREEITEM ht = GetSelectedItem();
	if(ht == NULL)	return;

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GetItemData(ht);
	SHFILEINFO sfi;
	HRESULT hr = SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME|SHGFI_ATTRIBUTES);
	if(FAILED(hr)) return;

	if(sfi.dwAttributes & SFGAO_FOLDER)
		return;

	CString strName = sfi.szDisplayName;
	int n = strName.ReverseFind('.');
	if(n < 0) return;
	CString strExtName = strName.Right(strName.GetLength() - n);
	if((strExtName.CompareNoCase(_T(".xml")) != 0)  && (strExtName.CompareNoCase(_T(".as")) != 0) && (strExtName.CompareNoCase(_T(".txt")) != 0) )
		return;

	CString strPath = GetItemPath(ht);
	if(!strPath.IsEmpty())
	{
		AfxGetApp()->OpenDocumentFile(strPath, TRUE);
	}
}


void CDockFileViewCtrl::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetSelectedItem() != NULL);
}


void CDockFileViewCtrl::OnEditDebugFile()
{
	HTREEITEM ht = GetSelectedItem();
	if(ht == NULL)	return;

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GetItemData(ht);
	TCHAR path[MAX_PATH];
	if(!SHGetPathFromIDList(pItem->pidlFQ, path))
		return;
	
	SHFILEINFO sfi;
	HRESULT hr = SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME);

	g_proj.SetStartupFile(sfi.szDisplayName);

	SetItemState(m_hStartupItem, 0, TVIS_BOLD);
	m_hStartupItem = ht;
	SetItemState(m_hStartupItem, TVIS_BOLD, TVIS_BOLD);


}


void CDockFileViewCtrl::OnUpdateEditDebugFile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetSelectedItem() != NULL);
}


void CDockFileViewCtrl::OnEditRefresh()
{
	// TODO: 在此添加命令处理程序代码
	InitFolder(m_strCurrentPath);
}


void CDockFileViewCtrl::OnEditDebugRun()
{
	HTREEITEM ht = GetSelectedItem();
	if(ht == NULL)	return;

	g_pThreadTest = new CThreadTest;
	g_pThreadTest->m_nTestFrom = 1;
	g_pThreadTest->m_strSpacialFile = GetItemPath(ht);
	g_pThreadTest->CreateThread();
}


void CDockFileViewCtrl::OnUpdateEditDebugRun(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hThreadTestHandle == NULL);
}


void CDockFileViewCtrl::OnEditCopyFileName()
{
	HTREEITEM ht = GetSelectedItem();
	if(ht == NULL)	return;

	LPAFX_SHELLITEMINFO pItem = (LPAFX_SHELLITEMINFO)GetItemData(ht);
	SHFILEINFO sfi;
	HRESULT hr = SHGetFileInfo((LPCTSTR)pItem->pidlFQ, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME);
	if(FAILED(hr)) return;

	CString strText = sfi.szDisplayName;
	if(OpenClipboard())
	{
		EmptyClipboard();

		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (strText.GetLength()+1) * sizeof(TCHAR));
		TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
		_tcscpy(buffer, strText);
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
		SetClipboardData(CF_TEXT, clipbuffer);
#endif
		CloseClipboard();
	}
}


void CDockFileViewCtrl::OnEditDebugNoFile()
{
	g_proj.SetStartupFile(_T(""));

	if(m_hStartupItem)
	{
		SetItemState(m_hStartupItem, 0, TVIS_BOLD);
		m_hStartupItem = NULL;
	}
}


void CDockFileViewCtrl::OnUpdateEditDebugNoFile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!g_proj.GetStartupFile().IsEmpty());
}
