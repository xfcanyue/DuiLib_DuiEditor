// ImageEditorView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorView.h"

#include "ImageEditor.h"
#include "UITracker.h"
// CImageEditorView

IMPLEMENT_DYNCREATE(CImageEditorView, CScrollView)

CImageEditorView::CImageEditorView()
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_pManager = NULL;
	m_zoom = 1;
}

CImageEditorView::~CImageEditorView()
{
}


BEGIN_MESSAGE_MAP(CImageEditorView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CImageEditorView 绘图

void CImageEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageEditorView::OnDraw(CDC* pDC)
{
	CMemDCEx memDC(pDC);

	CRect rcClient;
	this->GetClientRect(&rcClient);
	memDC->FillRect(rcClient, &afxGlobalData.brBlack);

	if(!m_pManager)
		return;

	CRect rcImage = m_rcImage;
	rcImage.OffsetRect(rcClient.CenterPoint() - m_rcImage.CenterPoint());

	CPoint pt=rcImage.CenterPoint();
	int width = int(rcImage.Width() * m_zoom);
	int height = int(rcImage.Height() * m_zoom);
	rcImage.SetRect(pt.x-width/2, pt.y-height/2, pt.x+width/2, pt.y+height/2);

	CBrush* pOldBrush=memDC->SelectObject(&afxGlobalData.brBlack);
	memDC->Rectangle(&rcClient);
	pOldBrush=memDC->SelectObject((CBrush*)memDC->SelectStockObject(NULL_BRUSH));
	CRect rcBorder=rcImage;
	rcBorder.DeflateRect(-1,-1);
	memDC->Rectangle(rcBorder);
	memDC->SelectObject(pOldBrush);

#ifndef DUILIB_VERSION_ORIGINAL
	CRenderEngine::DrawImageInfo(memDC->m_hDC, m_pManager, rcImage, rcClient, &m_drawInfo);
#else
	CRenderEngine::DrawImage(memDC->m_hDC, m_pManager, rcImage, rcClient, m_drawInfo);
#endif
}

// CImageEditorView 消息处理程序
BOOL CImageEditorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

void CImageEditorView::SetImage(LPCTSTR strImageInfo, CRect &rcImg)
{
	m_drawInfo.Clear();
#ifdef DUILIB_VERSION_ORIGINAL
	m_drawInfo.sDrawString = strImageInfo;
	m_drawInfo.bLoaded = false;
#else
	m_drawInfo.Parse(strImageInfo, NULL, m_pManager);
#endif

	m_rcImage = rcImg;
	m_zoom = 1;

	Invalidate();
}

BOOL CImageEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(zDelta < 0)
	{
		m_zoom -= 0.2f;
		if(m_zoom <= 0.2f)
			m_zoom = 0.2f;
	}
	else
	{
		m_zoom += 0.2f;
	}
	Invalidate();
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CImageEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CUITracker tracker;
	int htTest = tracker.HitTest(point);
	tracker.TrackRubberBand(this, point, TRUE);

	CRect rcClient;
	this->GetClientRect(&rcClient);
	CRect rc = tracker.m_rect;

	if(rc.IsRectEmpty())
		return;

	CRect rcImage = m_rcImage;
	rcImage.OffsetRect(rcClient.CenterPoint() - m_rcImage.CenterPoint());

	CRect rc2;
	rc2.IntersectRect(rc, rcImage);
	rc2.OffsetRect(m_rcImage.CenterPoint() - rcClient.CenterPoint());

	CImageEditorFrame *pFrame = (CImageEditorFrame *)GetParent();
	pFrame->OnSetSourcePos(rc2);

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageEditorView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CScrollView::PostNcDestroy();
}
