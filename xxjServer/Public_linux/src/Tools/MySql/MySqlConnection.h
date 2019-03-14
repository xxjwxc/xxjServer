/********************************************************************
    创建时间：    2015/09/23  9:34:18 
    文件名：        CMySqlConnection.h
    作者：        谢小军
    
    功能：       1.用于MySql数据库连接使用
                2.
                
    说明：       1. 
                2. 
*********************************************************************/
#ifndef __MYSQLCONNECTION_H_
#define __MYSQLCONNECTION_H_

#include <boost/thread.hpp>
#include "../MySingleton.h"

//mysql head
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>   
#include <cppconn/metadata.h> 
#include <cppconn/exception.h>
#include "MySqlBase.h"
//#include "..\..\data\MyIni.h"
//--------------end

//using namespace sql;


class CMySqlConnection:public MySingleton<CMySqlConnection>
{
public:
	virtual void OnInit() override;
public:
	sql::Connection *GetConnection(const DatabaseInfo & dbInfo);
	void ReleaseConnection(sql::Connection  *& conn);
private:
	void DestroyConnection(sql::Connection *& conn);
	sql::Driver * m_pDriver = nullptr;
	boost::mutex m_oMutex;

};

class CMyConnection:public boost::noncopyable
{
public:
	CMyConnection(sql::Connection * db_con)
	{
		px = db_con;
	}
	~CMyConnection()
	{
		OnDestroy();
	}
	sql::Connection & operator*() const  // never throws
	{
		assert(px != 0);
		return *px;
	}

	sql::Connection * operator&() const  // never throws
	{
		assert(px != 0);
		return px;
	}

	sql::Connection * operator->() const  // never throws
	{
		assert(px != 0);
		return px;
	}

	bool IsNull()
	{
		return px == nullptr;
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

private:
	sql::Connection * px = nullptr;
};

#endif // !CMySqlConnection_H_