#include "StdAfx.h"
#include "DuiPreviewer.h"
#include "WinMain.h"

HINSTANCE CWinMain::m_hInst = NULL;
CString CWinMain::m_strSelectFile;

CWinMain::CWinMain(HINSTANCE hInstance)
{
	m_hWndButton = NULL;

	m_hInst = hInstance;
	m_hWnd = NULL;
	LoadString(m_hInst, IDS_APP_TITLE, m_szTitle, 255);
	LoadString(hInstance, IDC_DUIPREVIEWER, m_szWindowClass, 255);
}


CWinMain::~CWinMain(void)
{
}

int CWinMain::Run()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInst;
	wcex.hIcon			= LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_DUIPREVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DUIPREVIEWER);
	wcex.lpszClassName	= m_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	// 执行应用程序初始化:
	m_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 400, 200,
		NULL, NULL, m_hInst, NULL);
	if (!m_hWnd)
	{
		return -1;
	}
	CenterWindow();
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDC_DUIPREVIEWER));

	// 主消息循环:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

void CWinMain::CenterWindow()
{
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWnd=m_hWnd;
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
	if (hWndCenter!=NULL)
		hWnd=hWndCenter;

	// 处理多显示器模式下屏幕居中
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
	rcArea = oMonitor.rcWork;

	if( hWndCenter == NULL || IsIconic(hWndCenter))
		rcCenter = rcArea;
	else
		::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left ) xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

#define BTN_OPEN	100
LRESULT CALLBACK CWinMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			CreateWindow(_T("Button"), _T("打开文件"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
			100, 45, 200, 65, hWnd, (HMENU)BTN_OPEN, m_hInst, NULL);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if(wmId == BTN_OPEN)
		{
			CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

			if(dlg.DoModal()==IDOK)
			{
				m_strSelectFile = dlg.GetPathName();
			}
			DestroyWindow(hWnd);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}