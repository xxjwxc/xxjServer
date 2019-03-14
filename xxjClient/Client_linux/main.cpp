
#include <iostream>
#include <Tools/ConsoleHandler.h>
#include <Tools/MyStream/MyStreamBase.hpp>
#include "src/Asio/MyClient.h"
#include "src/data/MyClientDataModel.h"



static bool s_bTerminal = false;
void OnTerminalCallBack()
{
	s_bTerminal = true;
}

int main(int argc, char *argv[])
{
	CMyClient::OnStartClient<CMyClientDataModel>();

	MyConsoleHandler::OnStart(OnTerminalCallBack);//控制退出
#ifdef _WIN32
	std::string str;
	while (!s_bTerminal)
	{
		std::cin >> str;
		if (str.length()>= 0)
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
			size_t size = tools.SerializingBinary(msg,m_buffer);
			
			CMyClient::WriteSomeSomething(m_buffer, size,nullptr);
		}
		//Sleep(1000);
	}
#else
	while (!s_bTerminal)
	{
#ifdef _WIN32
		Sleep(10000);
#else
		sleep(10);
#endif
	}
#endif

	CMyClient::OnStopClient();
	return 0;
}