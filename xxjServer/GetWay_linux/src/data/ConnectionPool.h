
/********************************************************************
    创建时间：    2015/11/02  15:05:44 
    文件名：      ConnectionPool.h
    作者：        谢小军
    
    功能：        1. 数据库连接池
                 2.
                
    说明：        1. 用于数据库连接管理
                 2. 
*********************************************************************/
#ifndef __CONNECTIONPOOL_H_
#define __CONNECTIONPOOL_H_

#define  __MYSQL	//MySql需要开启此宏
//#define  __ODBC		//ODBC需要开启此宏

#include "MyIni.h"
#include <map>
#include <boost/thread.hpp>

#include <Tools/MyTimes.h>
class ConnectionPool :public MySingleton < ConnectionPool >
{
public:
	ConnectionPool();
	~ConnectionPool();
	/*
		开始一个事件:
		info:orm值
		json:参数
	*/
	std::string OnDo(const SqlInfo & info, const CMyJson::JsonRoot & json = CMyJson::JsonRoot());

private:
	void Init();
private:
	std::string BuildSql(const std::vector<std::string> & sql, const std::vector<SqlPutInfo> & verInput, const CMyJson::JsonRoot & json);
	std::string GetSql(const SqlPutInfo & info, const CMyJson::JsonRoot & json);
};
#endif


