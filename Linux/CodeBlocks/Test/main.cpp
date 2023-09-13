#include <stdlib.h>

#include "App.h"

LPCTSTR GetText2()
{
    return _T("");
}
int main (int argc, char *argv[])
{
    //printf("%d, %s", argc, argv[0]);

    CDuiString s = GetText2();

    //设置主窗口的名字
	uiApp.SetAppName(_T("DuiLib Tutorial"));

	//设置xml资源载入方式，选择一种就行
	uiApp.SetResType_File(_T("skin"));								//从文件夹中载入xml文件

	CPaintManagerUI::SetRenderEngineType(DuiLib_Render_Cairo);

	//初始化duilib, 注册插件，创建主窗口
	if(!uiApp.InitInstance(argc, argv))
		return 0;

	//进入消息循环
	uiApp.Run();

	//清理资源。
	uiApp.ExitInstance();
    return 0;
}
