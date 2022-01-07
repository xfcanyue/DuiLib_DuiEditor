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

	virtual LPCTSTR GetClass() const override;
	virtual UINT GetControlFlags() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	virtual bool Add(CControlUI* pControl) override;
	virtual bool AddAt(CControlUI* pControl, int iIndex) override;

	virtual void DoInit() override;
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
	virtual SIZE EstimateSize(SIZE szAvailable) override;
protected:
	IGridUI *m_pOwner;
};

}