#pragma once
#include "src/Tools/ClientTypeDefine.h"
#include "src/Asio/MyClient.h"
#include "src/data/MyClientDataModel.h"
#include "src/Data/MyClientCallBack.h"

public ref class CClientCore
{
public:
	CClientCore();
	bool OnInit(CString ip,MY_UINT32 port);
	bool OnSendMsg(MsgInfo^ info);
	MyClientCallBack::CMyClientCallBack^ OnGetMyClientCallBack();
private:
	PTR(CMyClient) m_myclient;
	PTR(CMyClientDataModel) m_myclientModel;
	PTR(MyClientCallBack::CMyClientCallBack) m_myclientCallBack;
};

