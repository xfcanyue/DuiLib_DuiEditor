#pragma once
#ifndef __DUISTRING_H__
#define __DUISTRING_H__

#include <vector>

#ifdef __GNUC__

inline int MulDiv(int nNumber, int nNumerator, int nDenominator)
{
	long long x = nNumber;
	x *= nNumerator;
	x = (x + nDenominator - 1) / nDenominator;
	return static_cast<int>(x);
}

inline LPTSTR CharNext(LPCTSTR lpsz)
{
	return (LPTSTR)lpsz + 1;
}
#endif

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	enum DuiStringEncoding
	{
		duistring_encoding_auto,
		duistring_encoding_ansi,
		duistring_encoding_unicode,
		duistring_encoding_utf8
	};

#ifdef WIN32
	typedef __int64 Int64;
#else
	typedef __int64_t Int64;
#endif

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API DuiStringTraitsA
	{
	public:
		static size_t ui_strlen(const char *pstr);
		static char *ui_strchr(char *string, int ch);
		static char *ui_strrchr(char *string, int ch);
		static char *ui_strstr(char *_Str, const char *_SubStr);
		static char *ui_strrstr(char *_Str, const char *_SubStr);
		static char *ui_strcat(char *dst, const char *src);
		static char *ui_strncat(char *dst, const char *src, size_t n);
		static char *ui_strcpy(char *dest,const char *source);
		static char *ui_strncpy(char *dest,const char *source, size_t count);
		static int ui_strcmp (const char *src, const char *dst);
		static int ui_stricmp (const char *src, const char *dst);
		static char *ui_strupr(char *str);
		static char *ui_strlwr(char *str);
		static int ui_atoi(const char *str);
		static DuiLib::Int64 ui_atoi64(const char *str);
		static float ui_atof(const char *str);
		static double ui_strtod (const char *nptr);

		static int __cdecl formatV(char *&pstr, const char *pstrFormat, va_list ap);
		static int __cdecl format(char *string, size_t count, const char *format, va_list ap);

		static void bool_to_string(char *string, bool b);
		static void int_to_string(char *str, int n);
		static void int64_to_string(char *str, DuiLib::Int64 n);
		static void double_to_string(char *str, double s, int base=-1);
		static void float_to_string(char *str, float s, int base=-1);

		//////////////////////////////////////////////////////////////////////////
		static char *GetNullString();
		static void AllocString(char *&pstr, int nStrLength, int nSizeOfChar);
		static void FreeString(char *&pstr);
		static int GetBufferLength(char *pstr);
		static void Empty(char *&pstr);
		static void SetAt(char *&pstr, int nIndex, char ch);
		static void assign_string(char *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding, BOOL bMemString);
		static void append_string(char *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding);
		static void TrimLeft(char *&pstr);
		static void TrimRight(char *&pstr);
	};

	class UILIB_API DuiStringTraitsW
	{
	public:
		static size_t ui_strlen(const wchar_t *pstr);
		static wchar_t *ui_strchr(wchar_t *string, int ch);
		static wchar_t *ui_strrchr(wchar_t *string, int ch);
		static wchar_t *ui_strstr(wchar_t *_Str, const wchar_t *_SubStr);
		static wchar_t *ui_strrstr(wchar_t *_Str, const wchar_t *_SubStr);
		static wchar_t *ui_strcat(wchar_t *dst, const wchar_t *src);
		static wchar_t *ui_strncat(wchar_t *dst, const wchar_t *src, size_t n);
		static wchar_t *ui_strcpy(wchar_t *dest, const wchar_t *source);
		static wchar_t *ui_strncpy(wchar_t *dest, const wchar_t *source, size_t count);
		static int ui_strcmp(const wchar_t *src, const wchar_t *dst);
		static int ui_stricmp(const wchar_t *src, const wchar_t *dst);
		static wchar_t *ui_strupr(wchar_t *str);
		static wchar_t *ui_strlwr(wchar_t *str);
		static int ui_atoi(const wchar_t *str);
		static DuiLib::Int64 ui_atoi64(const wchar_t *str);
		static float ui_atof(const wchar_t *str);
		static double ui_strtod (const wchar_t *nptr);

		static int __cdecl formatV(wchar_t *&pstr, const wchar_t *pstrFormat, va_list ap);
		static int __cdecl format(wchar_t *string, size_t count, const wchar_t *format, va_list ap);

		static void bool_to_string(wchar_t *string, bool b);
		static void int_to_string(wchar_t *str, int n);
		static void int64_to_string(wchar_t *str, DuiLib::Int64 n);
		static void double_to_string(wchar_t *str, double s, int base=-1);
		static void float_to_string(wchar_t *str, float s, int base=-1);

		//////////////////////////////////////////////////////////////////////////
		static wchar_t *GetNullString();
		static void AllocString(wchar_t *&pstr, int nStrLength, int nSizeOfChar);
		static void FreeString(wchar_t *&pstr);
		static int GetBufferLength(wchar_t *pstr);
		static void Empty(wchar_t *&pstr);
		static void SetAt(wchar_t *&pstr, int nIndex, wchar_t ch);
		static void assign_string(wchar_t *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding, BOOL bMemString);
		static void append_string(wchar_t *&pstr, DuiStringEncoding dst_encoding, const void *src_string, int src_strlength, DuiStringEncoding src_encoding);
		static void TrimLeft(wchar_t *&pstr);
		static void TrimRight(wchar_t *&pstr);
	};

	template< typename uichar, class DuiTraits, DuiStringEncoding StringEncoding > class DuiStringT;
	typedef DuiStringT<wchar_t, DuiStringTraitsW, duistring_encoding_unicode>	CDuiStringW;
	typedef DuiStringT<char, DuiStringTraitsA, duistring_encoding_ansi>			CDuiStringA;
	typedef DuiStringT<char, DuiStringTraitsA, duistring_encoding_utf8>			CDuiStringUtf8;
#ifdef _UNICODE
	typedef CDuiStringW	CDuiString;
#elif defined _UTF8CODE
	typedef CDuiStringUtf8	CDuiString;
#else
	typedef CDuiStringA	CDuiString;
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	// DuiStringT模板类
	//
	template< typename uichar, class DuiTraits, DuiStringEncoding StringEncoding >
	class DuiStringT
	{
	public:
		DuiStringT()									{ m_pstr = DuiTraits::GetNullString();						}
		DuiStringT(const uichar ch)						{ m_pstr = DuiTraits::GetNullString(); Assign(&ch, 1);		}
		DuiStringT(const uichar *lpsz, int nLen = -1)	{ m_pstr = DuiTraits::GetNullString(); Assign(lpsz, nLen);	}

		DuiStringT(const CDuiStringA &str)
		{
			m_pstr = DuiTraits::GetNullString();
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		DuiStringT(const CDuiStringUtf8 &str)
		{
			m_pstr = DuiTraits::GetNullString();
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		DuiStringT(const CDuiStringW &str)
		{
			m_pstr = DuiTraits::GetNullString();
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		DuiStringT(int int_to_string) 
		{ 
			ASSERT(FALSE); //因为在以前的版本中，有这个构造函数，去掉。
		} 

		~DuiStringT() 
		{ 
			DuiTraits::FreeString(m_pstr); 
		}

		//设置缓冲区长度
		void SetBufferLength(int iLength) 
		{ 
			DuiTraits::AllocString(m_pstr, iLength, sizeof(uichar)); 
		}

		//获取缓冲区长度，返回值可能大于设置的长度
		int GetBufferLength() 
		{ 
			return DuiTraits::GetBufferLength(m_pstr); 
		}

		//返回字符串缓冲区, nMinBufferLength>0时，当前缓冲区小于nMinBufferLength，重新分配缓冲区。
		//没有对应的ReleaseBuffer().
		uichar *GetBuffer(int nMinBufferLength = -1)
		{
			if (GetBufferLength() < nMinBufferLength)
				DuiTraits::AllocString(m_pstr, nMinBufferLength, sizeof(uichar));
			return m_pstr;
		}

		void Empty() 
		{ 
			DuiTraits::Empty(m_pstr); 
		}

		int GetLength() const 
		{ 
			return static_cast<int>(DuiTraits::ui_strlen(m_pstr)); 
		}

		bool IsEmpty() const 
		{ 
			return DuiTraits::ui_strlen(m_pstr) == 0; 
		}

		void Assign(uichar ch)
		{
			DuiTraits::assign_string(m_pstr, StringEncoding, &ch, 1, StringEncoding, FALSE);
		}

		void Assign(const uichar *pstr, int cchMax = -1)
		{
			cchMax = (cchMax < 0 ? (int)DuiTraits::ui_strlen(pstr) : cchMax);
			DuiTraits::assign_string(m_pstr, StringEncoding, pstr, cchMax, StringEncoding, FALSE);
		}

		void Assign(const CDuiStringA &str)
		{
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		void Assign(const CDuiStringUtf8 &str)
		{
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		void Assign(const CDuiStringW &str)
		{
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding(), TRUE);
		}

		void Append(uichar ch)
		{
			uichar str[] = { ch, '\0' };
			Append(str);
		}

		void Append(const uichar *pstr, int cchMax=-1) 
		{ 
			int srclen = DuiTraits::ui_strlen(pstr);
			if(srclen == 0 || cchMax == 0)
				return;
			if(cchMax > 0 && cchMax < srclen)
				srclen = cchMax;
			DuiTraits::append_string(m_pstr, StringEncoding, pstr, srclen, StringEncoding); 
		}

		void Append(const CDuiStringA &str)
		{
			DuiTraits::append_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding());
		}

		void Append(const CDuiStringUtf8 &str)
		{
			DuiTraits::append_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding());
		}

		void Append(const CDuiStringW &str)
		{
			DuiTraits::append_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), str.GetEncoding());
		}

		const uichar *GetData() const 
		{ 
			return m_pstr; 
		}

		void SetAt(int nIndex, uichar ch) 
		{ 
			DuiTraits::SetAt(m_pstr, nIndex, ch); 
		}

		uichar GetAt(int nIndex) const
		{
			if(nIndex < 0 || nIndex >= GetLength())	return 0;
			return m_pstr[nIndex];
		}

		operator const uichar *() const 
		{ 
			return m_pstr; 
		}

		//由于地址复用的关系，这里可能会存在bug。 也就是，自己改了，别人的也改了。所以最好用SetAt()
		uichar &operator[] (int nIndex) const 
		{ 
			ASSERT(nIndex>=0 && nIndex<GetLength());
			return m_pstr[nIndex];
		}

		//创建新的内存，并且拷贝字符串
		void CopyFrom(const DuiStringT &str)
		{
			DuiTraits::AllocString(m_pstr, str.GetLength(), sizeof(uichar));
			DuiTraits::assign_string(m_pstr, StringEncoding, str.toString(), str.GetLength(), StringEncoding, FALSE);
		}

		const DuiStringT& operator=(const uichar ch)			{ Assign(ch);		return *this; }
		const DuiStringT& operator=(const uichar *pstr)			{ Assign(pstr);		return *this; }
		const DuiStringT& operator=(const CDuiStringA& src)		{ Assign(src);		return *this; }
		const DuiStringT& operator=(const CDuiStringUtf8& src)	{ Assign(src);		return *this; }
		const DuiStringT& operator=(const CDuiStringW& src)		{ Assign(src);		return *this; }

		const DuiStringT& operator+=(const uichar ch)			{ Append(ch);		return *this; }
		const DuiStringT& operator+=(const uichar *pstr)		{ Append(pstr);		return *this; }
		const DuiStringT& operator+=(const CDuiStringA& src)	{ Append(src);		return *this; }
		const DuiStringT& operator+=(const CDuiStringUtf8& src) { Append(src);		return *this; }
		const DuiStringT& operator+=(const CDuiStringW& src)	{ Append(src);		return *this; }

		DuiStringT operator+(const uichar ch)		const	{ DuiStringT sTemp = *this; sTemp.Append(ch);		return sTemp; }
		DuiStringT operator+(const uichar *lpStr)	const	{ DuiStringT sTemp = *this; sTemp.Append(lpStr);	return sTemp; }
		DuiStringT operator+(const CDuiStringA& src) const		{ DuiStringT sTemp = *this; sTemp.Append(src);	return sTemp; }
		DuiStringT operator+(const CDuiStringUtf8& src) const	{ DuiStringT sTemp = *this; sTemp.Append(src);	return sTemp; }
		DuiStringT operator+(const CDuiStringW& src) const		{ DuiStringT sTemp = *this; sTemp.Append(src);	return sTemp; }

		bool operator == (const uichar *str) const { return (Compare(str) == 0); };
		bool operator != (const uichar *str) const { return (Compare(str) != 0); };
		bool operator <= (const uichar *str) const { return (Compare(str) <= 0); };
		bool operator <  (const uichar *str) const { return (Compare(str) <  0); };
		bool operator >= (const uichar *str) const { return (Compare(str) >= 0); };
		bool operator >  (const uichar *str) const { return (Compare(str) >  0); };

		int Compare(const uichar *pstr) const
		{
			if(!pstr && IsEmpty())
				return 0;
			return DuiTraits::ui_strcmp(m_pstr, pstr);
		}

		int CompareNoCase(const uichar *pstr) const
		{
			if(!pstr && IsEmpty())
				return 0;
			return DuiTraits::ui_stricmp(m_pstr, pstr);
		}

		void MakeUpper()
		{
			DuiTraits::ui_strupr(m_pstr);
		}

		void MakeLower()
		{
			DuiTraits::ui_strlwr(m_pstr);
		}

		DuiStringT Left(int iLength) const
		{
			if( iLength < 0 ) iLength = 0;
			if( iLength > GetLength() ) iLength = GetLength();
			return DuiStringT(m_pstr, iLength);
		}

		DuiStringT Mid(int iPos, int iLength=-1) const
		{
			if( iLength < 0 ) iLength = GetLength() - iPos;
			if( iPos + iLength > GetLength() ) iLength = GetLength() - iPos;
			if( iLength <= 0 ) return DuiStringT();
			return DuiStringT(m_pstr + iPos, iLength);
		}

		DuiStringT Right(int iLength) const
		{
			int iPos = GetLength() - iLength;
			if( iPos < 0 ) {
				iPos = 0;
				iLength = GetLength();
			}
			return DuiStringT(m_pstr + iPos, iLength);
		}

// 		CDuiString sTemp = _T("aaa_bbb_ccc_ddd_eee");
// 		CDuiString s0 = sTemp.Section(_T("_"), 0);		//aaa
// 		CDuiString s1 = sTemp.Section(_T("_"), 1);		//bbb_ccc_ddd_eee
// 		CDuiString s2 = sTemp.Section(_T("_"), 2);		//ccc_ddd_eee
// 		CDuiString s3 = sTemp.Section(_T("_"), 0, 0);	//aaa
// 		CDuiString s4 = sTemp.Section(_T("_"), 1, 1);	//bbb
// 		CDuiString s5 = sTemp.Section(_T("_"), 2, 2);	//ccc
// 		CDuiString s6 = sTemp.Section(_T("_"), 0, 1);	//aaa
// 		CDuiString s7 = sTemp.Section(_T("_"), 1, 2);	//bbb_ccc
// 		CDuiString s8 = sTemp.Section(_T("_"), 2, 3);	//ccc_ddd
		DuiStringT Section(const uichar *pstr, int start, int end=-1)
		{
			if(start < 0)
				return DuiStringT();

			int pos1 = Find(pstr);
			if(pos1 < 0) DuiStringT();

			if(start == 0)
				return Left(pos1);

			int pos2 = 0;
			int findTimes = 1;
			int cchlen = DuiTraits::ui_strlen(pstr);
			while (start != findTimes)
			{
				pos2 = Find(pstr, pos1 + cchlen);
				if(pos2 < 0) return DuiStringT();
				findTimes++;
				pos1 = pos2;
			}

			if(end < 0 || end < start)
				return Mid(pos1+1, GetLength()-pos1-1);

			int posTemp = pos1;
			while (end+1 != findTimes)
			{
				pos2 = Find(pstr, posTemp + cchlen);
				if(pos2 < 0)
				{
					pos2 = GetLength()-pos1-1;
					break;
				}
				findTimes++;
				posTemp = pos2;
			}
			return Mid(pos1+1, pos2-pos1-1);
		}

// 		CDuiString sTemp = _T("aaa_bbb_ccc_ddd_eee");
// 		CDuiString s10 = sTemp.ReverseSection(_T("_"), 0);		//eee
// 		CDuiString s11 = sTemp.ReverseSection(_T("_"), 1);		//aaa_bbb_ccc_ddd
// 		CDuiString s12 = sTemp.ReverseSection(_T("_"), 2);		//aaa_bbb_ccc
// 		CDuiString s13 = sTemp.ReverseSection(_T("_"), 0, 0);	//eee
// 		CDuiString s14 = sTemp.ReverseSection(_T("_"), 1, 1);	//ddd
// 		CDuiString s15 = sTemp.ReverseSection(_T("_"), 2, 2);	//ccc
// 		CDuiString s16 = sTemp.ReverseSection(_T("_"), 0, 1);	//eee
// 		CDuiString s17 = sTemp.ReverseSection(_T("_"), 1, 2);	//ccc_ddd
// 		CDuiString s18 = sTemp.ReverseSection(_T("_"), 2, 3);	//bbb_ccc
		DuiStringT ReverseSection(const uichar *pstr, int start, int end=-1)
		{
			if(start < 0)
				return DuiStringT();

			int pos1 = ReverseFind(pstr);
			if(pos1 < 0) return DuiStringT();

			if(start == 0)
				return Right(GetLength() - pos1 - 1);

			int pos2 = 0;
			int findTimes = 1;
			int cchlen = DuiTraits::ui_strlen(pstr);
			while (start != findTimes)
			{
				pos2 = ReverseFind(pstr, GetLength() - pos1 + cchlen);
				if(pos2 < 0) return DuiStringT();
				findTimes++;
				pos1 = pos2;
			}

			if(end < 0 || end < start)
				return Left(pos1);

			int posTemp = pos1;
			while (end+1 != findTimes)
			{
				pos2 = ReverseFind(pstr,  GetLength() - posTemp + cchlen);
				if(pos2 < 0)
				{
					pos2 = GetLength()-pos1-1;
					break;
				}
				findTimes++;
				posTemp = pos2;
			}
			return Mid(pos2+1, pos1-pos2-1);
		}

		int Find(uichar ch, int iPos = 0) const
		{
			ASSERT(iPos>=0 && iPos<=GetLength());
			if( iPos != 0 && (iPos < 0 || iPos >= GetLength()) ) return -1;
			const uichar *p = DuiTraits::ui_strchr(m_pstr + iPos, ch);
			if( p == NULL ) return -1;
			return (int)(p - m_pstr);
		}

		int Find(const uichar *pstrSub, int iPos = 0) const
		{
			ASSERT(iPos>=0 && iPos<=GetLength());
			if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
			const uichar *p = DuiTraits::ui_strstr(m_pstr + iPos, pstrSub);
			if( p == NULL ) return -1;
			return (int)(p - m_pstr);
		}

		int ReverseFind(uichar ch, int iPos = 0) const
		{
			ASSERT(iPos>=0 && iPos<=GetLength());
			if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
			DuiStringT sTemp = Left(GetLength()-iPos);
			const uichar *p = DuiTraits::ui_strrchr(sTemp.m_pstr, ch);
			if( p == NULL ) return -1;
			return (int)(p - sTemp.m_pstr);
		}

		int ReverseFind(const uichar *pstrSub, int iPos = 0) const
		{
			ASSERT(iPos>=0 && iPos<=GetLength());
			if( iPos != 0 && (iPos < 0 || iPos > GetLength()) ) return -1;
			DuiStringT sTemp = Left(GetLength()-iPos);
			const uichar *p = DuiTraits::ui_strrstr(sTemp.m_pstr, pstrSub);
			if( p == NULL ) return -1;
			return (int)(p - sTemp.m_pstr);
		}

		int Replace(const uichar *pstrFrom, const uichar *pstrTo)
		{
			DuiStringT sTemp;
			int nCount = 0;
			int iPos = Find(pstrFrom);
			if( iPos < 0 ) return 0;
			int cchFrom = DuiTraits::ui_strlen(pstrFrom);
			int cchTo = DuiTraits::ui_strlen(pstrTo);
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

		DuiStringT &TrimLeft()
		{
			DuiTraits::TrimLeft(m_pstr);
			return *this;
		}

		DuiStringT &TrimRight()
		{
			DuiTraits::TrimRight(m_pstr);
			return *this;
		}

		DuiStringT &Remove(const uichar *pstr)
		{
			int pos = Find(pstr);
			if(pos < 0) return *this;
			int cchRemove = DuiTraits::ui_strlen(pstr);
			DuiStringT sTemp;
			while( pos >= 0 ) 
			{
				sTemp += Left(pos);
				sTemp += Mid(pos + cchRemove);
				pos = Find(pstr, pos + cchRemove);
			}
			Assign(sTemp);
			return *this;
		}

		DuiStringT &Delete(int nIndex, int nCount)
		{
			DuiStringT sTemp;
			sTemp += Left(nIndex);
			sTemp += Mid(nIndex+nCount, GetLength()-nIndex-nCount);
			Assign(sTemp);
			return *this;
		}

		DuiStringT &ReverseDelete(int nIndex, int nCount)
		{
			DuiStringT sTemp;
			sTemp += Left(GetLength()-nIndex-nCount);
			sTemp += Right(nIndex);
			Assign(sTemp);
			return *this;
		}

		int __cdecl AppendFormat(const uichar *pstrFormat, ...)
		{
			DuiStringT s;

			va_list Args;
			va_start(Args, pstrFormat);
			s.InnerFormat(pstrFormat, Args);
			va_end(Args);

			Append(s);
			return GetLength();
		}

		int __cdecl Format(const uichar *pstrFormat, ...)
		{
			int nRet;
			va_list Args;
			va_start(Args, pstrFormat);
			nRet = InnerFormat(pstrFormat, Args);
			va_end(Args);
			return nRet;
		}

		int __cdecl FormatV(const uichar *pstrFormat, va_list Args)
		{
			return DuiTraits::formatV(m_pstr, pstrFormat, Args);
		}

		int __cdecl SmallFormat(const uichar *pstrFormat, ...)
		{
			DuiStringT sFormat = pstrFormat;
			uichar szBuffer[64] = { 0 };
			va_list argList;
			va_start(argList, pstrFormat);
			int iRet = DuiTraits::format(szBuffer, sizeof(szBuffer), sFormat, argList);
			va_end(argList);
			Assign(szBuffer);
			return iRet;
		}

		int __cdecl InnerFormat(const uichar *pstrFormat, va_list Args)
		{
			return DuiTraits::formatV(m_pstr, pstrFormat, Args);
		}

		void setString(const uichar *s)			{ Assign(s, -1); }
		void setString(const uichar val)		{ Assign(&val, 1); }
		void setBool(bool val)					{ DuiTraits::AllocString(m_pstr, 64, sizeof(uichar)); DuiTraits::bool_to_string(m_pstr, val);			}
		void setInt(int val)					{ DuiTraits::AllocString(m_pstr, 64, sizeof(uichar)); DuiTraits::int_to_string(m_pstr, val);			}
		void setInt64(DuiLib::Int64 val)		{ DuiTraits::AllocString(m_pstr, 64, sizeof(uichar)); DuiTraits::int64_to_string(m_pstr, val);			}
		void setDouble(double val, int base=-1) { DuiTraits::AllocString(m_pstr, 64, sizeof(uichar)); DuiTraits::double_to_string(m_pstr, val, base);	}
		void setFloat(float val, int base=-1)	{ DuiTraits::AllocString(m_pstr, 64, sizeof(uichar)); DuiTraits::float_to_string(m_pstr, val, base);	}

		const uichar *toString() const						{ return (const uichar *)m_pstr; }
		bool toBool(bool def = false) const					{ return m_pstr[0]=='1' || m_pstr[0]=='t' || m_pstr[0]=='T' || m_pstr[0]=='Y' || m_pstr[0]=='y'; }
		int toInt(int def = 0) const						{ return DuiTraits::ui_atoi(m_pstr); }
		DuiLib::Int64 toInt64(DuiLib::Int64 def = 0) const	{ return DuiTraits::ui_atoi64(m_pstr); }
		double toDouble(double def = 0) const				{ return DuiTraits::ui_strtod(m_pstr); }
		float toFloat(float def = 0) const					{ return DuiTraits::ui_atof(m_pstr); }

		CDuiStringA convert_to_ansi() const		{ CDuiStringA s;	s=(*this);		return s; }
		CDuiStringUtf8 convert_to_utf8() const	{ CDuiStringUtf8 s;	s=(*this);		return s; }
		CDuiStringW convert_to_unicode() const	{ CDuiStringW s;	s=(*this);		return s; }
		CDuiString convert_to_auto() const 
		{
#ifdef _UNICODE
			return convert_to_unicode();
#elif defined _UTF8CODE
			return convert_to_utf8();
#else
			return convert_to_ansi();
#endif
		}

		DuiStringEncoding GetEncoding() const { return StringEncoding; }
	protected:
		uichar *m_pstr;
	};

	//////////////////////////////////////////////////////////////////////////
	//CDuiString数组类
	//
	template< typename uichar, class DuiString> class DuiStringArrayT;
	typedef DuiStringArrayT<wchar_t, CDuiStringW>	CDuiStringArrayW;
	typedef DuiStringArrayT<char,	CDuiStringA>	CDuiStringArrayA;
	typedef DuiStringArrayT<char,	CDuiStringUtf8>	CDuiStringArrayUtf8;
#ifdef _UNICODE
	typedef CDuiStringArrayW	CDuiStringArray;
#elif defined _UTF8CODE
	typedef CDuiStringArrayUtf8	CDuiStringArray;
#else
	typedef CDuiStringArrayA	CDuiStringArray;
#endif

	template< typename uichar, class DuiString>
	class UILIB_API DuiStringArrayT
	{
	public:
		DuiStringArrayT() 
		{
			m_bNoRepeat = false;
		}
		DuiStringArrayT(const DuiStringArrayT& src)
		{
			m_bNoRepeat = src.m_bNoRepeat;

			RemoveAll();
			for (int i=0; i<src.GetSize(); i++)
			{
				Add(src.GetAt(i), false);
			}
		}
		~DuiStringArrayT()
		{
			RemoveAll();
		}

		int GetSize() const
		{
			return m_arrString.size();
		}

		bool IsEmpty() const
		{
			return GetSize() <= 0;
		}

		void RemoveAll()
		{
			m_arrString.clear();
		}

		bool Add(const uichar *newElement, bool bNoRepeat=false)
		{
			if(newElement == NULL || *newElement == _T('\0')) return false;
			if(bNoRepeat && Find(newElement))  return false;
			m_arrString.push_back(newElement);
			return true;
		}

		bool Add(const DuiString &newElement, bool bNoRepeat=false)
		{
			if(bNoRepeat && Find(newElement))  return false;
			m_arrString.push_back(newElement);
			return true;
		}

		bool AddV(const uichar *pstrFormat, ...)
		{
			DuiString s;
			int nRet;
			va_list Args;
			va_start(Args, pstrFormat);
			s.InnerFormat(pstrFormat, Args);
			va_end(Args);
			if(!s.IsEmpty())
			{
				Add(s, m_bNoRepeat);
			}
			return true;
		}

		bool Remove(const uichar *element)
		{
			typename std::vector<DuiString>::iterator it;
			for (it=m_arrString.begin(); it!=m_arrString.end(); it++)
			{	
				if(*it == element)
				{
					m_arrString.erase(it);
					return true;
				}
			}
			return false;
		}

		bool Find(const uichar *element)
		{
			for (UINT i=0; i<m_arrString.size(); i++)
			{
				if(m_arrString[i] == element)
					return true;
			}
			return false;
		}

		DuiString GetAt(int iIndex) const
		{
			if(iIndex < 0 || iIndex>=GetSize())
				return DuiString();
			return m_arrString[iIndex];
		}

		DuiString& operator[] (int nIndex)
		{
			return m_arrString[nIndex];
		}

		//分割字符串来构造数组，src=需要分割的字符串，sp=分割符. 若分隔符为NULL，则非字母数字为分割符号。
		void SplitString(const uichar *src, const uichar *sp = NULL)
		{
			if(sp == NULL || *sp == _T('\0'))
			{
				DuiString s;
				while (*src != _T('\0'))
				{
					if(!_istalnum(*src))
					{
						if(!s.IsEmpty())
						{
							Add(s); s.Empty();
						}
					}
					else
					{
						s += *src;
					}
					src++;
				}
				if(!s.IsEmpty()) Add(s);
			}
			else
			{
				DuiString text(src);
				int pos = text.Find(sp, 0);
				while (pos >= 0)
				{
					DuiString t = text.Left(pos);
					t.TrimLeft(); t.TrimRight();
					if(!t.IsEmpty()) Add(t);
					text = text.Right(text.GetLength() - pos - _tcslen(sp));
					pos = text.Find(sp);
				}
				text.TrimLeft(); text.TrimRight();
				if(!text.IsEmpty()) Add(text);
			}
		}

	private:
		std::vector<DuiString> m_arrString;
		bool m_bNoRepeat;
	};

	class UILIB_API CBufferUI
	{
	public:
		CBufferUI();
		~CBufferUI();

		void Reset();

		LPBYTE GetBuffer();
		int GetLength();

		int AddBuffer(const void *buffer, int len);

	protected:
		void Alloc(UINT size);
	protected:
		BYTE *_buffer;
		int _bufferLen;
	};
	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API StringConverterUI
	{
	public:
		StringConverterUI();
		#ifdef WIN32
		StringConverterUI(UINT codepage);
		#endif //#ifdef WIN32
		~StringConverterUI();

	public:
		LPCTSTR A_to_T(const void *buffer, int bufferlen = -1);

		LPCSTR T_to_A(const void *buffer, int bufferlen = -1);

		LPCTSTR W_to_T(const void *buffer, int bufferlen = -1);

		LPCWSTR T_to_W(const void *buffer, int bufferlen = -1);

		LPCTSTR utf8_to_T(const void *buffer, int bufferlen = -1);

		LPCSTR T_to_utf8(const void *buffer, int bufferlen = -1);

		LPCWSTR A_to_W(const void *buffer, int bufferlen = -1);

		LPCSTR W_to_A(const void *buffer, int bufferlen = -1);

		LPCSTR A_to_utf8(const void *buffer, int bufferlen = -1);

		LPCSTR utf8_to_A(const void *buffer, int bufferlen = -1);

		LPCSTR W_to_utf8(const void *buffer, int bufferlen = -1);

		LPCWSTR utf8_to_W(const void *buffer, int bufferlen = -1);

	protected:
		void Alloc(UINT size);
		void Release();
	private:
		BYTE *_block;
#ifdef WIN32
		UINT m_cp;
#else
		std::string m_from;
		std::string m_to;
#endif // #ifdef WIN32
	};


#define UISTRING_CONVERSION					StringConverterUI _UISTRINGCONVERT_TEMP_INSTANCE_;
#define UISTRING_CONVERSION_EX(codepage)	StringConverterUI _UISTRINGCONVERT_TEMP_INSTANCE_(codepage);

#define UIA2T(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.A_to_T(p))
#define UIA2W(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.A_to_W(p))
#define UIA2UTF8(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.A_to_utf8(p))

#define UIW2T(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.W_to_T(p))
#define UIW2A(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.W_to_A(p))
#define UIW2UTF8(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.W_to_utf8(p))

#define UIT2A(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.T_to_A(p))
#define UIT2W(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.T_to_W(p))
#define UIT2UTF8(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.T_to_utf8(p))

#define UIUTF82T(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.utf8_to_T(p))
#define UIUTF82A(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.utf8_to_A(p))
#define UIUTF82W(p)	(_UISTRINGCONVERT_TEMP_INSTANCE_.utf8_to_W(p))

}// namespace DuiLib

#endif // __DUISTRING_H__
