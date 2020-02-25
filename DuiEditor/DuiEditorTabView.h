#pragma once
#include "DuiEditorViewDesign.h"
#include "DuiEditorViewCode.h"

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif

// CDuiEditorTabView 视图

class CDuiEditorTabView : public CView
{
	DECLARE_DYNCREATE(CDuiEditorTabView)

	class CInternalTabView : public CView
	{
		friend class CDuiEditorTabView;
	};

	class CInternalTabCtrl : public CMFCTabCtrl
	{
		virtual int GetTabsHeight() const { return 0; }
	};
protected:
	CDuiEditorTabView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDuiEditorTabView();

protected:
	CInternalTabCtrl m_wndTabs;
	BOOL    m_bIsReady;
	int     m_nFirstActiveTab;

	int		m_nLastActiveTab;	//上一次选择的标签序号

	CDuiEditorViewDesign *m_pFormDesign;
	CDuiEditorViewCode *m_pFormCode;

// Operations
public:
	int AddView(CRuntimeClass* pViewClass, const CString& strViewLabel, int iIndex = -1, CCreateContext* pContext = NULL);
	int FindTab(HWND hWndView) const;

	BOOL RemoveView(int iTabNum);
	BOOL SetActiveView(int iTabNum);
	CView* GetActiveView() const; // active view or NULL

// Overrides
public:
	virtual void OnInitialUpdate();

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	virtual void OnActivateView(CView* pView);
	virtual BOOL IsScrollBar() const { return FALSE; }

protected:
	//{{AFX_MSG(CTabView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnChangedActiveTab(WPARAM,LPARAM);
	afx_msg LRESULT OnChangingActiveTab(WPARAM,LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif
