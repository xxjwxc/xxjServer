#include <iostream>
#include <Tools/ConsoleHandler.h>
#include <Tools/TimerDeal/TimerDeal.h>
#ifdef _WIN32
#include <Tools/MyDump.h>
#endif

#include "src/data/OnHttpDeal.h"
#include "src/data/OnDataDeal.h"
#include "src/data/MyIni.h"
#include <chrono>
#include <Tools/MyStream/MyStreamBase.hpp>
#include <Tools/MyTools.h>
#include <Tools/MyTimes.h>

const MY_UINT32 getCurrentSystemTime()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	auto ss = duration_in_ms.count();
	return ss;
}

static bool s_bTerminal = false;
void OnTerminalCallBack()
{
	s_bTerminal = true;
}

int main()
{
	
	MY_ULONGLONG ss = 0 - 1;// 1497004948524;// getCurrentSystemTime();
	MY_LONGLONG ww = CMyTimes::GetUTCTime(false);
	std::cout << ss <<std::endl;
#ifdef _WIN32
	MEMORY_CHICK;//内存检测
	SetUnhandledExceptionFilter(MyDmp::MyUnhandledExceptionFilter);//设置回调函数
#endif
	OnDataDeal::instance()->OnLoadTimestamp();
// 	MyHttpDeal::instance()->SetTimeOut(CMyIni::instance()->m_timeOut);
// 	CTimerDeal::instance()->AddOneCall(boost::bind(&MyHttpDeal::OnTimeCallBack, MyHttpDeal::instance()));
// 	CTimerDeal::instance()->OnStart();
// 
// 	MyConsoleHandler::OnStart(OnTerminalCallBack);//控制退出
// 
// 	while (!s_bTerminal)
// 	{
// #ifdef _WIN32
// 		Sleep(10000);
// #else
// 		sleep(10);
// #endif
// 	}
// 
// 	//结束定时器
// 	CTimerDeal::instance()->OnStop();

	MyHttpDeal::instance()->OnTimeOut();

    return 0;
}