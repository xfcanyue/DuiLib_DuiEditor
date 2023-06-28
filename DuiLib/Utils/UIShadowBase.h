#ifndef __UISHADOW_BASE_H__
#define __UISHADOW_BASE_H__

#pragma once
#include <map>

namespace DuiLib
{

class UILIB_API CShadowBaseUI
{
public:
	CShadowBaseUI(void);
	virtual ~CShadowBaseUI(void);

	// bShow为真时才会创建阴影
	virtual void ShowShadow(bool bShow) { }	
	virtual bool IsShowShadow() const { return false; }

	virtual void DisableShadow(bool bDisable) {}
	virtual bool IsDisableShadow() const { return false; }

	// 算法阴影的函数
	virtual bool SetSize(int NewSize = 0) { return false; }
	virtual bool SetSharpness(unsigned int NewSharpness = 5) { return false; }
	virtual bool SetDarkness(unsigned int NewDarkness = 200) { return false; }
	virtual bool SetPosition(int NewXOffset = 5, int NewYOffset = 5) { return false; }
	virtual bool SetColor(COLORREF NewColor = 0) { return false; }

	// 图片阴影的函数
	virtual bool SetImage(LPCTSTR szImage) { return false; }
	virtual bool SetShadowCorner(RECT rcCorner) { return false; }	// 九宫格方式描述阴影

	// 把自己的阴影样式复制到传入参数
	virtual bool CopyShadow(CShadowBaseUI* pShadow) { return false; }

	//	创建阴影窗体，由CPaintManagerUI自动调用,除非自己要单独创建阴影
	virtual void Create(CPaintManagerUI* pPaintManager) {}

	// 父窗体改变大小，移动，或者主动重绘阴影时调用
	virtual void Update(UIWND hParent) {}
};

}

#endif //__UISHADOW_BASE_H__