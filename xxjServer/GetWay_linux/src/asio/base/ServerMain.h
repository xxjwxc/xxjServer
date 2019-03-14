/********************************************************************
    创建时间：    2015/08/10  10:40:08 
    文件名：      ServerRoot.h
    作者：        xxj
    
    功能:		1.服务器启动模块
				2.
                
    说明:		1. asio 启动模块
                2. 
*********************************************************************/
#ifndef __SERVER_MAIN_H_
#define __SERVER_MAIN_H_
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/noncopyable.hpp>
#include <Tools/MySingleton.h>

class CServerMain:public MySingleton<CServerMain>
{
public:
	void OnStart();
	void OnStop();
private:
	void StartServer(std::string ip, unsigned short port);
	boost::asio::io_service m_ioService;
	boost::thread m_mainThrd;//主线程

};

#endif