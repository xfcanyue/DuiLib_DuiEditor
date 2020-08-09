#include "StdAfx.h"
#include "DuiEditorDocTemplate.h"
#include "DuiEditorDoc.h"

CDuiEditorDocTemplate::CDuiEditorDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) 
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CDuiEditorDocTemplate::~CDuiEditorDocTemplate(void)
{
}

CDocument* CDuiEditorDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bCreateFromUiTemplate, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return NULL;
	}
	ASSERT_VALID(pFrame);

	if (lpszPathName == NULL)
	{
		// create a new document - with default document name
		SetDefaultTitle(pDocument);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		CDuiEditorDoc *pDocTemp = dynamic_cast<CDuiEditorDoc *>(pDocument);
		if(pDocTemp != NULL && bCreateFromUiTemplate) //是否从模板创建
		{
			if (!pDocTemp->OnNewDocumentFromUiTemplate())
			{
				// user has be alerted to what failed in OnNewDocument
				TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
				pFrame->DestroyWindow();
				return NULL;
			}
		}
		else
		{
			if (!pDocument->OnNewDocument())
			{
				// user has be alerted to what failed in OnNewDocument
				TRACE(traceAppMsg, 0, "CDocument::OnNewDocument returned FALSE.\n");
				pFrame->DestroyWindow();
				return NULL;
			}
		}
		
		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
#ifdef DUILIB_VERSION_ORIGINAL
		DuiLib::CWaitCursor wait;
#else
		CWaitCursor wait;
#endif
		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
			pFrame->DestroyWindow();
			return NULL;
		}
		pDocument->SetPathName(lpszPathName, bAddToMRU);
		pDocument->OnDocumentEvent(CDocument::onAfterOpenDocument);
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
}
