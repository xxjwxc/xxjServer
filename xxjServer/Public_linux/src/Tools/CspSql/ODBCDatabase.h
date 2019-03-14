/********************************************************************
	创建时间：	2015/05/17  22:38:38
	文件名：		ODBCDatabase.h
	作者：		xxj
	
	功能：		1. 专属连接odbc数据库使用
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef __ODBCDATABASE_H_
#define __ODBCDATABASE_H_
#include "DatabaseIO.h"
#include <boost/format.hpp>   
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\tokenizer.hpp> 

//#define  __ODBC		//ODBC需要开启此宏

class CODBCDatabaseIO : public CDatabaseIO
{
public:
	/*
	功能：
		连接数据库,并创建session池
	参数:
		Server:数据库地址
		Database:数据库名
		UID:用户名
		PWD:用户密码
	*/
	bool ConnectionODBC(const std::string& Server, const std::string& Database,const std::string& UID,
		const std::string &PWD, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U);

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

public:
	/*
	功能：
		插入一条数据到数据库
	参数：
		strSQL:sql语句，其中要插入的以:分隔
		如：INSERT INTO user_tbl(name,age,text,num) VALUES(?,?,?,?,?)
		其中:name, :address, :age为站位符号
		tuple中参数列表需跟站位符列表顺序一致
	*/
	template<typename... Args>
	bool InsertSql(const std::string & strSQL, const std::tuple<Args...> & item);

	/*
	功能：
		插入一组数据到数据库
	参数：
		strSQL:sql语句，其中要插入的以:分隔
		如：INSERT INTO user_tbl(name,age,text,num) VALUES(?,?,?,?,?)
		其中:name, :address, :age为站位符号
		tuple中参数列表需跟站位符列表顺序一致
	*/
	template<typename... Args>
	bool InsertSql(const std::string & strSQL, const std::vector<std::tuple<Args...>> & items);
private:
	template<typename... Args>
	std::string buildSQL(const std::string & strSQL,const std::tuple<Args...> & item);
};

template<std::size_t N, typename... Args>
struct  __buildSQLEx
{
	static void buildSQLEx(std::string & sql, std::vector<std::string> & ver, const std::tuple<Args...> & item);
	template <typename T>
	static std::string __GetItem(const T t)
	{
		return boost::lexical_cast<std::string>(t);
	}
	template <> static std::string __GetItem(const std::string t)
	{
		return std::string("'") + t + std::string("'");
	}
};

template<typename... Args>
struct  __buildSQLEx<0, Args...>
{
	static void buildSQLEx(std::string & sql, std::vector<std::string> & ver, const std::tuple<Args...> & item);
	template <typename T>
	static std::string __GetItem(const T t)
	{
		return boost::lexical_cast<std::string>(t);
	}
	template <> static std::string __GetItem(const std::string t)
	{
		return std::string("'") + boost::locale::conv::between(t, DATABASE_ENCODED1, CODE_ENCODED) + std::string("'");
	}
};

template<std::size_t N, typename... Args>
void __buildSQLEx<N, Args...>::buildSQLEx(std::string & sql, std::vector<std::string> & ver,
	const std::tuple<Args...> & item)
{
	sql = __GetItem(std::get<N>(item)) + ver.at(N + 1) + sql;
	__buildSQLEx<N - 1, Args...>::buildSQLEx(sql, ver, item);
}

template< typename... Args>
void __buildSQLEx<0, Args...>::buildSQLEx(std::string & sql, std::vector<std::string> & ver,
	const std::tuple<Args...> & item)
{
	sql = __GetItem(std::get<0>(item)) + ver.at(1) + sql;
}

template<typename... Args>
bool CODBCDatabaseIO::InsertSql(const std::string & strSQL, const std::vector<std::tuple<Args...>> & items)
{
	bool is = false;
	for (int i = 0; i < items.size(); i++)
	{
		is = InsertSql(strSQL, items.at(i));
		if (!is)
			return false;
	}
	return is;
}

template<typename... Args>
bool CODBCDatabaseIO::InsertSql(const std::string & strSQL, const std::tuple<Args...> & item)
{
	std::string sql  = buildSQL(strSQL, item);
	return CDatabaseIO::ExecuteSql(sql);
}



template<typename... Args>
std::string CODBCDatabaseIO::buildSQL(const std::string & strSQL, const std::tuple<Args...> & item)
{
	std::vector<std::string> SplitVec;
	boost::char_separator<char> separator("?");
	boost::tokenizer<boost::char_separator<char> > tokens(strSQL, separator);
	boost::tokenizer<boost::char_separator<char> >::iterator token_iter;
	for (token_iter = tokens.begin(); token_iter != tokens.end(); token_iter++)
		SplitVec.push_back(*token_iter);

	//boost::split(SplitVec,strSQL,boost::is_any_of("?"));
	//int n = sizeof...(Args);
	std::string sql = "";
	__buildSQLEx<sizeof...(Args)-1, Args...>::buildSQLEx(sql, SplitVec, item);
	sql = SplitVec.at(0)+sql;
	return sql;
}

#endif