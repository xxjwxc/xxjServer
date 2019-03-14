#include "ClientSession.h"
#include <boost/format.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include "../MyTools.h"

ClientSession::ClientSession(boost::asio::io_service & ioservice)
	:m_socket(ioservice)
{
	memset(_data, '\0', sizeof(_data));
}

ClientSession::~ClientSession()
{
	if (m_socket.is_open())
		m_socket.close();
}

void ClientSession::Start()
{
	ReadSomeSomething();
}

void ClientSession::ReadSomeSomething()
{
	//异步读取
	m_socket.async_read_some(boost::asio::buffer(_data, max_len),
		boost::bind(&ClientSession::handle_read, shared_from_this(),
		boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

void ClientSession::WriteSomeSomething( const boost::scoped_array<char> & str ,MY_UINT32 len)
{
		std::shared_ptr<char> _buffer(new char[len], std::default_delete<char[]>());
		//_buffer->reset(new char[len]);
		if (_buffer.get() != nullptr)
		{
			memcpy(_buffer.get(), str.get(), len);

			WriteSomeSomething(_buffer, len);
		}
		else {
			std::cout << "buffer error " << std::endl;
		}

// 	try {
// 		std::shared_ptr<char> _buffer(new char[len], std::default_delete<char[]>());
// 		//_buffer->reset(new char[len]);
// 		if (_buffer.get() != nullptr)
// 		{
// 			memcpy(_buffer.get(), str.get(), len);
// 
// 			boost::asio::async_write(m_socket,
// 				boost::asio::buffer(_buffer.get(), len),
// 				boost::asio::transfer_at_least(len),
// 				boost::bind(&ClientSession::handle_write,
// 					shared_from_this(), boost::asio::placeholders::error, _buffer));
// 		}
// 		else {
// 			std::cout << "buffer error " << std::endl;
// 		}
// 	}
// 	catch (std::exception & e) {
// 		std::cout <<e.what();
// 		std::string __errorInfo = e.what();
// 		MyTools::PrintError(__errorInfo);
// 		OnWriteDown();
// 	}


}

void ClientSession::WriteSomeSomething(const std::string & str)
{
		MY_UINT32 size = str.length() + 1;
		std::shared_ptr<char> _buffer(new char[size], std::default_delete<char[]>());
		//_buffer.reset(new char[size]);
		if (_buffer.get() != nullptr)
		{
			memcpy(_buffer.get(), str.c_str(), size);

			WriteSomeSomething(_buffer, size);
		}
		else {
			std::cout << "buffer error " << std::endl;
		}
// 	try {
// 		MY_UINT32 size = str.length() + 1;
// 		std::shared_ptr<char> _buffer(new char[size],std::default_delete<char []>());
// 		//_buffer.reset(new char[size]);
// 		if (_buffer.get() != nullptr)
// 		{
// 			memcpy(_buffer.get(), str.c_str(), size);
// 
// 			boost::asio::async_write(m_socket,
// 				boost::asio::buffer(_buffer.get(), size),
// 				boost::asio::transfer_at_least(size),
// 				boost::bind(&ClientSession::handle_write,
// 					shared_from_this(), boost::asio::placeholders::error, std::move(_buffer)));
// 		}
// 		else {
// 			std::cout << "buffer error " << std::endl;
// 		}
// 	}
// 	catch (std::exception & e) {
// 		std::cout << e.what();
// 		std::string __errorInfo = e.what();
// 		MyTools::PrintError(__errorInfo);
// 		OnWriteDown();
// 	}
}

void ClientSession::WriteSomeSomething(const std::shared_ptr<char> & _buffer, MY_UINT32 len)
{
	try {
		//std::shared_ptr<char> _buffer(new char[len], std::default_delete<char[]>());
		//_buffer->reset(new char[len]);
		if (_buffer.get() != nullptr)
		{
			//memcpy(_buffer.get(), str.get(), len);

			boost::asio::async_write(m_socket,
				boost::asio::buffer(_buffer.get(), len),
				boost::asio::transfer_at_least(len),
				boost::bind(&ClientSession::handle_write,
					shared_from_this(), boost::asio::placeholders::error, _buffer));
		}
		else {
			std::cout << "buffer error " << std::endl;
		}
	}
	catch (std::exception & e) {
		std::cout << e.what();
		std::string __errorInfo = e.what();
		MyTools::PrintError(__errorInfo);
		OnWriteDown();
	}
}

void ClientSession::handle_write(const boost::system::error_code& error, std::shared_ptr<char> _buffer)
{
	if (error)
	{
		//m_socket.close();
		DisConnect();
	}
	
	//_buffer.reset();
	OnWriteDown();

}

void ClientSession::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (!error)
	{
		if (ISocketBase::isLog)
			std::cout <<"read data from client:"<< bytes_transferred << std::endl;

		try
		{
//			this->UnSerializing(_data, bytes_transferred);//解包
// 			ReadSomeSomething();//继续读取
// 			std::vector<std::string> ver = m_myAsioTools->UnSerializingString(_data, bytes_transferred);//解包
// 			this->OnRead(ver);
// 			ReadSomeSomething();//继续读取
			std::vector<MsgInfo> ver = m_myAsioTools->UnSerializingBinary(_data, bytes_transferred);//解包
			ReadSomeSomething();//继续读取
			this->OnRead(ver);
		}
		catch (std::exception & e)
		{
			std::vector<std::string> ver;
			ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) +"\t"+e.what());
			MyTools::PrintToFile("MsgLog/error.txt",ver,false);
			std::cout << "error：" <<e.what()<< std::endl;//请输入参数
			
			//m_socket.close();
			DisConnect();
		}
	}
	else
	{
		//std::cout << "error handle_read:" << error.message() << std::endl;
		//m_socket.close();
		DisConnect();
	}
}

void ClientSession::CloseSocket()
{
	if (m_socket.is_open())
	{
		m_socket.close();
	}
}


