#pragma once

//提交CCanvasContainerUI，深圳—逸云(QQ：1112880)
//修改为CTrackerUI, liqs99
//2023-09-16
namespace DuiLib
{

class UILIB_API CTrackerUI: public CContainerUI
{
	DECLARE_DUICONTROL(CTrackerUI)
public:
	CTrackerUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	// 0    1    2
	// 3    4    5
	// 6    7    8
	RECT GetSizerRect(int iIndex);
	int GetSizerCursor(POINT& pt, int& iCursor);
	void PaintBorder(UIRender *pRender);
	void DoEvent(TEventUI& event);
	void MoveRect(int cx, int cy);
	void SizeRect(int cx, int cy);
	int GetCorIndex(int fx, int fy, int sx, int sy);
	//void DoPostPaint(HDC hDC, const RECT& rcPaint);
	RECT GetRect();
protected:
	int m_iCursor;
	UINT m_uButtonState;
	POINT m_ptLastMouse; // 
	RECT m_rcNewPos;
	bool m_bMouseDown;
};

};
