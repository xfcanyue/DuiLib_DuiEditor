#pragma once
#include "IGridListUI.h"

namespace DuiLib
{

class UILIB_API CGridListHeaderUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridListHeaderUI)
public:
	CGridListHeaderUI(void);
	virtual ~CGridListHeaderUI(void);

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
protected:
	IGridListUI *m_pOwner;
};

}