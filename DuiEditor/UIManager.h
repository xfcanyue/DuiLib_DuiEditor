#pragma once
#include "UIView.h"
#include "UIBuilder.h"
#include "DuiEditorDoc.h"

#define FORM_DEFAULT_SIZEX	600
#define FORM_DEFAULT_SIZEY	450

//#define FORM_OFFSET_X 0
//#define FORM_OFFSET_Y 0

class CDuiEditorViewDesign;
class CUIWindow;
class CDuiEditorCommandHistory;
class CUITrackerMuliti;
class CUIManager
{
public:
	friend class CDuiEditorViewDesign;

	CUIManager(void);
	virtual ~CUIManager(void);

	BOOL IsUiWindow(xml_node node);

	xml_node FindContainer(xml_node node); //获取控件对应的容器

	BOOL SelectItem(xml_node node);
	BOOL SelectItem(CControlUI *pControl);

	BOOL UpdateControlPos(xml_node node, CRect rect, BOOL bUpdateWithHeight=TRUE);
	BOOL UpdateControlWidth(xml_node node, int width);
	BOOL UpdateControlHeight(xml_node node, int height);
	BOOL UpdateControlUI(xml_node node);
	BOOL UpdateControlUI(xml_node node, xml_attribute attr);

	BOOL DeleteControl(xml_node node);

	void SetScrollSize();

	void SetZoom(int zoom);
protected:
	BOOL UpdateControlUI(CControlUI *pControl);

public: //统一管理各种指针
	CDuiEditorDoc *GetDocument() { return m_pDoc; }
	CDuiEditorCommandHistory *GetCmdHistory() { return m_pCmdHistory; }
	CDuiEditorViewDesign *GetView() { return m_pView; }
	CPaintManagerUI *GetManager() { return m_pManager; }
	CUIWindow *GetUiWindow() { return m_pUiWindow; }
	CUIFormView *GetUiFrom()	{ return (CUIFormView *)GetManager()->GetRoot(); }
	CUITrackerMuliti *GetUiTracker() { return m_pUiTracker; }
	CDockControlTreeCtrl *GetTreeView() { return m_pTreeView; }
private:
	CDuiEditorDoc *m_pDoc;
	CDuiEditorCommandHistory *m_pCmdHistory;
	CDuiEditorViewDesign *m_pView;
	CPaintManagerUI *m_pManager;
	CUIWindow *m_pUiWindow;
	CDockControlTreeCtrl *m_pTreeView;
	CUITrackerMuliti *m_pUiTracker;
};

