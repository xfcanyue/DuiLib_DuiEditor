
// DuiEditor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "DuiEditor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DuiEditorDocTemplate.h"
#include "DuiEditorDoc.h"
#include "DuiEditorView.h"
#include "DuiEditorViewDesign.h"

#include "ChildFrm2.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "ScriptEditorDocTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

CDuiProject g_proj;
CString g_strAppPath;
CDuiPropertyFile g_duiProp;
//IScriptEngine *g_pScriptEngine = NULL;
//IScriptHelper *ScriptHelper = NULL;
HWND g_hWndMsg = NULL;

// CDuiEditorApp

BEGIN_MESSAGE_MAP(CDuiEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CDuiEditorApp::OnAppAbout)
	// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, &CDuiEditorApp::OnUpdateRecentFileMenu)
	ON_COMMAND(ID_FILE_NEW, &CDuiEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CDuiEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW_XML, &CDuiEditorApp::OnFileNewXml)
	ON_COMMAND(ID_FILE_LOAD_TEMPLATE, &CDuiEditorApp::OnFileNewXmlFromTemplate)
	ON_COMMAND(ID_FILE_NEW_AS_SCRIPT, &CDuiEditorApp::OnFileNewAsScript)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_AS_SCRIPT, &CDuiEditorApp::OnUpdateFileNewAsScript)
END_MESSAGE_MAP()


// CDuiEditorApp 构造

CDuiEditorApp::CDuiEditorApp()
{
	m_hModuleScript = NULL;

	m_bHiColorIcons = TRUE;

	m_bIsDDEOpen = FALSE;
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DuiEditor.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CDuiEditorApp 对象

CDuiEditorApp theApp;


// CDuiEditorApp 初始化

BOOL CDuiEditorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	AfxInitRichEdit2();

	//_CrtSetBreakAlloc(528);

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	CoInitialize(NULL);
	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	//CShadowUI::Initialize(m_hInstance);
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("DuiEditor"));
	LoadStdProfileSettings(16);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	free((void*)m_pszAppName);
	m_pszAppName=_tcsdup(_T("DuiEditor"));

	//获取程序路径
	TCHAR tszModule[MAX_PATH + 1] = { 0 };
	::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
	g_strAppPath = tszModule;
	int pos = g_strAppPath.ReverseFind(_T('\\'));
	if( pos >= 0 ) g_strAppPath = g_strAppPath.Left(pos + 1);

	if(!g_duiProp.LoadPropertyFile())
	{
		return FALSE;
	}

	g_cfg.LoadConfig(g_strAppPath + _T("DuiEditor.xml"));

#ifndef DUILIB_VERSION_ORIGINAL

	//插件方式载入扩展控件
#ifdef _DEBUG
	CPaintManagerUI::LoadPlugin(g_strAppPath + _T("DuiPlugins_ud.dll"));
#else
	CPaintManagerUI::LoadPlugin(g_strAppPath + _T("DuiPlugins_u.dll"));
#endif

	//把脚本插件加进来
#ifdef _DEBUG
	m_hModuleScript = ::LoadLibrary(g_strAppPath +_T("DuiScript_ud.dll"));
#else
	m_hModuleScript = ::LoadLibrary(g_strAppPath +_T("DuiScript_u.dll"));
#endif
	if(m_hModuleScript)
	{
		m_funCreateScriptHelper = (CREATE_SCRIPT_HELPER)::GetProcAddress(m_hModuleScript, "CreateScriptHelper");
		m_funDeleteScriptHelper = (DELETE_SCRIPT_HELPER)::GetProcAddress(m_hModuleScript, "DeleteScriptHelper");
	}

	//工厂模式注册扩展控件
	DuiPluginsRegister();

	//调整DPI时，仅缩放显示图片。
	CPaintManagerUI::SetAdjustDPIRecource(false);

#endif //#ifndef DUILIB_VERSION_ORIGINAL

	
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CDuiEditorDocTemplate(IDR_DuiEditorTYPE,
		RUNTIME_CLASS(CDuiEditorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CDuiEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CScriptEditorDocTemplate(IDR_DuiScriptTYPE,
		RUNTIME_CLASS(CScriptEditorDoc),
		RUNTIME_CLASS(CChildFrame2), // 自定义 MDI 子框架
		RUNTIME_CLASS(CScriptEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen || cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand =  CCommandLineInfo::FileNothing;

	//不知道为什么，这里打开多个文件，mdi tabs不会刷新 -_-!

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE)
	{
		m_bIsDDEOpen = TRUE;
		//AfxMessageBox(_T("cmdInfo.m_nShellCommand == CCommandLineInfo::FileDDE"));
		//AfxMessageBox(GetCommandLine());
	}

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(SW_MAXIMIZE); //m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CDuiEditorApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
// 	if(m_funDeleteScriptHelper)
// 	{
// 		(*m_funDeleteScriptHelper)(ScriptHelper);
// 		ScriptHelper= NULL;
// 	}

	CPaintManagerUI::Term();
	CoUninitialize();
	AfxOleTerm(FALSE);
	CleanState();
	return CWinAppEx::ExitInstance();
}

BOOL CDuiEditorApp::ProcessSessionFileList()
{
//	if(m_bIsDDEOpen) return TRUE;
	xml_node nodeSession = g_cfg.Session();
	if(!nodeSession.child("File")) 
	{
		//AfxMessageBox(_T("if(!nodeSession.child(\"File\"))"));
		return FALSE;
	}

	BOOL bMoveNext = TRUE;
	xml_node node=nodeSession.child("File");
	while (node)
	{
		CString filename = LSUTF82T(node.attribute("filename").as_string());
		if(PathFileExists(filename))
		{		
			CDocTemplate* pTemplate;
			POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
			while(pos != NULL)
			{
				pTemplate = m_pDocManager->GetNextDocTemplate(pos);
				CDocument *pDocument = NULL;
				CDocTemplate::Confidence match = pTemplate->MatchDocType(filename, pDocument);
				if(match == CDocTemplate::yesAlreadyOpen) 
				{
					//不知道为什么重复了，删除这个节点
					xml_node nodeTemp = node.next_sibling("File");
					nodeSession.remove_child(node);
					bMoveNext = FALSE;
					node = nodeTemp;
					g_cfg.SaveConfig();
					break;
				}
				if(match == CDocTemplate::yesAttemptNative)
				{
					pTemplate->OpenDocumentFile(filename, TRUE, TRUE);
					break;
				}
			}
		}
		else
		{
			//文件不存在，删除这个节点
			xml_node nodeTemp = node.next_sibling("File");
			nodeSession.remove_child(node);
			bMoveNext = FALSE;
			node = nodeTemp;
			g_cfg.SaveConfig();
		}

		if(bMoveNext)
			node=node.next_sibling("File");
		bMoveNext = TRUE;
	}


	return TRUE;
}

void CDuiEditorApp::OpenSessionFile(LPCTSTR lpszFileName)
{
	OpenDocumentFile(lpszFileName);
}

CDocTemplate *CDuiEditorApp::GetUIDocTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			return pTemplate;
		}
	}
	return NULL;
}

CDocTemplate *CDuiEditorApp::GetScriptDocTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CScriptEditorDocTemplate)))
		{
			return pTemplate;
		}
	}
	return NULL;
}
// CDuiEditorApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CDuiEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDuiEditorApp 自定义加载/保存方法

void CDuiEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CDuiEditorApp::LoadCustomState()
{
}

void CDuiEditorApp::SaveCustomState()
{
	
}

void CDuiEditorApp::LoadStdProfileSettings(UINT nMaxMRU)
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	BOOL bNoRecentDocs = FALSE;
	GetSysPolicyValue(_AFX_SYSPOLICY_NORECENTDOCHISTORY, &bNoRecentDocs);
	if (nMaxMRU != 0 && !bNoRecentDocs )
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry,
			nMaxMRU, MAX_PATH);
		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

void CDuiEditorApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	/*
	CMenu *pMenu = pCmdUI->m_pMenu;
	if(pMenu != NULL)
	{
		int count = pMenu->GetMenuItemCount();
		for (int i=0; i<count; i++)
		{
			CString menustr;
			pMenu->GetMenuString(i, menustr, MF_BYPOSITION);
			{
				if (menustr == "最近的文件")
				{
					pCmdUI->m_pMenu = pMenu->GetSubMenu(i);
					CWinAppEx::OnUpdateRecentFileMenu(pCmdUI);
					pCmdUI->m_pMenu = pMenu;
					return;
				}
			}
		}
	}
	*/
	CWinAppEx::OnUpdateRecentFileMenu(pCmdUI);
}



void CDuiEditorApp::OnFileNew()
{
	CWinAppEx::OnFileNew();
}


void CDuiEditorApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}


void CDuiEditorApp::OnFileNewXml()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			pTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}

void CDuiEditorApp::OnFileNewXmlFromTemplate()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CDuiEditorDocTemplate)))
		{
			CDuiEditorDocTemplate *pTemp = dynamic_cast<CDuiEditorDocTemplate *>(pTemplate);
			if(pTemp != NULL)
			{
				pTemp->OpenDocumentFile(NULL, TRUE, TRUE, TRUE);
			}
			else
			{
				pTemplate->OpenDocumentFile(NULL);
			}
			return;
		}
	}
}

void CDuiEditorApp::OnFileNewAsScript()
{
	CDocTemplate* pTemplate;
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while(pos != NULL)
	{
		pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		if(pTemplate->IsKindOf(RUNTIME_CLASS(CScriptEditorDocTemplate)))
		{
			pTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
}


void CDuiEditorApp::OnUpdateFileNewAsScript(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(FALSE);
}
