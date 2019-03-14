/********************************************************************
	创建时间：	2014/08/07  10:58:17 
	文件名：    ServerConnection
	作者：		谢小军
	
	功能：		1.用户列表
				2.已经登录后的用户列表
				
	说明：	    1. 要求先登录完成
				2. 
*********************************************************************/
#ifndef _CLIENTLIST_H_
#define _CLIENTLIST_H_
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <map>
#include "../MySingleton.h"
//#include "MyClient.h"
#include "../Queue/MyQueue.h"


//通信接口
class CClientList:public MySingleton<CClientList>
{
public:
	typedef boost::shared_ptr<ClientSession> Session_ptr;
	CClientList();
	~CClientList();
public:	
	boost::mutex m_mutex;
	boost::shared_ptr<std::map<MY_UINT32,Session_ptr> > m_mapClient;
	void AddOneClient(const Session_ptr & _ptr);
	bool DeleteOnClient(Session_ptr & _ptr);

	void SendMsgToBroadcast(const MsgInfo & info);//发送消息 广播
	void SendMsgToOne(const MsgInfo & info);//发送消息 私人聊天

	void OnStart();
	void OnStop();
	void OnClear();
protected:
	void OnStartBroadcast();
	void OnStartOne();
private:
	bool m_isStop;
	boost::thread m_broadcastThrd;//广播线程
	boost::thread m_oneThrd;//私人聊天线程
	CMyQueue<MsgInfo> m_broadcastQue;
	CMyQueue<MsgInfo> m_oneQue;

private:
	//CMyJson m_myJsonOp;
};

#endif