#pragma once

#include "ScriptEditorDoc.h"
#include "SciWnd.h"
// CScriptEditorView 视图

class CScriptEditorView : public CView
{
	DECLARE_DYNCREATE(CScriptEditorView)

protected:
	CScriptEditorView();           // 动态创建所使用的受保护的构造函数
	virtual ~CScriptEditorView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

public:
	CScriptEditorDoc* GetDocument() const
	{ return reinterpret_cast<CScriptEditorDoc*>(m_pDocument); }

	void InitLexer();

	CSciWnd sci;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnInitialUpdate();
};


