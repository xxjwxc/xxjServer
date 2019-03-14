#include "MyClientDataModel.h"
#include "../Asio/MyClient.h"
#include "../Asio/IAsioTools.h"
#include "../Tools/MyStream/MyStreamBase.h"
#include "MyClientCallBack.h"

void CMyClientDataModel::OnInit()
{
	s_myqueue = gcnew CMyQueue<MsgQueueInfo ^>();
}

void CMyClientDataModel::OnRead(PTR(MsgInfo) info)
{
	MySingleton<MyClientCallBack::CMyClientCallBack^>::Instance()->OnDealENetStateCallBack(ENetState::ERead);
	Console::WriteLine("{0:HH:mm:ss}->{1}", DateTime::Now, info->m_head.unMsgID);
}

bool CMyClientDataModel::OnSend(array<Byte>^ str, MY_UINT32 len)
{
	CMyClient::WritePrepareData(str,len);
	return true;
}

void CMyClientDataModel::OnConnect()
{
	//OnTestSend();
	MySingleton<MyClientCallBack::CMyClientCallBack^>::Instance()->OnDealENetStateCallBack(ENetState::EConnect);
	OnWriteDown();
}

void CMyClientDataModel::DisConnect()
{
	MySingleton<MyClientCallBack::CMyClientCallBack^>::Instance()->OnDealENetStateCallBack(ENetState::EDesConnect);
}

void CMyClientDataModel::OnWriteDown()
{
	MySingleton<MyClientCallBack::CMyClientCallBack^>::Instance()->OnDealENetStateCallBack(ENetState::EWriteDown);
	System::Threading::Thread::Sleep(10);
	//OnTestSend();
	MsgQueueInfo ^ info = s_myqueue->Dequeue(true);
	if (info)
	{
		CMyClient::WriteSomeSomething(info->data, info->len);
	}
	
}

void CMyClientDataModel::UnSerializing(const char * data, int size)
{

}

void CMyClientDataModel::OnTestSend()
{
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

	IAsioTools tools;
	array<Byte>^ buffer;
	MY_UINT32 n = tools.SerializingBinary(msg, buffer);
	if (n > 0)
	{
		OnSend(buffer, n);
	}

}

void CMyClientDataModel::OnEnqueue(MsgQueueInfo^ info)
{
	if (s_myqueue)
	{
		s_myqueue->Enqueue(info);
	}
}

