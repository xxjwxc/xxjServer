/********************************************************************
    创建时间：    2015/08/06  10:16:23 
    文件名：      MyTimes.h
    作者：        xxj
    
    功能:		1.时间相关函数
				2.
                
    说明:		1. 
                2. 
*********************************************************************/
#ifndef _MYTIMES_H_
#define _MYTIMES_H_
#include "DefineBase.h"
#include <string>

#define GMT_OFFSET (8 * 60 * 60)
#define WEEK_OFFSET 3 //星期4
#define CST_OFFSET (14*60L*60L) //CST时间偏移
#define DAY_OFFSET (24*60L*60L) //CST时间偏移

class CMyTimes
{
public:
	/*
		获取当前时间 Unix时间戳 1970/1/1 8:0:0 秒
	*/
	static MY_UINT32 get_utc();
	static MY_LONGLONG GetUTCTime(bool ismicroseconds);
	static MY_UINT32 Get_current_day_0_0();//获取今天的0时0分0秒
	static MY_UINT32 Get_current_day_0_0(MY_UINT32 _time);//当前时间的0时0分0秒
	static int get_week(MY_UINT32 unCurrentTime);//1~0
	static int get_day(MY_UINT32 unCurrentTime);
	static int get_month(MY_UINT32 unCurrentTime);
	static MY_UINT32 get_day_time(MY_UINT32 unCurrentTime);//获取今天凌晨偏移时间
	static MY_UINT32 Get_Detail_from_str(const char * str);//时间转换成ACE_UINT32,格式：年-月-日[空格]时:分:秒(2015-04-21 12:00:00	
	static MY_UINT32 CSTtoUTC(const char * str);
	static std::string GetTimeStr(MY_UINT32 _time);//获取时间 返回格式：年-月-日[空格]时:分:秒(2015-04-21 12:00:00
	static std::string GetDayStr(MY_UINT32 _time);//获取时间 返回格式：年-月-日
	static MY_UINT32 Get_day_Offset(MY_UINT32 _time, int offset);//获取offset天的一天
	static int GetSubDay(MY_UINT32 _start,MY_UINT32 _end);
	static MY_UINT32 Get_Hour(MY_UINT32 _time);////获取今天 0分0秒 小时
	static MY_UINT32 Get_Month_Offset_0(MY_UINT32 _time, int offset);//获取offset个月前的1号 月初
	
};

#endif