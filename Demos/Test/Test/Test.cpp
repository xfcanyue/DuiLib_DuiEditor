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
	CDuiStringA s1 = CDuiStringA("我是s1;");
	CDuiStringUtf8 s2 = CDuiStringA("我是s2;");
	CDuiStringW s3 = CDuiStringW(L"我是s3;");

	CDuiStringA s7 = "我是s7;";
	CDuiStringUtf8 s8 = "我是s8;"; //在win32里面，不会自动转码的
	CDuiStringW s9 = L"我是s9;";

	CDuiString s13;
	s13 = s1;
	s13 = s2;
	s13 = s3;

	s13.Empty();
	s13 += s1;
	s13 += s2;
	s13 += s3;
	s13 += _T("xxxx");


	CDuiString s14;
	s14.setInt(1);


	CDuiString s24;
	s24.SetBufferLength(15);
	int x = s24.GetBufferLength();
	s24.Append(s1);
	s24.Append(s2);
	s24.Append(s3);
	s24.Append(_T("char;"), 3);
	s24.Append(_T('w'));

	CDuiString s25;
	bool bEmpty = s25.IsEmpty();
	s25.Assign(s1);
	bEmpty = s25.IsEmpty();
	s25.Assign(s2);
	s25.Assign(s3);
	s25.Assign(_T("char;"));
	s25.Assign(_T("wchar_t;"));
	CDuiString s25_2 = s25; //地址复用
	CDuiString s25_3; 
	s25_3.CopyFrom(s25); //创建新的内存，并且拷贝字符串
	s25[0] = _T('x');		//s25_2也改了。s25_3没有改动。
	s25.SetAt(1, _T('y'));	//不会再改动s25_2，因为创建了新的内存
	TCHAR ch = s25.GetAt(2);

	LPCTSTR const_pstr = s25.GetBuffer();
	LPTSTR var_pstr = s25.GetBuffer();
	int n = s25.GetLength();

// 	CDuiString s28;
// 	s28.CopyFrom(s25);

	CDuiString s29 = _T("abcdef");
	s29.MakeUpper();
	s29.MakeLower();
	CDuiString s30 = s29.Left(3);
	CDuiString s31 = s29.Mid(3,3);
	CDuiString s32 = s29.Right(3);

	int nFind1 = s29.Find(_T('b'));
	int nFind2 = s29.Find(_T("bc"));
	int nFind3 = s29.ReverseFind(_T('b'));
	s29.Replace(_T("bc"), _T("xy"));
	s29.AppendFormat(_T("%d"), 1234);

	CDuiString s33;
	s33.Format(_T("我是%d"), 33);

	CDuiString s34;
	s34.setString(_T("1234"));
	s34.setString(_T('A'));
	s34.setBool(true);
	s34.setBool(false);
	s34.setInt(2023);
	s34.setFloat(2023.123f, 3);
	float f = s34.toFloat();
	s34.setDouble(2023.1, 5);
	double d = s34.toDouble();

	double d2 = (double)1/10;

	CDuiString s35 = _T("我是s35;");
	CDuiStringA s36 = s35.convert_to_ansi();
	CDuiStringUtf8 s37 = s35.convert_to_utf8();
	CDuiStringW s38 = s35.convert_to_unicode();

	CDuiString s39(_T('a'));
	s39 += 'a';

	CDuiString s40;
	CDuiString s41 = _T("test");
	s41 = s40;

	CDuiString s42;
	s42.Format(_T("*DefaultPageRegion: w%dh%d"), 280, 510);

	//_CrtSetBreakAlloc();

	//设置主窗口的名字
	uiApp.SetAppName(_T("Test"));

	//设置程序为单一执行实例，允许多实例的话，就不需要这行了。
	uiApp.SetSingleApplication(true, _T("CBD4DD1F-3B18-4DA4-A043-0EA9AEC8D570"));

	//设置xml资源载入方式，选择一种就行	
#ifdef _DEBUG
	uiApp.SetResType_File(_T("skin"));								//从文件夹中载入xml文件
#else
	uiApp.SetResType_ZipResource(_T("skin"), _T(""), IDR_ZIPRES1, _T("ZIPRES"));	//从程序资源文件中载入xml文件
#endif

	CPaintManagerUI::SetRenderEngineType(DuiLib_Render_GdiPlus);

	//初始化duilib, 注册插件，创建主窗口
	if(!uiApp.InitInstance(hInstance)) 
		return 0;

	//进入消息循环
	uiApp.Run();

	//清理资源。
	uiApp.ExitInstance();
	return 0;
}
