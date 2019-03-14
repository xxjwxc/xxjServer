/********************************************************************
	创建时间：	2015/04/20  20:15:33
	文件名：		DatabaseIO.h
	作者：		谢小军
	
	功能：		1.数据库框架具体实现，使用c++11 tuple,variadic template实现
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef __DATABASE_IO_H_
#define __DATABASE_IO_H_
#include "IDatabaseIO.h"
#include "Poco/Exception.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/PooledSessionImpl.h"


#ifdef __SQLLITE
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#endif

#ifdef __MYSQL
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#endif

#ifdef __ODBC
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/ODBCException.h"
#endif
#include <memory>
#include <boost/thread.hpp>
#include "../MyTools.h"
#include "../asio/DefineBase.h"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

class CDatabaseIO :public IDataBaseObj
{
public:
	virtual bool Connection(const std::string& connector, const std::string& connectionString, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U);
	virtual void UnConnection();

	template<typename T>
	bool SelectSql(const std::string & strSQL, T & item);
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, Args& ...args);
	template<class T, class = typename std::enable_if<is_user_class<T>::value>::type>
	inline bool SelectSqlEx(const std::string & strSQL, T & t)
	{
		return SelectSql(strSQL, t.Meta());
	}
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::tuple<Args...> & item);
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::tuple<Args& ...> & item);
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items);
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items, std::size_t _limit);
	template<typename... Args>
	bool ContinueExecute(std::vector<std::tuple<Args...>> & items);

	template<typename... Args>
	bool InsertSql(const std::string & strSQL, const std::tuple<Args...> & item);
	template<typename... Args>
	bool InsertSql(const std::string & strSQL, const std::vector<std::tuple<Args...>> & items);

	virtual bool ExecuteSql(const std::string & strSQL);
	virtual MySesion GetSession();
	//virtual void ReleaseSession(Poco::Data::Session & session);

	virtual std::string GetLastError();
	virtual void ClearErrorInfo();
public:
	CDatabaseIO();
	~CDatabaseIO();

private:
	void Init();//初始化
	void UnInit();//反注册
	std::string __errorInfo;

private:
	template<class PocoTuple,class StdTuple, size_t N>
	void SetStandTupe(PocoTuple & pocoTuple, StdTuple & item);
	template<typename... Args>
	void GetPocoVerTuple(std::vector<Poco::Tuple<Args...>> ** p);

	std::shared_ptr<Poco::Data::SessionPool> m_SessionPoolPtr;
	std::shared_ptr<Statement> _statement_limit;
};


//Step by step derivation
template<std::size_t N, typename... Args>
class __StandTupe
{
public:
	//poco tuple to stand tuple
	static void SetStandTupe(const Poco::Tuple<Args...> & pocoTuple, std::tuple<Args& ...> & item);
	//stand tuple to poco tuple
	static void SetPocoTupe(const std::tuple<Args...> & item, Poco::Tuple<Args...> & pocoTuple);
};

//特化版本，终点
template<typename... Args>
class __StandTupe < 0, Args...>
{
public:
	static void SetStandTupe(const Poco::Tuple<Args...> & pocoTuple, std::tuple<Args& ...> & item);
	static void SetPocoTupe(const std::tuple<Args...> & item, Poco::Tuple<Args...> & pocoTuple);
};

// 版本二
// template<size_t N, class PocoTuple, class StdTuple>
// class __StandTupe
// {
// public:
// 	static void SetStandTupe(PocoTuple & pocoTuple, StdTuple & item)
// 	{
// 		std::get<N>(item) = pocoTuple.get<N>();
// 		__StandTupe<N - 1, PocoTuple, StdTuple>::SetStandTupe(pocoTuple, item);
// 	}
// };
// 
// //特化版本，终点
// template<class PocoTuple, class StdTuple>
// class __StandTupe < 0, PocoTuple, StdTuple>
// {
// public:
// 	static void SetStandTupe(PocoTuple & pocoTuple, StdTuple & item)
// 	{
// 		std::get<0>(item) = pocoTuple.get<0>();
// 	}
// };

#endif