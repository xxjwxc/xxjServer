#include "MyClientDataModel.h"
#include <iostream>
#include "../Asio/MyAsioTools.h"
#include "../Asio/MyClient.h"
#include <Tools/MyStream/MyStreamBase.hpp>

void CMyClientDataModel::OnRead(std::vector<MsgInfo> & ver1)
{
	MY_UINT32 test = 10;
	bool b = true;
	std::vector<MY_UINT32> ver;
	ver.push_back(123);
	std::string str = "我们";
	std::map<std::string, std::string> map;
	map["aaa"] = "bbb";
	//序列化
	CMyStreamBase ostream;
	ostream << map << str << b;
	ostream.Push(test).Push(ver);

	//const auto & ver1 = ostream.OnGetBuffer();
	boost::scoped_array<unsigned char> ptr;
	int len = ostream.OnGetBuffer(ptr);

	MsgInfo msg;
	msg.m_body_len = ostream.OnGetBuffer(msg.m_body);
	msg.m_head.unMsgID = 1024;

	boost::scoped_array<char> m_buffer;
	CMyAsioTools tools;
	size_t size = tools.SerializingBinary(msg, m_buffer);

	CMyClient::WriteSomeSomething(m_buffer, size, nullptr);
}

void CMyClientDataModel::OnRead(std::vector<std::string> & ver)
{

}

bool CMyClientDataModel::OnSend(const boost::scoped_array<char> & str, MY_UINT32 len)
{
	return true;
}

void CMyClientDataModel::OnConnect()
{
	std::cout << "connectionEd" << std::endl;
}

void CMyClientDataModel::DisConnect()
{

}

void CMyClientDataModel::OnWriteDown()
{

}

void CMyClientDataModel::UnSerializing(const char * data, int size)
{

}

