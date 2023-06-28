#ifndef __DPI_Base_H__
#define __DPI_Base_H__
#pragma once

namespace DuiLib
{
	class UILIB_API CDpiBase
	{
	public:
		CDpiBase();

		virtual int GetMainMonitorDPI();
	public:
		virtual UINT GetDPI();
		virtual UINT GetScale();
		void SetScale(UINT uDPI);

		virtual int ScaleInt(int iValue);
		virtual int  ScaleIntBack(int iValue);

		RECT ScaleRect(RECT rcRect);
		void ScaleRect(RECT *pRect);
		void ScaleRectBack(RECT *pRect);

		POINT ScalePoint(POINT ptPoint);
		void ScalePoint(POINT *pPoint);

		SIZE ScaleSize(SIZE szSize);
		void ScaleSize(SIZE *pSize);


	protected:
		int m_nScaleFactor;
		int m_nScaleFactorSDA;
	};
}
#endif //__DPI_Base_H__