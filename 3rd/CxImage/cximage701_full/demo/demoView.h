// demoView.h : interface of the CDemoView class
//
/////////////////////////////////////////////////////////////////////////////
#define CXIMAGE_DEMO_SELECT 1

// <VATI> double precision coo type
typedef struct {
    double x, y;
} Dpoint2d;

class CDemoView : public CScrollView
{
protected: // create from serialization only
	CDemoView();
	DECLARE_DYNCREATE(CDemoView)

// Attributes
public:
	CDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetImageXY(CDemoDoc *pDoc, CxImage *ima, long *x, long *y);
	bool SetImageRectSelection(CDemoDoc *pDoc,CRect *rect);
	virtual ~CDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#if CXIMAGE_DEMO_SELECT
	long m_SelCount;
	long m_SelShift;
	void DrawSelection();
	CPoint m_RefPoint,m_RefScroll;
#endif
protected:
	CBrush m_brHatch;

	CRectTracker m_tracker;

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnNewImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewProgress(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
#ifdef VATI_EXTENSIONS
private:
	bool     m_suppressContextMenu;
	Dpoint2d m_oldPnt;
	Dpoint2d m_orgPnt;
	Dpoint2d m_dpnt[6];
#endif
};

#ifndef _DEBUG  // debug version in demoView.cpp
inline CDemoDoc* CDemoView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
