
/********************************************************************
    创建时间:     2017/04/07  15:03:10 
    文件名:       MyClientDataModel.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.与服务器的数据接口的数据回调接口
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef MY_CLIENTDATAMODEL_H_
#define MY_CLIENTDATAMODEL_H_
#include "GameDefine.h"
#include "../Tools/MyQueue/MyQueue.h"


public ref class CMyClientDataModel
{
public:
	//初始化
	void OnInit();
public:
	//读取到的文件
	void OnRead(PTR(MsgInfo) ver);

	//发送事件
	virtual bool OnSend(array<Byte>^ str, MY_UINT32 len);

	//连接成功调用
	virtual void OnConnect();

	//断开
	virtual void DisConnect();

	//写完成
	virtual void OnWriteDown();

	//反序列化
	virtual void UnSerializing(const char * data, int size);


//消息相关
public:
	void OnEnqueue(MsgQueueInfo^ info);
	CMyQueue<MsgQueueInfo^>^ s_myqueue;
private:

	void OnTestSend();
};
#endif
