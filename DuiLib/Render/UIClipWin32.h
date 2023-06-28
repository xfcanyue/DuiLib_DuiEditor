#ifndef __UICLIP_WIN32_H__
#define __UICLIP_WIN32_H__

#pragma once
#ifdef DUILIB_WIN32
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIClipWin32 : public UIClipBase
	{
	public:
		UIClipWin32();
		virtual ~UIClipWin32();
	public:
		virtual void GenerateClip(UIRender *pRender, RECT rc) override;
		virtual void GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY) override;
		virtual void UseOldClipBegin(UIRender *pRender) override;
		virtual void UseOldClipEnd(UIRender *pRender) override;

	protected:
		RECT m_rcItem;
		SIZE m_szRound;
		HDC m_hDC;
		HRGN m_hRgn;
		HRGN m_hOldRgn;
	};
} // namespace DuiLib
#endif //#ifdef DUILIB_WIN32

#endif // __UIRENDER_H__
