#pragma once
class CWinMain
{
public:
	CWinMain(HINSTANCE hInstance);
	~CWinMain(void);

	int Run();
	void CenterWindow();
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static CString m_strSelectFile;		// 当前选择的文件
private:
	HWND m_hWndButton;
private:
	static HINSTANCE m_hInst;			// 当前实例

	HWND m_hWnd;
	TCHAR m_szTitle[255];				// 标题栏文本
	TCHAR m_szWindowClass[255];			// 主窗口类名
};

