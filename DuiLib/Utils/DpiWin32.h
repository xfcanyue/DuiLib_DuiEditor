#ifndef __DPI_Win32_H__
#define __DPI_Win32_H__
#pragma once

#ifndef DPI_ENUMS_DECLARED

typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI = 0,
	MDT_ANGULAR_DPI = 1,
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

#define DPI_ENUMS_DECLARED
#endif // (DPI_ENUMS_DECLARED)

#ifdef DUILIB_WIN32
namespace DuiLib
{
	class UILIB_API CDpiWin32 : public CDpiBase
	{
	public:
		CDpiWin32();

	public:
		virtual int GetMainMonitorDPI() override;
		int GetDPIOfMonitor(HMONITOR hMonitor);
		int GetDPIOfMonitorNearestToPoint(POINT pt);
	public:
		PROCESS_DPI_AWARENESS GetDPIAwareness();
		BOOL SetDPIAwareness(PROCESS_DPI_AWARENESS Awareness);
		virtual UINT GetDPI() override;
		virtual UINT GetScale() override;
		virtual int ScaleInt(int iValue) override;
		virtual int ScaleIntBack(int iValue) override;

	private:
		PROCESS_DPI_AWARENESS m_Awareness;
	};
}
#endif //#ifdef DUILIB_WIN32
#endif //__DPI_Win32_H__