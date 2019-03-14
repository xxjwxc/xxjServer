/********************************************************************
    创建时间：    2015/08/06  14:56:24 
    文件名：      TimerDeal.h
    作者：        xxj
    
    功能:		1.定时器
				2.没秒执行一次 调用SignalTrivial
                
    说明:		1. 
                2. 
*********************************************************************/
#ifndef TIMERDEAL_H_
#define TIMERDEAL_H_
#include "../MySingleton.h"
#include "../SingnalTrivial/SignalSlotTrivial.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

class CTimerDeal:public MySingleton<CTimerDeal>
{
public:
	CTimerDeal():m_dt(NULL){}
	void OnStart();
	void OnStop();
	void AddOneCall(SignalTrivial::Functor func);
	void OnClear();
private:
	void OnCall(const boost::system::error_code& e,  boost::asio::deadline_timer & t);
	void ThreadRun() ;
	SignalTrivial m_singnalTrivial;
	boost::thread m_thread;
	boost::asio::io_service m_io;
	boost::asio::deadline_timer * m_dt;
};

#endif