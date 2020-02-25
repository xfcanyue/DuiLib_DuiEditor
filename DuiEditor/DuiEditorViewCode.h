#pragma once
#include "SciWnd.h"


#include "SciFind.h"
// CDuiEditorCode 视图

class CDuiEditorViewCode : public CView
{
	DECLARE_DYNCREATE(CDuiEditorViewCode)

	struct xml_string_writer : pugi::xml_writer
	{
		virtual void write(const void* data, size_t size)
		{
			pSciWnd->SendEditor(SCI_APPENDTEXT, size, (LPARAM)data);
		}
		CSciWnd *pSciWnd;
	};
public:
	CDuiEditorViewCode();           // 动态创建所使用的受保护的构造函数
	virtual ~CDuiEditorViewCode();

	BOOL ApplyDocument();

	BOOL IsModify() { return sci.sci_GetModify(); }

	CSciWnd sci;

	void Init();
	void UpdateFrameStatus();

	CString GetNodeName();
	void AutoCompleteNode(CStringW objectNameW);		//自动完成控件名
	CStringW AutoCompleteProperty(CStringW objectNameW, CStringW AttrName);	//自动完成属性名

	CUIManager *m_pManager;
protected:
	CSciFind		*m_pDlgFind;
	HWND			m_hDlgFind;
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditFind();
};


