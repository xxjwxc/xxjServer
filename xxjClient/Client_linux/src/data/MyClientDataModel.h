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
#include <Tools/Asio/ISocketBase.h>
#include <Tools/MySingleton.h>

class CMyClientDataModel:public ISocketBase,public MySingleton<CMyClientDataModel>
{
public:
	//读取到的文件
	virtual void OnRead(std::vector<MsgInfo> & ver);

	//读取到的文件
	virtual void OnRead(std::vector<std::string> & ver);

	//发送事件
	virtual bool OnSend(const boost::scoped_array<char> & str, MY_UINT32 len);

	//连接成功调用
	virtual void OnConnect();

	//断开
	virtual void DisConnect();

	//写完成
	virtual void OnWriteDown();

	//反序列化
	virtual void UnSerializing(const char * data, int size);
};
#endif
