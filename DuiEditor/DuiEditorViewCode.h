#pragma once
#include "SciWnd.h"
#include "SciXmlWriter.h"
// CDuiEditorCode 视图

class CUIManager;
class CDuiEditorViewCode : public CView
{
	DECLARE_DYNCREATE(CDuiEditorViewCode)
public:
	CDuiEditorViewCode();           // 动态创建所使用的受保护的构造函数
	virtual ~CDuiEditorViewCode();

	CDuiEditorDoc* GetDocument() const;
	void LoadDocument();
	BOOL ApplyDocument();	//强制更新
	BOOL UpdateDocument(); //增量更新
	BOOL IsModify() { return sci.sci_GetModify(); }

	void SelectXmlNode(CControlUI *pControl);
	void SelectXmlNode(xml_node node);

	//设计界面中增加控件，即时更新XML
	void AddNode(xml_node node);
	//设计界面中删除控件，即时更新XML
	void DeleteNode(xml_node node);

	//设计界面中增加属性，即时更新XML
	void AddAttribute(xml_node node, xml_attribute attr);
	//设计界面中修改属性，即时更新XML
	void ModifyAttribute(xml_node node, xml_attribute attr);
	//设计界面中删除属性，即时更新XML
	void DeleteAttribute(xml_node node, xml_attribute attr);

public:
	CSciWnd *GetSciWnd() { return &sci; }
	xml_parse_result *GetParseResult() { return &m_xmlParseResult; }
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

protected:
	CSciWnd sci;
	int m_nTargetLine;					//载入文档后滚动到目标行
	BOOL m_bAutoUpdateDesign;			//是否需要更新到视图
	BOOL m_bNeedUpdate;					//是否允许更新视图
	BOOL m_bUpdateDesignWhileModified; //当修改文档时，是否立即增量更新视图
	xml_parse_result m_xmlParseResult;
protected:
	BOOL SelectControlUI(int pos, xml_node node);
	void UpdateFrameStatus();

	CString GetNodeName();
	void AutoCompleteNode(CString objectName);		//自动完成控件名
	CString AutoCompleteProperty(CString objectName, CString AttrName);	//自动完成属性名

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
	afx_msg LRESULT OnSciClick(WPARAM WParam, LPARAM LParam);
	afx_msg LRESULT OnSciMouseMove(WPARAM WParam, LPARAM LParam);
	
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditRefresh();
	afx_msg void OnEditSelectAll();
	afx_msg void OnSciUpdateDesign();
	afx_msg void OnUpdateSciUpdateDesign(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSciParseXml();
	afx_msg void OnSciApplyDesign();
	afx_msg void OnUpdateSciApplyDesign(CCmdUI *pCmdUI);
	afx_msg void OnSciWrapLine();
	afx_msg void OnUpdateSciWrapLine(CCmdUI *pCmdUI);
	afx_msg void OnSciUpdateWhileModify();
	afx_msg void OnUpdateSciUpdateWhileModify(CCmdUI *pCmdUI);
};
