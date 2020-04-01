#include "stdafx.h"
#include "Misc.h"




/*static*/ int CMisc::Message(LPCTSTR szFmt, ...)
{
	TCHAR	szBuffer[2048];
	va_list args;
	int		bytesWriten = 0;

	va_start(args, szFmt);
	bytesWriten = _vstprintf(szBuffer, szFmt, args);

	va_end(args);


	return ::MessageBox(NULL, szBuffer, NULL, MB_ICONSTOP | MB_OK);
}


