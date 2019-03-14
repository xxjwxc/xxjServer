
#include <iostream>
#include "src/data/flatbuffers/test_generated.h"
#include <Tools/ConsoleHandler.h>
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

	MyConsoleHandler::OnStart(OnTerminalCallBack);//¿ØÖÆÍË³ö
#ifdef _WIN32
	std::string str;
	while (!s_bTerminal)
	{
		std::cin >> str;
		if (str.length()>= 0)
		{
			flatbuffers::FlatBufferBuilder builder;
			// Create list 
			std::vector<size_t> vec;
			for (size_t i = 0; i < 10; i++)
			{
				vec.push_back(i);
			}
			auto id = 123;
			auto name = builder.CreateString("test\0");
			auto list = builder.CreateVector(vec); // vector
			uint8_t flag = 35;
			auto kv = TestApp::KV(1, 1.0); // struct
								  // table 
			auto mloc = TestApp::CreateTestObj(builder, id, name, flag, list, &kv);
			builder.Finish(mloc);

			char* ptr = (char*)builder.GetBufferPointer();
			uint64_t size = builder.GetSize();

			////////// Deserialize //////////
			auto obj = TestApp::GetTestObj((uint8_t*)ptr);

			std::cout << obj->id() << std::endl;
			std::cout << obj->name()->c_str() << std::endl;
			std::cout << obj->flag() << std::endl;
			for (size_t i = 0; i < obj->list()->size(); i++)
			{
				std::cout << obj->list()->Get(i) << std::endl;
			}
			std::cout << obj->kv()->key() << std::endl;
			std::cout << obj->kv()->value() << std::endl;
			//-----------------------------end


			MsgInfo msg;
			msg.m_body.reset(new char[size]);
			memcpy(msg.m_body.get(), ptr,size);
			msg.m_body_len = size;
			msg.m_head.unMsgID = 1024;

			boost::scoped_array<char> m_buffer;
			CMyAsioTools tools;
			size = tools.SerializingBinary(msg,m_buffer);
			
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