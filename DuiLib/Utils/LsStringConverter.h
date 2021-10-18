////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define _LSSTRINGCONVERT_TEMP_INSTANCE_	__lsstring__convert__temp__instance__
class LsStringConverterUI
{
public:
	LsStringConverterUI() { m_cp = CP_ACP; _block = NULL; }
	LsStringConverterUI(UINT codepage) { m_cp = codepage; _block = NULL; }
	~LsStringConverterUI() { Release(); }

public:
	LPCTSTR A_to_T(const void *buffer)
	{
#ifdef _UNICODE
		return A_to_W(buffer);
#else
		return (LPCTSTR)buffer;
#endif
	}

	LPCSTR T_to_A(const void *buffer)
	{
#ifdef _UNICODE
		return W_to_A(buffer);
#else
		return (LPCSTR)buffer;
#endif
	}

	LPCTSTR W_to_T(const void *buffer)
	{
#ifdef _UNICODE
		return (LPCTSTR)buffer;
#else
		return W_to_A(buffer);
#endif
	}

	LPCWSTR T_to_W(const void *buffer)
	{
#ifdef _UNICODE
		return (LPCWSTR)buffer;
#else
		return A_to_W(buffer);
#endif
	}

	LPCTSTR utf8_to_T(const void *buffer)
	{
#ifdef _UNICODE
		return utf8_to_W(buffer);
#else
		return utf8_to_A(buffer);
#endif
	}

	LPCSTR T_to_utf8(const void *buffer)
	{
#ifdef _UNICODE
		return W_to_utf8(buffer);
#else
		return A_to_utf8(buffer);
#endif
	}

	LPCWSTR A_to_W(const void *buffer)
	{
		int len = MultiByteToWideChar(m_cp, 0, (const char *)buffer, -1, NULL, 0);

		Alloc((len+1) * sizeof(wchar_t));
		MultiByteToWideChar(m_cp, 0, (const char *)buffer, -1, (wchar_t *)_block, len);
		return (LPCWSTR)_block;
	}

	LPCSTR W_to_A(const void *buffer)
	{
		int len = WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, -1, NULL, 0, NULL, NULL);

		Alloc((len + 1) * sizeof(char));
		WideCharToMultiByte(m_cp, 0, (const wchar_t *)buffer, -1, (LPSTR)_block, len, NULL, NULL);
		return (LPCSTR)_block;
	}

	LPCSTR A_to_utf8(const void *buffer)
	{
		LsStringConverterUI conv;
		LPCWSTR wstr = conv.A_to_W(buffer);
		return W_to_utf8(wstr);
	}

	LPCSTR utf8_to_A(const void *buffer)
	{
		LsStringConverterUI conv;
		LPCWSTR wstr = conv.utf8_to_W(buffer);
		return W_to_A(wstr);
	}

	LPCSTR W_to_utf8(const void *buffer)
	{
		int len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, -1, NULL, 0, NULL, NULL);

		Alloc((len + 1) * sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, (const wchar_t *)buffer, -1, (LPSTR)_block, len, NULL, NULL);
		return (LPCSTR)_block;
	}

	LPCWSTR utf8_to_W(const void *buffer)
	{
		int len = ::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, -1, NULL, 0);

		Alloc((len + 1) * sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, NULL, (const char *)buffer, -1, (wchar_t *)_block, len);
		return (LPWSTR)_block;
	}

protected:
	void Alloc(int size)
	{
		if(_block)
		{
			Release();
		}
		_block = new BYTE[size];
		memset(_block, 0, size);
	}
	void Release()
	{
		if(_block != NULL) { delete []_block; _block = NULL; }
	}
private:
	BYTE *_block;
	UINT m_cp;
};

#define LSSTRING_CONVERSION					LsStringConverterUI _LSSTRINGCONVERT_TEMP_INSTANCE_;
#define LSSTRING_CONVERSION_EX(codepage)	LsStringConverterUI _LSSTRINGCONVERT_TEMP_INSTANCE_(codepage);

#define LSA2T(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.A_to_T(p))
#define LSA2W(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.A_to_W(p))
#define LSA2UTF8(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.A_to_utf8(p))

#define LSW2T(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.W_to_T(p))
#define LSW2A(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.W_to_A(p))
#define LSW2UTF8(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.W_to_utf8(p))

#define LST2A(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.T_to_A(p))
#define LST2W(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.T_to_W(p))
#define LST2UTF8(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.T_to_utf8(p))

#define LSUTF82T(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.utf8_to_T(p))
#define LSUTF82A(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.utf8_to_A(p))
#define LSUTF82W(p)	(_LSSTRINGCONVERT_TEMP_INSTANCE_.utf8_to_W(p))