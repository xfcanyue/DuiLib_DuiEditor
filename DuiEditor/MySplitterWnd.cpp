// MySplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "MySplitterWnd.h"
#include "UIManager.h"

// HitTest return values (values and spacing between values is important)
enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};\
// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWndEx)

CMySplitterWnd::CMySplitterWnd()
{
	m_cxBorder = m_cyBorder = 0;

	m_nMode = SPLIT_UPDOWN;

	m_pDesignView = NULL;
	m_pDesignCode = NULL;
	m_nLeftWidth = 0;
}

CMySplitterWnd::~CMySplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWndEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMySplitterWnd 消息处理程序
void CMySplitterWnd::SetViewMode(int nMode)
{
	if(m_nMode != nMode)
	{
		m_nMode = nMode;
		RecalcLayout();
	}
}

BOOL CMySplitterWnd::CreateView(int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	if (GetDlgItem(IdFromRowCol(row, col)) != NULL)
	{
		TRACE(traceAppMsg, 0, "Error: CreateView - pane already exists for row %d, col %d.\n",
			row, col);
		ASSERT(FALSE);
		return FALSE;
	}
#endif

	// set the initial size for that pane
	m_pColInfo[col].nIdealSize = sizeInit.cx;
	m_pRowInfo[row].nIdealSize = sizeInit.cy;

	BOOL bSendInitialUpdate = FALSE;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = (CView*)GetActivePane();
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(contextT.m_pCurrentFrame == NULL);
			contextT.m_pLastView = pOldView;
			contextT.m_pCurrentDoc = pOldView->GetDocument();
			if (contextT.m_pCurrentDoc != NULL)
				contextT.m_pNewDocTemplate =
				contextT.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &contextT;
		bSendInitialUpdate = TRUE;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE(traceAppMsg, 0, "Out of memory creating a splitter pane.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW & ~WS_BORDER;

	// Create with the right size (wrong position)
	CRect rect(CPoint(0,0), sizeInit);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, this, IdFromRowCol(row, col), pContext))
	{
		TRACE(traceAppMsg, 0, "Warning: couldn't create client pane for splitter.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	ASSERT((int)::GetDlgCtrlID(pWnd->m_hWnd) == IdFromRowCol(row, col));

	// send initial notification message
	if (bSendInitialUpdate)
		pWnd->SendMessage(WM_INITIALUPDATE);

	if(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CDuiEditorViewDesign)))
	{
		m_pDesignView = (CDuiEditorViewDesign *)pWnd;
	}
	else if(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CDuiEditorViewCode)))
	{
		m_pDesignCode = (CDuiEditorViewCode *)pWnd;
	}
	return TRUE;
}

void CMySplitterWnd::RecalcLayout() 
{
	__super::RecalcLayout();

	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);

	if(m_nMode == SPLIT_DESIGN)
	{
		m_pDesignView->MoveWindow(rectClient);
		m_pDesignCode->MoveWindow(0,0,0,0);
	}
	else if(m_nMode == SPLIT_CODE)
	{
		m_pDesignView->MoveWindow(0,0,0,0);
		m_pDesignCode->MoveWindow(rectClient);
	}
	else if(m_nMode == SPLIT_LEFTRIGHT)
	{
		if(m_nLeftWidth == 0)
			m_nLeftWidth = (rectClient.Width() - m_cxSplitterGap)/2;

		m_pDesignView->MoveWindow(0,0,m_nLeftWidth, rectClient.Height());
		m_pDesignCode->MoveWindow(m_nLeftWidth + m_cxSplitterGap, 0, rectClient.Width()-m_nLeftWidth - m_cxSplitterGap, rectClient.Height());
	}
}

void CMySplitterWnd::DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside)
{
	if(m_nMode == SPLIT_DESIGN || m_nMode == SPLIT_CODE)
		return;

	if(m_nMode == SPLIT_UPDOWN)
	{
		return __super::DrawAllSplitBars(pDC, cxInside, cyInside);
	}

	// draw column split bars
	CRect rect;
	GetClientRect(rect);
	{
		rect.left += m_nLeftWidth;
		rect.right = rect.left + m_cxSplitter;
		OnDrawSplitter(pDC, splitBar, rect);
	}
}

int CMySplitterWnd::HitTest(CPoint pt) const
{
	ASSERT_VALID(this);

	if(m_nMode == SPLIT_DESIGN || m_nMode == SPLIT_CODE)
		return noHit;

	if(m_nMode == SPLIT_UPDOWN)
	{
		return __super::HitTest(pt);
	}

	CRect rect;
	GetClientRect(rect);
	rect.left += m_nLeftWidth;
	rect.right = rect.left + m_cxSplitter;

	if(rect.PtInRect(pt))
	{
		return hSplitterBar1;
	}

	return noHit;
}

void CMySplitterWnd::TrackColumnSize(int x, int col)
{
	if(m_nMode == SPLIT_DESIGN || m_nMode == SPLIT_CODE)
		return;

	if(m_nMode == SPLIT_UPDOWN)
	{
		return __super::TrackColumnSize(x, col);
	}

	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_hWnd, &pt);

// 	CPoint pt(x, 0);
// 	ClientToScreen(&pt);
// 	m_pDesignView->ScreenToClient(&pt);
	m_nLeftWidth = pt.x;      // new size

	CString temp;
	temp.Format(_T("LeftWidth=%d"), m_nLeftWidth);
	InsertMsg(temp);
}