#pragma once
#include "DuiEditorViewCode.h"
#include "DuiEditorViewDesign.h"

// CMySplitterWnd

class CMySplitterWnd : public CSplitterWndEx
{
	DECLARE_DYNAMIC(CMySplitterWnd)

public:
	CMySplitterWnd();
	virtual ~CMySplitterWnd();

	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext);

	void SetViewMode(int nMode);
	int  GetViewMode() const { return m_nMode; }
	virtual void RecalcLayout();    // call after changing sizes

	CDuiEditorViewDesign *GetDesignView() const { return m_pDesignView; }
	CDuiEditorViewCode *GetDesignCode() const { return m_pDesignCode; }
private:
	int m_nMode;
	CDuiEditorViewDesign *m_pDesignView;
	CDuiEditorViewCode *m_pDesignCode;

	int m_nLeftWidth;
protected:
	virtual void DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside);
	virtual int HitTest(CPoint pt) const;
	virtual void TrackColumnSize(int x, int col);
protected:
	DECLARE_MESSAGE_MAP()
};


