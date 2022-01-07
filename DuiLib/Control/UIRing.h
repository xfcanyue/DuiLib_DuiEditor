#ifndef __UIROTATE_H__
#define __UIROTATE_H__

#pragma once

namespace DuiLib
{
	class CRingUI : public CLabelUI
	{
		enum
		{ 
			RING_TIMERID = 100,
		};
		DECLARE_DUICONTROL(CRingUI)
	public:
		CRingUI();
		~CRingUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual void SetBkImage(LPCTSTR pStrImage) override;	
		virtual void DoEvent(TEventUI& event) override;
		virtual void PaintBkImage(UIRender *pRender) override;	

	private:
		void InitImage();
		void DeleteImage();

		Gdiplus::Image*	GdiplusLoadImage(LPCTSTR pstrPath);
	public:
		float m_fCurAngle;
		Gdiplus::Image* m_pBkimage;
	};
}

#endif // __UIROTATE_H__