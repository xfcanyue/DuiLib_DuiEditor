// DuiEditorTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DuiEditorTabView.h"

#include "DuiEditorViewDesign.h"
#include "DuiEditorViewCode.h"
// CDuiEditorTabView

IMPLEMENT_DYNCREATE(CDuiEditorTabView, CView)

CDuiEditorTabView::CDuiEditorTabView()
{
	m_bIsReady = FALSE;
	m_nFirstActiveTab = -1;

	m_nLastActiveTab = -1;

	m_pFormDesign = NULL;
	m_pFormCode = NULL;
}

CDuiEditorTabView::~CDuiEditorTabView()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorTabView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CDuiEditorTabView::OnChangedActiveTab)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGING_ACTIVE_TAB, &CDuiEditorTabView::OnChangingActiveTab)
END_MESSAGE_MAP()


// CDuiEditorTabView 消息处理程序
void CDuiEditorTabView::OnDraw(CDC* /*pDC*/)
{
}

int CDuiEditorTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(IsScrollBar () ? CMFCTabCtrl::STYLE_FLAT_SHARED_HORZ_SCROLL : CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create tab window\n");
		return -1;      // fail to create
	}

	m_wndTabs.SetFlatFrame();
	m_wndTabs.SetTabBorderSize(0);
	m_wndTabs.AutoDestroyWindow(FALSE);

	AddView(RUNTIME_CLASS(CDuiEditorViewDesign), _T("0"));
	AddView(RUNTIME_CLASS(CDuiEditorViewCode), _T("1"));

	m_pFormDesign = (CDuiEditorViewDesign *)m_wndTabs.GetTabWnd(0);
	m_pFormCode = (CDuiEditorViewCode *)m_wndTabs.GetTabWnd(1);

	return 0;
}


void CDuiEditorTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTabs.SetWindowPos(NULL, -1, -1, cx + 1, cy + 3, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CDuiEditorTabView::AddView(CRuntimeClass* pViewClass, const CString& strViewLabel, int iIndex /*= -1*/, CCreateContext* pContext/* = NULL*/)
{
	ASSERT_VALID(this);
	ENSURE(pViewClass != NULL);
	ENSURE(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CView)));

	CView* pView = DYNAMIC_DOWNCAST(CView, pViewClass->CreateObject());
	ASSERT_VALID(pView);

	if (!pView->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabs, (UINT) -1, pContext))
	{
		TRACE1("CTabView:Failed to create view '%s'\n", pViewClass->m_lpszClassName);
		return -1;
	}

	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		ASSERT_VALID(pDoc);

		BOOL bFound = FALSE;
		for (POSITION pos = pDoc->GetFirstViewPosition(); !bFound && pos != NULL;)
		{
			if (pDoc->GetNextView(pos) == pView)
			{
				bFound = TRUE;
			}
		}

		if (!bFound)
		{
			pDoc->AddView(pView);
		}
	}

	m_wndTabs.InsertTab(pView, strViewLabel, iIndex);

	int nTabs = m_wndTabs.GetTabsNum();
	return nTabs - 1;
}

LRESULT CDuiEditorTabView::OnChangedActiveTab(WPARAM wp, LPARAM)
{
	if (!m_bIsReady)
	{
		m_nFirstActiveTab = (int) wp;
		return 0;
	}

	CFrameWnd* pFrame = AFXGetParentFrame(this);
	ASSERT_VALID(pFrame);

	int iTabNum = (int) wp;
	if (iTabNum >= 0)
	{
		CView* pView = DYNAMIC_DOWNCAST(CView, m_wndTabs.GetTabWnd(iTabNum));
		ASSERT_VALID(pView);

		pFrame->SetActiveView(pView);

		OnActivateView(pView);
	}
	else
	{
		pFrame->SetActiveView(NULL);

		OnActivateView(NULL);
	}

	return 0;
}

int CDuiEditorTabView::FindTab(HWND hWndView) const
{
	ASSERT_VALID(this);

	for (int i = 0; i < m_wndTabs.GetTabsNum(); i++)
	{
		if (m_wndTabs.GetTabWnd(i)->GetSafeHwnd() == hWndView)
		{
			return i;
		}
	}

	return -1;
}

BOOL CDuiEditorTabView::RemoveView(int iTabNum)
{
	ASSERT_VALID(this);
	return m_wndTabs.RemoveTab(iTabNum);
}

BOOL CDuiEditorTabView::SetActiveView(int iTabNum)
{
	ASSERT_VALID(this);
	
	return m_wndTabs.SetActiveTab(iTabNum);
}

CView* CDuiEditorTabView::GetActiveView() const
{
	ASSERT_VALID(this);

	int iActiveTab = m_wndTabs.GetActiveTab();
	if (iActiveTab < 0)
	{
		return NULL;
	}

	return DYNAMIC_DOWNCAST(CView, m_wndTabs.GetTabWnd(iActiveTab));
}

int CDuiEditorTabView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CView* pCurrView = GetActiveView();
	if (pCurrView == NULL)
	{
		return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}

	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // frame does not want to activate

	CFrameWnd* pParentFrame = AFXGetParentFrame(this);
	if (pParentFrame != NULL)
	{
		// eat it if this will cause activation
		ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));

		// either re-activate the current view, or set this view to be active
		CView* pView = pParentFrame->GetActiveView();
		HWND hWndFocus = ::GetFocus();
		if (pView == pCurrView && pCurrView->m_hWnd != hWndFocus && !::IsChild(pCurrView->m_hWnd, hWndFocus))
		{
			// re-activate this view
			((CInternalTabView*)pCurrView)->OnActivateView(TRUE, pCurrView, pCurrView);
		}
		else
		{
			// activate this view
			pParentFrame->SetActiveView(pCurrView);
		}
	}

	return nResult;
}

void CDuiEditorTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_bIsReady = TRUE;
	OnChangedActiveTab(m_nFirstActiveTab, 0);
}

void CDuiEditorTabView::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	//InsertMsg(_T("CDuiEditorTabView::OnActivateFrame"));
}

void CDuiEditorTabView::OnActivateView(CView* pView)
{
	//InsertMsg(_T("CDuiEditorTabView::OnActivateView"));
	if(pView == m_pFormCode)
	{
		//InsertMsg(_T("active view code"));
		m_nLastActiveTab = 1;
		return;
	}

	if(pView == m_pFormDesign)
	{
		//InsertMsg(_T("active view design"));
		m_nLastActiveTab = 0;
	}

	//InsertMsg(_T("CDuiEditorTabView::OnActivateView(CView* pView)"));
}


LRESULT CDuiEditorTabView::OnChangingActiveTab(WPARAM wp, LPARAM)
{

	if (!m_bIsReady)
	{
		m_nFirstActiveTab = (int) wp;
		return FALSE;
	}

	int iTabNum = (int) wp;

	if(iTabNum == 1)
	{
		m_pFormCode->Init();
		return FALSE;
	}

	if(iTabNum == 0 && m_nLastActiveTab == 1)
	{
		if(m_pFormCode->IsModify())
		{
			if(!m_pFormCode->ApplyDocument())
			{
				return TRUE;	//不允许切换标签
			}
		}
	}

	return FALSE; //允许切换标签
}