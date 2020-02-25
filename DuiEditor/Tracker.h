#pragma once

//这个类是从MFC里面拷贝出来的, 不纠结
class CTracker
{
public:
	// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};
public:
	// Constructors
	CTracker();
	CTracker(LPCRECT lpSrcRect, UINT nStyle);
	virtual ~CTracker();

	// Attributes
	UINT m_nStyle;      // current state
	CRect m_rect;       // current position (always in pixels)
	CSize m_sizeMin;    // minimum X and Y size during track operation
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)

	// Operations
	virtual void Draw(CDC* pDC) const;
	virtual void GetTrueRect(LPRECT lpTrueRect) const;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	virtual BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = NULL);
	virtual BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	virtual int  HitTest(CPoint point) const;
	virtual int  NormalizeHit(int nHandle) const;

	// Overridables
	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;

protected:
	BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

	// implementation helpers
	virtual int HitTestHandles(CPoint point) const;
	virtual void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	virtual void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
	virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	virtual BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void Construct();

	//另外添加的函数
public:
	BOOL m_bFocus;			//是否焦点状态
	BOOL m_bMuiltiSelected; //是否多选状态
	BOOL m_bDrawHandle;
	BOOL m_bDrawMoveHandle;
	virtual BOOL SetCursor(CWnd* pWnd, UINT nHitTest, CPoint &point) const;
	virtual void DrawTrackerRect(LPCRECT lpRect, CDC* pDC); //画拖动的时候
};

