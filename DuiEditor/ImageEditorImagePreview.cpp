// ImageEditorImagePreview.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorImagePreview.h"

#include "UIManager.h"
#include "ImageEditor.h"
#include "ImageEditorFrame.h"
#include "ImageEditorPaneImage.h"
// CImageEditorImagePreview

IMPLEMENT_DYNCREATE(CImageEditorImagePreview, CScrollView)

CImageEditorImagePreview::CImageEditorImagePreview()
{
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	m_tracker.m_nStyle |= CRectTracker::resizeOutside;
}

CImageEditorImagePreview::~CImageEditorImagePreview()
{
}


BEGIN_MESSAGE_MAP(CImageEditorImagePreview, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CImageEditorImagePreview 绘图
int CImageEditorImagePreview::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CImageEditorImagePreview::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 计算此视图的合计大小

	InitData();
}

void CImageEditorImagePreview::InitData()
{
	CSize sizeTotal;
	sizeTotal.cx = g_pEditorImage->m_image.GetWidth() + 20;
	sizeTotal.cy = g_pEditorImage->m_image.GetHeight() + 20;
	SetScrollSizes(MM_TEXT, sizeTotal);

	RecalcImageRect();

	Invalidate(TRUE);

	CString strCaption;
	strCaption.Format(_T("原始图片  %d * %d"), g_pEditorImage->m_image.GetWidth(), g_pEditorImage->m_image.GetHeight());
	CImageEditorPaneImage *pPane = (CImageEditorPaneImage *)GetParent();
	pPane->SetCaptionText(strCaption);
}

void CImageEditorImagePreview::RecalcImageRect()
{
	CRect rcControl = g_pEditorImage->m_rcImage;

	CRect rcClient;
	GetClientRect(&rcClient);
	CPoint pt = rcClient.CenterPoint();
	CRect rcTemp = rcControl;
	if(rcClient.Width() > rcControl.Width())
	{
		rcControl.left = pt.x - rcTemp.Width()/2;
		rcControl.right = pt.x + rcTemp.Width()/2;
	}
	if(rcClient.Height() > rcControl.Height())
	{
		rcControl.top = pt.y - rcTemp.Height()/2;
		rcControl.bottom = pt.y + rcTemp.Height()/2;
	}
	m_rcImage = rcControl;

	xml_attribute attrDest = g_pEditorImage->m_nodedata.attribute(XTEXT("source"));
	if(attrDest)
	{
		CDuiRect rc(XML2T(attrDest.as_string(XTEXT("0,0,0,0"))));
		m_tracker.m_rect = rc;
		m_tracker.m_rect.OffsetRect(m_rcImage.left, m_rcImage.top);
		CPoint pt = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-pt.x, -pt.y);
	}
	else
	{
		m_tracker.m_rect = m_rcImage;
		m_tracker.m_rect.OffsetRect(m_rcImage.left, m_rcImage.top);
		CPoint pt = GetScrollPosition();
		m_tracker.m_rect.OffsetRect(-pt.x, -pt.y);
	}
}

void CImageEditorImagePreview::OnChangeRect()
{
	CRect rc = m_tracker.m_rect;
	rc.OffsetRect(-m_rcImage.left, -m_rcImage.top);
	rc.OffsetRect(GetScrollPosition());

	g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, _T("source"), RectToString(rc), NULL);

	g_pEditorImage->m_pFrame->m_wndImage.m_pView->InitData();
	g_pEditorImage->m_pFrame->m_wndView.InitData();
	g_pEditorImage->m_pFrame->m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	g_pEditorImage->m_pFrame->m_wndProperty.OnExpandAllProperties();
	g_pEditorImage->m_pFrame->m_wndPaneAdjust.m_pForm->InitData();

	g_pEditorImage->m_pFrame->ParentPreview();

	Invalidate();
}

void CImageEditorImagePreview::OnDraw(CDC* pDC)
{
	CMemDCEx memDC(pDC);

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcArea = rcClient;
	rcArea.OffsetRect(GetScrollPosition());
	memDC->FillRect(rcArea, &afxGlobalData.brBlack);

	memDC->FillSolidRect(m_rcImage, g_cfg.crDesignerBkColor);
	g_pEditorImage->m_image.Draw(memDC->m_hDC, m_rcImage);

	m_tracker.Draw(&memDC);
}

BOOL CImageEditorImagePreview::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}


// CImageEditorImagePreview 消息处理程序


void CImageEditorImagePreview::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();

	BOOL bNewRect = FALSE;

	int htTest = m_tracker.HitTest(point);
	if(htTest > 0 )
	{
		if(m_tracker.Track(this, point, TRUE))
		{
			m_tracker.m_rect.NormalizeRect();
			switch (htTest)
			{
			case CRectTracker::hitTopLeft:
				//break;
			case CRectTracker::hitTopRight:
				//break;
			case CRectTracker::hitBottomRight:
				//break; 
			case CRectTracker::hitBottomLeft:
				//break;
			case CRectTracker::hitTop:
				//break;
			case CRectTracker::hitRight:
				//break; 
			case CRectTracker::hitBottom:
				//break;
			case CRectTracker::hitLeft:
				//break; 
			case CRectTracker::hitMiddle: //移动
				bNewRect = TRUE;
				break;
			}		
		}
	}
	else
	{
		CRectTracker tracker;
		if(tracker.TrackRubberBand(this, point))
		{
			tracker.m_rect.NormalizeRect();
			CRect rc;
			rc.IntersectRect(tracker.m_rect, m_rcImage);

			m_tracker.m_rect = rc;
			bNewRect = TRUE;
		}
	}

	if(bNewRect)
	{
		OnChangeRect();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CImageEditorImagePreview::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_tracker.SetCursor( pWnd, nHitTest )) 
		return TRUE;
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageEditorImagePreview::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case VK_UP:
		m_tracker.m_rect.OffsetRect(0,-1);
		OnChangeRect();
		break;
	case VK_DOWN:
		m_tracker.m_rect.OffsetRect(0,1);
		OnChangeRect();
		break;
	case VK_LEFT:
		m_tracker.m_rect.OffsetRect(-1,0);
		OnChangeRect();
		break;
	case VK_RIGHT:
		m_tracker.m_rect.OffsetRect(1,0);
		OnChangeRect();
		break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CImageEditorImagePreview::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(GetSafeHwnd() == NULL) return;
	RecalcImageRect();
}


BOOL CImageEditorImagePreview::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: 在此添加专用代码和/或调用基类
	RecalcImageRect();
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}
