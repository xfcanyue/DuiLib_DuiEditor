#ifndef __UIRENDER_GDIPLUS_H__
#define __UIRENDER_GDIPLUS_H__

#pragma once
#include "UIRender_gdi.h"
#include "UIObject_gdiplus.h"

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIRender_gdiplus : public UIRender_gdi
	{
	public:
		UIRender_gdiplus();
		virtual ~UIRender_gdiplus();

// 		virtual UIObject* SelectObject(UIObject *pObject) override;
// 		virtual void RestoreObject(UIObject *pObject = NULL) override;
// 		virtual void RestoreDefaultObject() override;

		virtual void DrawColor(const RECT& rc, const SIZE &round, DWORD color) override;
		//virtual void DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) override;

		virtual void DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) override;

	};

} // namespace DuiLib

#endif // __UIRENDER_H__
