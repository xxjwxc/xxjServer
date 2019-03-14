/********************************************************************
	创建时间：	2015/04/25  0:37:22
	文件名：	MyClient.h
	作者：		谢小军
	
	功能：		1. 主要逻辑类，在这里处理所有具体消息
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef __MYCLIENT_H_
#define __MYCLIENT_H_

#include <Tools/Asio/ClientSession.h>
#include <Tools/Json/MyJson.h>
#include <Tools/Asio/IAsioTools.h>
#include <Tools/Asio/ServerConnection.h>


class CMyClient:public ClientSession
{
public:
	CMyClient(boost::asio::io_service & ioservice);
	~CMyClient();
public://IGameBase 接口
	//读取到的文件
	virtual void OnRead(std::vector<MsgInfo> & ver);

	//读取到的文件
	virtual void OnRead(std::vector<std::string> & ver);

	//发送事件
	virtual bool OnSend(const boost::scoped_array<char> & str,MY_UINT32 len);

	//写完成
	virtual void OnWriteDown();

	//连接成功调用
	virtual void OnConnect();
	//断开
	virtual void DisConnect();

	//反序列化
	virtual void UnSerializing(const char * data, int size);
private://序列化相关
	MY_UINT32 m_loginTime = 0;
	std::vector<std::string> m_verMsg;
	boost::shared_ptr<IAsioTools> m_myAsioTools = nullptr;
	void WriteResult(const std::string & str);//发送操作结果消息

public://IModelBase接口
	virtual void OnInit();
	virtual bool OnTimeOut();
public://处理消息
	bool DealMsg(MsgInfo & Msg);//消息处理函数开始
	void OnDealMessage(std::string str);
private:
	void OnDestroye();//销毁自己

};

#endif // !__MYCLIENT_H_
