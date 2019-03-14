/********************************************************************
创建时间：    2016/06/05  07:05:44
文件名：      ConnectionPool.h
作者：        谢小军

功能：        1. 数据库连接池
2.

说明：        1. 用于数据库连接管理
2.
*********************************************************************/
#ifndef MSSQLCONN_POLL_H
#define MSSQLCONN_POLL_H
#include "../MySingleton.h"
#include "../Queue/MyQueue.h"
#include "MsSqlBase.h"
#include <string>
#include <xutility>


class MsSqlConnPool :public MySingleton<MsSqlConnPool>
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
	db_timeout:连接池超时时间
	*/
	bool ConnectionMySql(const std::string& host, const std::string& port, const std::string & Database, const std::string& user,
		const std::string &PWD, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U, std::size_t db_timeout = 0U);

	//回收一个
	void OnReleaseOne(_ConnectionPtr con,unsigned int creatTime);

	//获取一个
	ConnectionMS GetConnection(); //获得数据库连接
public:
	MsSqlConnPool(){};
	~MsSqlConnPool();

private:
	bool m_isInit = false;
	CMyQueue<ConnectionMS> m_queue;
	void OnDestroy(_ConnectionPtr & px);
private:
	DatabaseInfo db_info;
	int minSession = 0;
	int maxSession = 0;
	std::size_t timeout = 60U; /*= 60U*/
	std::size_t db_timeOut = 0U;
};



class CMsConnectionEx :public CMSsqlBase, public CPointBase<CMsConnectionEx>, public boost::noncopyable
{
public:
	CMsResultSet executeQuery(const std::string & sql);
public:
	CMsConnectionEx(ConnectionMS & info)
	{
		px_back = info.con;
		m_creatTime = info.m_creatTime;
		px = this;
	}

	//防止sql 注入
	// 	sql::SQLString EscapeString(const sql::SQLString & src){
	// 		return src;
	// 		sql::mysql::MySQL_Connection * myConn = dynamic_cast<sql::mysql::MySQL_Connection *> (px);
	// 		return myConn->escapeString(src);
	// 	}

	~CMsConnectionEx()
	{
		OnRelease();
	}

	void OnDestroy()
	{
		if (px_back)
		{
			CMsSqlConnection::instance()->ReleaseConnection(px_back);
			px = nullptr;
		}
	}

	//回收数据库连接
	void OnRelease()
	{
		MsSqlConnPool::instance()->OnReleaseOne(px_back, m_creatTime);
	}


private:
	unsigned int m_creatTime = 0;
	_ConnectionPtr px_back = nullptr;
};

#endif

