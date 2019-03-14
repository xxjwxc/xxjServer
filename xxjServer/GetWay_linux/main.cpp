#include <iostream>
#include <Tools/ConsoleHandler.h>
#include <Tools/TimerDeal/TimerDeal.h>
#include "src/asio/base/ServerMain.h"
#include "test.h"

#ifdef _WIN32
#include <Tools/MyDump.h>
#endif
#include "src/data/MyIni.h"
#include <thread>
#include <Tools/Rand/MyRand.h>
#include <Tools/MyStream/MyStreamBase.hpp>
#include <Tools/MyTools.h>

static bool s_bTerminal = false;
void OnTerminalCallBack()
{
	s_bTerminal = true;
}

int main()
{
#ifdef _WIN32
	MEMORY_CHICK;//内存检测
	SetUnhandledExceptionFilter(MyDmp::MyUnhandledExceptionFilter);//设置回调函数
#endif

	CServerMain::instance()->OnStart();//用于与客户端连接线程

	MyConsoleHandler::OnStart(OnTerminalCallBack);//控制退出
// 
// 	DealTest::instance()->OnInit();
// 	//DealTest::instance()->SetTimeOut(CMyIni::instance()->m_timeOut);
// 	//CTimerDeal::instance()->AddOneCall(boost::bind(&DealTest::OnTimeCallBack, DealTest::instance()));
// 	//CTimerDeal::instance()->OnStart();
// 	std::thread tn[200];
// 	CMyRand randm;
// 	while (true)
// 	{
// 		for (int i = 0; i < 200; i++)
// 		{
// #ifdef _WIN32
// 			Sleep(randm.GetRand(10));
// #else
// 			usleep(randm.GetRand(10*1000));
// #endif
// 			
// 			tn[i] = std::thread(&DealTest::OnDeal, DealTest::instance());//创建一个分支线程，回调到myThread函数里
// 		}
// 
// 		for (int i = 0; i < 200; i++)
// 		{
// 			tn[i].join();
// 		}
// 		
// 		for (int i = 0; i < 10; i++)
// 		{
// 			std::cout << i << std::endl;
// #ifdef _WIN32
// 			Sleep(1000);
// #else
// 			sleep(1);
// #endif
// 			
// 		}
// 		
// 	}


	


	while (!s_bTerminal)
	{
		//int *p = new int[100];
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(10);
#endif
	}

	//结束定时器
	CTimerDeal::instance()->OnStop();

	CServerMain::instance()->OnStop();//关闭网络IO
    return 0;
}