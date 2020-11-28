#include "StdAfx.h"
#include "FormSciEditor.h"


CFormSciEditor::CFormSciEditor(void)
{
}


CFormSciEditor::~CFormSciEditor(void)
{
}

void CFormSciEditor::InitWindow()
{
	m_pSci = (CSciEditUI *)FindControl(_T("sciedit_1"));

	m_pSci->GetSciWnd()->InitXML(); //使用XML词法解析器
	int x = m_pSci->GetSciWnd()->sci_GetMargins();
#ifdef _DEBUG
	//载入当前的布局文件
	m_pSci->GetSciWnd()->LoadFile(GetManager()->GetResourcePath() + GetMainWnd()->GetSkinFile());
#else
	//Release版本从资源中载入文件, 
	//读入的文件是UTF8编码，不能使用CSciEdit::SetText(LPCTSTR pstrText)
	LPBYTE pByte = NULL;
	int size = CRenderEngine::LoadImage2Memory(GetMainWnd()->GetSkinFile().GetData(), 0, pByte);
	CStringA str((const char *)pByte, size);
	m_pSci->GetSciWnd()->sci_SetText(str);
	CRenderEngine::FreeMemory(pByte);
#endif
}

void CFormSciEditor::OnNotifySciNotify(TNotifyUI &msg)
{
	if(IsControl(msg, _T("sciedit_1")))
	{
		SCNotification *pMsg = (SCNotification*)msg.lParam;
		switch (pMsg->nmhdr.code)
		{
		case SCK_BACK:
			break;
		case SCN_CHARADDED:	//输入字符
			break;
		case SCN_MODIFIED:	//文档被修改
			break;
		case SCN_SAVEPOINTREACHED:	//文件被保存
			break;
		case SCN_SAVEPOINTLEFT: //文件被修改
			break;
		case SCN_UPDATEUI:
			break;
		case SCN_AUTOCSELECTION: //语法提示自动自动时
			break;
		}	
	}
}