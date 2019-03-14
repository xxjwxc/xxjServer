#include "MyClient.h"
#include "../data/MyClientDataModel.h"
IAsioTools m_myAsioTools;//序列化工具

CMyClient::CMyClient(CString ip, MY_UINT32 port)
{
	Creat();
}

void CMyClient::Conn_handler(PTR(System::IAsyncResult) iar)
{
	if (m_isStart)
	{
		try
		{
			m_tcpClient = (TcpClient ^)iar->AsyncState;
			m_tcpClient->EndConnect(iar);
			//MessageBox.Show("connected");
			m_tcpIsOpen = m_tcpClient->Connected;

			m_isStart = false;
			if ((m_tcpClient != nullptr) && (m_tcpClient->Connected))
			{
				m_ns = m_tcpClient->GetStream();

				StateObject^ state = gcnew StateObject();
				state->client = m_tcpClient;
				NetworkStream^ stream = state->client->GetStream();

				if (stream->CanRead)
				{
					//array<Byte>^ buffer = gcnew array<Byte>(m_tcpClient->ReceiveBufferSize);
					stream->BeginRead(state->buffer, 0, MAX_RESERVE_SIZE, gcnew AsyncCallback(Read_handler), state);
				}

				MySingleton<CMyClientDataModel^>::Instance()->OnConnect();
			}
			else {
				MySingleton<CMyClientDataModel^>::Instance()->DisConnect();
			}
			
		}
		catch (Exception^ e)
		{
			Console::WriteLine("{0:HH:mm:ss}->{1}", DateTime::Now, e->ToString());
			MySingleton<CMyClientDataModel^>::Instance()->DisConnect();
			m_isStart = false;
			MySingleton<CMyClient^>::Instance()->ReStart();
		}
	}
}

void CMyClient::Read_handler(PTR(System::IAsyncResult) iar)
{
	if (!m_isStart)
	{
		try {
			if (iar == nullptr)
				return;
			StateObject^ state = (StateObject^)iar->AsyncState;
			if ((state->client == nullptr) || (!state->client->Connected)) return;
			int NumOfBytesRead;
			NetworkStream^ ns = state->client->GetStream();
			NumOfBytesRead = ns->EndRead(iar);
			if (NumOfBytesRead > 0)
			{
				array<Byte>^ buffer = gcnew array<Byte>(NumOfBytesRead);
				Array::Copy(state->buffer, 0, buffer, 0, NumOfBytesRead);
				//ReceiveAppend(buffer, TcpUdp.StringType.String);
				UnSerializing(buffer, NumOfBytesRead);


				ns->BeginRead(state->buffer, 0, state->BufferSize, gcnew AsyncCallback(Read_handler), state);
			}
			else
			{
				ns->Close();
				state->client->Close();
				ns = nullptr;
				state = nullptr;
				MySingleton<CMyClientDataModel^>::Instance()->DisConnect();
				MySingleton<CMyClient^>::Instance()->ReStart();
			}
		}
		catch (Exception^ e)
		{
			Console::WriteLine("{0:HH:mm:ss}->{1}", DateTime::Now, e->ToString());
			MySingleton<CMyClientDataModel^>::Instance()->DisConnect();
			MySingleton<CMyClient^>::Instance()->ReStart();
		}
	}
}

void CMyClient::Write_handler(PTR(System::IAsyncResult) iar)
{
	if (!m_isStart)
	{
		try {
			if (!iar->AsyncWaitHandle->WaitOne())
			{
				Console::WriteLine("fail to wait");
			}
			Console::WriteLine("write seccess!\n");
			NetworkStream^ ns = (NetworkStream ^)iar->AsyncState;
			ns->EndWrite(iar);
			MySingleton<CMyClientDataModel^>::Instance()->OnWriteDown();
		}
		catch (Exception^ e)
		{
			Console::WriteLine("{0:HH:mm:ss}->{1}", DateTime::Now, e->ToString());
			MySingleton<CMyClientDataModel^>::Instance()->DisConnect();
			MySingleton<CMyClient^>::Instance()->ReStart();

		}
	}
}

void CMyClient::Creat()
{
	MySingleton<CMyClientDataModel^>::Instance()->OnInit();
	//m_model = gcnew CMyClientDataModel();
}

void CMyClient::UnSerializing(array<Byte>^ buffer, int size)
{
	//static IAsioTools m_myAsioTools;
	char  * data = MyTools::ArrayToChar(buffer);
	array<MsgInfo^>^ ver = m_myAsioTools.UnSerializingBinary(data, size);//解包
	for (int i=0;i<ver->Length;i++)
	{
		MySingleton<CMyClientDataModel^>::Instance()->OnRead(ver[i]);
	}

	//MySingleton<CMyClientDataModel^>::Instance()->OnTestSend();
}

bool CMyClient::WriteSomeSomething(const array<Byte>^ str, MY_UINT32 len)
{
	if (!m_isStart)
	{
		if (m_tcpIsOpen)
		{
			if (m_ns != nullptr)
			{
				//ns.Write(buffer, 0, buffer.Length);
				m_ns->BeginWrite(str, 0, len, gcnew AsyncCallback(Write_handler), m_ns);
				// 			if (!ar->AsyncWaitHandle->WaitOne())
				// 			{
				// 				Console::WriteLine("fail to wait");
				// 			}
				//m_ns->EndWrite(ar);
				return true;
			}
		}
	}


	return false;
}

bool CMyClient::WritePrepareData(const array<Byte>^ str, MY_UINT32 len)
{
	MsgQueueInfo^ info = gcnew MsgQueueInfo();
	info->data = str;
	info->len = len;
	MySingleton<CMyClientDataModel^>::Instance()->OnEnqueue(info);
	//s_myqueue->Enqueue(info);
	return true;
}

void CMyClient::Start()
{
	if (m_isStart)
	{
		Close();
		try {
			m_tcpClient = gcnew TcpClient();

			m_tcpClient->ReceiveTimeout = 10;
			//异步连接
			m_tcpClient->BeginConnect(m_ip, m_port, gcnew AsyncCallback(Conn_handler), m_tcpClient);
		}
		catch (Exception^ ex)
		{
			Console::WriteLine("{0:HH:mm:ss}->{1}", DateTime::Now, ex->ToString());
		}
	}
}

void CMyClient::ReStart()
{
	if (!m_isStart)
	{
		m_mutex->WaitOne();
		if (!m_isStart)
		{
			m_isStart = true;

			System::Threading::Thread::Sleep(1000);
			Start();
		}
		m_mutex->ReleaseMutex();
	}
}

void CMyClient::Close()
{
	if ((m_tcpClient != nullptr) && (m_tcpClient->Connected))
	{
		if (m_ns !=nullptr)
			m_ns->Close();
		m_tcpClient->Close();
		//m_tcpIsOpen = m_tcpClient->Connected;
	}

	m_ns = nullptr;
	m_tcpClient = nullptr;
	m_tcpIsOpen = false;
	m_myAsioTools.Clear();
}

void CMyClient::OnInit(CString ip, MY_UINT32 port)
{
	m_ip = ip;
	m_port = port;
	m_isStart = true;
	m_mutex = gcnew System::Threading::Mutex();
	Creat();
}
