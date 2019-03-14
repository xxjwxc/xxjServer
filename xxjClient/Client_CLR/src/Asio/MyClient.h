#ifndef MYCLIENT_H_
#define MYCLIENT_H_
#include "../Tools/ClientTypeDefine.h"
#include "../Tools/MySingleton.h"
#include "../Asio/IAsioTools.h"
#include "../Tools/MyQueue/MyQueue.h"

ref class StateObject {
public:
	PTR(TcpClient) client;
	MY_UINT32 BufferSize;
	array<Byte>^ buffer;
public:
	StateObject()
	{
		client = nullptr;
		BufferSize = MAX_RESERVE_SIZE;
		buffer = gcnew array<Byte>(MAX_RESERVE_SIZE);
	}
};

//#define max_len 1024

ref class CMyClient
{
public:
	CMyClient(){};
	CMyClient(CString ip, MY_UINT32 port);
	~CMyClient() {};
private:
	System::Threading::Mutex^ m_mutex;
	static CString m_ip;
	static MY_UINT32 m_port;
	static PTR(TcpClient) m_tcpClient;
	//static bool m_tcpPic;
	static bool m_tcpIsOpen;
	static bool m_isStart = false;
	static  System::Net::Sockets::NetworkStream^ m_ns;
	//static PTR(CMyClientDataModel) m_model;
private:
	static void Conn_handler(PTR(System::IAsyncResult) iar);

	static void Read_handler(PTR(System::IAsyncResult) iar);

	static void Write_handler(PTR(System::IAsyncResult) iar);

	static void Creat();
	static void UnSerializing(array<Byte>^ buffer, int size);

public:
	static bool WriteSomeSomething(const array<Byte>^ str, MY_UINT32 len);
	static bool WritePrepareData(const array<Byte>^ str, MY_UINT32 len);
	void Start();
	void ReStart();
	static void Close();
	void OnInit(CString ip, MY_UINT32 port);
};

#endif
