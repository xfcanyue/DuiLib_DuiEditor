#pragma once

#include "ScriptEditorDoc.h"
#include "ScriptLexer.h"
#include <stack>


// CScriptEditorView 视图

class CScriptEditorView : public CView
{
	DECLARE_DYNCREATE(CScriptEditorView)

protected:
	CScriptEditorView();           // 动态创建所使用的受保护的构造函数
	virtual ~CScriptEditorView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图

	CStringA m_strTextCode; 
	BOOL m_bFirstLoad;
public:
	CScriptEditorDoc* GetDocument() const { return reinterpret_cast<CScriptEditorDoc*>(m_pDocument); }
	CSciWnd sci;
	IScriptHelper *m_pHelper;;
	CScriptLexer m_lexer;
	void InitLexer();

	char m_arrSpace[1024];
	char m_arrTab[1024];
	BOOL IsSpace(char ch){ return ch==' ' || ch=='	'; }
	int ReplaceSpace(int start, int end, int lineIndent);
	void AddNewLine();

	void AutoIndentation(char ch); //自动缩进
	void AutoCompletion(int ich); //自动补齐括号引号等等
	void AutoCompletionShow2(); //语法提示
	void CallTips(int pos);

	std::stack<char> m_arrBrace; 
	void BraceHighlight();
	BOOL  FindBrace(const char * brace, int pos, int &start, int &end);
	BOOL CheckBraceChar(int pos);

	void UpdateFrameStatus();
	CStringA VariantToString(void *value, asUINT typeId, int expandMembers, asIScriptEngine *engine);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg LRESULT OnSciMouseMove(WPARAM WParam, LPARAM LParam);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnScriptInsertBreakPoint();
	afx_msg LRESULT OnRefreshStack(WPARAM wparam, LPARAM lparam);
	afx_msg void OnScriptExcute();
	afx_msg void OnUpdateScriptExcute(CCmdUI *pCmdUI);
	afx_msg void OnScriptRun();
	afx_msg void OnUpdateScriptRun(CCmdUI *pCmdUI);
	afx_msg void OnScriptAbort();
	afx_msg void OnUpdateScriptAbort(CCmdUI *pCmdUI);
	afx_msg void OnScriptStepInto();
	afx_msg void OnUpdateScriptStepInto(CCmdUI *pCmdUI);
	afx_msg void OnScriptStepOver();
	afx_msg void OnUpdateScriptStepOver(CCmdUI *pCmdUI);
	afx_msg void OnScriptStepReturn();
	afx_msg void OnUpdateScriptStepReturn(CCmdUI *pCmdUI);
	afx_msg void OnScriptStepCursor();
	afx_msg void OnUpdateScriptStepCursor(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnScriptAddFun1();
	afx_msg void OnScriptAddFun2();
	afx_msg void OnScriptAddFun3();
	afx_msg void OnScriptAddFun4();
	afx_msg void OnScriptAddFun5();
	afx_msg void OnScriptAddFun6();
	afx_msg void OnScriptAddFun7();
	afx_msg void OnScriptAddFun8();
	afx_msg void OnScriptAddFun9();
	afx_msg void OnScriptAddFun10();
	afx_msg void OnScriptAddFun11();
	afx_msg void OnScriptAddFun12();
	afx_msg void OnScriptAddFun13();
	afx_msg void OnScriptParseToken();
};


