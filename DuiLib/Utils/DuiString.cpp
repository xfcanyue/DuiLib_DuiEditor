#include "StdAfx.h"
#include "DuiString.h"
#include "Utils.h"

namespace DuiLib
{

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	struct duistringdata : public ILinkedList
	{
		int nRefs;				//引用次数
		UINT nAllocLength;		//分配的长度, 可能大于字符串实际长度。
		TCHAR* data()			// TCHAR* to managed data 
		{ return (TCHAR*)(this+1); }
	};

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
			OutputDebugStringA("construct duistringmem\r\n");
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
			OutputDebugStringA("destroy duistringmem\r\n");
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
				memset(s, 0, memsize);
				s->nAllocLength = strlength;
				s->nRefs = 0;
			}

			if(s) s->nRefs++;
			return s;	
		}

		void Free(duistringdata *s)
		{
			if(s == m_pEmptyString)
			{
				//空字符串不处理
			}
			else if(s->nAllocLength <= 16)
			{
				RecoverStringData(m_list16, 16, s);
			}
			else if(s->nAllocLength <= 32)
			{
				RecoverStringData(m_list32, 32, s);
			}
			else if(s->nAllocLength <= 64)
			{
				RecoverStringData(m_list64, 64, s);
			}
			else if(s->nAllocLength <= 128)
			{
				RecoverStringData(m_list128, 128, s);
			}
			else
			{
				CDuiInnerLock lock(&m_lock);
				s->nRefs--;
				if(s->nRefs == 0)
					free((BYTE *)s);
			}
		}

		LPTSTR GetEmptyString()
		{
			return m_pEmptyString->data();
		}
	protected:
		void MakeEmptyString()
		{
			int memsize = sizeof(duistringdata) + sizeof(TCHAR);
			m_pEmptyString = (duistringdata *)malloc(memsize);
			memset(m_pEmptyString, 0, memsize);
			m_pEmptyString->nAllocLength = sizeof(TCHAR);
			m_pEmptyString->nRefs = 0;
			m_pEmptyString->data()[0] = _T('\0');
		}

		duistringdata *MakeStringData(CStdLinkList<duistringdata> &listdata, int strlength, int &nMakeCount)
		{
			if(listdata.empty())
			{
				int memsize = sizeof(duistringdata) + strlength;
				BYTE *pBlock = (BYTE *)malloc(memsize * nMakeCount); //每次申请 nMakeCount 个字符串
				memset(pBlock, 0, memsize * nMakeCount);
				m_listMem.Add(pBlock);

				for (int i=0; i<nMakeCount; i++)
				{
					duistringdata *s = (duistringdata *)(pBlock + (i * memsize));
					s->nAllocLength = strlength;
					s->nRefs = 0;
					listdata.push_back(s);
				}

				nMakeCount *= 2;

#ifdef _DEBUG
				CString ss;
				ss.Format(_T("分配字符串内存池，长度=%d, 个数=%d。\r\n"), strlength, nMakeCount);
				OutputDebugString(ss);
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
			s->nRefs--;
			if(s->nRefs == 0)
			{
				memset(s->data(), 0, s->nAllocLength);
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

	class DuiStringMgr
	{
	public:
		static duistringmem* GetInstance()
		{
			//确保在每个string构造之前构造，确保在每个string析构之后析构
			static duistringmem smem;
			return &smem;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//
	CDuiString::CDuiString()
	{
		//确保构造string之前构造DuiStringMgr，否则全局string和DuiStringMgr的析构顺序变的很不可靠，下同。
		m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
	}

	CDuiString::CDuiString(const TCHAR ch)
	{
		m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
		Assign(&ch, 1);
	}

	CDuiString::CDuiString(LPCTSTR lpsz, int nLen)
	{      
		m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
		ASSERT(!::IsBadStringPtr(lpsz,-1) || lpsz==NULL);
		Assign(lpsz, nLen);
	}

	CDuiString::CDuiString(const CDuiString& src)
	{
		DuiStringMgr::GetInstance()->GetEmptyString();
		//m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
		//FreeString();

		//拷贝构造时复用地址，引用+1
		m_pstr = src.m_pstr;
		duistringdata *data = ((duistringdata *)m_pstr)-1;
		data->nRefs++;

		//Assign(src.m_pstr);
	}

	CDuiString::~CDuiString()
	{
		FreeString();
	}

	void CDuiString::AllocString(int strlength)
	{
		UINT nNeedAlloc = (strlength + 1) * sizeof(TCHAR);
		duistringdata *data = ((duistringdata *)m_pstr)-1;

		// 长度不够申请新的, 
		// 引用大于1，也要申请新的. 引用大于1时任何修改字符串都要自己建新的，不要把别人的改了。
		if(data->nAllocLength < nNeedAlloc || data->nRefs > 1)
		{
			duistringdata *newdata = DuiStringMgr::GetInstance()->Alloc(nNeedAlloc);
			_tcscat(newdata->data(), m_pstr);
			DuiStringMgr::GetInstance()->Free(data);
			m_pstr = newdata->data();
		}
	}

	void CDuiString::FreeString()
	{
		if(m_pstr)
		{
			duistringdata *data = ((duistringdata *)m_pstr)-1;
			DuiStringMgr::GetInstance()->Free(data);
			m_pstr = NULL;
		}
	}

	void CDuiString::SetBufferLength(int iLength)
	{
		AllocString(iLength);
	}

	int CDuiString::GetBufferLength()
	{
		duistringdata *data = ((duistringdata *)m_pstr)-1;
		return data->nAllocLength;
	}

	int CDuiString::GetLength() const
	{
		return (int) _tcslen(m_pstr); 
	}

	CDuiString::operator LPCTSTR() const 
	{ 
		return m_pstr; 
	}

	void CDuiString::Append(LPCTSTR pstr)
	{
		if( pstr == NULL || *pstr == _T('\0')) return;
		int nNewLength = GetLength() + (int) _tcslen(pstr);

		AllocString(nNewLength);
		_tcscat(m_pstr, pstr);
	}

	void CDuiString::Assign(LPCTSTR pstr, int cchMax)
	{
		if( pstr == NULL || *pstr == _T('\0')) 
		{
			FreeString();
			m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
			return;
		}
		cchMax = (cchMax < 0 ? (int) _tcslen(pstr) : cchMax);
		AllocString(cchMax);
		_tcsncpy(m_pstr, pstr, cchMax);
		m_pstr[cchMax] = '\0';
	}

	bool CDuiString::IsEmpty() const 
	{
		return m_pstr[0] == '\0'; 
	}

	void CDuiString::Empty() 
	{ 
		if(GetLength() > 0)
		{
			duistringdata *s = ((duistringdata *)m_pstr)-1;
			if(s->nRefs > 1)
			{
				FreeString();
				m_pstr = DuiStringMgr::GetInstance()->GetEmptyString();
			}
			else
			{
				memset(s->data(), 0, s->nAllocLength);
			}
		}
	}

	LPCTSTR CDuiString::GetData() const
	{
		return m_pstr;
	}

	TCHAR CDuiString::GetAt(int nIndex) const
	{
		if(nIndex < 0 || nIndex >= GetLength())	return _T('\0');
		return m_pstr[nIndex];
	}

	TCHAR CDuiString::operator[] (int nIndex) const
	{ 
		return GetAt(nIndex);
	}   

	const CDuiString& CDuiString::operator=(const CDuiString& src)
	{      
		FreeString();
		m_pstr = src.m_pstr;
		duistringdata *data = ((duistringdata *)m_pstr)-1;
		data->nRefs++;

		//Assign(src);
		return *this;
	}

	const CDuiString& CDuiString::operator=(LPCTSTR lpStr)
	{      
		if ( lpStr )
		{
			ASSERT(!::IsBadStringPtr(lpStr,-1));
			Assign(lpStr);
		}
		else
		{
			Empty();
		}
		return *this;
	}

#ifdef _UNICODE

	const CDuiString& CDuiString::operator=(LPCSTR lpStr)
	{
		if ( lpStr )
		{
			ASSERT(!::IsBadStringPtrA(lpStr,-1));
// 			int cchStr = (int) strlen(lpStr) + 1;
// 			LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
// 			if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
//			Assign(pwstr);
			UISTRING_CONVERSION;
			Assign(UIA2T(lpStr));
		}
		else
		{
			Empty();
		}
		return *this;
	}

	const CDuiString& CDuiString::operator+=(LPCSTR lpStr)
	{
		if ( lpStr )
		{
			ASSERT(!::IsBadStringPtrA(lpStr,-1));
// 			int cchStr = (int) strlen(lpStr) + 1;
// 			LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
// 			if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
// 			Append(pwstr);
			UISTRING_CONVERSION;
			Append(UIA2T(lpStr));
		}
		
		return *this;
	}

#else

	const CDuiString& CDuiString::operator=(LPCWSTR lpwStr)
	{      
		if ( lpwStr )
		{
			ASSERT(!::IsBadStringPtrW(lpwStr,-1));
// 			int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
// 			LPSTR pstr = (LPSTR) _alloca(cchStr);
// 			if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
// 			Assign(pstr);
			UISTRING_CONVERSION;
			Assign(UIW2T(lpwStr));
		}
		else
		{
			Empty();
		}
		
		return *this;
	}

	const CDuiString& CDuiString::operator+=(LPCWSTR lpwStr)
	{
		if ( lpwStr )
		{
			ASSERT(!::IsBadStringPtrW(lpwStr,-1));
// 			int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
// 			LPSTR pstr = (LPSTR) _alloca(cchStr);
// 			if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
// 			Append(pstr);
			UISTRING_CONVERSION;
			Append(UIW2T(lpwStr));
		}
		
		return *this;
	}

#endif // _UNICODE

	const CDuiString& CDuiString::operator=(const TCHAR ch)
	{
		Assign(&ch, 1);
		return *this;
	}

	CDuiString CDuiString::operator+(const CDuiString& src) const
	{
		CDuiString sTemp = *this;
		sTemp.Append(src);
		return sTemp;
	}

	CDuiString CDuiString::operator+(LPCTSTR lpStr) const
	{
		if ( lpStr )
		{
			ASSERT(!::IsBadStringPtr(lpStr,-1));
			CDuiString sTemp = *this;
			sTemp.Append(lpStr);
			return sTemp;
		}

		return *this;
	}

	const CDuiString& CDuiString::operator+=(const CDuiString& src)
	{      
		Append(src);
		return *this;
	}

	const CDuiString& CDuiString::operator+=(LPCTSTR lpStr)
	{      
		if ( lpStr )
		{
			ASSERT(!::IsBadStringPtr(lpStr,-1));
			Append(lpStr);
		}
		
		return *this;
	}

	const CDuiString& CDuiString::operator+=(const TCHAR ch)
	{      
		TCHAR str[] = { ch, '\0' };
		Append(str);
		return *this;
	}

	bool CDuiString::operator == (LPCTSTR str) const { return (Compare(str) == 0); };
	bool CDuiString::operator != (LPCTSTR str) const { return (Compare(str) != 0); };
	bool CDuiString::operator <= (LPCTSTR str) const { return (Compare(str) <= 0); };
	bool CDuiString::operator <  (LPCTSTR str) const { return (Compare(str) <  0); };
	bool CDuiString::operator >= (LPCTSTR str) const { return (Compare(str) >= 0); };
	bool CDuiString::operator >  (LPCTSTR str) const { return (Compare(str) >  0); };

	void CDuiString::SetAt(int nIndex, TCHAR ch)
	{
		ASSERT(nIndex>=0 && nIndex<GetLength());

		duistringdata *s = ((duistringdata *)m_pstr)-1;
		if(s->nRefs > 1)
		{
			AllocString(GetLength());
		}

		m_pstr[nIndex] = ch;
	}

	int CDuiString::Compare(LPCTSTR lpsz) const 
	{ 
		return _tcscmp(m_pstr, lpsz); 
	}

	int CDuiString::CompareNoCase(LPCTSTR lpsz) const 
	{ 
		return _tcsicmp(m_pstr, lpsz); 
	}

	void CDuiString::MakeUpper() 
	{ 
		_tcsupr(m_pstr); 
	}

	void CDuiString::MakeLower() 
	{ 
		_tcslwr(m_pstr); 
	}

	CDuiString CDuiString::Left(int iLength) const
	{
		if( iLength < 0 ) iLength = 0;
		if( iLength > GetLength() ) iLength = GetLength();
		return CDuiString(m_pstr, iLength);
	}

	CDuiString CDuiString::Mid(int iPos, int iLength) const
	{
		if( iLength < 0 ) iLength = GetLength() - iPos;
		if( iPos + iLength > GetLength() ) iLength = GetLength() - iPos;
		if( iLength <= 0 ) return CDuiString();
		return CDuiString(m_pstr + iPos, iLength);
	}

	CDuiString CDuiString::Right(int iLength) const
	{
		int iPos = GetLength() - iLength;
		if( iPos < 0 ) {
			iPos = 0;
			iLength = GetLength();
		}
		return CDuiString(m_pstr + iPos, iLength);
	}

	int CDuiString::Find(TCHAR ch, int iPos /*= 0*/) const
	{
		ASSERT(iPos>=0 && iPos<=GetLength());
		if( iPos != 0 && (iPos < 0 || iPos >= GetLength()) ) return -1;
		LPCTSTR p = _tcschr(m_pstr + iPos, ch);
		if( p == NULL ) return -1;
		return (int)(p - m_pstr);
	}

	int CDuiString::Find(LPCTSTR pstrSub, int iPos /*= 0*/) const
	{
		ASSERT(!::IsBadStringPtr(pstrSub,-1));
		ASSERT(iPos>=0 && iPos<=GetLength());
		if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
		LPCTSTR p = _tcsstr(m_pstr + iPos, pstrSub);
		if( p == NULL ) return -1;
		return (int)(p - m_pstr);
	}

	int CDuiString::ReverseFind(TCHAR ch) const
	{
		LPCTSTR p = _tcsrchr(m_pstr, ch);
		if( p == NULL ) return -1;
		return (int)(p - m_pstr);
	}

	int CDuiString::Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo)
	{
		CDuiString sTemp;
		int nCount = 0;
		int iPos = Find(pstrFrom);
		if( iPos < 0 ) return 0;
		int cchFrom = (int) _tcslen(pstrFrom);
		int cchTo = (int) _tcslen(pstrTo);
		while( iPos >= 0 ) {
			sTemp = Left(iPos);
			sTemp += pstrTo;
			sTemp += Mid(iPos + cchFrom);
			Assign(sTemp);
			iPos = Find(pstrFrom, iPos + cchTo);
			nCount++;
		}
		return nCount;
	}

	int CDuiString::AppendFormat(LPCTSTR pstrFormat, ...)
	{
		CDuiString s;

		va_list Args;
		va_start(Args, pstrFormat);
		s.InnerFormat(pstrFormat, Args);
		va_end(Args);

		Append(s);
		return GetLength();
	}

    int CDuiString::Format(LPCTSTR pstrFormat, ...)
    {
        int nRet;
        va_list Args;

        va_start(Args, pstrFormat);
        nRet = InnerFormat(pstrFormat, Args);
        va_end(Args);

        return nRet;

    }

	int CDuiString::SmallFormat(LPCTSTR pstrFormat, ...)
	{
		CDuiString sFormat = pstrFormat;
		TCHAR szBuffer[64] = { 0 };
		va_list argList;
		va_start(argList, pstrFormat);
		int iRet = ::_vsntprintf(szBuffer, sizeof(szBuffer), sFormat, argList);
		va_end(argList);
		Assign(szBuffer);
		return iRet;
	}
	
    int CDuiString::InnerFormat(LPCTSTR pstrFormat, va_list Args)
    {
#if _MSC_VER <= 1400
        TCHAR *szBuffer = NULL;
        int size = 512, nLen, counts;
        szBuffer = (TCHAR*)malloc(size);
        ZeroMemory(szBuffer, size);
        while (TRUE){
            counts = size / sizeof(TCHAR);
            nLen = _vsntprintf (szBuffer, counts, pstrFormat, Args);
            if (nLen != -1 && nLen < counts){
                break;
            }
            if (nLen == -1){
                size *= 2;
            }else{
                size += 1 * sizeof(TCHAR);
            }

            if ((szBuffer = (TCHAR*)realloc(szBuffer, size)) != NULL){
                ZeroMemory(szBuffer, size);
            }else{
                break;
            }
        }

        Assign(szBuffer);
        free(szBuffer);
        return nLen;
#else
        int nLen = _vsntprintf(NULL, 0, pstrFormat, Args);
		AllocString(nLen);
        nLen = _vsntprintf(m_pstr, GetBufferLength(), pstrFormat, Args);
        return nLen;
#endif
    }
	
	//////////////////////////////////////////////////////////////////////////
	//
	//
	StringConverterUI::StringConverterUI() 
	{ 
		DuiStringMgr::GetInstance()->GetEmptyString();
		m_cp = CP_ACP; 
		_block = NULL; 
	}

	StringConverterUI::StringConverterUI(UINT codepage) 
	{ 
		DuiStringMgr::GetInstance()->GetEmptyString();
		m_cp = codepage; 
		_block = NULL; 
	}

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
#else
		return utf8_to_A(buffer, bufferlen);
#endif
	}

	LPCSTR StringConverterUI::T_to_utf8(const void *buffer, int bufferlen)
	{
#ifdef _UNICODE
		return W_to_utf8(buffer, bufferlen);
#else
		return A_to_utf8(buffer, bufferlen);
#endif
	}

	LPCWSTR StringConverterUI::A_to_W(const void *buffer, int bufferlen)
	{
		int len = MultiByteToWideChar(m_cp, 0, (const char *)buffer, bufferlen, NULL, 0);

		Alloc((len+1) * sizeof(wchar_t));
		MultiByteToWideChar(m_cp, 0, (const char *)buffer, bufferlen, (wchar_t *)_block, len);
		return (LPCWSTR)_block;
	}

	LPCSTR StringConverterUI::W_to_A(const void *buffer, int bufferlen)
	{
		int len = WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, bufferlen, NULL, 0, NULL, NULL);

		Alloc((len + 1) * sizeof(char));
		WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, bufferlen, (LPSTR)_block, len, NULL, NULL);
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
		int len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, bufferlen, NULL, 0, NULL, NULL);

		Alloc((len + 1) * sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, bufferlen, (LPSTR)_block, len, NULL, NULL);
		return (LPCSTR)_block;
	}

	LPCWSTR StringConverterUI::utf8_to_W(const void *buffer, int bufferlen)
	{
		int len = ::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, bufferlen, NULL, 0);

		Alloc((len + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, bufferlen, (wchar_t *)_block, len);
		return (LPWSTR)_block;
	}

	void StringConverterUI::Alloc(int size)
	{
		if(_block)
		{
			duistringdata *data = ((duistringdata *)_block)-1;
			if(data->nAllocLength < size)
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