/********************************************************************
    创建时间：    2015/08/08  16:22:27 
    文件名：      GameDefine.h
    作者：        xxj
    
    功能:		1.消息定义
				2.
                
    说明:		1. 
                2. 
*********************************************************************/
#ifndef __GAMEDEFINE_H_
#define __GAMEDEFINE_H_
#include "../Tools/ClientTypeDefine.h"
//#using <System.dll>

#ifndef interface
#define interface class
#endif

public ref class MsgBaseHead
{
public:
	MY_UINT32 unMsgID;

	MsgBaseHead()
	{
		Clear();
	}
	void Clear()
	{
		unMsgID = 0;
	}
};

public ref class MsgInfo
{
public:
	MsgBaseHead m_head;//消息头
	array<Byte>^ m_body;
	MY_UINT32 m_body_len;
	
	MsgInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_head.Clear();
		m_body_len = 0;
		m_body = QNEW array<Byte>(0);
	}
};
//"uID"

ref class MsgQueueInfo
{
public:
	array<Byte>^ data;
	MY_UINT32 len;
};

public enum class ENetState
{
	EConnect = 0,//连接成功标记
	EDesConnect,//连接失败标记
	EWriteDown,//写完成
	ERead,//读到信息
};

#endif
