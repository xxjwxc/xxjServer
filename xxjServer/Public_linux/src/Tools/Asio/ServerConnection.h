/********************************************************************
	创建时间：	2014/08/07  10:58:17 
	文件名：    ServerConnection
	作者：		谢小军
	
	功能：		1.通信接口类
				2.只处理登录时，超时过后就踢掉
				
	说明：	    1. 服务器中连接的客户端连接信息
				2. 
*********************************************************************/
#ifndef _SERVERCONNECTION1_H_
#define _SERVERCONNECTION1_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>
#include "DefineBase.h"
#include "../MySingleton.h"
#include "ClientSession.h"




//通信接口
class CServerConnectionData:public MySingleton<CServerConnectionData>
{
public:
	typedef boost::shared_ptr<ClientSession> Session_ptr;
	CServerConnectionData():m_verConnection(new std::vector<Session_ptr>),m_timeDida(0){}
	~CServerConnectionData();
public:	
	boost::mutex m_mutex;
	boost::shared_ptr<std::vector<Session_ptr> > m_verConnection;
	void AddOneConnection(const Session_ptr & _ptr);
	bool DeleteOneConnection(const Session_ptr & _ptr);
	bool DeleteOneNoSoket(Session_ptr & _ptr);//删除一个缓存，但是不删除socket
	//void SendAllMessage(const std::string strMsg);//发送消息
public:
	//执行回调事件
	void OnTimerCallBack();//单个线程出现
	MY_UINT32 m_timeDida;//时间回调
};

#endif