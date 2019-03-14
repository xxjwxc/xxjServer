#ifndef __DATABASE_IO_HPP_
#define __DATABASE_IO_HPP_
#include "DatabaseIO.h"
#include <utility>
#include <boost/locale.hpp>

//-------------------------
template<std::size_t N,typename... Args, typename T, typename... Rest>
void eval(std::tuple<Args...> & tup, T &t, Rest&... re)
{
	t = std::get<N>(tup);
	eval<N + 1, Args...>(tup, re...);
}

template<std::size_t N,typename... Args, typename T>
void eval(std::tuple<Args...> & tup, T &t)
{
	t = std::get<N>(tup);
}

//--------------------------解析
//编码方式
template<typename T>
void SQLTOVS(const T & first, T & second)//数据库编码方式转换成vs中编码方式
{
	second = first;
	return;
}
void SQLTOVS(const std::string & first, std::string & second)
{
	second = boost::locale::conv::between(first, CODE_ENCODED, DATABASE_ENCODED);
	return;
}

template<typename T>
void VSTOSQL(const T & first, T & second)//数据库编码方式转换成vs中编码方式
{
	second = first;
	return;
}
void VSTOSQL(const std::string & first, std::string & second)
{
	second = boost::locale::conv::between(first, DATABASE_ENCODED,CODE_ENCODED);
	return;
}
//--------------------
template<std::size_t N, typename... Args>
void __StandTupe<N, Args... >::SetStandTupe(const Poco::Tuple<Args...> & pocoTuple, std::tuple<Args& ...> & item)
{
	SQLTOVS(pocoTuple.get<N>(), std::get<N>(item));
	//std::get<N>(item) = pocoTuple.get<N>();
	__StandTupe<N - 1, Args...>::SetStandTupe(pocoTuple, item);
}

template<typename... Args>
void __StandTupe<0, Args...>::SetStandTupe(const Poco::Tuple<Args...> & pocoTuple, std::tuple<Args& ...> & item)
{
	SQLTOVS(pocoTuple.get<0>(), std::get<0>(item));
	//std::get<0>(item) = pocoTuple.get<0>();
}

template<std::size_t N, typename... Args>
void __StandTupe<N, Args... >::SetPocoTupe(const std::tuple<Args...> & item, Poco::Tuple<Args...> & pocoTuple)
{
	VSTOSQL(std::get<N>(item), pocoTuple.get<N>());
	//pocoTuple.get<N>() = std::get<N>(item);
	__StandTupe<N - 1, Args...>::SetPocoTupe(item, pocoTuple);
}

template<typename... Args>
void __StandTupe<0, Args...>::SetPocoTupe(const std::tuple<Args...> & item, Poco::Tuple<Args...> & pocoTuple)
{
	VSTOSQL(std::get<0>(item), pocoTuple.get<0>());
	//pocoTuple.get<0>() = std::get<0>(item);
}

template<typename... Args>
void CDatabaseIO::GetPocoVerTuple(std::vector<Poco::Tuple<Args...>> ** p)
{
	static std::vector<Poco::Tuple<Args...>> itemTemp;
	*p = &itemTemp;
}

template<typename T>
bool CDatabaseIO::SelectSql(const std::string & strSQL, T & item)
{
	try
	{
		Session session(m_SessionPoolPtr->get());
		Statement select(session);
		select << strSQL, into(item), now;
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::string s = boost::locale::conv::between(__errorInfo, DATABASE_ENCODED, CODE_ENCODED);
		//std::cerr << s << std::endl;
		return false;
	}
}

template<typename... Args>
bool CDatabaseIO::SelectSql(const std::string & strSQL, Args& ...args)
{
	std::tuple<Args...> item;
	if (SelectSql(strSQL, item))
	{
		eval<0,Args...>(item, args...);
		return true;
	}
	return false;
}

template<typename... Args>
bool CDatabaseIO::SelectSql(const std::string & strSQL, std::tuple<Args...> & item)
{
	try
	{
		Poco::Tuple<Args...> itemTemp;
		//std::vector<Poco::tuple<int, std::string, int> > ver;
		Session session(m_SessionPoolPtr->get());
		Statement select(session);
		select << strSQL, into(itemTemp), now;
		
		__StandTupe<sizeof...(Args)-1, Args...>::SetStandTupe(itemTemp, item);
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}

template<typename... Args>
bool CDatabaseIO::SelectSql(const std::string & strSQL, std::tuple<Args& ...> & item)
{
	try
	{
	Poco::Tuple<Args...> itemTemp;
	//std::vector<Poco::tuple<int, std::string, int> > ver;
	Session session(m_SessionPoolPtr->get());
	Statement select(session);
	select << strSQL, into(itemTemp), now;
	__StandTupe<sizeof...(Args)-1, Args...>::SetStandTupe(itemTemp, item);
	return true;
	}
	catch (Poco::Exception& exc)
	{
	__errorInfo = exc.displayText();
	MyTools::PrintError(__errorInfo);
	return false;
	}
}

template<typename... Args>
bool CDatabaseIO::SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items)
{
	try
	{
		std::vector<Poco::Tuple<Args...>> itemTemp;
		Session session(m_SessionPoolPtr->get());
		Statement select(session);
		select << strSQL, into(itemTemp), now;

		for (const Poco::Tuple<Args...> & vep : itemTemp)
		{
			std::tuple<Args...> temp;
			__StandTupe<sizeof...(Args)-1, Args...>::SetStandTupe(vep, temp);
			items.push_back(std::move(temp));
		}
			
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}

template<typename... Args>
bool CDatabaseIO::SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items, std::size_t _limit)
{
	try
	{
		std::vector<Poco::Tuple<Args...>> * itemTemp = nullptr;
		GetPocoVerTuple<Args...>(&itemTemp);
		Session session(m_SessionPoolPtr->get());
		_statement_limit = std::shared_ptr<Statement>(new Statement(session ));
		(*_statement_limit) << strSQL, into(*itemTemp), limit(_limit);
		_statement_limit->execute();

		for (const Poco::Tuple<Args...> vep:*itemTemp)
		{
			std::tuple<Args...> temp;
			__StandTupe<sizeof...(Args)-1, Args...>::SetStandTupe(vep, temp);
			items.push_back(std::move(temp));
		}

		itemTemp->clear();
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}

template<typename... Args>
bool CDatabaseIO::InsertSql(const std::string & strSQL,const std::tuple<Args...> & item)
{
	try
	{
		Poco::Tuple<Args...> pocoTupleTemp;
		__StandTupe<sizeof...(Args)-1, Args...>::SetPocoTupe(item, pocoTupleTemp);
		Session session(m_SessionPoolPtr->get());
		Statement select(session);
		select << strSQL, use(pocoTupleTemp), now;
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}


template<typename... Args>
bool CDatabaseIO::InsertSql(const std::string & strSQL,const std::vector<std::tuple<Args...>> & items)
{
	try
	{
		std::vector<Poco::Tuple<Args...>> verPoco;
		for (const std::tuple<Args...> & vep:items)
		{
			Poco::Tuple<Args...> pocoTupleTemp;
			__StandTupe<sizeof...(Args)-1, Args...>::SetPocoTupe(vep, pocoTupleTemp);
			verPoco.push_back(pocoTupleTemp);
		}

		Session session(m_SessionPoolPtr->get());
		Statement select(session);
		select << strSQL, use(verPoco), now;
		return true;
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}

template<typename... Args>
bool CDatabaseIO::ContinueExecute(std::vector<std::tuple<Args...>> & items)
{
	try
	{
		std::vector<Poco::Tuple<Args...>> * itemTemp = nullptr;
		GetPocoVerTuple<Args...>(&itemTemp);
		itemTemp->clear();
		_statement_limit->execute();
		for (const Poco::Tuple<Args...> vep:*itemTemp)
		{
			std::tuple<Args...> temp;
			__StandTupe<sizeof...(Args)-1, Args...>::SetStandTupe(vep, temp);
			items.push_back(std::move(temp));
		}
		return !_statement_limit->done();//是否完成
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}
#endif
