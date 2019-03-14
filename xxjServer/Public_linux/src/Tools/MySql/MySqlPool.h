/********************************************************************
创建时间：    2016/06/05  07:05:44
文件名：      ConnectionPool.h
作者：        谢小军

功能：        1. 数据库连接池
2.

说明：        1. 用于数据库连接管理
2.
*********************************************************************/
#ifndef MYSQLCONN_POLL_H
#define MYSQLCONN_POLL_H
#include "../MySingleton.h"
#include "../Queue/MyQueue.hpp"
#include "../MySql/MySqlConnection.h"
#include "MySqlBase.h"
#include <string>


class MySqlConnPool :public MySingleton<MySqlConnPool>
{
public:
	/*
	功能：
	连接数据库,并创建session池
	参数:
	host:数据库地址
	port:端口号
	Database:数据库名
	user:用户名
	PWD:用户密码
	*/
	bool ConnectionMySql(const std::string& host, const std::string& port, const std::string & Database, const std::string& user,
		const std::string &PWD, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U);

	bool ConnectionMySql(DatabaseInfo info, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U);

	//回收一个
	void OnReleaseOne(sql::Connection *,unsigned int creatTime);

	//获取一个
	Connection GetConnection(); //获得数据库连接
public:
	MySqlConnPool(){};
	~MySqlConnPool();

	bool m_isLoop = true;
private:
	bool m_isInit = false;
	CMyQueue<Connection> m_queue;
	void OnDestroy(sql::Connection *& px);
private:
	DatabaseInfo db_info;
	int minSession = 0;
	int maxSession = 0;
	std::size_t timeout = 60U; /*= 60U*/
};


class CMyConnectionEx :public CMysqlBase, public CPointBase<sql::Connection>, public boost::noncopyable
{
public:
	CMyConnectionEx(const Connection & info)
	{
		px = info.con;
		m_creatTime = info.m_creatTime;
	}

	//防止sql 注入
	sql::SQLString EscapeString(const sql::SQLString & src){
		sql::mysql::MySQL_Connection * myConn = dynamic_cast<sql::mysql::MySQL_Connection *> (px);
		return myConn->escapeString(src);
	}

	~CMyConnectionEx()
	{
		OnRelease();
	}

	void OnDestroy()
	{
		if (px)
		{
			px->commit();
			CMySqlConnection::instance()->ReleaseConnection(px);
			px = nullptr;
		}
	}

	//回收数据库连接
	void OnRelease()
	{
		MySqlConnPool::instance()->OnReleaseOne(px, m_creatTime);
	}

	void setOutTime()
	{
		m_creatTime = CMyTimes::get_utc() - 24 * 3600;
	}
private:
	//sql::Connection * px = nullptr;
	unsigned int m_creatTime = 0;
};
#endif

