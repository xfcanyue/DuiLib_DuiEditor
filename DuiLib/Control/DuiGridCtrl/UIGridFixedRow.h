#pragma once
#include "UIGridRow.h"

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridFixedRowUI : public CGridRowUI
{
	DECLARE_DUICONTROL(CGridFixedRowUI)
public:
	CGridFixedRowUI();
	virtual ~CGridFixedRowUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool Add(CControlUI* pControl);
	virtual CGridCellUI *GetCell(int col);

	SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc, bool bNeedInvalidate = true);

	virtual void PaintBkColor(HDC hDC);
	virtual void PaintBorder(HDC hDC);
private:
	CControlUI *m_pOwner;
};


}
