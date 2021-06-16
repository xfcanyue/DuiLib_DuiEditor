#pragma once
#include "IGridUI.h"

namespace DuiLib
{

class UILIB_API CGridHeaderUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridHeaderUI)
public:
	CGridHeaderUI(void);
	virtual ~CGridHeaderUI(void);

	void SetOwner(IGridUI *pGrid) { m_pOwner = pGrid; }
	IGridUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual SIZE EstimateSize(SIZE szAvailable);
protected:
	IGridUI *m_pOwner;
};

}