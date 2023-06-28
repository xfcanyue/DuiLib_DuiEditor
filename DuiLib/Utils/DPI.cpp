#include "StdAfx.h"
#include "DPI.h"

namespace DuiLib
{
	//96 DPI = 100% scaling
	//120 DPI = 125% scaling
	//144 DPI = 150% scaling
	//168 DPI = 175% scaling
	//192 DPI = 200% scaling

	CDpiBase::CDpiBase()
	{
		m_nScaleFactor = 0;
		m_nScaleFactorSDA = 0;
		SetScale(96);
	}

	int CDpiBase::GetMainMonitorDPI()
	{
		return 96;
	}

	UINT DuiLib::CDpiBase::GetDPI()
	{
		return MulDiv(m_nScaleFactor, 96, 100);
	}

	UINT CDpiBase::GetScale()
	{
		return m_nScaleFactor;
	}


	void CDpiBase::SetScale(UINT uDPI)
	{
		m_nScaleFactor = MulDiv(uDPI, 100, 96);
		if (m_nScaleFactorSDA == 0) {
			m_nScaleFactorSDA = m_nScaleFactor;
		}
	}

	int  CDpiBase::ScaleInt(int iValue)
	{
		return MulDiv(iValue, m_nScaleFactor, 100);
	}

	int  CDpiBase::ScaleIntBack(int iValue) 
	{
		return MulDiv(iValue, 100, m_nScaleFactor);
	}

	RECT CDpiBase::ScaleRect(RECT rcRect)
	{
		RECT rcScale = rcRect;
		int sw = ScaleInt(rcRect.right - rcRect.left);
		int sh = ScaleInt(rcRect.bottom - rcRect.top);
		rcScale.left = ScaleInt(rcRect.left);
		rcScale.top = ScaleInt(rcRect.top);
		rcScale.right = rcScale.left + sw;
		rcScale.bottom = rcScale.top + sh;
		return rcScale;
	}

	void CDpiBase::ScaleRect(RECT *pRect)
	{
		int sw = ScaleInt(pRect->right - pRect->left);
		int sh = ScaleInt(pRect->bottom - pRect->top);
		pRect->left = ScaleInt(pRect->left);
		pRect->top = ScaleInt(pRect->top);
		pRect->right = pRect->left + sw;
		pRect->bottom = pRect->top + sh;
	}

	void CDpiBase::ScaleRectBack(RECT *pRect)
	{
		int sw = ScaleIntBack(pRect->right - pRect->left);
		int sh = ScaleIntBack(pRect->bottom - pRect->top);
		pRect->left = ScaleIntBack(pRect->left);
		pRect->top = ScaleIntBack(pRect->top);
		pRect->right = pRect->left + sw;
		pRect->bottom = pRect->top + sh;
	}

	void CDpiBase::ScalePoint(POINT *pPoint)
	{
		pPoint->x = ScaleInt(pPoint->x);
		pPoint->y = ScaleInt(pPoint->y);
	}

	POINT CDpiBase::ScalePoint(POINT ptPoint)
	{
		POINT ptScale = ptPoint;
		ptScale.x = ScaleInt(ptPoint.x);
		ptScale.y = ScaleInt(ptPoint.y);
		return ptScale;
	}

	void CDpiBase::ScaleSize(SIZE *pSize)
	{
		pSize->cx = ScaleInt(pSize->cx);
		pSize->cy = ScaleInt(pSize->cy);
	}

	SIZE CDpiBase::ScaleSize(SIZE szSize)
	{
		SIZE szScale = szSize;
		szScale.cx = ScaleInt(szSize.cx);
		szScale.cy = ScaleInt(szSize.cy);
		return szScale;
	}
}
