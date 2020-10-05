// DefaultEditorView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DefaultEditorView.h"

#include "DefaultEditor.h"
#include "UIBuilder.h"
// CDefaultEditorView

IMPLEMENT_DYNAMIC(CDefaultEditorView, CWnd)

CDefaultEditorView::CDefaultEditorView()
{
	m_pControl = NULL;
	m_bUiEffect = FALSE;
}

CDefaultEditorView::~CDefaultEditorView()
{
}


BEGIN_MESSAGE_MAP(CDefaultEditorView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CDefaultEditorView 消息处理程序




int CDefaultEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_tracker.m_nHandleSize = 6;
	m_tracker.m_nStyle = CUITracker::dottedLine|CUITracker::resizeInside;

	CRect rcClient;
	GetClientRect(rcClient);
	CString strRc;
	strRc.Format(_T("%d,%d"), rcClient.Width(), rcClient.Height());

	xml_document doc;
	xml_node nodeWindow = doc.root().append_child(XTEXT("Window"));
	nodeWindow.attribute_auto(XTEXT("size")).set_value(T2XML(strRc));
	xml_node nodeVert = nodeWindow.append_child(XTEXT("VerticalLayout"));
	nodeVert.attribute_auto(XTEXT("bkcolor")).set_value(XTEXT("#FF696969"));

	GetManager()->Init(m_hWnd);

	CUIBuilder builder;
	CControlUI* pRoot=NULL;
	pRoot = builder.Create(doc, NULL, GetManager());
	if (pRoot==NULL)
	{
		MessageBox(_T("Loading resource files error."),_T("Duilib"),MB_OK|MB_ICONERROR);
		return 0;
	}
	GetManager()->AttachDialog(pRoot);

	return 0;
}


BOOL CDefaultEditorView::CreateControl(LPCTSTR pstrClassName)
{
	CVerticalLayoutUI *pRoot = (CVerticalLayoutUI *)GetManager()->GetRoot();

	CRect rcControl(0,0,150,150);
	if(CompareString(pstrClassName, _T("VScrollBar")))
	{
		m_pControl = CUIBuilder::CreateControl(_T("ScrollBar"));

		rcControl.SetRect(0, 0, 10, 200);
	}
	else if(CompareString(pstrClassName, _T("HScrollBar")))
	{
		m_pControl = CUIBuilder::CreateControl(_T("ScrollBar"));

		rcControl.SetRect(0, 0, 200, 10);
		((CScrollBarUI *)m_pControl)->SetHorizontal(true);
	}
	else			
	{
		m_pControl = CUIBuilder::CreateControl(pstrClassName);
	}

	if(!m_pControl)	return FALSE;

	m_pControl->SetFloat(true);

	CRect rcClient;
	GetClientRect(rcClient);
	CPoint ptCenter = rcClient.CenterPoint();		
	rcControl.SetRect(ptCenter.x - rcControl.Width()/2,
		ptCenter.y - rcControl.Height()/2,
		ptCenter.x + rcControl.Width()/2,
		ptCenter.y + rcControl.Height()/2);


	pRoot->Add(m_pControl);
	m_pControl->SetPos(rcControl);
	pRoot->NeedUpdate();

	SetControlAttribute();

	m_rcControl = rcControl;
	return TRUE;
}

void CDefaultEditorView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(m_pControl == NULL)
		return;

	CRect rcClient, rcControl;
	GetClientRect(rcClient);

	CPoint ptCenter = rcClient.CenterPoint();		
	rcControl.SetRect(ptCenter.x - m_rcControl.Width()/2,
		ptCenter.y - m_rcControl.Height()/2,
		ptCenter.x + m_rcControl.Width()/2,
		ptCenter.y + m_rcControl.Height()/2);

// 	m_pControl->SetPos(rcControl);
// 	m_pControl->NeedParentUpdate();
 
	m_pControl->SetFixedXY(CSize(rcControl.left, rcControl.top));
	m_pControl->SetFixedWidth(rcControl.Width());
	m_pControl->SetFixedHeight(rcControl.Height());
}

void CDefaultEditorView::SetControlAttribute()
{
	CDefaultEditor *pDlgMain = (CDefaultEditor *)GetParent()->GetParent();
	if(m_pControl)
	{
		xml_node root = pDlgMain->m_nodeControl.root().first_child(); //.child(pDlgMain->m_strClassName);
		for (xml_attribute attr=root.first_attribute(); attr; attr=attr.next_attribute())
		{
			m_pControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));
		}
	}
}

LRESULT CDefaultEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if(m_bUiEffect)
	{
		GetManager()->MessageHandler(message, wParam, lParam, lRes);
	}
	else
	{
		//不显示UI效果，仅仅绘图就可以
		if(message == WM_PAINT)
		{
			GetManager()->MessageHandler(message, wParam, lParam, lRes);
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}


void CDefaultEditorView::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(m_bUiEffect)
	{
		return;
	}
	int nHit = m_tracker.HitTest(pt);
	if(nHit > 0 )
	{
		m_tracker.Track(this, pt, TRUE, NULL);

		if(m_pControl)
		{
			m_pControl->SetAttribute(_T("pos"), RectToString(m_tracker.m_rect));
			m_pControl->SetPos(m_tracker.m_rect);
		}
	}
	else
	{
		m_tracker.m_rect.SetRectEmpty();

		CControlUI *pFindControl = GetManager()->FindControl(pt);
		if(pFindControl == m_pControl)
			m_tracker.m_rect = m_pControl->GetPos();
	}

	Invalidate();

	CWnd::OnLButtonDown(nFlags, pt);
}


BOOL CDefaultEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_tracker.SetCursor(pWnd, nHitTest))
		return TRUE;

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CDefaultEditorView::OnPaint()
{
	if(!m_bUiEffect)
	{
		CDC *pDC = GetDC();
		m_tracker.Draw(pDC);
		ReleaseDC(pDC);
	}
}
