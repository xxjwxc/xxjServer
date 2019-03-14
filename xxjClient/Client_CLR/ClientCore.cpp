#include "ClientCore.h"

CClientCore::CClientCore()
{
}

bool CClientCore::OnInit(CString ip, MY_UINT32 port)
{
	m_myclient = MySingleton<CMyClient^>::Instance();// gcnew CMyClient(ip, port);
	m_myclientModel = MySingleton<CMyClientDataModel^>::Instance();
	m_myclientCallBack = MySingleton<MyClientCallBack::CMyClientCallBack^>::Instance();
	m_myclient->OnInit(ip, port);
	m_myclient->Start();
	return true;
}

bool CClientCore::OnSendMsg(MsgInfo^ info)
{
// 	MsgInfo^msg = gcnew MsgInfo();
// 	msg->m_head.unMsgID = 1024;
// 	msg->m_body = gcnew array<Byte>(10);
// 	for (int i = 0; i < 10; i++)
// 		msg->m_body[i] = i;
// 	msg->m_body_len = 10;
	IAsioTools tools;
	array<Byte>^ buffer;
	MY_UINT32 n = tools.SerializingBinary(info, buffer);
	if (n > 0)
	{
		return m_myclientModel->OnSend(buffer, n);
	}
	return false;
}

MyClientCallBack::CMyClientCallBack^ CClientCore::OnGetMyClientCallBack()
{
	return m_myclientCallBack;
}
