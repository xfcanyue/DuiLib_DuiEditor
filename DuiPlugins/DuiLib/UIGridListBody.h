#pragma once
#include "IGridListUI.h"

namespace DuiLib
{

class UILIB_API CGridListBodyUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridListBodyUI)
public:
	CGridListBodyUI(void);
	virtual ~CGridListBodyUI(void);

	void SetOwner(IGridListUI *pGrid) { m_pOwner = pGrid; }
	IGridListUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual SIZE EstimateSize(SIZE szAvailable);

	virtual void LineUp();
	virtual void LineDown();
	virtual void PageUp();
	virtual void PageDown();
	virtual void HomeUp();
	virtual void EndDown();
protected:
	IGridListUI *m_pOwner;

private:
};


}