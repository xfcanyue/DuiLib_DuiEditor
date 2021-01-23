#include "StdAfx.h"
#include "ScriptEditorDocTemplate.h"
#include "ScriptEditorDoc.h"

IMPLEMENT_DYNAMIC(CScriptEditorDocTemplate, CMultiDocTemplate)
CScriptEditorDocTemplate::CScriptEditorDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) 
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CScriptEditorDocTemplate::~CScriptEditorDocTemplate(void)
{
}

BOOL CScriptEditorDocTemplate::GetDocString(CString& rString, enum DocStringIndex i) const
{
	CString strTemp,strLeft,strRight;
	int nFindPos;
	AfxExtractSubString(strTemp, m_strDocStrings, (int)i);

	if(i == CDocTemplate::filterExt)  {
		nFindPos=strTemp.Find(';');
		if(-1 != nFindPos) {
			//string contains two extensions
			strLeft=strTemp.Left(nFindPos+1);
			strRight=strTemp.Right(lstrlen((LPCTSTR)strTemp)-nFindPos-1);
			strTemp=strLeft+strRight;
		}
	}
	rString = strTemp;
	return TRUE;

}

CDocTemplate::Confidence CScriptEditorDocTemplate::MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch)
{
	ASSERT(lpszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (pDoc->GetPathName() == lpszPathName) {
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}  // end while

	// see if it matches either suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
		!strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == '.');
		CString ext1,ext2;
		int nDot = CString(lpszPathName).ReverseFind('.');
		LPCTSTR pszDot = nDot < 0 ? NULL : lpszPathName + nDot;

		int nSemi = strFilterExt.Find(';');
		if(-1 != nSemi)   {
			// string contains two extensions
			ext1=strFilterExt.Left(nSemi);
			ext2=strFilterExt.Mid(nSemi+1);
			// check for a match against either extension
			if (nDot >= 0 && (lstrcmpi(lpszPathName+nDot, ext1) == 0
				|| lstrcmpi(lpszPathName+nDot,ext2) ==0))
				return yesAttemptNative; // extension matches
		}
		else
		{ // string contains a single extension
			if (nDot >= 0 && (lstrcmpi(lpszPathName+nDot,
				strFilterExt)==0))
				return yesAttemptNative;  // extension matches
		}
	}
	return yesAttemptForeign; //unknown document type
}