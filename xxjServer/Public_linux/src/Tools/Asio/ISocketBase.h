/********************************************************************
	创建时间：	2014/08/07  10:58:17 
	文件名：    	IGameBase
	作者：		谢小军
	
	功能：		1.通信接口类
				2.
				
	说明：	    1. MyClient.h 中使用Asio 实现，这里只是定义了一个规则
				2. 
*********************************************************************/
#ifndef IGAMEBASE_H_
#define IGAMEBASE_H_
#include <string>
#include <vector>
#include "GameDefine.h"
#include <boost/smart_ptr/scoped_array.hpp>

// #ifndef interface
// #define interface class
// #endif // !interface


//通信接口
class ISocketBase
{
public:
	bool isLog = true;
public:
	//读取到的文件
	virtual void OnRead(std::vector<MsgInfo> & ver) = 0;

	//读取到的文件
	virtual void OnRead(std::vector<std::string> & ver) = 0;

	//发送事件
	virtual bool OnSend(const boost::scoped_array<char> & str,MY_UINT32 len) = 0;

	//连接成功调用
	virtual void OnConnect() = 0;

	//断开
	virtual void DisConnect() = 0;

	//写完成
	virtual void OnWriteDown()=0;

	//反序列化
	virtual void UnSerializing(const char * data, int size) = 0;
};

#endif