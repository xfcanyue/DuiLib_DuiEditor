#pragma once
#include "UITrackerMuliti.h"

// CDefaultEditorView

class CDefaultEditorView : public CWnd
{
	DECLARE_DYNAMIC(CDefaultEditorView)

public:
	CDefaultEditorView();
	virtual ~CDefaultEditorView();

	CControlUI		*m_pControl;
	CRect			m_rcControl;
	CTracker		m_tracker;
	BOOL			m_bUiEffect;

public:
	CPaintManagerUI *GetManager() { return &m_PaintManager; }
	CPaintManagerUI m_PaintManager;
public:
	BOOL CreateControl(LPCTSTR pstrClassName);
	void SetControlAttribute();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
};


