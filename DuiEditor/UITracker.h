#pragma once
#include "Tracker.h"

//////////////////////////////////////////////////////////////////////////
class CUITracker : public CTracker
{
public:
	// Constructors
	CUITracker();
	CUITracker(LPCRECT lpSrcRect, UINT nStyle);
	~CUITracker();

	virtual void Draw(CDC* pDC) const;
	void GetMoveHandleRect(CRect* pHandleRect) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest, const CPoint &ptDPtoLP, const CSize &szOffset) const;
	virtual int HitTestHandles(CPoint point) const;

	BOOL TrackHandle(int nHandle, CWnd* pWnd, CDC* pDCClipTo);
public:
	xml_node m_node;

	HBITMAP  m_hMoveHandleBitmap;
	int m_nMoveHandleSize;//size of move handle
};
