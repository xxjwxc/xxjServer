#ifndef __IDATABASE_IO_H_
#define __IDATABASE_IO_H_
#include <iostream>
#include <string>
#include <vector>
#include "SQLDefine.h"
#include "DatabaseIO.h"
#include <Poco\Data\Statement.h>
#include <boost/thread.hpp>
#include "../asio/DefineBase.h"
#include <Poco/Data/Session.h>
#include "traits.hpp"//类型萃取
#include <Poco/Data/SessionPool.h>
#include <memory>

class MySesion
{
public:
	MySesion(std::shared_ptr<Poco::Data::SessionPool> & sessionPtr)
	{
		Dequeue();
		m_session = std::shared_ptr<Poco::Data::Session>(new Poco::Data::Session(sessionPtr->get()));
		//如果与数据库建立会话成功,输出连接信息
		while(!m_session->isConnected())
		{
			Sleep(1000);
			m_session = std::shared_ptr<Poco::Data::Session>(new Poco::Data::Session(sessionPtr->get()));
		} 
	};
	~MySesion()
	{
		m_session->commit();
		m_session->close();
		Enqueue();
	}

public:
	std::shared_ptr<Poco::Data::Session> m_session;
private:
	void Dequeue()//条件变量得到一个消息
	{
		boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
		while (m_nowSession >= m_maxSession)//必须循环，必须在判断之后再wait()
		{
			m_cond.wait(m_oMutex);//会原子性的unlock m_oMutex 并进入等待
			//wait执行完毕之后会自动重现加锁
		}

		assert(m_nowSession < m_maxSession);

		++m_nowSession;
	}
	void Enqueue()//插入一个元素
	{
		{
			boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
			--m_nowSession;
		}

		m_cond.notify_one();//提示一个开启
	}

	static boost::mutex m_oMutex;
	static boost::condition_variable_any m_cond;//条件变量
public:
	static MY_UINT32 m_minSession;
	static MY_UINT32 m_maxSession;
	static MY_UINT32 m_nowSession;
};

class IDataBaseObj
{
public://连接相关
	/*
		功能：
			连接数据库,并创建session池
		参数:
			connector:连接数据库名字，可以指定 "SQLite" "MySQL" 可通过ODBC驱动支持Oracle, SQLite, DB2, SQLServer和PostgreSQL
			connectionString:连接数据库字符串

	*/
	virtual bool Connection(const std::string& connector, const std::string& connectionString, int minSession = 1, int maxSession = 2, std::size_t timeout = 60U) = 0;
	
	/*
		功能断开连接
	*/
	virtual void UnConnection()=0;
public://查询相关
	/*
	功能：
		查询数据库
	参数：
		strSQL:sql语句，查询用语句
		item:返回查询的结果
	说明：
		本函数只适用于返回一列的情况(其中T可以是一个vector数组)
	*/
	template<typename T>
	bool SelectSql(const std::string & strSQL, T & item);

	/*
	功能：
		查询数据库
	参数：
		strSQL:sql语句，查询用语句
		item:返回查询的结果
		T：用户自定义类型 (必须实现) META（）
		结构这里需要一个宏定义META，这个META的作用就是获取对象的元信息，有了这个元信息我们就可以很方便的实现序列化和反序列化了。
		struct Person
		{
		int age;
		string name;
		string city;

		META(age, name, city)
		};
	说明：
		本函数只适用于返回一列的情况(其中T可以是一个vector数组)
	*/
	template<class T,class = typename std::enable_if<is_user_class<T>::value>::type>
	inline bool SelectSqlEx(const std::string & strSQL, T & t);

	/*
	功能：
		查询数据库
	参数：
		strSQL:sql语句，查询用语句
		args:返回查询的结果
	说明：
		其中args中参数列表需跟sql中select中参数列表一致，
		如果使用select * 需要与数据库列表一致
		实现无tuple版本，只适用于单个结果集情况
	*/
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, Args& ...args);

	/*
		功能：
			查询数据库
		参数：
			strSQL:sql语句，查询用语句
			item:返回查询的结果
		说明：
			其中tuple中参数列表需跟sql中select中参数列表一致，
			如果使用select * 需要与数据库列表一致
	*/
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::tuple<Args...> & item);

	/*
	功能：
		查询数据库（返回集合）
	参数：
		strSQL:sql语句，查询用语句
		items:返回查询的结果
	说明：
		其中tuple中参数列表需跟sql中select中参数列表一致，
		如果使用select * 需要与数据库列表一致
	*/
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items);
	
	/*
	功能：
		查询数据库（返回集合）
	参数：
		strSQL:sql语句，查询用语句
		items:返回查询的结果
		limit:间隔数量，以limit为一个间隔,使用ContinueExecute得到下一个limit
	说明：
		其中tuple中参数列表需跟sql中select中参数列表一致，
		如果使用select * 需要与数据库列表一致
	*/
	template<typename... Args>
	bool SelectSql(const std::string & strSQL, std::vector<std::tuple<Args...>> & items, std::size_t _limit);

	/*
		功能：
			得到上一个limit限定符区域的下一个区域值
	*/
	template<typename... Args>
	bool ContinueExecute(std::vector<std::tuple<Args...>> & items);

public://插入相关
	/*
		功能：
			插入一条数据到数据库
		参数：
			strSQL:sql语句，其中要插入的以:分隔
			如：INSERT INTO user_tbl VALUES(:name, :address, :age)
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
		如：INSERT INTO user_tbl VALUES(:name, :address, :age)
		其中:name, :address, :age为站位符号
		tuple中参数列表需跟站位符列表顺序一致
	*/
	template<typename... Args>
	bool InsertSql(const std::string & strSQL,const std::vector<std::tuple<Args...>> & items);


public://其它(update,delete,完整sql不需要返回值的)
	/*
		功能：
			执行sql语句
		参数：
			strSQL:完整sql语句
	*/
	virtual bool ExecuteSql(const std::string & strSQL)=0;
	/*
	功能：
	获得一个连接
	参数：
	strSQL:完整sql语句
	statement:返回的结果集
	*/
	virtual MySesion GetSession() = 0;
// 	/*
// 	功能：释放一个连接
// 	*/
// 	virtual void ReleaseSession(Poco::Data::Session & session) = 0;
public:
	/*
		功能：
			获取异常信息
		说明：
			如果没有异常，返回空
	*/
	virtual std::string GetLastError() = 0;

	/*
		功能：
			清除异常信息
	*/
	virtual void ClearErrorInfo() = 0;

// private:
// 	virtual void Init() = 0;//初始化
// 	virtual void UnInit() = 0;//反注册
};


#endif