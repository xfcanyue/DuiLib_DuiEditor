#pragma once


// CDuiEditorView 视图

class CDuiEditorView : public CView
{
	DECLARE_DYNCREATE(CDuiEditorView)

protected:
	CDuiEditorView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDuiEditorView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


