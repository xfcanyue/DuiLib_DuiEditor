// DuiEditorView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DuiEditorView.h"


// CDuiEditorView

IMPLEMENT_DYNCREATE(CDuiEditorView, CView)

CDuiEditorView::CDuiEditorView()
{

}

CDuiEditorView::~CDuiEditorView()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorView, CView)
END_MESSAGE_MAP()


// CDuiEditorView 绘图

void CDuiEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CDuiEditorView 诊断

#ifdef _DEBUG
void CDuiEditorView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDuiEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDuiEditorView 消息处理程序
