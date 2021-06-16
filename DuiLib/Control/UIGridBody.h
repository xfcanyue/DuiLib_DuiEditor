#pragma once
#include "IGridUI.h"

namespace DuiLib
{

class UILIB_API CGridBodyUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridBodyUI)
public:
	CGridBodyUI(void);
	virtual ~CGridBodyUI(void);

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
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	virtual void LineUp();
	virtual void LineDown();
	virtual void PageUp();
	virtual void PageDown();
	virtual void HomeUp();
	virtual void EndDown();

	void SetCyNeeded(int cyNeeded) { m_nCyNeeded = cyNeeded; }
	int GetCyNeeded() const { return m_nCyNeeded; }
protected:
	IGridUI *m_pOwner;
	int m_nCyNeeded;
private:
};


}