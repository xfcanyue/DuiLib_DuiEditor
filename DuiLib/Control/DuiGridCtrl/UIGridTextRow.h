#pragma once
#include "UIGridRow.h"

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

#define DUI_GRID_MAX_COLUMN		64

class CGridTextRowUI : public CGridRowUI
{
	DECLARE_DUICONTROL(CGridTextRowUI)
public:
	CGridTextRowUI(void);
	virtual ~CGridTextRowUI(void);

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual CGridCellUI *GetCell(int col);

	SIZE EstimateSize(SIZE szAvailable);

	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
private:
	CGridCellUI m_arrCell[DUI_GRID_MAX_COLUMN];
};


}
