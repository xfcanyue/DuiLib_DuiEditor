#pragma once



// CImageEditorView 视图

class CImageEditorView : public CScrollView
{
	DECLARE_DYNCREATE(CImageEditorView)

public:
	CImageEditorView();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageEditorView();

public:
	void SetImage(LPCTSTR strImageInfo, CRect &rcImg);

	CPaintManagerUI *m_pManager;
	TDrawInfo m_drawInfo;

	CRect m_rcImage;
	double m_zoom;
protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
};


