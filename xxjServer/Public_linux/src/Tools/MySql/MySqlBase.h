
/********************************************************************
    创建时间：    2015/10/15  16:38:12 
    文件名：      MySqlBase.h
    作者：        谢小军
    
    功能：       1.mysql 公共工具类
                2.主要用与mysql数据库的操作工具函数集合
                
    说明：       1. 
                2. 
*********************************************************************/
#ifndef MYSQLBASE_H_
#define MYSQLBASE_H_

#include <boost/thread.hpp>
//mysql head
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>   
#include <cppconn/metadata.h> 
#include <cppconn/exception.h>
#include "../MyTimes.h"
#include "../PointBase.h"
//--------------end

class CMysqlBase
{
public:
	void GetString(const sql::SQLString & str, char *dest, unsigned int len);
	void GetString(const sql::SQLString & str, std::string & dest, unsigned int len);
	void destroy_db_res(sql::PreparedStatement *& prep_stmt, sql::ResultSet *& db_res);
	void destroy_db_res(sql::PreparedStatement *& prep_stmt);
	void destroy_db_res(sql::ResultSet *& db_res);
	void OnCatch(std::exception & e,std::string path = "error.txt");
};

class Connection{
public:
	sql::Connection * con = nullptr;
	unsigned int m_creatTime = 0;
public:
	bool isClosed();
private:
	bool OnDealisClosed();
};

class CMyResultSet : public CMysqlBase, public CPointBase<sql::ResultSet>
{
public:
	CMyResultSet(sql::ResultSet * ps)
	{
		px = ps;
	}
	~CMyResultSet()
	{
		OnDestroy();
	}
private:
	void OnDestroy()
	{
		destroy_db_res(px);
	}
};

class CMyPreparedStatement : public CMysqlBase, public CPointBase<sql::PreparedStatement>
{
public:
	CMyPreparedStatement(sql::PreparedStatement * ps)
	{
		px = ps;
	}
	~CMyPreparedStatement()
	{
		OnClearResultSet();
		OnDestroy();
	}
public:
	void OnClearResultSet()
	{
		if (px)
		{
			while (px->getMoreResults())
			{
				CMyResultSet db_res = px->getResultSet();
				std::cout << "more" << std::endl;
			}
		}
	}
private:

	void OnDestroy()
	{
		destroy_db_res(px);
	}
};

class CMyStatement : public CMysqlBase, public CPointBase<sql::Statement>
{
public:
	CMyStatement(sql::Statement * ps)
	{
		px = ps;
	}
	~CMyStatement()
	{
		OnClearResultSet();
		OnDestroy();
	}
public:
	void OnClearResultSet()
	{
		if (px)
		{
			while (px->getMoreResults())
			{
				CMyResultSet db_res = px->getResultSet();
				std::cout << "more" << std::endl;
			}
		}
	}
private:
	void OnDestroy()
	{
		if (px != NULL)
		{
			try
			{
				px->close();
			}
			catch (sql::SQLException& e)
			{
				printf("fuck!");
			}

			delete px;
			px = NULL;
		}
	}

};

#endif // !MYSQLBASE_H_
