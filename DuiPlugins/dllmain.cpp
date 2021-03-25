// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			TCHAR czProgName[MAX_PATH];
			int  iNiValue = GetModuleFileName(hModule, czProgName, MAX_PATH);
			if(iNiValue > 0)
			{
				CDuiString temp = czProgName;
				g_strDuiPluginsPath = temp.Left(temp.ReverseFind('\\') + 1);
			}
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

