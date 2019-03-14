#include "MyClient.h"
#include <Tools/Asio/GameDefine.h>


//#include "../data/MyIni.h"


boost::asio::io_service CMyClient::g_ios;
//bool ISocketBase::isLog = false;
CMyClient::sock_pt g_sock;
boost::thread CMyClient::g_thrd;

CMyClient::CMyClient(boost::asio::io_service & io, std::string ip, unsigned short port, void * iSB) :ios(io),
m_ep(boost::asio::ip::address::from_string(ip), port)
{
	m_iSocketBase = static_cast<ISocketBase *>(iSB);
	assert(m_iSocketBase != nullptr && "ISocketBase is null");//不允许构造
	m_iSocketBase->isLog = true;
}

void CMyClient::Start()
{
	//m_str = "123";
	g_sock.reset();
	sock_pt sock(new boost::asio::ip::tcp::socket(ios));
	if (m_iSocketBase->isLog)
		std::cout << "connection [ip:port]:" <<m_ep.address()<<":"<< m_ep.port() << std::endl;
	sock->async_connect(m_ep,//异步连接
		boost::bind(&CMyClient::Conn_handler,this,boost::asio::placeholders::error,sock));
}

void CMyClient::Conn_handler( const boost::system::error_code & ec,sock_pt sock )
{
	if (ec)
	{
		sock->async_connect(m_ep,//异步连接
			boost::bind(&CMyClient::Conn_handler,this,boost::asio::placeholders::error,sock));
		m_iSocketBase->DisConnect();
		//g_ios.stop();
		return;
	}

	g_sock = sock;
	m_iSocketBase->OnConnect();

	// 		sock->async_write_some(boost::asio::buffer(m_str),
	// 			boost::bind(&CMyClient::Write_handler,this,boost::asio::placeholders::error,sock));//先请求
	// 		sock->async_read_some(boost::asio::buffer(_Data), 
	// 			boost::bind(&CMyClient::Read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	//boost::shared_ptr<std::vector<char> > str(new std::vector<char>(1024,0));
	sock->async_read_some(boost::asio::buffer(_Data),boost::bind(&CMyClient::Read_handler,this,boost::asio::placeholders::error,sock,boost::asio::placeholders::bytes_transferred));

}

void CMyClient::Read_handler( const boost::system::error_code & ec, sock_pt sock,std::size_t bytes_transferred )
{
	if (ec)
	{
		m_iSocketBase->DisConnect();
		Start();
		//g_ios.stop();
		return;
	}

	std::vector<MsgInfo> ver = m_myAsioTools.UnSerializingBinary(_Data, bytes_transferred);

	m_iSocketBase->OnRead(ver);

	//具体业务逻辑处理
// 	std::vector<std::string>::iterator it = ver.begin();
// 	while(it != ver.end())
// 	{
// 		std::cout<<*it<<std::endl;
// 		
// 		it++;
// 	}
// 	CDoAction doaction;
// 	std::vector<std::string>::iterator it = ver.begin();
// 	while(it != ver.end())
// 	{
// 		doaction.OnMessage(*it);
// 		it++;
// 	}
	//---------------------end

	//boost::shared_ptr<std::vector<char> > _str(new std::vector<char>(1024,0));
	sock->async_read_some(boost::asio::buffer(_Data),boost::bind(&CMyClient::Read_handler,this,boost::asio::placeholders::error,sock,boost::asio::placeholders::bytes_transferred));


	// 		sock->close();//关闭链接
	// 		g_ios.stop();
};

void CMyClient::Write_handler( const boost::system::error_code & ec,sock_pt sock,void * isb)
{
	if (ec)
	{
		//Start();
		//g_ios.stop();
		return;
	}


	if (isb)
		((ISocketBase *)isb)->OnWriteDown();

	//str;
	//sock->close();//关闭链接
};



void CMyClient::WriteSomeSomething(const boost::scoped_array<char> & str, MY_UINT32 len, void * isb)
{
	if (g_sock)
	{
		g_sock->async_write_some(boost::asio::buffer(str.get(), len),
			boost::bind(&CMyClient::Write_handler, boost::asio::placeholders::error, g_sock,isb));//先请求
		std::cout << "write seccess!\n";
	}
}

void CMyClient::OnStopClient()
{
	CMyClient::g_ios.stop();
	g_thrd.join();
}



