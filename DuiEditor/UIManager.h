#pragma once
#include "UIBuilder.h"
#include "UIWindowEx.h"
#include "UIView.h"
#include "DuiEditorDoc.h"
#include "DuiEditorViewDesign.h"
#include "DuiEditorViewCode.h"
#include "UITrackerMuliti.h"
#include "DockControlTreeCtrl.h"
#include "UIPropertyGridCtrl.h"

#define FORM_DEFAULT_SIZEX	600
#define FORM_DEFAULT_SIZEY	450

#define SPLIT_DESIGN		0
#define SPLIT_CODE			1
#define SPLIT_UPDOWN		2
#define SPLIT_LEFTRIGHT		3
class CUIManager
{
public:
	CUIManager(void);
	virtual ~CUIManager(void);

	BOOL IsUiWindow(xml_node node);
	xml_node FindContainer(xml_node node); //获取控件对应的容器

	BOOL SelectItem(xml_node node);
	BOOL SelectItem(CControlUI *pControl);

	void EnsureVisible(xml_node node);
	void EnsureVisible(CControlUI *pControl);

	BOOL UpdateControlPos(xml_node node, CRect rect, BOOL bUpdateWithHeight=TRUE);
	BOOL UpdateControlWidth(xml_node node, int width);
	BOOL UpdateControlHeight(xml_node node, int height);
	BOOL UpdateControlUI(xml_node node);
	BOOL UpdateControlUI(xml_node node, xml_attribute attr);

	BOOL DeleteControl(xml_node node);
	void SetScrollSize();
	void SetZoom(int zoom);

	void SetSplitterMode(int nMode);
	int  GetSplitterMode() const;
protected:
	BOOL UpdateControlUI(CControlUI *pControl);

private:
	int m_nSplitterMode;

//////////////////////////////////////////////////////////////////////////

public: //统一管理各种指针
	CDuiEditorDoc *GetDocument() { return m_pDoc; }
	CDuiEditorViewDesign *GetDesignView() { return m_pDesignView; }
	CDuiEditorViewCode *GetCodeView() { return m_pCodeView; }
	CPaintManagerUI *GetManager() { return m_pManager; }
	CUIWindow *GetUiWindow() { return m_pUiWindow; }
	CUIFormView *GetUiFrom()	{ return (CUIFormView *)GetManager()->GetRoot(); }
	CUITrackerMuliti *GetUiTracker() { return m_pUiTracker; }
	CDockControlTreeCtrl *GetTreeView() { return m_pTreeView; }
	CUIPropertyGridCtrl *GetPropList() { return m_pPropList; }

public:
	void _setDocument(CDuiEditorDoc *pDoc) { m_pDoc = pDoc; }
	void _setDesignView(CDuiEditorViewDesign *pView) { m_pDesignView = pView; }
	void _setDesignCode(CDuiEditorViewCode *pView) { m_pCodeView = pView; }
	void _setManager(CPaintManagerUI *paintManager) { m_pManager = paintManager; }
	void _setUIWindow(CUIWindow *pUIWindow) { m_pUiWindow = pUIWindow; }
	void _setUITrackerMuliti(CUITrackerMuliti *pTrack) { m_pUiTracker = pTrack; }
	void _setControllTree(CDockControlTreeCtrl *pTree) { m_pTreeView = pTree; }
	void _setPorpList(CUIPropertyGridCtrl *pPropList) { m_pPropList = pPropList; }

private:
	CDuiEditorDoc *m_pDoc;
	CDuiEditorViewDesign *m_pDesignView;
	CDuiEditorViewCode *m_pCodeView;
	CPaintManagerUI *m_pManager;
	CUIWindow *m_pUiWindow;
	CUITrackerMuliti *m_pUiTracker;
	CDockControlTreeCtrl *m_pTreeView;
	CUIPropertyGridCtrl *m_pPropList;
};

