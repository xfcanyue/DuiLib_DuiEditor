#pragma once
#ifndef __DUISTRING_H__
#define __DUISTRING_H__

namespace DuiLib
{
	/////////////////////////////////////////////////////////////////////////////////////
	// CDuiString内存池版本
	//
	class UILIB_API CDuiString
	{
	public:
		CDuiString();
		CDuiString(const TCHAR ch);
		CDuiString(const CDuiString& src);
		CDuiString(LPCTSTR lpsz, int nLen = -1);
		~CDuiString();

		//设置缓冲区长度
		void SetBufferLength(int iLength);

		//获取缓冲区长度，返回值可能大于设置的长度
		int GetBufferLength();

		//返回字符串缓冲区, nMinBufferLength>0时，当前缓冲区小于nMinBufferLength，重新分配缓冲区。
		//没有对应的ReleaseBuffer().
		LPTSTR GetBuffer(int nMinBufferLength = -1);

		void Empty();
		int GetLength() const;
		bool IsEmpty() const;
		TCHAR GetAt(int nIndex) const;
		void Append(LPCTSTR pstr);
		void Assign(LPCTSTR pstr, int nLength = -1);
		LPCTSTR GetData() const;

		void SetAt(int nIndex, TCHAR ch);
		operator LPCTSTR() const;

		TCHAR operator[] (int nIndex) const;
		const CDuiString& operator=(const CDuiString& src);
		const CDuiString& operator=(const TCHAR ch);
		const CDuiString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
		const CDuiString& operator=(LPCSTR lpStr);
		const CDuiString& operator+=(LPCSTR lpStr);
#else
		const CDuiString& operator=(LPCWSTR lpwStr);
		const CDuiString& operator+=(LPCWSTR lpwStr);
#endif
		CDuiString operator+(const CDuiString& src) const;
		CDuiString operator+(LPCTSTR pstr) const;
		const CDuiString& operator+=(const CDuiString& src);
		const CDuiString& operator+=(LPCTSTR pstr);
		const CDuiString& operator+=(const TCHAR ch);

		bool operator == (LPCTSTR str) const;
		bool operator != (LPCTSTR str) const;
		bool operator <= (LPCTSTR str) const;
		bool operator <  (LPCTSTR str) const;
		bool operator >= (LPCTSTR str) const;
		bool operator >  (LPCTSTR str) const;

		int Compare(LPCTSTR pstr) const;
		int CompareNoCase(LPCTSTR pstr) const;

		void MakeUpper();
		void MakeLower();

		CDuiString Left(int nLength) const;
		CDuiString Mid(int iPos, int nLength = -1) const;
		CDuiString Right(int nLength) const;

		int Find(TCHAR ch, int iPos = 0) const;
		int Find(LPCTSTR pstr, int iPos = 0) const;
		int ReverseFind(TCHAR ch) const;
		int Replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

		int __cdecl AppendFormat(LPCTSTR pstrFormat, ...);
		int __cdecl Format(LPCTSTR pstrFormat, ...);
		int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

		int __cdecl InnerFormat(LPCTSTR pstrFormat, va_list Args);

	protected:
		void AllocString(int strlength);
		void FreeString();
	protected:
		LPTSTR m_pstr;
	};
	
	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API StringConverterUI
	{
	public:
		StringConverterUI();
		StringConverterUI(UINT codepage);
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
		void Alloc(int size);
		void Release();
	private:
		BYTE *_block;
		UINT m_cp;
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