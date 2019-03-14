#include "MyTimes.h"
#include <boost/timer.hpp>
#include <boost/date_time.hpp>

#include <ctime>
#include <chrono>

MY_UINT32 CMyTimes::get_utc()
{
	return (MY_UINT32)std::time(NULL);
	//return std::mktime(&boost::posix_time::to_tm(boost::posix_time::second_clock::local_time()));
	//std::cout<<boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
}

MY_UINT32 CMyTimes::Get_current_day_0_0()
{
	time_t t;
	time(&t);
	struct tm target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r( &t, &target_time);
#endif // _WIN32
	
	target_time.tm_hour = 0;
	target_time.tm_min = 0;
	target_time.tm_sec = 0;
	target_time.tm_isdst = -1;

	return (MY_UINT32)mktime(&target_time);
}

MY_UINT32 CMyTimes::get_day_time(MY_UINT32 unCurrentTime)
{
	MY_UINT32 v = 24L * 60L * 60L;
	MY_UINT32 t = (unCurrentTime + GMT_OFFSET) % v;
	return t;
}

int CMyTimes::get_week(MY_UINT32 unCurrentTime)
{
	MY_UINT32 t = (unCurrentTime + GMT_OFFSET) / DAY_OFFSET;

	t = t + WEEK_OFFSET + 1;

	return (t % 7);
}

MY_UINT32 CMyTimes::Get_Detail_from_str(const char * str)
{
	time_t time1;
	try
	{
		struct tm tm1;
#ifdef _WIN32
		sscanf_s(str, "%4d-%2d-%2d %2d:%2d:%2d",
			&tm1.tm_year,
			&tm1.tm_mon,
			&tm1.tm_mday,
			&tm1.tm_hour,
			&tm1.tm_min,
			&tm1.tm_sec
		);
#else
		sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
			&tm1.tm_year,
			&tm1.tm_mon,
			&tm1.tm_mday,
			&tm1.tm_hour,
			&tm1.tm_min,
			&tm1.tm_sec
		);
#endif // _WIN32


		tm1.tm_year -= 1900;
		tm1.tm_mon--;
		tm1.tm_isdst = -1;
		time1 = mktime(&tm1);
	}
	catch (...)
	{
		printf("error Get_Time_from_str");
	}


	return time1;
}

MY_UINT32 CMyTimes::CSTtoUTC(const char * str)
{
	// 北京时间 = CST(美国中央时间)+ 14小时
	MY_UINT32 _time = Get_Detail_from_str(str);
	return _time;// +CST_OFFSET;
}

MY_UINT32 CMyTimes::Get_current_day_0_0(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r( &t, &target_time); //获取当地时间
#endif // DEBUG
	
	target_time.tm_hour = 0;
	target_time.tm_min = 0;
	target_time.tm_sec = 0;
	target_time.tm_isdst = -1;

	return (MY_UINT32)mktime(&target_time);
}

std::string CMyTimes::GetTimeStr(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
	target_time.tm_year += 1900;
	target_time.tm_mon++;
	target_time.tm_isdst = -1;
	char str[32];
	memset(str, 0, 32);
#ifdef _WIN32
	sprintf_s(str, 31, "%4d-%02d-%02d %02d:%02d:%02d",
		target_time.tm_year,
		target_time.tm_mon,
		target_time.tm_mday,
		target_time.tm_hour,
		target_time.tm_min,
		target_time.tm_sec
	);
#else 
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d",
		target_time.tm_year,
		target_time.tm_mon,
		target_time.tm_mday,
		target_time.tm_hour,
		target_time.tm_min,
		target_time.tm_sec
	);
#endif // _WIN32


	return std::string(str);
}

std::string CMyTimes::GetDayStr(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
	target_time.tm_year += 1900;
	target_time.tm_mon++;
	target_time.tm_isdst = -1;
	char str[16];
	memset(str, 0, 16);
#ifdef _WIN32
	sprintf_s(str, 15, "%4d-%02d-%02d",
		target_time.tm_year,
		target_time.tm_mon,
		target_time.tm_mday
	);
#else
	sprintf(str, "%4d-%02d-%02d",
		target_time.tm_year,
		target_time.tm_mon,
		target_time.tm_mday
	);
#endif // _WIN32


	return std::string(str);
}

int CMyTimes::get_day(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
	target_time.tm_year += 1900;
	target_time.tm_mon++;
	target_time.tm_isdst = -1;

	return target_time.tm_mday;
}

MY_UINT32 CMyTimes::Get_day_Offset(MY_UINT32 _time, int offset)
{
	return _time + offset*(DAY_OFFSET);
}

int CMyTimes::get_month(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
	target_time.tm_year += 1900;
	target_time.tm_mon++;
	target_time.tm_isdst = -1;

	return target_time.tm_mon;
}

int CMyTimes::GetSubDay(MY_UINT32 _start, MY_UINT32 _end)
{
	return (Get_current_day_0_0(_end) - Get_current_day_0_0(_start)) / DAY_OFFSET;
}

MY_UINT32 CMyTimes::Get_Hour(MY_UINT32 _time)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
	target_time.tm_year += 1900;
	target_time.tm_mon++;
	target_time.tm_isdst = -1;

	return target_time.tm_hour;
}

MY_UINT32 CMyTimes::Get_Month_Offset_0(MY_UINT32 _time, int offset)
{
	time_t t = _time;
	struct tm  target_time;
#ifdef _WIN32
	localtime_s(&target_time, &t); //获取当地时间
#else
	localtime_r(&t, &target_time); //获取当地时间
#endif // DEBUG
								   // 	target_time.tm_year += 1900;
								   // 	target_time.tm_mon++;
								   // 	target_time.tm_mday = 1;
								   // 	target_time.tm_hour = 0;
								   // 	target_time.tm_min = 0;
								   // 	target_time.tm_sec = 0;
								   // 	target_time.tm_isdst = -1;

	boost::gregorian::date d = boost::gregorian::date_from_tm(target_time);
	boost::gregorian::months m(offset);
	d = d + m;

	struct tm  tm1;
	tm1 = boost::gregorian::to_tm(d);
	// 	tm1.tm_year += 1900;
	// 	tm1.tm_mon++;
	// 	tm1.tm_isdst = -1;
	return (MY_UINT32)mktime(&tm1);
}

MY_LONGLONG CMyTimes::GetUTCTime(bool ismicroseconds /*= false*/)
{
	if (ismicroseconds)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch())
			.count();
		//return timeGetTime();
// 		boost::posix_time::ptime time_now = boost::posix_time::microsec_clock::universal_time();
// 		return time_now.time_of_day().total_microseconds();
	}
	else
		return get_utc();

}

