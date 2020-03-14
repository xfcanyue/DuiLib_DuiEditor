// DlgTemplateSave.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgTemplateSave.h"
#include "afxdialogex.h"


// CDlgTemplateSave 对话框

IMPLEMENT_DYNAMIC(CDlgTemplateSave, CDialogEx)

CDlgTemplateSave::CDlgTemplateSave(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTemplateSave::IDD, pParent)
{

	m_strTemplateName = _T("");
}

CDlgTemplateSave::~CDlgTemplateSave()
{
}

void CDlgTemplateSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_staPicture);
	DDX_Text(pDX, IDC_EDIT1, m_strTemplateName);
}


BEGIN_MESSAGE_MAP(CDlgTemplateSave, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgTemplateSave::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTemplateSave 消息处理程序


BOOL CDlgTemplateSave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTemplateSave::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	//遍历文件夹
	CFileFind finder;
	BOOL bFind;
	CString FileName;
	bFind = finder.FindFile(g_strAppPath + _T("DuiTemplate\\*.*"));
	while(bFind)
	{
		bFind = finder.FindNextFile();
		if(!finder.IsDots())	//是不是目录(.   ..)
		{
			if(finder.IsDirectory())	//是文件夹
			{
				if(FileName == m_strTemplateName)
				{
					AfxMessageBox(_T("已经已存在相同名字的模板"));
					return;
				}
			}
		}
	}
	finder.Close();	

	CreateDirectory(g_strAppPath + _T("DuiTemplate\\"), NULL);

	//创建目录
	CString strPath = g_strAppPath + _T("DuiTemplate\\") + m_strTemplateName;
	CreateDirectory(strPath, NULL);

	//保存截图
	m_staPicture.GetImage()->Save(strPath+_T("\\skin.jpg"), Gdiplus::ImageFormatJPEG);

	//过滤默认属性
	xml_node root = m_pDoc->m_doc.root().child(_T("Window"));
	m_pDoc->FilterDefaultValue(root);

	//创建一个拷贝, 然后保存拷贝
	xml_document doc;
	doc.root().append_copy(root);
	bool bSave = doc.save_file(strPath+_T("\\skin.xml"), PUGIXML_TEXT("\t"), format_default, encoding_utf8);
	if(!bSave)	return;


	CDialogEx::OnOK();
}
