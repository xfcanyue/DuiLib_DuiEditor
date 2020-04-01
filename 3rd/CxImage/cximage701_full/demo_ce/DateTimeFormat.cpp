// DateTimeFormat.cpp: implementation of the CDateTimeFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DateTimeFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CDateTimeFormat::CDateTimeFormat()
{
	ClearCounters();
	m_szRes[0] = 0;
	m_pszBuf = m_szRes;
}


CDateTimeFormat::~CDateTimeFormat()
{

}


void CDateTimeFormat::ClearCounters()
{
	m_nd =
	m_nM =
	m_ny =
	m_nw =
	m_nq =
	m_ng =
	m_nh =
	m_nH =
	m_nm =
	m_ns =
	m_nt = 0;
}


// CDateTimeFormat::SetDateTime
//
//		Sets the date and time values
//
void CDateTimeFormat::SetDateTime(COleDateTime &odt)
{
	if(odt.GetStatus() == COleDateTime::valid)
	{
		m_odt		= odt;
		m_fValid	= TRUE;
	}
	else
		m_fValid = FALSE;
}


// CDateTimeFormat::FormatDay
//
//		Formats a day
//
void CDateTimeFormat::FormatDay()
{
	TCHAR	szFmt[128];
	LCTYPE	lct;
	LCTYPE	lcAbbr[] = {	LOCALE_SABBREVDAYNAME7,		// Sunday
							LOCALE_SABBREVDAYNAME1,		// Monday
							LOCALE_SABBREVDAYNAME2,		// Tuesday
							LOCALE_SABBREVDAYNAME3,		// Wednesday
							LOCALE_SABBREVDAYNAME4,		// Thursday
							LOCALE_SABBREVDAYNAME5,		// Friday
							LOCALE_SABBREVDAYNAME6 };	// Saturday
	LCTYPE	lcFull[] = {	LOCALE_SDAYNAME7,
							LOCALE_SDAYNAME1,
							LOCALE_SDAYNAME2,
							LOCALE_SDAYNAME3,
							LOCALE_SDAYNAME4,
							LOCALE_SDAYNAME5,
							LOCALE_SDAYNAME6 };

	switch(m_nd)
	{
	case 1:
		swprintf(szFmt, _T("%d"), m_odt.GetDay());
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), m_odt.GetDay());
		break;

	case 3:
		lct = lcAbbr[m_odt.GetDayOfWeek()-1];
		GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lct, szFmt, 128);
		break;

	case 4:
		lct = lcFull[m_odt.GetDayOfWeek()-1];
		GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lct, szFmt, 128);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	};

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatWeek
//
//		Formats a week number
//
void CDateTimeFormat::FormatWeek()
{
	TCHAR			szFmt[16];
	int				nDayOfYear = m_odt.GetDayOfYear(),
					nWeek;
	COleDateTime	odtFirst(m_odt.GetYear(), 1, 1, 0, 0, 0);

	nWeek = (nDayOfYear + odtFirst.GetDayOfWeek()) / 7 + 1;

	switch(m_nw)
	{
	case 1:
		swprintf(szFmt, _T("%d"), nWeek);
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), nWeek);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	}

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatMonth
//
//		Formats a month
//
void CDateTimeFormat::FormatMonth()
{
	TCHAR	szFmt[128];
	LCTYPE	lct;
	LCTYPE	lcAbbr[] = {	LOCALE_SABBREVMONTHNAME1,
							LOCALE_SABBREVMONTHNAME2,
							LOCALE_SABBREVMONTHNAME3,
							LOCALE_SABBREVMONTHNAME4,
							LOCALE_SABBREVMONTHNAME5,
							LOCALE_SABBREVMONTHNAME6,
							LOCALE_SABBREVMONTHNAME7,
							LOCALE_SABBREVMONTHNAME8,
							LOCALE_SABBREVMONTHNAME9,
							LOCALE_SABBREVMONTHNAME10,
							LOCALE_SABBREVMONTHNAME11,
							LOCALE_SABBREVMONTHNAME12,
							LOCALE_SABBREVMONTHNAME13	};
	LCTYPE	lcFull[] = {	LOCALE_SMONTHNAME1,
							LOCALE_SMONTHNAME2,
							LOCALE_SMONTHNAME3,
							LOCALE_SMONTHNAME4,
							LOCALE_SMONTHNAME5,
							LOCALE_SMONTHNAME6,
							LOCALE_SMONTHNAME7,
							LOCALE_SMONTHNAME8,
							LOCALE_SMONTHNAME9,
							LOCALE_SMONTHNAME10,
							LOCALE_SMONTHNAME11,
							LOCALE_SMONTHNAME12,
							LOCALE_SMONTHNAME13	};

	switch(m_nM)
	{
	case 1:
		swprintf(szFmt, _T("%d"), m_odt.GetMonth());
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), m_odt.GetMonth());
		break;

	case 3:
		lct = lcAbbr[m_odt.GetMonth()-1];
		GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lct, szFmt, 127);
		break;

	case 4:
		lct = lcFull[m_odt.GetMonth()-1];
		GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lct, szFmt, 127);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	};

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatQuarter
//
//		Formats a quarter number
//
void CDateTimeFormat::FormatQuarter()
{
	TCHAR	szFmt[16];

	switch(m_nq)
	{
	case 1:
		swprintf(szFmt, _T("%d"), (m_odt.GetMonth() - 1) / 3 + 1);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	}

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatYear
//
//		Formats a year
//
void CDateTimeFormat::FormatYear()
{
	TCHAR	szFmt[16];

	switch(m_ny)
	{
	case 1:
		swprintf(szFmt, _T("%d"), m_odt.GetYear());
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), m_odt.GetYear() % 100);
		break;

	case 4:
		swprintf(szFmt, _T("%04d"), m_odt.GetYear());
		break;

	default:
		wcscpy(szFmt, _T("???"));
	};

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatHour
//
//		Formats an hour
//
void CDateTimeFormat::FormatHour()
{
	TCHAR	szFmt[16];
	int		nHour = m_odt.GetHour();
	int		nDig;

	//
	// Check if we are formatting for a 12-hour clock
	//
	if(m_nh)
	{
		if(nHour > 12)
		{
			nHour -= 12;
		}
		nDig = m_nh;
	}
	else
		nDig = m_nH;

	switch(nDig)
	{
	case 1:
		swprintf(szFmt, _T("%d"), nHour);
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), nHour);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	};

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatDigit
//
//		Formats numbers like '2' or '02'
//
void CDateTimeFormat::FormatDigit(int nDigits, int nValue)
{
	TCHAR	szFmt[16];

	switch(nDigits)
	{
	case 1:
		swprintf(szFmt, _T("%d"), nValue);
		break;

	case 2:
		swprintf(szFmt, _T("%02d"), nValue);
		break;

	default:
		wcscpy(szFmt, _T("???"));
	};

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatAMPM
//
//		Inserts the AM / PM symbols
//
void CDateTimeFormat::FormatAMPM()
{
	TCHAR	szFmt[128];
	int		nHour;

	if(m_nt > 0 && m_nt < 3)
	{
		nHour = m_odt.GetHour();	// Returns a value between 0 and 23
		if(nHour >= 12)
		{
			GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_S2359, szFmt, 127);
		}
		else
		{
			GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_S1159, szFmt, 127);
		}

		if(m_nt == 1)
			szFmt[1] = 0;
	}
	else
	{
		wcscpy(szFmt, _T("???"));
	}

	wcscat(m_pszBuf, szFmt);
}


// CDateTimeFormat::FormatSection
//
//		Come here whenever a section is ready to be formatted
//
void CDateTimeFormat::FormatSection()
{
	CString	str;

	if(m_nd)
	{
		FormatDay();
	}
	else if(m_nM)
	{
		FormatMonth();
	}
	else if(m_ny)
	{
		FormatYear();
	}
	else if(m_nq)
	{
		FormatQuarter();
	}
	else if(m_nw)
	{
		FormatWeek();
	}
	else if(m_ng)
	{
		// Not yet implemented
	}
	else if(m_nh || m_nH)
	{
		FormatHour();
	}
	else if(m_nm)
	{
		FormatDigit(m_nm, m_odt.GetMinute());
	}
	else if(m_ns)
	{
		FormatDigit(m_ns, m_odt.GetSecond());
	}
	else if(m_nt)
	{
		FormatAMPM();
	}
	else
		wcscat(m_pszBuf, _T("???") );

	ClearCounters();
}


// CDateTimeFormat::SetFormat
//
//		Set and scan the format string
//		Generates the result string
//
void CDateTimeFormat::SetFormat(LPCTSTR pszFormat)
{
	LPCTSTR	p;
	TCHAR	c = 0;
	BOOL	fFmt = FALSE,			// Formatting
			fLit = FALSE;			// Literal characters
	TCHAR	szChar[2]	= {0, 0},
			szLit[128]	= {0};
	TCHAR*	pLit		= szLit;

	m_pszBuf[0] = 0;	// Clear the string the C way
	ClearCounters();	// Clear the internal counters

	for(p = pszFormat; *p; p++)
	{
		//
		// Scanning formats?
		//
		if(fFmt)
		{
			if(*p != c)				// Did the format char changed?
			{
				if(szLit[0])
				{
					*pLit = 0;
					wcscat(m_pszBuf, szLit);

					pLit = szLit;
					*pLit = 0;
				}

				FormatSection();	// So format the section,
				fFmt = FALSE;		// and say we are not formatting
			}
		}
		c = *p;

		//
		// What literal mode?
		//
		if(!fLit)
		{
			//
			// Not in literal mode
			//
			switch(c)
			{
			case _T('d'):	m_nd++; fFmt = TRUE; break;
			case _T('M'):	m_nM++; fFmt = TRUE; break;
			case _T('y'):	m_ny++; fFmt = TRUE; break;
			case _T('g'):	m_ng++; fFmt = TRUE; break;
			case _T('h'):	m_nh++; fFmt = TRUE; break;
			case _T('H'):	m_nH++; fFmt = TRUE; break;
			case _T('m'):	m_nm++; fFmt = TRUE; break;
			case _T('s'):	m_ns++; fFmt = TRUE; break;
			case _T('t'):	m_nt++; fFmt = TRUE; break;
			case _T('w'):	m_nw++; fFmt = TRUE; break;
			case _T('q'):	m_nq++; fFmt = TRUE; break;
			case _T('\''):	fLit = TRUE; break;
			default:
				*pLit++ = c;
				break;
			}
		}
		else
		{
			//
			// In literal mode
			//
			if(c == _T('\''))
			{
				fLit = FALSE;				// Terminate the literal mode
				*pLit = 0;					// Terminate the literal string
				wcscat(m_pszBuf, szLit);	// Append the literal string

				pLit = szLit;				// Reinitialize the literal string
				*pLit = 0;
			}
			else
				*pLit++ = c;				// Copy character to literal string
		}
	}

	//
	// Last check for a pending format
	//
	if(fFmt)
	{
		if(*szLit)
		{
			*pLit = 0;
			wcscat(m_pszBuf, szLit);
		}

		FormatSection();
	}
}
