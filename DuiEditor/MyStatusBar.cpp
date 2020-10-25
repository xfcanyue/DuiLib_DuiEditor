// MyStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "MyStatusBar.h"

#include "MainFrm.h"
#include "UIManager.h"

const int nTextMargin = 4; // Gap between image and text
// CMyStatusBar

IMPLEMENT_DYNAMIC(CMyStatusBar, CMFCStatusBar)

CMyStatusBar::CMyStatusBar()
{
	m_bXmlUpdateSuccess = TRUE;
}

CMyStatusBar::~CMyStatusBar()
{
	::DeleteObject(m_hIconXmlOk);
	::DeleteObject(m_hIconXmlErr);
}


BEGIN_MESSAGE_MAP(CMyStatusBar, CMFCStatusBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CMyStatusBar::GetIndexByID(UINT nID)
{
	for (int i = 0; i < m_nCount; i++)
	{
		CMFCStatusBarPaneInfo *pPaneInfo = ((CMFCStatusBarPaneInfo*)m_pData) + i;
		if(pPaneInfo->nID == nID)
			return i;
	}

	return -1;
}

BOOL CMyStatusBar::SetPaneTextByID(UINT nID, LPCTSTR lpszNewText, BOOL bCalcPaneWidth)
{
	int nIndex = GetIndexByID(nID);
	if(nIndex < 0)	return FALSE;

	SetPaneText(nIndex, lpszNewText);

	if(bCalcPaneWidth)
	{
		CDC *pDC = GetDC();

		CRect rc;
		pDC->DrawText(lpszNewText, rc, DT_CALCRECT);

		int nNewWith = rc.Width() + 10;
		if(nNewWith < 80) nNewWith = 80;
		SetPaneWidth(nIndex, nNewWith);

		ReleaseDC(pDC);
	}	
	return TRUE;
}

BOOL CMyStatusBar::SetPaneWidthByID(UINT nID, int cx)
{
	int nIndex = GetIndexByID(nID);
	if(nIndex < 0)	return FALSE;

	SetPaneWidth(nIndex, cx);
	return TRUE;
}

// CMyStatusBar 消息处理程序
void CMyStatusBar::OnDrawPane(CDC* pDC, CMFCStatusBarPaneInfo* pPane)
{
	ASSERT_VALID(pDC);
	ENSURE(pPane != NULL);

	CRect rectPane = pPane->rect;
	if (rectPane.IsRectEmpty() || !pDC->RectVisible(rectPane))
	{
		return;
	}

	// Fill pane background:
	if (pPane->clrBackground != (COLORREF)-1)
	{
		CBrush brush(pPane->clrBackground);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->PatBlt(rectPane.left, rectPane.top, rectPane.Width(), rectPane.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);
	}

	// Draw pane border:
	CMFCVisualManager::GetInstance()->OnDrawStatusBarPaneBorder(pDC, this, rectPane, pPane->nID, pPane->nStyle);

	if (!(pPane->nStyle & SBPS_NOBORDERS)) // only adjust if there are borders
	{
		rectPane.DeflateRect(2 * AFX_CX_BORDER, AFX_CY_BORDER);
	}

	//这个icon, 我们自己来画
	if(pPane->nID == ID_INDICATOR_XML_STATUS)
	{
		return OnDrawXmlUpdateStatus(pDC, pPane);
	}

	// Draw icon
	if (pPane->hImage != NULL && pPane->cxIcon > 0)
	{
		CRect rectIcon = rectPane;
		rectIcon.right = rectIcon.left + pPane->cxIcon;

		int x = max(0, (rectIcon.Width() - pPane->cxIcon) / 2);
		int y = max(0, (rectIcon.Height() - pPane->cyIcon) / 2);

		::ImageList_DrawEx(pPane->hImage, pPane->nCurrFrame, pDC->GetSafeHdc(), rectIcon.left + x, rectIcon.top + y, pPane->cxIcon, pPane->cyIcon, CLR_NONE, 0, ILD_NORMAL);
	}

	CRect rectText = rectPane;
	rectText.left += pPane->cxIcon;

	if (pPane->cxIcon > 0)
	{
		rectText.left += nTextMargin;
	}

	if (pPane->nProgressTotal > 0)
	{
		// Draw progress bar:
		CRect rectProgress = rectText;
		rectProgress.DeflateRect(1, 1);

		COLORREF clrBar = (pPane->clrProgressBar == (COLORREF)-1) ? afxGlobalData.clrHilite : pPane->clrProgressBar;

		CMFCVisualManager::GetInstance()->OnDrawStatusBarProgress(pDC, this, rectProgress,
			pPane->nProgressTotal, pPane->nProgressCurr, clrBar, pPane->clrProgressBarDest, pPane->clrProgressText, pPane->bProgressText);
	}
	else
	{
		// Draw text
		if (pPane->lpszText != NULL && pPane->cxText > 0)
		{
			COLORREF clrText = pDC->SetTextColor(CMFCVisualManager::GetInstance()->GetStatusBarPaneTextColor(this, pPane));

			if(pPane->nID > 0) //改了这里，居中显示。
				pDC->DrawText(pPane->lpszText, lstrlen(pPane->lpszText), rectText, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			else
				pDC->DrawText(pPane->lpszText, lstrlen(pPane->lpszText), rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			pDC->SetTextColor(clrText);
		}
	}
}

void CMyStatusBar::OnDrawXmlUpdateStatus(CDC* pDC, CMFCStatusBarPaneInfo* pPane)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CUIManager *pManager = pMain->GetActiveUIManager();
	if(!pManager) return;
	xml_parse_result *pret = pManager->GetCodeView()->GetParseResult();

	CRect rect = pPane->rect;
	rect.OffsetRect(2, 2);
	if(pret->status == pugi::status_ok)
		DrawIconEx(pDC->GetSafeHdc(),rect.left,rect.top,m_hIconXmlOk,16,16,0,NULL,DI_NORMAL);
	else
		DrawIconEx(pDC->GetSafeHdc(),rect.left,rect.top,m_hIconXmlErr,16,16,0,NULL,DI_NORMAL);

	//InsertMsg(_T("OnDrawXmlUpdateStatus"));
}

INT_PTR CMyStatusBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);

	// check child windows first by calling CPane
	INT_PTR nHit = (INT_PTR) CPane::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CMFCStatusBarPaneInfo* pSBP = HitTest(point);
	if(pSBP == NULL || pSBP->nID != ID_INDICATOR_XML_STATUS)
		return __super::OnToolHitTest(point, pTI);
	
	nHit = pSBP->nID;
	CString strTipText;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CUIManager *pManager = pMain->GetActiveUIManager();
	if(!pManager) return nHit;
	xml_parse_result *pret = pManager->GetCodeView()->GetParseResult();

	int row = pManager->GetCodeView()->GetSciWnd()->sci_LineFromPosition(pret->offset);
	int col = pret->offset - pManager->GetCodeView()->GetSciWnd()->sci_PositionFromLine(row);
	if(pret->status != pugi::status_ok)
		strTipText.Format(_T("行: %d, 列: %d, 位置: %d, 错误信息: %s"), row+1, col+1, pret->offset, LSA2T(pret->description()));
	else
		strTipText = LSA2T(pret->description());

	if (pTI != NULL)
	{
		pTI->lpszText = (LPTSTR) ::calloc((strTipText.GetLength() + 1), sizeof(TCHAR));
		lstrcpy(pTI->lpszText, strTipText);

		pTI->rect = pSBP->rect;
		pTI->uId = 0;
		pTI->hwnd = m_hWnd;
	}	

	CToolTipCtrl* pToolTip = AfxGetModuleState()->m_thread.GetDataNA()->m_pToolTip;
	if (pToolTip != NULL && pToolTip->GetSafeHwnd() != NULL)
	{
		pToolTip->SetFont(&afxGlobalData.fontTooltip, FALSE);
	}

	return nHit;
}

int CMyStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//这么载入的话，icon的size居然变成了 32*32 ????
// 	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
// 	m_hIconXmlOk = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_XML_OK));
// 	m_hIconXmlErr = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_XML_ERR));

	m_hIconXmlOk = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_XML_OK), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_hIconXmlErr = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_XML_ERR), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

	return 0;
}

void CMyStatusBar::SetXmlUpdateStatus(BOOL bUpdateSuccess)
{
	if(m_bXmlUpdateSuccess == bUpdateSuccess) return;

	int nIndex = GetIndexByID(ID_INDICATOR_XML_STATUS);
	if(nIndex < 0)	return;

	m_bXmlUpdateSuccess = bUpdateSuccess;
	InvalidatePaneContent(nIndex);
}