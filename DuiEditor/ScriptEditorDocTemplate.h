#pragma once
class CScriptEditorDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CScriptEditorDocTemplate)
public:
	CScriptEditorDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CScriptEditorDocTemplate(void);

	virtual BOOL GetDocString(CString& rString, enum DocStringIndex index) const;
	virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch);
};

