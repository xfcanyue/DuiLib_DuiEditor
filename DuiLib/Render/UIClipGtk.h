#ifndef __UICLIP_GTK_H__
#define __UICLIP_GTK_H__

#pragma once
#ifdef DUILIB_GTK
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIClipGtk : public UIClipBase
	{
	public:
		UIClipGtk();
		virtual ~UIClipGtk();
	public:
		virtual void GenerateClip(UIRender *pRender, RECT rc) override;
		virtual void GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY) override;
		virtual void UseOldClipBegin(UIRender *pRender) override;
		virtual void UseOldClipEnd(UIRender *pRender) override;
	};
} // namespace DuiLib
#endif //#ifdef DUILIB_GTK

#endif // __UIRENDER_H__
