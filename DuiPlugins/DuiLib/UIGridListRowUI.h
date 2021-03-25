#pragma once
#include "IGridListUI.h"

namespace DuiLib
{

class UILIB_API CGridListRowUI : public CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CGridListRowUI)
public:
	CGridListRowUI(void);
	~CGridListRowUI(void);

	void SetOwner(IGridListUI *pGrid) { m_pOwner = pGrid; }
	IGridListUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetRowIndex(int nIndex);
	int GetRowIndex();
	bool IsFixedRow();

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	void Selected(BOOL bSelect);
	BOOL IsSelected() { return m_bSelected; }

	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual SIZE EstimateSize(SIZE szAvailable);

	virtual void DoEvent(TEventUI& event);
	virtual void PaintBkColor(HDC hDC);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	IGridListUI *m_pOwner;
	BOOL m_bSelected;
	int m_nRowIndex;
};


}