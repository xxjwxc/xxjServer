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

#ifndef interface
#define interface class
#endif

#include<cstring>
#include <string>
#include <vector>
#include <boost/smart_ptr/scoped_array.hpp>
//#include <boost/enable_shared_from_this.hpp>//可进行安全的share_ptr转换
#include "DefineBase.h"

// 
// 
// //"ID"
// #define MSG_SEND_RESULT_ID 0 //返回消息
// 
// #define MSG_LOGIN_ID 1//登录
// #define MSG_CHANGE_NAME_ID 2//改名字
// #define MSG_CHANGE_LEVEL_ID 3//改名字
// #define MSG_SEND_FROM_MSG_ID 4//发送消息
// 
// #define MSG_SEND_ALL_MSG_ID 10//收消息 (广播)
// #define MSG_SEND_TO_MSG_ID 11//收消息 (私人消息)
// 
// 
typedef struct tagMsgBaseHead
{
	MY_UINT32 unMsgID;

	tagMsgBaseHead()
	{
		Clear();
	}
	void Clear()
	{
		unMsgID = 0;
	}
}MsgBaseHead;
// 
// typedef struct tagUserInfo
// {
// 	MY_UINT32 m_id;//谁说
// 	MY_UINT32 m_level;//等级
// 	MY_UINT32 m_vipLevel;//vip等级
// 	std::vector<char> m_name;
// 	//std::string m_name;
// 
// 	tagUserInfo()
// 	{
// 		Clear();		
// 	}
// 	void Clear()
// 	{
// 		m_id = 0;
// 		m_level = 0;
// 		m_vipLevel = 0;
// 		m_name.clear();
// 		//m_name.swap(std::vector<char>());
// 	}
// }UserInfo;
// 
typedef struct tagMsgInfo
{
public:
	MsgBaseHead m_head;//消息头
	boost::scoped_array<unsigned char> m_body;
	MY_UINT32 m_body_len;
	
	tagMsgInfo()
	{
		Clear();
	}

	tagMsgInfo(const tagMsgInfo && src){
		Clone(src,*this);
	}
	
	void Clone(const tagMsgInfo & src, tagMsgInfo & dir)
	{
		dir.m_head = src.m_head;
		dir.m_body_len = src.m_body_len;
		dir.m_body.reset(new unsigned char[dir.m_body_len]);
		memcpy(dir.m_body.get(), src.m_body.get(), dir.m_body_len);
	}

	void Clear()
	{
		m_head.Clear();
		m_body.reset();
		m_body_len = 0;
	}
}MsgInfo;
//"uID"


#endif
