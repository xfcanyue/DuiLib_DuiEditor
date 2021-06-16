#ifndef __UIDYNAMICLAYOUT_H__
#define __UIDYNAMICLAYOUT_H__

#pragma once

namespace DuiLib
{
	//允许动态设置布局方式的类
	class UILIB_API CDynamicLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CDynamicLayoutUI)
	public:
		CDynamicLayoutUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetLayoutMode(int nMode) { m_nLayoutMode = nMode; }
		int GetLayoutMode() const { return m_nLayoutMode; }
		bool IsHorizontalLayoutMode() const { return m_nLayoutMode == 0; }
		bool IsVerticalLayoutMode() const { return m_nLayoutMode == 1; }

		void SetSepWidth(int iWidth);
		int GetSepWidth() const;
		void SetSepHeight(int iHeight);
		int GetSepHeight() const;
		void SetSepImmMode(bool bImmediately);
		bool IsSepImmMode() const;
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DoEvent(TEventUI& event);
		void DoEventHorizontalLayout(TEventUI& event);
		void DoEventVerticalLayout(TEventUI& event);

		virtual SIZE EstimateSize(SIZE szAvailable);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void SetPosHorizontalLayout(RECT rc, bool bNeedInvalidate = true);
		void SetPosVerticalLayout(RECT rc, bool bNeedInvalidate = true);
		virtual bool CalcPos(CControlUI *pChildControl, RECT &rcChild); //子控件调用询问父控件，你将会给我分配多大的rect。
		bool CalcPosHorizontalLayout(CControlUI *pChildControl, RECT &rcChild);
		bool CalcPosVerticalLayout(CControlUI *pChildControl, RECT &rcChild);
		void DoPostPaint(HDC hDC, const RECT& rcPaint);

		RECT GetThumbRect(bool bUseNew = false) const;
		RECT GetThumbRectHorizontalLayout(bool bUseNew = false) const;
		RECT GetThumbRectVerticalLayout(bool bUseNew = false) const;

	protected:
		int m_iSepWidth;
		int m_iSepHeight;
		UINT m_uButtonState;
		POINT m_ptLastMouse;
		RECT m_rcNewPos;
		bool m_bImmMode;

		int m_nLayoutMode; //0=HorizontalLayout, 1=VerticalLayout
	};
}
#endif // __UIHORIZONTALLAYOUT_H__
