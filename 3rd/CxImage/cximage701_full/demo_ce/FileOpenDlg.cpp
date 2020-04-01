// FileOpenDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Resourceppc.h"
#include "FileOpenDlg.h"

#include "DateTimeFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CALLBACK SortNameAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK SortNameDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

int CALLBACK SortSizeAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK SortSizeDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

int CALLBACK SortDateAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK SortDateDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);


//---------------------------------------------------------------------------
//
//	CFileOpenDlg dialog
//
//---------------------------------------------------------------------------


CFileOpenDlg::CFileOpenDlg(CWnd* pParent /*=NULL*/)
:	CExDialog	(CFileOpenDlg::IDD, pParent),
	m_bOk		(FALSE),
	m_bTracking	(FALSE),
	m_ySplit	(152),
	m_nSort		(1)
{
	TCHAR		szFmt[65];

	//{{AFX_DATA_INIT(CFileOpenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	SetTitle(_T("File Open"));

	m_strFilter = _T("*");
	m_nState	= vsStandard;

	//
	// Get the locale date format
	//
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, szFmt, 64);
	m_strDateFormat = szFmt;
}


void CFileOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CExDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileOpenDlg)
	DDX_Control(pDX, IDC_FILE_LIST, m_listFile);
	DDX_Control(pDX, IDC_FOLDER_TREE, m_treeFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileOpenDlg, CExDialog)
	//{{AFX_MSG_MAP(CFileOpenDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FOLDER_TREE, OnSelchangedFolderTree)
	ON_NOTIFY(NM_CLICK, IDC_FILE_LIST, OnClickFileList)
	ON_COMMAND(ID_VIEW_STANDARD, OnViewStandard)
	ON_COMMAND(ID_VIEW_TREE, OnViewTree)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_COMMAND(ID_BAR_OK, OnBarOk)
	ON_COMMAND(ID_BAR_CANCEL, OnBarCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHeaderClickFileList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FILE_LIST,	OnCustomDrawList)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_FOLDER_TREE, OnItemExpandingFolderTree)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_FOLDER_TREE, OnGetDispInfoFolderTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//---------------------------------------------------------------------------
//
//	CFileOpenDlg virtual functions
//
//---------------------------------------------------------------------------


// CFileOpenDlg::DoPaint
//
//		Paints the splitter if it is displayed
//
void CFileOpenDlg::DoPaint(CPaintDC& dc) 
{
	if(m_nState == vsStandard)
	{
		CRect	rcClient;
		int		y;

		GetClientRect(&rcClient);

		y = m_ySplit;

		dc.MoveTo(0, y);
		dc.LineTo(rcClient.right, y);

		y += 6;		// This is the splitter height

		dc.MoveTo(0, y);
		dc.LineTo(rcClient.right, y);
	}
}


//---------------------------------------------------------------------------
//
//	CFileOpenDlg utility functions
//
//---------------------------------------------------------------------------


// CFileOpenDlg::DoSortList
//
//		Sorts the list according to the specified criterion
//
void CFileOpenDlg::DoSortList()
{
	switch(m_nSort)
	{
	case 1:
		m_listFile.SortItems(SortNameAsc, (DWORD)&m_cont);
		break;

	case -1:
		m_listFile.SortItems(SortNameDes, (DWORD)&m_cont);
		break;

	case 2:
		m_listFile.SortItems(SortSizeAsc, (DWORD)&m_cont);
		break;

	case -2:
		m_listFile.SortItems(SortSizeDes, (DWORD)&m_cont);
		break;

	case 3:
		m_listFile.SortItems(SortDateAsc, (DWORD)&m_cont);
		break;

	case -3:
		m_listFile.SortItems(SortDateDes, (DWORD)&m_cont);
		break;
	}
}


// CFileOpenDlg::SetColumnIcons
//
//		Correctly sets the columns sort icons
//
void CFileOpenDlg::SetColumnIcons()
{
	CHeaderCtrl*	pHeader = m_listFile.GetHeaderCtrl();

	if(pHeader)
	{
		HDITEM	hdi;
		int		i,
				nItems	= pHeader->GetItemCount();

		for(i = 0; i < nItems; ++i)
		{
			hdi.mask = HDI_IMAGE | HDI_FORMAT;
			pHeader->GetItem(i, &hdi);

			if(i+1 == m_nSort)
			{
				hdi.iImage = 1;
				hdi.fmt |= HDF_IMAGE | HDF_BITMAP_ON_RIGHT;
			}
			else if(i+1 == -m_nSort)
			{
				hdi.iImage = 2;
				hdi.fmt |= HDF_IMAGE | HDF_BITMAP_ON_RIGHT;
			}
			else
			{
				hdi.iImage = 0;
				hdi.fmt &= ~(HDF_IMAGE | HDF_BITMAP_ON_RIGHT);
			}
			pHeader->SetItem(i, &hdi);
		}
	}
}


// CFileOpenDlg::EnumDirs
//
//		Enumerates all subdirectories. Recurses depth-first 
//
int CFileOpenDlg::EnumDirs(LPCTSTR pszPath, LPCTSTR pszFilter, HTREEITEM hItemParent)
{
	WIN32_FIND_DATA	fd;
	HANDLE			hFind;
	BOOL			bFind;
	HTREEITEM		hItem = TVI_LAST;
	CString			strSearch(pszPath),
					strBase(pszPath);
	int				nCount	= 0;

	strSearch += pszFilter;

	hFind = FindFirstFile(strSearch, &fd);
	bFind = (hFind != INVALID_HANDLE_VALUE);

	while(bFind)
	{
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
		   !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			TVINSERTSTRUCT	tvi;
			CString			strSub(strBase);
			int				iIcon;

			strSub += fd.cFileName;
			iIcon = GetIconIndex(strSub);

			tvi.hParent				= hItemParent;
			tvi.hInsertAfter		= TVI_LAST;
			tvi.item.mask			= TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
									  TVIF_TEXT | TVIF_PARAM;
			tvi.item.pszText		= fd.cFileName;
			tvi.item.iImage			= iIcon;
			tvi.item.iSelectedImage	= iIcon;
			tvi.item.cChildren		= I_CHILDRENCALLBACK;
			tvi.item.lParam			= -1;

			hItem = m_treeFolder.InsertItem(&tvi);

			++nCount;
/*
			strSub += _T("\\");
			if(hItem)
			{
				m_treeFolder.SetItemData(hItem, 1);

				EnumDirs(strSub, pszFilter, hItem);
			}*/
		}

		bFind = FindNextFile(hFind, &fd);
	}
	return nCount;
}


// CFileOpenDlg::EnumFiles
//
//		Enumerates all files in a folder
//
void CFileOpenDlg::EnumFiles(LPCTSTR pszPath, LPCTSTR pszFilter)
{
	WIN32_FIND_DATA	fd;
	HANDLE			hFind;
	BOOL			bFind;
	CString			strSearch(pszPath),
					strFile;
	int				i = 0,
					iItem;
	COleDateTime	odt;
	CDateTimeFormat	dtf;
	CWaitCursor		wait;

	//
	// Flush the list
	//
	m_listFile.DeleteAllItems();
	m_cont.clear();

	//
	// Fill in the list
	//
	strSearch += pszFilter;

	hFind = FindFirstFile(strSearch, &fd);
	bFind = (hFind != INVALID_HANDLE_VALUE);

	//
	// Disable painting the list
	//
	m_listFile.SetRedraw(FALSE);

	while(bFind)
	{
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		   !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
		{
			strFile = pszPath;
			strFile += fd.cFileName;
			iItem = m_listFile.InsertItem(i, fd.cFileName, GetIconIndex(strFile));
			if(iItem != -1)
			{
				//
				// Format size according to locale
				//
				m_listFile.SetItemText(iItem, 1, FormatSize(fd.nFileSizeLow));

				//
				// Format date according to locale
				//
				odt = fd.ftLastWriteTime;
				dtf.SetDateTime(odt);
				dtf.SetFormat(m_strDateFormat);

				m_listFile.SetItemText(iItem, 2, dtf.GetString());
				m_listFile.SetItemData(iItem, i);

				m_cont.push_back(fd);
			}

			i++;
		}

		bFind = FindNextFile(hFind, &fd);
	}
	DoSortList();

	//
	// Enable painting the list
	//
	m_listFile.SetRedraw(TRUE);

}


// CFileOpenDlg::GetIconIndex
//
//		Retrieves the file's icon index
//		Code contributed by Amit Dey.
//
int CFileOpenDlg::GetIconIndex(LPCTSTR pszFileName)
{
	SHFILEINFO	ssfi;

	SHGetFileInfo(pszFileName, 0, &ssfi, sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	return ssfi.iIcon;
}


// CFileOpenDlg::GetFullPath
//
//		Returns the full path of a node
//
CString CFileOpenDlg::GetFullPath(HTREEITEM hItem)
{
	CString		strFullPath,
				strFolder,
				strSep(_T("\\"));
	
	if(hItem)
	{
		while(hItem)
		{
			strFolder = m_treeFolder.GetItemText(hItem);

			if(strFolder != strSep)
				strFullPath = strFolder + strSep + strFullPath;

			hItem = m_treeFolder.GetParentItem(hItem);
		}
		strFullPath = strSep + strFullPath;
	}

	return strFullPath;
}


// CFileOpenDlg::FormatSize
//
//		Returns a string with the appropriate formatting
//
CString CFileOpenDlg::FormatSize(DWORD dwSize)
{
	CString			strFmt,
					strLet;

	if(dwSize > (1024 * 1024))
	{
		dwSize /= 1024 * 1024;
		strLet += _T("M");
	}
	else if(dwSize > 1024)
	{
		dwSize /= 1024;
		strLet += _T("K");
	}
	strFmt.Format(_T("%d"), (int)dwSize);
	strFmt += strLet;

	return strFmt;
}


// CFileOpenDlg::GetSystemName
//
//		Returns the system name
//
CString CFileOpenDlg::GetSystemName()
{
	HKEY		hKey;
	LONG		nRval;
	DWORD		dwSize = 64,
				dwType;
	CString		strSysName;
	TCHAR		szName[65];

	//
	// Get the system name
	//
	nRval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("\\Ident"), 0, 0, &hKey);
	if(ERROR_SUCCESS == nRval)
	{
		nRval = RegQueryValueEx(hKey, _T("Name"), NULL, &dwType, 
			(LPBYTE) szName, &dwSize);

		if(ERROR_SUCCESS == nRval)
			strSysName = szName;

		RegCloseKey(hKey);
	}
	
	//
	// On any error, use the default name
	//
	if(ERROR_SUCCESS != nRval)
		strSysName = _T("My Device");

	return strSysName;
}


// CFileOpenDlg::UpdateControls
//
//		Updates the control states
//
void CFileOpenDlg::UpdateControls()
{
	CToolBarCtrl&	tb = m_wndCommandBar.GetToolBarCtrl();

	tb.CheckButton(ID_VIEW_STANDARD,	m_nState == vsStandard);
	tb.CheckButton(ID_VIEW_TREE,		m_nState == vsTreeOnly);
	tb.CheckButton(ID_VIEW_LIST,		m_nState == vsListOnly);

	tb.EnableButton(ID_BAR_OK, m_bOk);
}


//---------------------------------------------------------------------------
//
//	CFileOpenDlg message handlers
//
//---------------------------------------------------------------------------


// CFileOpenDlg::OnInitDialog
//
//		Initialize the dialog
//
BOOL CFileOpenDlg::OnInitDialog() 
{
	CWinApp*		pApp = AfxGetApp();
	HTREEITEM		hItemRoot;
	HIMAGELIST		hImlSys;
	SHFILEINFO		ssfi;
	CWaitCursor		wait;
	TCHAR			szRoot[]	= _T("\\");

	CExDialog::OnInitDialog();

	//
	// Create the toolbar
	//

	if(!m_wndCommandBar.Create(this) ||
	   !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	   !m_wndCommandBar.AddAdornments() ||
	   !m_wndCommandBar.LoadToolBar(IDR_FILEOPEN))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}
	


	//m_pCmdBar = (CCommandBar*)m_pWndEmptyCB;
	//m_pCmdBar->LoadToolBar(IDR_FILEOPEN);

	//
	// Position the tree.
	//
	GetClientRect(&m_rcTree);
	m_rcTree.top	= GetHeaderHeight() + 1;
	m_rcTree.bottom	= m_ySplit;
	m_treeFolder.MoveWindow(&m_rcTree);

	//
	// Position the list
	//
	GetClientRect(&m_rcList);
	m_rcList.top	= m_ySplit + nTrackHeight + 1;
	m_listFile.MoveWindow(&m_rcList);

	//
	// Hide the OK button
	//
	ModifyStyle(0, WS_NONAVDONEBUTTON, SWP_NOSIZE); 
	//::SHDoneButton(m_hWnd, SHDB_HIDE);

	//
	// Retrieve the small system image list
	//
	hImlSys = (HIMAGELIST)SHGetFileInfo(_T("dummy.txt"), 0, &ssfi,
				sizeof(SHFILEINFO), 
				SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	
	m_imlSystem.Attach(hImlSys);
	m_listFile.SetImageList(&m_imlSystem, LVSIL_SMALL);
	m_treeFolder.SetImageList(&m_imlSystem, TVSIL_NORMAL);
	m_imlSystem.Detach();

	//
	// Initialize the tree
	//
	hItemRoot = TVI_ROOT;//m_treeFolder.InsertItem(GetSystemName(), 0, 1);
	if(hItemRoot)
	{
		TVINSERTSTRUCT	tvi;
		int				iIcon		= GetIconIndex(szRoot);

		tvi.hParent				= TVI_ROOT;
		tvi.hInsertAfter		= TVI_LAST;
		tvi.item.mask			= TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE |
								  TVIF_TEXT | TVIF_PARAM;
		tvi.item.pszText		= szRoot;
		tvi.item.iImage			= iIcon;
		tvi.item.iSelectedImage	= iIcon;
		tvi.item.cChildren		= I_CHILDRENCALLBACK;
		tvi.item.lParam			= -1;

		m_treeFolder.InsertItem(&tvi);

		//m_treeFolder.SetItemData(hItemRoot, 0);

		//EnumDirs(_T("\\"), _T("*"), hItemRoot);
		//m_treeFolder.Expand(hItemRoot, TVE_EXPAND);
	}

	//
	// Initialize the list
	//
	m_listFile.InsertColumn(0, _T("Name"), LVCFMT_LEFT,  80, -1);
	m_listFile.InsertColumn(1, _T("Size"), LVCFMT_RIGHT, 60,  1);
	m_listFile.InsertColumn(2, _T("Date"), LVCFMT_LEFT,  60,  2);
	EnumFiles(szRoot, m_strFilter);
	
	//
	// Initialize the header imagelist
	//
	if(m_imlHeader.Create(IDB_SORT_ARROWS, 16, 8, RGB(255,0,255)))
	{
		CHeaderCtrl*	pHeader = m_listFile.GetHeaderCtrl();

		if(pHeader)
			pHeader->SetImageList(&m_imlHeader);
	}

	SetColumnIcons();

	//
	// Update toolbar buttons
	//
	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// CFileOpenDlg::OnSelchangedFolderTree
//
//		The tree has changed its selection
//
void CFileOpenDlg::OnSelchangedFolderTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW*	pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM		hItem;
	
	hItem = pNMTreeView->itemNew.hItem;
	if(hItem)
	{
		m_strPath = GetFullPath(hItem);

		//
		// If displaying only the tree, do not enumerate files
		//
		if(m_nState != vsTreeOnly)
			EnumFiles(m_strPath, m_strFilter);
	}

	*pResult = 0;
}


// CFileOpenDlg::OnClickFileList
//
//		The user clicked the file list
//
void CFileOpenDlg::OnClickFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iItem;

	iItem = m_listFile.GetNextItem(-1, LVNI_SELECTED);
	if(iItem != -1)
	{
		m_strFile = m_listFile.GetItemText(iItem, 0);
	}

	//
	// Enable or disable the OK button
	//
	m_bOk = iItem != -1;
	UpdateControls();
	
	*pResult = 0;
}


// CFileOpenDlg::OnLButtonDown
//
//		Processes the button down message.

void CFileOpenDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nState == vsStandard)
	{
		SetCapture();

		m_bTracking = TRUE;
		m_yTrackDel = point.y - m_ySplit;
		m_yTrack	= m_ySplit;
	}

	Default();
}


// CFileOpenDlg::OnLButtonUp
//
//		The user stopped dragging
//
void CFileOpenDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_nState == vsStandard)
	{
		ReleaseCapture();

		m_bTracking = FALSE;

		InvalidateRect(NULL);
		UpdateWindow();
	}

	CExDialog::OnLButtonUp(nFlags, point);
}


// CFileOpenDlg::OnMouseMove
//
//		The user is dragging the stylus.
//
void CFileOpenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_nState == vsStandard && m_bTracking)
	{
		m_yTrack = point.y - m_yTrack;
		m_ySplit = point.y - m_yTrackDel;

		//
		// Position the controls
		//
		m_rcTree.bottom = m_ySplit;
		m_treeFolder.MoveWindow(&m_rcTree);

		m_rcList.top	= m_ySplit + nTrackHeight + 1;
		m_listFile.MoveWindow(&m_rcList);

		InvalidateRect(NULL);
		UpdateWindow();
	}

	CExDialog::OnMouseMove(nFlags, point);
}


// CFileOpenDlg::OnHeaderClickFileList
//
//		The header was clicked: sort the file list
//
void CFileOpenDlg::OnHeaderClickFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY*	phdn = (HD_NOTIFY *) pNMHDR;
	int			iItem;

	iItem = phdn->iItem + 1;

	if(m_nSort == iItem)
		m_nSort = -iItem;
	else
		m_nSort = iItem;

	SetColumnIcons();
	DoSortList();
	
	*pResult = 0;
}


// CFileOpenDlg::OnCustomDrawList
//
//		Draws the list with stripes
//
void CFileOpenDlg::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW*	pLVCD = (NMLVCUSTOMDRAW*) pNMHDR;
	DWORD			dwDrawStage;

	*pResult = CDRF_DODEFAULT;

	dwDrawStage = pLVCD->nmcd.dwDrawStage;

	if(dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if(dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		COLORREF	colBack;
		int			/*iItem,*/
					iSubItem;
		
		//
		// Get the list column pointer
		//
//		iItem		= pLVCD->nmcd.dwItemSpec;
		iSubItem	= pLVCD->iSubItem;

		if(iSubItem + 1 == abs(m_nSort))
			colBack = RGB(224, 224, 255);	// Light blue?
		else
			colBack = RGB(255, 255, 255);	// White

			pLVCD->clrTextBk = colBack;

		*pResult = CDRF_DODEFAULT;
	}
}


// CFileOpenDlg::OnItemExpandingFolderTree
//
//		The user is expanding a tree node
//
void CFileOpenDlg::OnItemExpandingFolderTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if(pNMTreeView->action & TVE_EXPAND)
	{
		TVITEM	item;

		item.mask	= TVIF_HANDLE | TVIF_PARAM;
		item.hItem	= pNMTreeView->itemNew.hItem;

		if(pNMTreeView->itemNew.lParam == -1)
		{
			CString	strFullPath = GetFullPath(item.hItem);
			RECT	rc;

//			if(strFullPath != _T("\\"))
//				strFullPath += _T("\\");

			if(EnumDirs(strFullPath, _T("*"), item.hItem))
				item.lParam	   = 1;
			else
				item.lParam	   = 0;
			m_treeFolder.SetItem(&item);

			m_treeFolder.GetItemRect(item.hItem, &rc, FALSE);
			m_treeFolder.InvalidateRect(&rc);
			m_treeFolder.UpdateWindow();
		}
	}

	*pResult = 0;
}


// CFileOpenDlg::OnGetDispInfoFolderTree
//
//		
void CFileOpenDlg::OnGetDispInfoFolderTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if(pTVDispInfo->item.mask & TVIF_CHILDREN)
	{
		if(pTVDispInfo->item.lParam)
			pTVDispInfo->item.cChildren	= 1;
		else
			pTVDispInfo->item.cChildren	= 0;
	}
	*pResult = 0;
}


//---------------------------------------------------------------------------
//
//	CFileOpenDlg command handlers
//
//---------------------------------------------------------------------------


// CFileOpenDlg::OnViewStandard
//
//		Enables the standard view
//
void CFileOpenDlg::OnViewStandard() 
{
	m_treeFolder.ShowWindow(SW_SHOW);
	m_treeFolder.MoveWindow(&m_rcTree);

	m_listFile.ShowWindow(SW_SHOW);
	m_listFile.MoveWindow(&m_rcList);

	if(m_nState == vsTreeOnly)
		EnumFiles(m_strPath, m_strFilter);
	m_nState = vsStandard;

	UpdateControls();
}


// CFileOpenDlg::OnViewTree
//
//		Enables the tree view only
//
void CFileOpenDlg::OnViewTree() 
{
	m_treeFolder.ShowWindow(SW_SHOW);
	m_treeFolder.MoveWindow(m_rcTree.left,
							m_rcTree.top,
							m_rcList.right - m_rcList.left,
							m_rcList.bottom - m_rcTree.top);

	m_listFile.ShowWindow(SW_HIDE);

	m_nState = vsTreeOnly;

	UpdateControls();
}


// CFileOpenDlg::OnViewList
//
//		Enables the list view only
//
void CFileOpenDlg::OnViewList() 
{
	m_treeFolder.ShowWindow(SW_HIDE);

	m_listFile.ShowWindow(SW_SHOW);
	m_listFile.MoveWindow(m_rcTree.left,
							m_rcTree.top,
							m_rcList.right - m_rcList.left,
							m_rcList.bottom - m_rcTree.top);

	if(m_nState == vsTreeOnly)
		EnumFiles(m_strPath, m_strFilter);
	m_nState = vsListOnly;

	UpdateControls();
}


// CFileOpenDlg::OnBarOk
//
//		Confirms the selection
//
void CFileOpenDlg::OnBarOk() 
{
	EndDialog(IDOK);
}


// CFileOpenDlg::OnBarCancel
//
//		Cancels the operation
//
void CFileOpenDlg::OnBarCancel() 
{
	EndDialog(IDCANCEL);
}


//---------------------------------------------------------------------------
//
//	Sorting functions
//
//---------------------------------------------------------------------------


static int SortNameAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return _wcsicmp((*pCont)[lParam1].cFileName, (*pCont)[lParam2].cFileName);
}


static int SortNameDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return _wcsicmp((*pCont)[lParam2].cFileName, (*pCont)[lParam1].cFileName);
}


static int SortSizeAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return (int)(*pCont)[lParam1].nFileSizeLow - 
		   (int)(*pCont)[lParam2].nFileSizeLow;
}


static int SortSizeDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return (int)(*pCont)[lParam2].nFileSizeLow - 
		   (int)(*pCont)[lParam1].nFileSizeLow;
}


static int SortDateAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return CompareFileTime(&(*pCont)[lParam1].ftLastWriteTime,  
						   &(*pCont)[lParam2].ftLastWriteTime);
}


static int SortDateDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CFileInfoCont*	pCont = (CFileInfoCont*)lParamSort;

	return CompareFileTime(&(*pCont)[lParam2].ftLastWriteTime,  
						   &(*pCont)[lParam1].ftLastWriteTime);
}
