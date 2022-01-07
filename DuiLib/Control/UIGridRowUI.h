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

	virtual LPCTSTR GetClass() const override;
	virtual UINT GetControlFlags() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	void SetRowIndex(int nIndex);
	int GetRow();

	virtual bool IsSelected() const override;
	virtual void Selected(bool bSelected, bool bTriggerEvent=true) override;

	virtual bool IsHotState() const override;

	virtual bool Add(CControlUI* pControl) override;
	virtual bool AddAt(CControlUI* pControl, int iIndex) override;

	virtual void DoInit() override;
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
	virtual SIZE EstimateSize(SIZE szAvailable) override;

	virtual void DoEvent(TEventUI& event) override;
	virtual void PaintBkColor(UIRender *pRender) override;
protected:
	IGridUI *m_pOwner;
	int m_row;
};


}