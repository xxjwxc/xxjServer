#ifndef MYCLIENT_H_
#define MYCLIENT_H_
#include <string>
#include <vector>

//boost asio head 
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif 
// #define BOOST_REGEX_NO_LIB
// #define BOOST_DATE_TIME_SOURCE
// #define BOOST_SYSTEM_NO_LOB
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/smart_ptr/scoped_array.hpp>
#include <Tools/MyTools.h>
#include <boost/thread/thread.hpp>
#include "MyAsioTools.h"
#include <Tools/Asio/ISocketBase.h>



typedef unsigned int MY_UINT32;

#define MAX_RESERVE_SIZE 1024*8  //最大接收尺寸
//#define max_len 1024

class CMyClient
{
public:
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;//client 不需要accept，直接connect

	static boost::asio::io_service g_ios;
private:
	static boost::thread g_thrd;
	boost::asio::io_service & ios;//io_server对象
	boost::asio::ip::tcp::endpoint m_ep;//TCP 端点
	CMyAsioTools m_myAsioTools;
	char _Data[MAX_RESERVE_SIZE];
	ISocketBase * m_iSocketBase = nullptr;
private:
	CMyClient() = delete;
	 CMyClient(boost::asio::io_service & io,std::string ip, unsigned short port,void * iSB );
	~CMyClient(){};

	void Start();

	void Conn_handler(const boost::system::error_code & ec,sock_pt sock);

	void Read_handler(const boost::system::error_code & ec, sock_pt sock,std::size_t bytes_transferred);

	static void Write_handler(const boost::system::error_code & ec,sock_pt sock, void * isb);

	template <typename T>
	static void StartClient_thread();
public:
	static void WriteSomeSomething(const boost::scoped_array<char> & str, MY_UINT32 len,void * isb);
	template <typename T>
	static void OnStartClient();//开始服务器客户端交互
	static void OnStopClient();
	
};

template <typename T>
void CMyClient::StartClient_thread()
{
	try
	{
		//CMyClient cl(g_ios,CMyIni::instance()->m_serverIP,CMyIni::instance()->m_serverPort);
		CMyClient cl(g_ios, "127.0.0.1",5123, (&(*(T::instance()))));
		cl.Start();
		g_ios.run();
		g_ios.reset();
	}
	catch (std::exception & e)
	{
		std::cout << "StartClient_thread:" << e.what() << std::endl;
	}
}

template <typename T>
void CMyClient::OnStartClient()
{
	try
	{
		g_thrd = boost::thread(&CMyClient::StartClient_thread<T>);
		g_thrd.yield();
	}
	catch (std::exception & e)
	{
		std::cout << "OnStartClient error:" << e.what() << std::endl;
	}

}



#endif