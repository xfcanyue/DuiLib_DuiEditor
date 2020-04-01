// DateTimeFormat.h: interface for the CDateTimeFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATETIMEFORMAT_H__6F7F9D68_B703_4159_9437_5133AAA087B5__INCLUDED_)
#define AFX_DATETIMEFORMAT_H__6F7F9D68_B703_4159_9437_5133AAA087B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ATLComTime.h>

//! Formats a date and time value
//
class CDateTimeFormat  
{
public:
	CDateTimeFormat();
	virtual ~CDateTimeFormat();

	//! Sets the date time value
	void SetDateTime(COleDateTime &odt);

	//! Sets the format string.
	void SetFormat(LPCTSTR pszFormat);

	//! Sets the format buffer for in-place formatting.
	//! This avoids an eventual CString copy on GetString.
	void SetFormatBuf(LPTSTR pszBuf) {m_pszBuf = pszBuf;}

	//! Returns the formatted string
	LPCTSTR	GetString() {return m_pszBuf;}

protected:
	void	ClearCounters	();
	void	FormatSection	();
	void	FormatDay		();
	void	FormatMonth		();
	void	FormatYear		();
	void	FormatHour		();
	void	FormatAMPM		();
	void	FormatWeek		();
	void	FormatQuarter	();
	void	FormatDigit		(int nDigits, int nValue);

	COleDateTime	m_odt;

	BOOL			m_fValid;		//!< Is the date valid?
	int				m_nd,			//!< Day format
					m_nM,			//!< Month format
					m_ny,			//!< Year format
					m_nw,			//!< Week format
					m_nq,			//!< Quarter format
					m_ng,			//!< Period format
					m_nh,			//!< Hour format (12 hours)
					m_nH,			//!< Hour format (24 hours)
					m_nm,			//!< Minute format
					m_ns,			//!< Seconds format
					m_nt;			//!< Time marker
	TCHAR			m_szRes[256];	//!< Resulting formatted string
	TCHAR*			m_pszBuf;		//!< Pointer to buffer (local or not)
};

#endif // !defined(AFX_DATETIMEFORMAT_H__6F7F9D68_B703_4159_9437_5133AAA087B5__INCLUDED_)
