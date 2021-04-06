#pragma once
#include "Tracker.h"


// CImageEditorImagePreview 视图

//左边的原图显示窗口内的图片窗口
class CImageEditorImagePreview : public CScrollView
{
	DECLARE_DYNCREATE(CImageEditorImagePreview)

public:
	CImageEditorImagePreview();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageEditorImagePreview();

public:
	virtual void OnInitialUpdate();     // 构造后的第一次
	void InitData();
	void RecalcImageRect();
	void OnChangeRect();

	CRect m_rcImage;
	CTracker m_tracker;
	double m_zoom;

	CDuiRect m_rcSource;
protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


