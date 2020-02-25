#pragma once

//add by liqs99 ±Ì∏Ò¿‡
namespace DuiLib {

class CGridHeaderUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL(CGridHeaderUI)
public:
	CGridHeaderUI(void);
	virtual ~CGridHeaderUI(void);

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetOwner(CControlUI *pOwnder);
	CControlUI *GetOwner();

	SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc, bool bNeedInvalidate = true);

	virtual void PaintBorder(HDC hDC);
private:
	CControlUI *m_pOwner;
};


}
