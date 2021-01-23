#pragma once
class CDuiEditorDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CDuiEditorDocTemplate)
public:
	CDuiEditorDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CDuiEditorDocTemplate(void);

	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bCreateFromUiTemplate, BOOL bAddToMRU = TRUE, BOOL bMakeVisible = TRUE);
};

