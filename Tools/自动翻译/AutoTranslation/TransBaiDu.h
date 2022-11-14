#pragma once
class CTransBaiDu
{
public:
	CTransBaiDu(void);
	virtual ~CTransBaiDu(void);

	BOOL Translate(LPCTSTR sSource, CString &sDest);

	BOOL __Translate(LPCTSTR sSource, CString &sDest);

	CURL *curl;
	CStringA m_appid;
	CStringA m_key;
	CStringA m_from;
	CStringA m_to;
};

