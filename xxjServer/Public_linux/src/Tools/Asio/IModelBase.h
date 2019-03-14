/********************************************************************
	创建时间：	2014/08/07  10:58:17 
	文件名：    	IGameBase
	作者：		谢小军
	
	功能：		1.模块消息类接口 主要用于逻辑调用
				2.
				
	说明：	    1. 用于逻辑接口定义
				2. 
*********************************************************************/
#ifndef _IMODELBASE_H_
#define _IMODELBASE_H_
#include <string>
#include <vector>
#include "GameDefine.h"

//逻辑接口
class IModelBase
{
public:
	bool m_isInit;
	bool m_isDelete;

//	UserInfo m_user;
// 	MY_UINT32 m_userID;
// 	std::string m_name;

	IModelBase(){m_isInit = false;m_isDelete = false;}

public:
	//连接
	virtual void OnInit() = 0;

	//判断是否超时了
	virtual bool OnTimeOut() = 0;

};

#endif