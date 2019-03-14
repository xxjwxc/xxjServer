// Client_CLR_Test.cpp: 主项目文件。

#include "stdafx.h"
#include "../Client_CLR/src/Tools/ClientTypeDefine.h"


using namespace System;
#using <Client_CLRD.dll>


 static void _OnENetStateCallBack(ENetState e)
 {
 	Console::WriteLine(L"OnENetStateCallBack:{0}\n" ,e);
 };

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hello World");
	CClientCore^ clientcore = gcnew CClientCore();
	bool b = clientcore->OnInit("127.0.0.1", 5123);
	Console::WriteLine(L"b:"+ b);

	MyClientCallBack::OnENetStateCallBack^ met = gcnew MyClientCallBack::OnENetStateCallBack(_OnENetStateCallBack);
	//设置回调
	clientcore->OnGetMyClientCallBack()->OnSetENetStateCallBack(met);
	
	while (true)
	{
		System::Threading::Thread::Sleep(10);
		//String^ s = Console::ReadLine();//
		//Console::WriteLine(s);

		MY_UINT32 test = 10;
		bool b = true;
		Vector<MY_UINT32>^ ver = gcnew Vector<MY_UINT32>(1);
		ver[0] = 123;
		std::string^ str = "我们";
		Map<String^, String^>^ map = gcnew Map<String ^, String ^>();
		map["aaa"] = "bbb";

		//序列化
		CMyStreamBase^ ostream = gcnew CMyStreamBase();
		ostream->Push(map);
		ostream->Push(str);
		ostream->Push(b);
		ostream->Push(test)->Push(ver);

		//const auto & ver1 = ostream.OnGetBuffer();
		array<Byte>^ ptr;
		int len = ostream->OnGetBuffer(ptr);

		MsgInfo^ msg = gcnew MsgInfo();
		msg->m_body_len = ostream->OnGetBuffer(msg->m_body);
		static int _n = 0;
		_n++;
		msg->m_head.unMsgID = _n;

		clientcore->OnSendMsg(msg);

	}
    return 0;
}
