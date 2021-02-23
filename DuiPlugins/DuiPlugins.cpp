// DuiPlugins.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DuiPlugins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CDuiPluginsApp

BEGIN_MESSAGE_MAP(CDuiPluginsApp, CWinApp)
END_MESSAGE_MAP()


// CDuiPluginsApp 构造

CDuiPluginsApp::CDuiPluginsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDuiPluginsApp 对象

CDuiPluginsApp theApp;


// CDuiPluginsApp 初始化

BOOL CDuiPluginsApp::InitInstance()
{
	CWinApp::InitInstance();

	CString temp;
	TCHAR czProgName[MAX_PATH];
	int  iNiValue = GetModuleFileName(m_hInstance, czProgName, MAX_PATH);
	if(iNiValue > 0)
	{
		temp = czProgName;
		g_strDuiPluginsPath = temp.Left(temp.ReverseFind('\\') + 1);
	}
	return TRUE;
}

#include "Include/DuiPlugins.h"


UILIB_PLUGIN_API void DuiPluginsRegister()
{
	REGIST_DUICONTROL(CGridCtrlUI);
//	REGIST_DUICONTROL(CMsgWndUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CKeyBoardNumberUI);
	REGIST_DUICONTROL(CKeyBoardSimpleUI);
	REGIST_DUICONTROL(CImageBoxUI);
	REGIST_DUICONTROL(CPictureControlUI);
// 	REGIST_DUICONTROL(CComboExUI);
// 	REGIST_DUICONTROL(CIconButtonUI);
// 	REGIST_DUICONTROL(CDateTimeExUI);
// 	REGIST_DUICONTROL(CAccordionPaneUI);
// 	REGIST_DUICONTROL(CImageBoxExUI);
// 	REGIST_DUICONTROL(CRollTextExUI);
	REGIST_DUICONTROL(CSciEditUI);
	REGIST_DUICONTROL(CGridListUI);
	REGIST_DUICONTROL(CGridListHeaderUI);
	REGIST_DUICONTROL(CGridListBodyUI);
	REGIST_DUICONTROL(CGridListRowUI);
	REGIST_DUICONTROL(CGridListCellUI);
}


extern "C" UILIB_PLUGIN_API CControlUI *CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("GridCtrl")) == 0 ) 
	{
		return new CGridCtrlUI;
	}
// 	else if( _tcscmp(pstrClass, _T("MsgWnd")) == 0 ) 
// 	{
// 		return new CMsgWndUI;
// 	}
	else if( _tcscmp(pstrClass, _T("KeyBoard")) == 0 ) 
	{
		return new CKeyBoardUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardNumber")) == 0 ) 
	{
		return new CKeyBoardNumberUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardSimple")) == 0 ) 
	{
		return new CKeyBoardSimpleUI;
	}
	else if( _tcscmp(pstrClass, _T("ImageBox")) == 0 ) 
	{
		return new CImageBoxUI;
	}
	else if( _tcscmp(pstrClass, _T("PictureControl")) == 0 ) 
	{
		return new CPictureControlUI;
	}
	else if( _tcscmp(pstrClass, _T("ComboEx")) == 0 ) 
	{
		return new CComboExUI;
	}
// 	else if( _tcscmp(pstrClass, _T("IconButton")) == 0 ) 
// 	{
// 		return new CIconButtonUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("DateTimeEx")) == 0 ) 
// 	{
// 		return new CDateTimeExUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("AccordionPane")) == 0 ) 
// 	{
// 		return new CAccordionPaneUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("ImageBoxEx")) == 0 ) 
// 	{
// 		return new CImageBoxExUI;
// 	}
// 	else if( _tcscmp(pstrClass, _T("RollTextEx")) == 0 ) 
// 	{
// 		return new CRollTextExUI;
// 	}
	else if( _tcscmp(pstrClass, _T("SciEdit")) == 0 ) 
	{
		return new CSciEditUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLIST) == 0 ) 
	{
		return new CGridListUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTHEADER) == 0 ) 
	{
		return new CGridListHeaderUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTBODY) == 0 ) 
	{
		return new CGridListBodyUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTROW) == 0 ) 
	{
		return new CGridListRowUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTCELL) == 0 ) 
	{
		return new CGridListCellUI;
	}
	return NULL;
}

UILIB_PLUGIN_API void InsertMsgUI(LPCTSTR pstring, COLORREF cr)
{
	CMsgWndUI::InsertMsg(pstring, cr);
}

UILIB_PLUGIN_API void InsertMsgUiV(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	CMsgWndUI::InsertMsg(strText, RGB(0,0,0));
}