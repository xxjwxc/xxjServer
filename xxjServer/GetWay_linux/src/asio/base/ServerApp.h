/********************************************************************
	创建时间：	2015/04/25  0:49:21
	文件名：	ServerApp.h
	作者：		谢小军
	
	功能：		1.asio服务器 
				2.用于接收连接 并 创建初始对象
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef __SERVERAPP_H_
#define __SERVERAPP_H_

#include "../MyClient.h"

class CserverApp
{
	typedef boost::shared_ptr<CMyClient> Session_ptr;
public:
	CserverApp(boost::asio::io_service& ioservice, boost::asio::ip::tcp::endpoint& endpoint);
	~CserverApp();
public:
	void OnAccept();//等待连接
	void Handle_accept(const boost::system::error_code& error, Session_ptr& session);
private:
	boost::asio::io_service& m_ioservice;
	boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // !__SERVERAPP_H_
