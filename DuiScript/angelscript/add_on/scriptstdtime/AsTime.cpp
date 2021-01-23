#include "AsTime.h"

#include "windows.h"
#include "assert.h"

datetime::datetime()
{
	refCount = 1;
	m_time=0;
}

//拷贝构造函数
datetime::datetime(const datetime& timeSrc)
{
	m_time = timeSrc.m_time;
}

void datetime::GetTime()	//获取当前电脑时间
{
	m_time = ::_time64(NULL);
}

inline struct tm* datetime::GetLocalTm(struct tm* ptm) const
{
	// Ensure ptm is valid
	assert( ptm != NULL );

	if (ptm != NULL)
	{
		struct tm ptmTemp;
		errno_t err = _localtime64_s(&ptmTemp, &m_time);

		if (err != 0)
		{
			return NULL;    // indicates that m_time was not initialized!
		}

		*ptm = ptmTemp;
		return ptm;
	}

	return NULL;
}

int datetime::GetYear() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? (ptm->tm_year) + 1900 : 0 ;
}

int datetime::GetMonth() const      // month of year (1 = Jan)
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_mon + 1 : 0;
}

int datetime::GetDay() const         // day of month (0-31)
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_mday : 0 ;
}

int datetime::GetHour() const        // hour in day (0-23)
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_hour : -1 ;
}

int datetime::GetMin() const      // minute in hour (0-59)
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_min : -1 ;
}

int datetime::GetSec() const      // second in minute (0-59)
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_sec : -1 ;
}

int datetime::GetDayOfWeek() const   // 1=Sun, 2=Mon, ..., 7=Sat
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_wday + 1 : 0 ;
}

int datetime::GetDayOfYear() const   // days since start of year, Jan 1 = 1 [0~365]
{
	struct tm ttm;
	struct tm * ptm;

	ptm = GetLocalTm(&ttm);
	return ptm ? ptm->tm_yday + 1 : 0 ;
}

int datetime::GetDays(const datetime& t2) const
{
	if(*this>t2)	return 0;
	datetime TempT1, TempT2;
	TempT1.SetDateTime(GetYear(),GetMonth(),GetDay(),0,0,0);
	TempT2.SetDateTime(t2.GetYear(),t2.GetMonth(),t2.GetDay(),23,59,59);
	return TempT1.DiffTotalDays(TempT2);
}

bool datetime::SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant

	assert( nYear >= 1900 );
	assert( nMonth >= 1 && nMonth <= 12 );
	assert( nDay >= 1 && nDay <= 31 );
	assert( nHour >= 0 && nHour <= 23 );
	assert( nMin >= 0 && nMin <= 59 );
	assert( nSec >= 0 && nSec <= 59 );

#pragma warning (pop)

	struct tm atm;

	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	atm.tm_mday = nDay;
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = 0; //nDST;

	m_time = _mktime64(&atm);
	return m_time != -1;       // indicates an illegal input time
}

bool datetime::SetDate(int nYear, int nMonth, int nDay)
{
	return SetDateTime(nYear, nMonth, nDay, GetMonth(), GetMin(), GetSec());	
}

bool datetime::SetTime(int nHour, int nMin, int nSec)
{
	return SetDateTime(GetYear(), GetMonth(), GetDay(), nHour, nMin, nSec);
}

bool datetime::AddDay(int n)
{
	m_time += n*24*3600;
	return true;
}

bool datetime::AddHour(int n)
{
	m_time += n*3600;
	return true;
}

bool datetime::AddMin(int n)
{
	m_time += n*60;
	return true;
}

bool datetime::AddSec(int n)
{
	m_time += n;
	return true;	
}

string datetime::Format(const string &str) const
{
	if(str.empty())
	{
		return str;
	}

	char szBuffer[128];
	struct tm ptmTemp;

	if (_localtime64_s(&ptmTemp, &m_time) != 0)
	{
		return "";
	}

	if (!strftime(szBuffer, 128, str.c_str(), &ptmTemp))
	{
		szBuffer[0] = '\0';
	}

	return szBuffer;
}

datetime&  datetime::operator=(const datetime& timeSrc)
{
	m_time = timeSrc.m_time;
	return *this;
}

bool  datetime::operator==(datetime time) const
{
	return m_time == time.m_time;
}

bool  datetime::operator!=(datetime time) const
{
	return m_time != time.m_time;
}

bool  datetime::operator<(datetime time) const
{
	return m_time < time.m_time;
}

bool  datetime::operator>(datetime time) const
{
	return m_time > time.m_time;
}

bool  datetime::operator<=(datetime time) const
{
	return m_time <= time.m_time;
}

bool  datetime::operator>=(datetime time) const
{
	return m_time >= time.m_time;
}


//////////////////////////////////////////////////////////////////////////
long datetime::DiffTotalDays(const datetime& t2)	 //跨越的天数
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(t/ (24*3600L));
}

long datetime::DiffTotalDaysEx(const datetime& t2) //跨越的天数, 不满一天算一天
{
	if(*this>t2)	return 0;
	datetime TempT1, TempT2;
	TempT1.SetDateTime(GetYear(),GetMonth(),GetDay(),0,0,0);
	TempT2.SetDateTime(t2.GetYear(),t2.GetMonth(),t2.GetDay(),23,59,59);
	TempT2.AddSec(1);
	return long(TempT1.DiffTotalDays(TempT2));
}

long datetime::DiffTotalHours(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(t/3600);
}

long datetime::DiffTotalHoursEx(const datetime& t2)
{
	if(*this>t2)	return 0;
	if(DiffMins(t2) + DiffSecs(t2) > 0)
		return long(DiffTotalHours(t2)+1);
	else 
		return long(DiffTotalHours(t2));
}

long datetime::DiffHours(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(DiffTotalHours(t2) - DiffTotalDays(t2)*24);
}

long datetime::DiffHoursEx(const datetime& t2)
{
	if(*this>t2)	return 0;
	if(DiffMins(t2) + DiffSecs(t2) > 0)
		return long(DiffHours(t2)+1);
	else 
		return long(DiffHours(t2));
}

long datetime::DiffTotalMins(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(t/60);
}

long datetime::DiffTotalMinsEx(const datetime& t2)
{
	if(*this>t2)	return 0;
	if(DiffSecs(t2) > 0)
		return long(DiffTotalMins(t2)+1);
	else 
		return long(DiffTotalMins(t2));
}

long datetime::DiffMins(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(DiffTotalMins(t2) - DiffTotalHours(t2)*60);
}

long datetime::DiffMinsEx(const datetime& t2)
{
	if(*this>t2)	return 0;
	if(DiffSecs(t2) > 0)
		return long(DiffMins(t2)+1);
	else 
		return long(DiffMins(t2));
}

long datetime::DiffTotalSecs(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(t);
}

long datetime::DiffSecs(const datetime& t2)
{
	if(*this>t2)	return 0;
	time_t t = t2.m_time-m_time;
	return long(DiffTotalSecs(t2) - DiffTotalMins(t2)*60);
}







//////////////////////////////////////////////////////////////////////////
long DiffTotalDays(const datetime& t1, const datetime& t2)	 //跨越的天数
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return long(t/ (24*3600L));
}

long DiffTotalDaysEx(const datetime& t1, const datetime& t2) //跨越的天数, 不满一天算一天
{
	if(t1>t2)	return 0;
	datetime TempT1, TempT2;
	TempT1.SetDateTime(t1.GetYear(),t1.GetMonth(),t1.GetDay(),0,0,0);
	TempT2.SetDateTime(t2.GetYear(),t2.GetMonth(),t2.GetDay(),23,59,59);
	TempT2.AddSec(1);
	return TempT1.DiffTotalDays(TempT2);
}

long DiffTotalHours(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return long(t/3600);
}

long DiffTotalHoursEx(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	if(DiffMins(t1,t2) + DiffSecs(t1,t2) > 0)
		return DiffTotalHours(t1,t2)+1;
	else 
		return DiffTotalHours(t1,t2);
}

long DiffHours(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return DiffTotalHours(t1,t2) - DiffTotalDays(t1,t2)*24;
}

long DiffHoursEx(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	if(DiffMins(t1,t2) + DiffSecs(t1,t2) > 0)
		return DiffHours(t1,t2)+1;
	else return DiffHours(t1,t2);
}

long DiffTotalMins(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return long(t/60);
}

long DiffTotalMinsEx(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	if(DiffSecs(t1,t2) > 0)
		return DiffTotalMins(t1,t2)+1;
	else 
		return DiffTotalMins(t1,t2);
}

long DiffMins(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return DiffTotalMins(t1,t2) - DiffTotalHours(t1,t2)*60;
}

long DiffMinsEx(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	if(DiffSecs(t1,t2) > 0)
		return DiffMins(t1,t2)+1;
	else return DiffMins(t1,t2);
}

long DiffTotalSecs(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return long(t);
}

long DiffSecs(const datetime& t1, const datetime& t2)
{
	if(t1>t2)	return 0;
	time_t t = t2.m_time-t1.m_time;
	return DiffTotalSecs(t1,t2) - DiffTotalMins(t1,t2)*60;
}