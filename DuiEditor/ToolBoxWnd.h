#pragma once
#include "ToolBoxCtrl.h"

// CToolBoxWnd

class CDockToolBoxWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockToolBoxWnd)

public:
	CDockToolBoxWnd();
	virtual ~CDockToolBoxWnd();

public:
	CDockToolBoxCtrl m_ctlToolList;

public:
	CDockToolBoxElement* GetCurSel() const { return m_ctlToolList.GetCurSel(); }
	void SetCurSel(int nClass,BOOL bRedraw=TRUE) { m_ctlToolList.SetCurSel(nClass,bRedraw); }

	void SetDefaultPoint()
	{
		SetCurSel(TREENODETYPE_POINTER);
	}

	bool IsDefaultPoint()
	{
		CDockToolBoxElement *pSel = GetCurSel();
		if(!pSel) return true;
		if(pSel->GetClass() == TREENODETYPE_POINTER)
			return true;
		return false;
	}

	UINT_PTR GetCurSelTag() const 
	{ 
		CDockToolBoxElement *pElem = GetCurSel();
		if(!pElem)	return NULL;
		return pElem->GetTag(); 
	}
protected:
	void AdjustLayout();
	void InitToolList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

extern CDockToolBoxWnd *g_pToolBox;

