#ifndef __UIGROUPBOX_H__
#define __UIGROUPBOX_H__

#pragma once

namespace DuiLib
{

	class UILIB_API CGroupBoxUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CGroupBoxUI)
	public:
		CGroupBoxUI();
		~CGroupBoxUI();
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void PaintText(UIRender *pRender);
		virtual void PaintBorder(UIRender *pRender);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		SIZE CalcrectSize(SIZE szAvailable);
	};
}
#endif // __UIGROUPBOX_H__