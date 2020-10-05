#pragma once

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridRowUI : public CHorizontalLayoutUI
{
public:
	CGridRowUI(void);
	virtual ~CGridRowUI(void);

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetOwner(CControlUI *pOwnder);
	CControlUI *GetOwner();

	void DoEvent(TEventUI& event);

	virtual CGridCellUI *GetCell(int col) = 0;
private:
	CControlUI *m_pOwner;

	bool m_bDragable;
};


}
