#pragma once
#include "IGridUI.h"

namespace DuiLib
{

class UILIB_API CGridRowUI : public COptionLayoutUI
{
	DECLARE_DUICONTROL(CGridRowUI)
public:
	CGridRowUI(void);
	~CGridRowUI(void);

	void SetOwner(IGridUI *pGrid) { m_pOwner = pGrid; }
	IGridUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetRowIndex(int nIndex);
	int GetRow();

	virtual bool IsSelected() const;
	virtual void Selected(bool bSelected, bool bTriggerEvent=true);

	virtual bool IsHot();

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual SIZE EstimateSize(SIZE szAvailable);

	virtual void DoEvent(TEventUI& event);
	virtual void PaintBkColor(HDC hDC);
protected:
	IGridUI *m_pOwner;
	int m_row;
};


}