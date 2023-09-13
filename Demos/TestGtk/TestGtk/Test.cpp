// DuiLibTemplate.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Test.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
	{
		return 1;
	}

	//_CrtSetBreakAlloc();

	//设置主窗口的名字
	uiApp.SetAppName(_T("Test"));

	CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetInstancePath(CPaintManagerUI::GetInstancePath());

	//设置程序为单一执行实例，允许多实例的话，就不需要这行了。
//	uiApp.SetSingleApplication(true, _T("CBD4DD1F-3B18-4DA4-A043-0EA9AEC8D570"));

	//设置xml资源载入方式，选择一种就行	
#ifdef _DEBUG
	uiApp.SetResType_File(_T("skin"));								//从文件夹中载入xml文件
#else
	uiApp.SetResType_ZipResource(_T("skin"), _T(""), IDR_ZIPRES1, _T("ZIPRES"));	//从程序资源文件中载入xml文件
#endif

	CPaintManagerUI::SetRenderEngineType(DuiLib_Render_Cairo);

	//初始化duilib, 注册插件，创建主窗口
	if(!uiApp.InitInstance(0,NULL)) 
		return 0;

	//进入消息循环
	uiApp.Run();

	//清理资源。
	uiApp.ExitInstance();
	return 0;
}
