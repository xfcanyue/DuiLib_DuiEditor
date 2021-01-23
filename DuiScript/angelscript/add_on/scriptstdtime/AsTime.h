// AsTime.h: interface for the CAsTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTIME_H__769E4098_F982_47F5_8E40_E0ADB6187DE9__INCLUDED_)
#define AFX_ASTIME_H__769E4098_F982_47F5_8E40_E0ADB6187DE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "time.h"
#include <assert.h>
#include <string>
using namespace std;

class datetime  
{
public:
  	void AddRef() {refCount++;}  	
  	void Release() {refCount--; if( refCount == 0 ) delete this;}  	
  	int refCount;  
	//////////////////////////////////////////////////////////////////////
	datetime();

	//拷贝构造函数
	datetime(const datetime& timeSrc);

	void GetTime();	//获取当前电脑时间

	struct tm* GetLocalTm(struct tm* ptm = NULL) const;
	int GetYear() const;
	int GetMonth() const;      // month of year (1 = Jan)
	int GetDay() const;         // day of month (0-31)
	int GetHour() const;        // hour in day (0-23)
	int GetMin() const;      // minute in hour (0-59)
	int GetSec() const;      // second in minute (0-59)
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
	int GetDayOfYear() const;   // days since start of year, Jan 1 = 1
	int GetDays(const datetime& t2) const; //跨越的实际天数

	bool SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
	bool SetDate(int nYear, int nMonth, int nDay);
	bool SetTime(int nHour, int nMin, int nSec);

	bool AddDay(int n);	
	bool AddHour(int n);
	bool AddMin(int n);
	bool AddSec(int n);

	string Format(const string &str) const;

	datetime& operator=(const datetime& timeSrc);
	bool operator==(datetime time) const;
	bool operator!=(datetime time) const;
	bool operator<(datetime time) const;	
	bool operator>(datetime time) const;
	bool operator<=(datetime time) const;
	bool operator>=(datetime time) const;
	//////////////////////////////////////////////////////////////////////////

	long DiffTotalDays(const datetime& t2);	 //跨越的天数
	long DiffTotalDaysEx(const datetime& t2); //跨越的天数, 不满一天算一天
	
	long DiffTotalHours(const datetime& t2);
	long DiffTotalHoursEx(const datetime& t2);
	long DiffHours(const datetime& t2);
	long DiffHoursEx(const datetime& t2);
	
	long DiffTotalMins(const datetime& t2);
	long DiffTotalMinsEx(const datetime& t2);
	long DiffMins(const datetime& t2);
	long DiffMinsEx(const datetime& t2);
	
	long DiffTotalSecs(const datetime& t2);
	long DiffSecs(const datetime& t2);

	//////////////////////////////////////////////////////////////////////////
	__time64_t m_time;
};


long DiffTotalDays(const datetime& t1, const datetime& t2);	 //跨越的天数
long DiffTotalDaysEx(const datetime& t1, const datetime& t2); //跨越的天数, 不满一天算一天

long DiffTotalHours(const datetime& t1, const datetime& t2);
long DiffTotalHoursEx(const datetime& t1, const datetime& t2);
long DiffHours(const datetime& t1, const datetime& t2);
long DiffHoursEx(const datetime& t1, const datetime& t2);

long DiffTotalMins(const datetime& t1, const datetime& t2);
long DiffTotalMinsEx(const datetime& t1, const datetime& t2);
long DiffMins(const datetime& t1, const datetime& t2);
long DiffMinsEx(const datetime& t1, const datetime& t2);

long DiffTotalSecs(const datetime& t1, const datetime& t2);
long DiffSecs(const datetime& t1, const datetime& t2);	

#endif // !defined(AFX_ASTIME_H__769E4098_F982_47F5_8E40_E0ADB6187DE9__INCLUDED_)
