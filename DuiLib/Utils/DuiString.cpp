#include "StdAfx.h"

namespace DuiLib
{

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	struct duistringdata : public ILinkedList
	{
		//friend class duistringmem;
	protected:
		int nRefs;				//引用次数
		int nDataLength;
		UINT nAllocLength;		//分配的长度, 可能大于字符串实际长度。
	public:
		void AddRef() 
		{ 
			if(IsLock()) 
				return;
			nRefs++; 
		}
		void ReleaseRef() 
		{ 
			if(IsLock()) 
				return;
			nRefs--; 
		}
		BOOL IsNeedFree() { return nRefs == 0; }
		BOOL IsShared() { return nRefs > 1; }
		BOOL IsLock() { return nRefs < 0; }
		void Lock() { nRefs = -1; }
		void SetAllocLength(int nAlloc) { nAllocLength = nAlloc;}
		int GetAllocLength() { return nAllocLength; }
		int GetDataLength() const	{ return nDataLength; }
		void SetDataLength(int n)	{ nDataLength = n; }
		LPBYTE data()			// TCHAR* to managed data
		{ return LPBYTE(this+1); }
	};

	//////////////////////////////////////////////////////////////////////////
	class duistringmem
	{
	public:
		duistringmem()
		{
			m_nextCount16	= 128;
			m_nextCount32	= 128;
			m_nextCount64	= 64;
			m_nextCount128	= 32;
			MakeEmptyString();
#ifdef _DEBUG
//			OutputDebugStringA("construct duistringmem\r\n");
#endif
		}

		~duistringmem()
		{
			for (int i=0; i<m_listMem.GetSize(); i++)
			{
				free( (BYTE*)m_listMem[i] );
			}
			free((BYTE* )m_pEmptyString);
#ifdef _DEBUG
//			OutputDebugStringA("destroy duistringmem\r\n");
#endif
		}

		 //分配字符串内存，申请时strlength = 字符串长度 + 1
		duistringdata *Alloc(int strlength)
		{
			CDuiInnerLock lock(&m_lock);
			duistringdata *s = NULL;
			if(strlength <= 16)
			{
				s = MakeStringData(m_list16, 16, m_nextCount16);
			}
			else if(strlength <= 32)
			{
				s = MakeStringData(m_list32, 32, m_nextCount32);
			}
			else if(strlength <= 64)
			{
				s = MakeStringData(m_list64, 64, m_nextCount64);
			}
			else if(strlength <= 128)
			{
				s = MakeStringData(m_list128, 128, m_nextCount128);
			}
			else
			{
				int memsize = sizeof(duistringdata) + strlength;
				s = (duistringdata *)malloc(memsize);
				memset(static_cast<void*>(s), 0, memsize);
				s->SetAllocLength(strlength);				
			}

			ASSERT(s);
			if(s)
			{
				s->SetDataLength(strlength);
				s->AddRef();
			}
			return s;
		}

		void Free(duistringdata *s)
		{
			if(s->IsLock())
				return;

			if(s == m_pEmptyString)
			{
				//空字符串不处理
			}
			else if(s->GetAllocLength() <= 16)
			{
				RecoverStringData(m_list16, 16, s);
			}
			else if(s->GetAllocLength() <= 32)
			{
				RecoverStringData(m_list32, 32, s);
			}
			else if(s->GetAllocLength() <= 64)
			{
				RecoverStringData(m_list64, 64, s);
			}
			else if(s->GetAllocLength() <= 128)
			{
				RecoverStringData(m_list128, 128, s);
			}
			else
			{
				CDuiInnerLock lock(&m_lock);
				s->ReleaseRef();
				if(s->IsNeedFree())
					free((BYTE *)s);
			}
		}

		LPBYTE GetEmptyString()
		{
			return m_pEmptyString->data();
		}
	protected:
		void MakeEmptyString()
		{
			int memsize = sizeof(duistringdata) + 16;
			m_pEmptyString = (duistringdata *)malloc(memsize);
			memset(static_cast<void*>(m_pEmptyString), 0, memsize);
			m_pEmptyString->SetAllocLength(16);
			m_pEmptyString->Lock();
		}

		duistringdata *MakeStringData(CStdLinkList<duistringdata> &listdata, int nAllocLength, int &nMakeCount)
		{
			if(listdata.empty())
			{
				int memsize = sizeof(duistringdata) + nAllocLength;
				BYTE *pBlock = (BYTE *)malloc(memsize * nMakeCount); //每次申请 nMakeCount 个字符串
				memset(pBlock, 0, memsize * nMakeCount);
				m_listMem.Add(pBlock);

				for (int i=0; i<nMakeCount; i++)
				{
					duistringdata *s = (duistringdata *)(pBlock + (i * memsize));
					s->SetAllocLength(nAllocLength);
					listdata.push_back(s);
				}

				nMakeCount *= 2;

#ifdef _DEBUG
// 				CString ss;
// 				ss.Format(_T("分配字符串内存池，长度=%d, 个数=%d。\r\n"), strlength, nMakeCount);
// 				OutputDebugString(ss);
#endif
			}
#ifdef _DEBUG
			duistringdata *data = listdata.pop_back();
// 			CString ss;
// 			ss.Format(_T("获取字符串(%d), 剩余空闲=%d。\r\n"), strlength, listdata.size());
// 			OutputDebugString(ss);
			return data;
#else
			return listdata.pop_back();
#endif
		}

		void RecoverStringData(CStdLinkList<duistringdata> &listdata, int strlength, duistringdata *s)
		{
			CDuiInnerLock lock(&m_lock);
			s->ReleaseRef();
			if(s->IsNeedFree())
			{
				memset(s->data(), 0, s->GetAllocLength());
				s->SetDataLength(0);
				listdata.push_back(s);
#ifdef _DEBUG
// 				CString ss;
// 				ss.Format(_T("回收字符串(%d), 剩余空闲=%d。\r\n"), strlength, listdata.size());
// 				OutputDebugString(ss);
#endif
			}
		}
	protected:
		CDuiLock m_lock;
		duistringdata *m_pEmptyString;				//空字符串
		CStdPtrArray m_listMem;						//已申请的内存块列表
		CStdLinkList<duistringdata> m_list16;		//空闲的长度为16的字符串列表
		int m_nextCount16;
		CStdLinkList<duistringdata> m_list32;		//空闲的长度为32的字符串列表
		int m_nextCount32;
		CStdLinkList<duistringdata> m_list64;		//空闲的长度为64的字符串列表
		int m_nextCount64;
		CStdLinkList<duistringdata> m_list128;		//空闲的长度为128的字符串列表
		int m_nextCount128;
	};

	//////////////////////////////////////////////////////////////////////////
	class DuiStringMgr
	{
	public:
		static duistringmem* GetInstance()
		{
			//确保在每个string构造之前构造，确保在每个string析构之后析构
			static duistringmem smem;
			return &smem;
		}

		static duistringdata *GetStringData(const void *pstr)
		{
			return ((duistringdata *)pstr)-1;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//
	size_t DuiStringTraitsA::ui_strlen(const char *pstr)							{ return pstr ? ::strlen(pstr) : 0; }
	char *DuiStringTraitsA::ui_strchr(char *string, int ch)							{ return ::strchr(string, ch); }
	char *DuiStringTraitsA::ui_strrchr(char *string, int ch)						{ return ::strrchr(string, ch); }
	char *DuiStringTraitsA::ui_strstr(char *_Str, const char *_SubStr)				{ return ::strstr(_Str, _SubStr); }
	char *DuiStringTraitsA::ui_strrstr(char *_Str, const char *_SubStr)				
	{ 
		int len = ui_strlen(_Str);
		if(len == 0) 
			return NULL;

		char *cpstart = _Str;
		char *cp = (char *) _Str + len - 1;
		char *s1, *s2;

		if ( !*_SubStr )
			return((char *)_Str);

		while (*cp)
		{
			s1 = cp;
			s2 = (char *) _SubStr;

			while ( *s1 && *s2 && !(*s1-*s2) )
				s1++, s2++;

			if (!*s2)
				return(cp);

			if(cp == cpstart)
				break;

			cp--;
		}

		return(NULL);
	}
	char *DuiStringTraitsA::ui_strcat(char *dst, const char *src)					{ return ::strcat(dst, src); }
	char *DuiStringTraitsA::ui_strncat(char *dst, const char *src, size_t n)		{ return ::strncat(dst, src, n); }
	char *DuiStringTraitsA::ui_strcpy(char *dest,const char *source)				{ return ::strcpy(dest, source); }
	char *DuiStringTraitsA::ui_strncpy(char *dest,const char *source, size_t count) { return ::strncpy(dest, source, count); }
	int DuiStringTraitsA::ui_strcmp (const char *src, const char *dst)				
	{ 
		if(!src && !dst) return 0;
		if(!src && dst) return -1;
		if(src && !dst) return 1;
		return ::strcmp(src, dst); 
	}

	int DuiStringTraitsA::ui_stricmp (const char *src, const char *dst) 
	{
		if(!src && !dst) return 0;
		if(!src && dst) return -1;
		if(src && !dst) return 1;
#ifdef WIN32
		return ::stricmp(src, dst); 
#else
		return ::strcasecmp(src, dst);
#endif
	}

	char *DuiStringTraitsA::ui_strupr(char *str)		
	{
#ifdef WIN32
		return ::strupr(str); 
#else
		char *pTemp = str;
		while (*pTemp != '\0')
		{
			if (*pTemp >= 'a' && *pTemp <= 'z') {
				//*src -= 32;
				*pTemp = static_cast<char>(*pTemp - 32);
			}
			pTemp++;
		}
		return str;
#endif
	}

	char *DuiStringTraitsA::ui_strlwr(char *str)		
	{
#ifdef WIN32
		return ::strlwr(str);
#else
		char *pTemp = str;
		while (*pTemp != '\0')
		{
			if (*pTemp >= 'A' && *pTemp <= 'Z') {
				*pTemp = static_cast<char>(*pTemp + 32);
			}
			pTemp++;
		}
		return str;
#endif
	}

	int DuiStringTraitsA::ui_atoi(const char *str)		{ return ::atoi(str); }
	DuiLib::Int64 ui_atoi64(const char *str)			
	{ 
#ifdef WIN32
		return _atoi64(str);
#else
		return (DuiLib::Int64)strtoll(str, NULL, 10);
#endif
	}
	float DuiStringTraitsA::ui_atof(const char *str)	{ return static_cast<float>(::atof(str)); }
	double DuiStringTraitsA::ui_strtod (const char *nptr) { return ::strtod(nptr, 0); }

	int __cdecl DuiStringTraitsA::formatV(char *&pstr, const char *pstrFormat, va_list Args)
	{
#if _MSC_VER <= 1400
		int nLen = 0;
		while (TRUE)
		{
			int buffLen = GetBufferLength(pstr);
			va_list cpArgs;
			va_copy(cpArgs, Args);
			nLen = format(pstr, buffLen, pstrFormat, cpArgs);
			va_end(cpArgs);
			if (nLen != -1 && nLen < buffLen)
				break;
			if (nLen == -1)
				break;
			AllocString(pstr, nLen+1, sizeof(char));
		}
		return nLen;
#else
		int nLen = format(NULL, 0, pstrFormat, Args);
		AllocString(pstr, nLen, sizeof(char));
		nLen = format(pstr, GetBufferLength(pstr), pstrFormat, Args);
		return nLen;
#endif
	}

	int __cdecl DuiStringTraitsA::format(char *string, size_t count, const char *format, va_list ap) 
	{ 
#ifdef WIN32
		return ::_vsnprintf(string, count, format, ap);
#else
		return ::vsnprintf(string, count, format, ap);
#endif
	}

	void DuiStringTraitsA::bool_to_string(char *string, bool b) 
	{ 
		ui_strcpy(string, b ? "true" : "false");
	}

	void DuiStringTraitsA::int_to_string(char *str, int n)
	{
		sprintf(str, "%d", n);
	}

	void DuiStringTraitsA::int64_to_string(char *str, DuiLib::Int64 n)
	{
		sprintf(str, "%I64d", n);
	}

	void DuiStringTraitsA::double_to_string(char *str, double s, int base)
	{ 
		if(base >= 0) 
		{ 
			char sFormat[32]; 
			sprintf(sFormat, "%%.%df", base); 
			sprintf(str, sFormat, s); 
		}
		else 
		{ 
			sprintf(str, "%f", s); 
		}
	}

	void DuiStringTraitsA::float_to_string(char *str, float s, int base)
	{ 
		if(base >= 0) 
		{ 
			char sFormat[32]; 
			sprintf(sFormat, "%%.%df", base); 
			sprintf(str, sFormat, s); 
		}
		else 
		{ 
			sprintf(str, "%f", s); 
		}
	}

	char *DuiStringTraitsA::GetNullString() { return (char *)DuiStringMgr::GetInstance()->GetEmptyString(); }

	void DuiStringTraitsA::AllocString(char *&pstr, int nStrLength, int nSizeOfChar)
	{
		duistringdata *mem = DuiStringMgr::GetStringData(pstr);
		UINT nNeedAlloc = (nStrLength + 1) * nSizeOfChar;

		// 长度不够申请新的,
		// 引用大于1，也要申请新的. 引用大于1时任何修改字符串都要自己建新的，不要把别人的改了。
		if(mem->GetAllocLength() < nNeedAlloc || mem->IsShared() || mem->IsLock())
		{
			duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(nNeedAlloc);

			int oldLen = DuiStringTraitsA::ui_strlen(pstr);
			if(oldLen > 0 && oldLen <= nStrLength) //新分配的字符串短，原来的字符串长，就越界了！！！
			{
				DuiStringTraitsA::ui_strcpy((char *)newdata->data(), pstr);
			}

			DuiStringMgr::GetInstance()->Free(mem);
			pstr = (char *)newdata->data();
		}
		else
		{
			mem->SetDataLength(nNeedAlloc);
		}
	}

	void DuiStringTraitsA::FreeString(char *&pstr)
	{
		if(pstr)
		{
			duistringdata *data = DuiStringMgr::GetStringData(pstr);
			if(!data->IsLock())
			{
				DuiStringMgr::GetInstance()->Free(data);
				pstr = GetNullString();
			}
		}
	}

	int DuiStringTraitsA::GetBufferLength(char *pstr)
	{
		duistringdata *data = DuiStringMgr::GetStringData(pstr);
		return data->GetDataLength() / sizeof(char);
	}

	void DuiStringTraitsA::Empty(char *&pstr)
	{
		if(ui_strlen(pstr) > 0)
		{
			duistringdata *data = DuiStringMgr::GetStringData(pstr);
			if(data->IsShared())
				FreeString(pstr);
			else
				memset(data->data(), 0, data->GetDataLength());
		}
	}

	void DuiStringTraitsA::SetAt(char *&pstr, int nIndex, char ch)
	{
		if(nIndex < 0 || nIndex >= ui_strlen(pstr))
			return;

		duistringdata *data = DuiStringMgr::GetStringData(pstr);
		if(data->IsShared())
		{
			AllocString(pstr, ui_strlen(pstr), sizeof(char));
		}
		pstr[nIndex] = ch;
	}

	void DuiStringTraitsA::assign_string(char *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding, BOOL bMemString)
	{
		if(src_strlength == 0)
		{
			FreeString(pstr);
			return;
		}

		if(dst_encoding == src_encoding)
		{
			if(bMemString)
			{
				duistringdata *src = DuiStringMgr::GetStringData(src_string);
				FreeString(pstr);
				pstr = (char *)src->data();
				src->AddRef();
				return;
			}

			AllocString(pstr, src_strlength, sizeof(char));
			ui_strncpy(pstr, (char *)src_string, src_strlength);
			pstr[src_strlength] = '\0';
			return;
		}

		StringConverterUI conv;
		const char *pNewString = NULL;
		if(dst_encoding == duistring_encoding_ansi)
		{
			if(src_encoding == duistring_encoding_ansi)
			{

			}
			else if(src_encoding == duistring_encoding_utf8)
			{
				pNewString = conv.utf8_to_A(src_string, src_strlength);
				assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_ansi, FALSE);
			}
			else if(src_encoding == duistring_encoding_unicode)
			{
				pNewString = conv.W_to_A(src_string, src_strlength);
				assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_ansi, FALSE);
			}
		}
		else if(dst_encoding == duistring_encoding_utf8)
		{
			if(src_encoding == duistring_encoding_ansi)
			{
				pNewString = conv.A_to_utf8(src_string, src_strlength);
				assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_utf8, FALSE);
			}
			else if(src_encoding == duistring_encoding_utf8)
			{

			}
			else if(src_encoding == duistring_encoding_unicode)
			{
				pNewString = conv.W_to_utf8(src_string, src_strlength);
				assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_utf8, FALSE);
			}
		}		
	}

	void DuiStringTraitsA::append_string(char *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding)
	{
		if(src_strlength == 0) return;

		if(dst_encoding == src_encoding)
		{
			int oldLen = ui_strlen(pstr);
			AllocString(pstr, oldLen + src_strlength,  sizeof(char));
			ui_strncat(pstr, (char *)src_string, src_strlength);
			pstr[oldLen + src_strlength] = '\0';
			return;
		}

		StringConverterUI conv;
		if(dst_encoding == duistring_encoding_ansi)
		{
			if(src_encoding == duistring_encoding_ansi)
			{

			}
			else if(src_encoding == duistring_encoding_utf8)
			{
				const char *pNewString = conv.utf8_to_A(src_string, src_strlength);
				int oldLen = ui_strlen(pstr);
				AllocString(pstr, oldLen + src_strlength, sizeof(char));
				ui_strncat((char *)pstr, pNewString, src_strlength);
				pstr[oldLen + src_strlength] = '\0';
			}
			else if(src_encoding == duistring_encoding_unicode)
			{
				const char *pNewString = conv.W_to_A(src_string, src_strlength);
				int oldLen = ui_strlen(pstr);
				AllocString(pstr, oldLen + src_strlength, sizeof(char));
				ui_strncat((char *)pstr, pNewString, src_strlength);
				pstr[oldLen + src_strlength] = '\0';
			}
		}
		else if(dst_encoding == duistring_encoding_utf8)
		{
			if(src_encoding == duistring_encoding_ansi)
			{
				const char *pNewString = conv.A_to_utf8(src_string, src_strlength);
				int oldLen = ui_strlen(pstr);
				AllocString(pstr, oldLen + src_strlength, sizeof(char));
				ui_strncat((char *)pstr, pNewString, src_strlength);
				pstr[oldLen + src_strlength] = '\0';
			}
			else if(src_encoding == duistring_encoding_utf8)
			{

			}
			else if(src_encoding == duistring_encoding_unicode)
			{
				const char *pNewString = conv.W_to_utf8(src_string, src_strlength);
				int oldLen = ui_strlen(pstr);
				AllocString(pstr, oldLen + src_strlength, sizeof(char));
				ui_strncat((char *)pstr, pNewString, src_strlength);
				pstr[oldLen + src_strlength] = '\0';
			}
		}
	}

	void DuiStringTraitsA::TrimLeft(char *&pstr)
	{
		const char *lpsz = pstr;
		while (isspace(*lpsz))
			lpsz++;

		int nNewLen = strlen(lpsz);
		char *pNewString = GetNullString();
		AllocString(pNewString, nNewLen, sizeof(char));
		ui_strcpy(pNewString, lpsz);
		ui_strcpy(pstr, pNewString);
		FreeString(pNewString);
	}

	void DuiStringTraitsA::TrimRight(char *&pstr)
	{
		int nStrLen = strlen(pstr);
		for (int i=nStrLen-1; i>=0; i--)
		{
			if(isspace(pstr[i]))
				pstr[i] = '\0';
			else
				break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	//
	size_t DuiStringTraitsW::ui_strlen(const wchar_t *pstr)						{ return pstr ? ::wcslen(pstr) : 0; }
	wchar_t *DuiStringTraitsW::ui_strchr(wchar_t *string, int ch)				{ return ::wcschr(string, ch); }
	wchar_t *DuiStringTraitsW::ui_strrchr(wchar_t *string, int ch)				{ return ::wcsrchr(string, ch); }
	wchar_t *DuiStringTraitsW::ui_strstr(wchar_t *_Str, const wchar_t *_SubStr) { return ::wcsstr(_Str, _SubStr); }
	wchar_t *DuiStringTraitsW::ui_strrstr(wchar_t *_Str, const wchar_t *_SubStr) 
	{ 
		int len = ui_strlen(_Str);
		if(len == 0) 
			return NULL;

		wchar_t *cpstart = _Str;
		wchar_t *cp = (wchar_t *)_Str + len - 1;
		wchar_t *s1, *s2;

		if ( !*_SubStr )
			return((wchar_t *)_Str);

		while (*cp)
		{
			s1 = cp;
			s2 = (wchar_t *) _SubStr;

			while ( *s1 && *s2 && !(*s1-*s2) )
				s1++, s2++;

			if (!*s2)
				return(cp);

			if(cp == cpstart)
				break;

			cp--;
		}

		return(NULL);
	}
	wchar_t *DuiStringTraitsW::ui_strcat(wchar_t *dst, const wchar_t *src)		{ return ::wcscat(dst, src); }
	wchar_t *DuiStringTraitsW::ui_strncat(wchar_t *dst, const wchar_t *src, size_t n)	{ return ::wcsncat(dst, src, n); }
	wchar_t *DuiStringTraitsW::ui_strcpy(wchar_t *dest, const wchar_t *source)	{ return ::wcscpy(dest, source); }
	wchar_t *DuiStringTraitsW::ui_strncpy(wchar_t *dest, const wchar_t *source, size_t count) { return ::wcsncpy(dest, source, count); }
	int DuiStringTraitsW::ui_strcmp(const wchar_t *src, const wchar_t *dst)		
	{ 
		if(!src && !dst) return 0;
		if(!src && dst) return -1;
		if(src && !dst) return 1;
		return ::wcscmp(src, dst); 
	}
	int DuiStringTraitsW::ui_stricmp(const wchar_t *src, const wchar_t *dst) 
	{  
		if(!src && !dst) return 0;
		if(!src && dst) return -1;
		if(src && !dst) return 1;
#ifdef WIN32
		return ::wcsicmp(src, dst); 
#else
		int f, l;
		do
		{
			if (((f = (wchar_t)(*(dst++))) >= 'A') && (f <= 'Z'))
				f -= L'A' - L'a';
			if (((l = (wchar_t)(*(src++))) >= L'A') && (l <= L'Z'))
				l -= 'A' - 'a';
		} while (f && (f == l));
		return(f - l);
#endif
	}

	wchar_t *DuiStringTraitsW::ui_strupr(wchar_t *str) 
	{ 
#ifdef WIN32
		return ::wcsupr(str); 
#else
		wchar_t *pTemp = str;
		while (*pTemp != '\0')
		{
			if (*pTemp >= 'a' && *pTemp <= 'z') {
				*pTemp = static_cast<wchar_t>(*pTemp - 32);
			}
			pTemp++;
		}
		return str;
#endif
	}

	wchar_t *DuiStringTraitsW::ui_strlwr(wchar_t *str) 
	{
#ifdef WIN32
		return ::wcslwr(str);
#else
		wchar_t *pTemp = str;
		while (*pTemp != '\0')
		{
			if (*pTemp >= 'A' && *pTemp <= 'Z') {
				*pTemp = static_cast<wchar_t>(*pTemp + 32);
			}
			pTemp++;
		}
		return str;
#endif
	}

	int DuiStringTraitsW::ui_atoi(const wchar_t *str) 
	{ 
		return (int)wcstol(str, NULL, 10);
	}

	DuiLib::Int64 DuiStringTraitsW::ui_atoi64(const wchar_t *str) 
	{ 
#ifdef WIN32
		return (DuiLib::Int64)_wcstoi64(str, NULL, 10);
#else
		return (DuiLib::Int64)wcstoll(str, NULL, 10);
#endif
	}

	float DuiStringTraitsW::ui_atof(const wchar_t *str) 
	{ 
		return static_cast<float>(::wcstod(str, 0));
	}

	double DuiStringTraitsW::ui_strtod (const wchar_t *nptr) 
	{ 
		return ::wcstod(nptr, 0); 
	}

	int __cdecl DuiStringTraitsW::formatV(wchar_t *&pstr, const wchar_t *pstrFormat, va_list Args)
	{
#if _MSC_VER <= 1400
		int nLen = 0;
		while (TRUE)
		{
			int buffLen = GetBufferLength(pstr);
			va_list cpArgs;
			va_copy(cpArgs, Args);
			nLen = format(pstr, buffLen, pstrFormat, cpArgs);
			va_end(cpArgs);
			if (nLen != -1 && nLen < buffLen)
				break;
			if (nLen == -1)
				break;
			AllocString(pstr, nLen+1, sizeof(wchar_t));
		}
		return nLen;
#else
		int nLen = format(NULL, 0, pstrFormat, Args);
		AllocString(pstr, nLen, sizeof(wchar_t));
		nLen = format(pstr, GetBufferLength(pstr), pstrFormat, Args);
		return nLen;
#endif
	}

	int __cdecl DuiStringTraitsW::format(wchar_t *string, size_t count, const wchar_t *format, va_list ap) 
	{ 
#ifdef WIN32
		return ::_vsnwprintf(string, count, format, ap); 
#else
		return ::vswprintf(string, count, format, ap); 
#endif
	}

	void DuiStringTraitsW::bool_to_string(wchar_t *string, bool b) 
	{ 
		ui_strcpy(string, b ? L"true" : L"false");
	}

	void DuiStringTraitsW::int_to_string(wchar_t *str, int n)
	{
		swprintf(str, 64, L"%d", n);
	}

	void DuiStringTraitsW::int64_to_string(wchar_t *str, DuiLib::Int64 n)
	{
		swprintf(str, 64, L"%I64d", n);
	}

	void DuiStringTraitsW::double_to_string(wchar_t *str, double s, int base)
	{ 
		if(base >= 0) 
		{ 
			wchar_t sFormat[32]; 
			swprintf(sFormat, 32, L"%%.%df", base); 
			swprintf(str, 64, sFormat, s); 
		}
		else 
		{ 
			swprintf(str, 64, L"%f", s); 
		}
	}

	void DuiStringTraitsW::float_to_string(wchar_t *str, float s, int base)
	{ 
		if(base >= 0) 
		{ 
			wchar_t sFormat[32]; 
			swprintf(sFormat, 32, L"%%.%df", base); 
			swprintf(str, 64, sFormat, s); 
		}
		else 
		{ 
			swprintf(str, 64, L"%f", s); 
		}
	}

	//////////////////////////////////////////////////////////////////////////
	wchar_t *DuiStringTraitsW::GetNullString() { return (wchar_t *)DuiStringMgr::GetInstance()->GetEmptyString(); }

	void DuiStringTraitsW::AllocString(wchar_t *&pstr, int nStrLength, int nSizeOfChar)
	{
		duistringdata *mem = DuiStringMgr::GetStringData(pstr);
		UINT nNeedAlloc = (nStrLength + 1) * nSizeOfChar;

		// 长度不够申请新的,
		// 引用大于1，也要申请新的. 引用大于1时任何修改字符串都要自己建新的，不要把别人的改了。
		if(mem->GetAllocLength() < nNeedAlloc || mem->IsShared() || mem->IsLock())
		{
			duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(nNeedAlloc);

			int oldLen = DuiStringTraitsW::ui_strlen(pstr);
			if(oldLen > 0 && oldLen <= nStrLength) //新分配的字符串短，原来的字符串长，就越界了！！！
			{
				DuiStringTraitsW::ui_strcpy((wchar_t *)newdata->data(), pstr);
			}
			DuiStringMgr::GetInstance()->Free(mem);
			pstr = (wchar_t *)newdata->data();
		}
		else
		{
			mem->SetDataLength(nNeedAlloc);
		}
	}

	void DuiStringTraitsW::FreeString(wchar_t *&pstr)
	{
		if(pstr)
		{
			duistringdata *data = DuiStringMgr::GetStringData(pstr);
			if(!data->IsLock())
			{
				DuiStringMgr::GetInstance()->Free(data);
				pstr = GetNullString();
			}
		}
	}

	int DuiStringTraitsW::GetBufferLength(wchar_t *pstr)
	{
		duistringdata *data = DuiStringMgr::GetStringData(pstr);
		return data->GetDataLength() / sizeof(wchar_t);
	}

	void DuiStringTraitsW::Empty(wchar_t *&pstr)
	{
		if(ui_strlen(pstr) > 0)
		{
			duistringdata *data = DuiStringMgr::GetStringData(pstr);
			if(data->IsShared())
				FreeString(pstr);
			else
				memset(data->data(), 0, data->GetDataLength());
		}
	}

	void DuiStringTraitsW::SetAt(wchar_t *&pstr, int nIndex, wchar_t ch)
	{
		if(nIndex < 0 || nIndex >= ui_strlen(pstr))
			return;

		duistringdata *data = DuiStringMgr::GetStringData(pstr);
		if(data->IsShared())
		{
			AllocString(pstr, ui_strlen(pstr), sizeof(wchar_t));
		}
		pstr[nIndex] = ch;
	}

	void DuiStringTraitsW::assign_string(wchar_t *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding, BOOL bMemString)
	{
		if(src_strlength == 0)
		{
			FreeString(pstr);
			return;
		}

		if(src_encoding == duistring_encoding_ansi)
		{
			StringConverterUI conv;
			const wchar_t *pNewString = conv.A_to_W(src_string, src_strlength);
			assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_unicode, FALSE);
		}
		else if(src_encoding == duistring_encoding_utf8)
		{
			StringConverterUI conv;
			const wchar_t *pNewString = conv.utf8_to_W(src_string, src_strlength);
			assign_string(pstr, dst_encoding, pNewString, ui_strlen(pNewString), duistring_encoding_unicode, FALSE);
		}
		else if(src_encoding == duistring_encoding_unicode)
		{
			if(bMemString)
			{
				duistringdata *src = DuiStringMgr::GetStringData(src_string);
				FreeString(pstr);
				pstr = (wchar_t *)src->data();
				src->AddRef();
				return;
			}

			AllocString(pstr, src_strlength, sizeof(wchar_t));
			DuiStringTraitsW::ui_strncpy(pstr, (wchar_t *)src_string, src_strlength);
			pstr[src_strlength] = L'\0';
		}
	}

	void DuiStringTraitsW::append_string(wchar_t *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding)
	{
		if(src_strlength == 0) return;

		StringConverterUI conv;
		const wchar_t *pNewString = NULL;
		if(src_encoding == duistring_encoding_ansi)
		{
			pNewString = conv.A_to_W(src_string, src_strlength);
		}
		else if(src_encoding == duistring_encoding_utf8)
		{
			pNewString = conv.utf8_to_W(src_string, src_strlength);
		}
		else if(src_encoding == duistring_encoding_unicode)
		{
			pNewString = (const wchar_t *)src_string;
		}	

		if(pNewString)
		{
			int oldLen = ui_strlen(pstr);
			AllocString(pstr, oldLen + src_strlength, sizeof(wchar_t));
			ui_strncat((wchar_t *)pstr, pNewString, src_strlength);
			pstr[oldLen + src_strlength] = L'\0';
		}
	}

	void DuiStringTraitsW::TrimLeft(wchar_t *&pstr)
	{
		const wchar_t *lpsz = pstr;
		while (iswspace(*lpsz))
			lpsz++;

		int nNewLen = wcslen(lpsz);
		wchar_t *pNewString = GetNullString();
		AllocString(pNewString, nNewLen, sizeof(wchar_t));
		ui_strcpy(pNewString, lpsz);
		ui_strcpy(pstr, pNewString);
		FreeString(pNewString);
	}

	void DuiStringTraitsW::TrimRight(wchar_t *&pstr)
	{
		int nStrLen = wcslen(pstr);
		for (int i=nStrLen-1; i>=0; i--)
		{
			if(iswspace(pstr[i]))
				pstr[i] = L'\0';
			else
				break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	CBufferUI::CBufferUI()
	{
		DuiStringMgr::GetInstance();
		_buffer = NULL;
		_bufferLen = 0;
	}

	CBufferUI::~CBufferUI()
	{
		Reset();
	}

	void CBufferUI::Reset()
	{
		if(_buffer != NULL)
		{
			duistringdata *data = ((duistringdata *)_buffer)-1;
			DuiStringMgr::GetInstance()->Free(data);
			_buffer = NULL;
			_bufferLen = 0;
		}
	}

	LPBYTE CBufferUI::GetBuffer()
	{
		return _buffer;
	}

	int CBufferUI::GetLength()
	{
		return _bufferLen;
	}

	int CBufferUI::AddBuffer(const void *buffer, int len)
	{
		int newSize = _bufferLen + len;
		Alloc(newSize);
		if(buffer)
		{
			memcpy(_buffer+_bufferLen, buffer, len);
		}
		_bufferLen += len;
		return _bufferLen;
	}

	void CBufferUI::Alloc(UINT size)
	{
		if(_buffer == NULL)
		{
			duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(size);
			_buffer = (BYTE *)newdata->data();
		}
		else
		{
			duistringdata *data = ((duistringdata *)_buffer)-1;
			if(data->GetAllocLength() < size)
			{
				duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(size);
				LPVOID pNewBuffer = newdata->data();
				memcpy(pNewBuffer, _buffer, _bufferLen);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	//
	StringConverterUI::StringConverterUI()
	{
		DuiStringMgr::GetInstance()->GetEmptyString();

		#ifdef WIN32
		m_cp = CP_ACP;
		#else
		m_from = _T("GBK");
		#endif // #ifdef WIN32

		_block = NULL;
	}

    #ifdef WIN32
	StringConverterUI::StringConverterUI(UINT codepage)
	{
		DuiStringMgr::GetInstance()->GetEmptyString();
		m_cp = codepage;
		_block = NULL;
	}
	#endif //#ifdef WIN32

	StringConverterUI::~StringConverterUI()
	{
		Release();
	}

	LPCTSTR StringConverterUI::A_to_T(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return A_to_W(buffer, bufferlen);
#else
		return (LPCTSTR)buffer;
#endif
	}

	LPCSTR StringConverterUI::T_to_A(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return W_to_A(buffer);
#else
		return (LPCSTR)buffer;
#endif
	}

	LPCTSTR StringConverterUI::W_to_T(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return (LPCTSTR)buffer;
#else
		return W_to_A(buffer, bufferlen);
#endif
	}

	LPCWSTR StringConverterUI::T_to_W(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return (LPCWSTR)buffer;
#else
		return A_to_W(buffer, bufferlen);
#endif
	}

	LPCTSTR StringConverterUI::utf8_to_T(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return utf8_to_W(buffer, bufferlen);
#elif defined _UTF8CODE
		return (LPCSTR)buffer;
#else
		return utf8_to_A(buffer, bufferlen);
#endif
	}

	LPCSTR StringConverterUI::T_to_utf8(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return W_to_utf8(buffer, bufferlen);
#elif defined _UTF8CODE
		return (LPCSTR)buffer;
#else
		return A_to_utf8(buffer, bufferlen);
#endif
	}

	LPCWSTR StringConverterUI::A_to_W(const void *buffer, int bufferlen)
	{
        #ifdef WIN32
			int len = ::MultiByteToWideChar(m_cp, 0, (const char *)buffer, bufferlen, NULL, 0);
			Alloc((len+1) * sizeof(wchar_t));
			::MultiByteToWideChar(m_cp, 0, (const char *)buffer, bufferlen, (wchar_t *)_block, len);
		#else
			//iconv_t icd = iconv_open(m_from,"UNICODE");
			//iconv_close(icd);
		#endif //WIN32

		return (LPCWSTR)_block;
	}

	LPCSTR StringConverterUI::W_to_A(const void *buffer, int bufferlen)
	{
        #ifdef WIN32
			int len = WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, bufferlen, NULL, 0, NULL, NULL);
			Alloc((len + 1) * sizeof(char));
			WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, bufferlen, (LPSTR)_block, len, NULL, NULL);
		#else
		#endif //#ifdef WIN32
		return (LPCSTR)_block;
	}

	LPCSTR StringConverterUI::A_to_utf8(const void *buffer, int bufferlen)
	{
		StringConverterUI conv;
		LPCWSTR wstr = conv.A_to_W(buffer, bufferlen);
		return W_to_utf8(wstr, bufferlen);
	}

	LPCSTR StringConverterUI::utf8_to_A(const void *buffer, int bufferlen)
	{
		StringConverterUI conv;
		LPCWSTR wstr = conv.utf8_to_W(buffer, bufferlen);
		return W_to_A(wstr, bufferlen);
	}

	LPCSTR StringConverterUI::W_to_utf8(const void *buffer, int bufferlen)
	{
    #ifdef WIN32
		int len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, bufferlen, NULL, 0, NULL, NULL);
		Alloc((len + 1) * sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, bufferlen, (LPSTR)_block, len, NULL, NULL);
	#else
	#endif // #ifdef WIN32

		return (LPCSTR)_block;
	}

	LPCWSTR StringConverterUI::utf8_to_W(const void *buffer, int bufferlen)
	{
        #ifdef WIN32
			int len = ::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, bufferlen, NULL, 0);
			Alloc((len + 1) * sizeof(wchar_t));
			::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, bufferlen, (wchar_t *)_block, len);
		#else
		#endif //  #ifdef WIN32

		return (LPWSTR)_block;
	}

	void StringConverterUI::Alloc(UINT size)
	{
		if(_block)
		{
			duistringdata *data = ((duistringdata *)_block)-1;
			if(data->GetAllocLength() < size)
			{
				DuiStringMgr::GetInstance()->Free(data);
				duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(size);
				_block = (BYTE *)newdata->data();
			}
		}
		else
		{
			duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(size);
			_block = (BYTE *)newdata->data();
		}

// 		if(_block) Release();
// 		_block = new BYTE[size];
// 		memset(_block, 0, size);
	}

	void StringConverterUI::Release()
	{
		if(_block != NULL)
		{
			duistringdata *data = ((duistringdata *)_block)-1;
			DuiStringMgr::GetInstance()->Free(data);
			_block = NULL;
		}

		//if(_block != NULL) { delete []_block; _block = NULL; }
	}

} // namespace DuiLib

