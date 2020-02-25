#pragma once
#include "UIManager.h"
#include "UIWindowEx.h"

class CDuiEditorCommandHistory
{
public:
	CDuiEditorCommandHistory(void);
	virtual ~CDuiEditorCommandHistory(void);

	void SetInitBOOL(BOOL bInit) { m_bInit = bInit; }
	void Init(CDuiEditorDoc *pDoc);
	void UnInit();

	void Undo();
	void Redo();
	BOOL CanUndo();
	BOOL CanRedo();

	BOOL AddNewControl(xml_node node);

	xml_node AddModifyControl(xml_node node);

	BOOL AddDeleteControl(xml_node node);
	
	CUIManager *m_pManager;
protected:
	void PrepareAddCommand();
	void _add(xml_node nodeControl, xml_node nodeParent, xml_node nodeSibling);
	void _addChild(xml_node nodeDocument, CControlUI *pParent, HTREEITEM hTreeParent);
	void _modify(xml_node nodeControl);
	void _delete(xml_node nodeControl);
private:
	BOOL m_bInit;
	CDuiEditorDoc *m_pDoc;
	xml_node m_root;
	xml_node m_nodeUndo;
	xml_node m_nodeRedo;
	xml_node m_current;

	BOOL m_bCanRedo;
	BOOL m_bCanUndo;
};

