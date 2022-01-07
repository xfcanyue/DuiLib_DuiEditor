#pragma once
#include "IGridUI.h"

namespace DuiLib
{

class UILIB_API CGridBodyUI : public CContainerUI
{
	DECLARE_DUICONTROL(CGridBodyUI)
public:
	CGridBodyUI(void);
	virtual ~CGridBodyUI(void);

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
	virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

	virtual void LineUp() override;
	virtual void LineDown() override;
	virtual void PageUp() override;
	virtual void PageDown() override;
	virtual void HomeUp() override;
	virtual void EndDown() override;

	void SetCyNeeded(int cyNeeded) { m_nCyNeeded = cyNeeded; }
	int GetCyNeeded() const { return m_nCyNeeded; }
protected:
	IGridUI *m_pOwner;
	int m_nCyNeeded;
private:
};


}