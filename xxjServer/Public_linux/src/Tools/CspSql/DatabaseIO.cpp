#include "DatabaseIO.h"

boost::condition_variable_any MySesion::m_cond;
boost::mutex MySesion::m_oMutex;
MY_UINT32 MySesion::m_nowSession = 0;
MY_UINT32 MySesion::m_maxSession = 0;
MY_UINT32 MySesion::m_minSession = 0;

CDatabaseIO::CDatabaseIO()
{
	Init();
}

CDatabaseIO::~CDatabaseIO()
{
	if (_statement_limit)
	{
		_statement_limit.reset();
		_statement_limit = nullptr;
	}
		
	UnInit();
}

bool CDatabaseIO::Connection(const std::string& connector, const std::string& connectionString, int minSession /*= 1*/, int maxSession /*= 2*/, std::size_t timeout /*= 60U*/)
{
	try
	{
		MySesion::m_minSession = minSession;
		MySesion::m_maxSession = maxSession;
		m_SessionPoolPtr = std::shared_ptr<Poco::Data::SessionPool>(new Poco::Data::SessionPool(connector, connectionString, minSession, maxSession, timeout));
		return m_SessionPoolPtr->isActive();
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
		return false;
	}
}

void CDatabaseIO::UnConnection()
{
	UnInit();
}

void CDatabaseIO::Init()
{
	try
	{
		m_SessionPoolPtr.reset();
		// register database connector
#ifdef __SQLLITE
		Poco::Data::SQLite::Connector::registerConnector();
#endif

#ifdef __MYSQL
		Poco::Data::MySQL::Connector::registerConnector();
#endif

#ifdef __ODBC
		Poco::Data::ODBC::Connector::registerConnector();
#endif
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
	}

}

void CDatabaseIO::UnInit()
{
	try
	{
		if (m_SessionPoolPtr)
		{
			m_SessionPoolPtr->shutdown();//关闭
			m_SessionPoolPtr.reset();
			m_SessionPoolPtr = nullptr;
		}
		// register database connector
#ifdef __SQLLITE
		Poco::Data::SQLite::Connector::unregisterConnector();
#endif

#ifdef __MYSQL
		Poco::Data::MySQL::Connector::unregisterConnector();
#endif

#ifdef __ODBC
		Poco::Data::ODBC::Connector::unregisterConnector();
#endif
	}
	catch (Poco::Exception& exc)
	{
		__errorInfo = exc.displayText();
		MyTools::PrintError(__errorInfo);
		//std::cerr << exc.displayText() << std::endl;
	}
}

std::string CDatabaseIO::GetLastError()
{
	return __errorInfo;
}

void CDatabaseIO::ClearErrorInfo()
{
	__errorInfo.clear();
}

bool CDatabaseIO::ExecuteSql(const std::string & strSQL)
{
	try
	{
		Session session(m_SessionPoolPtr->get());
		Statement execute(session);
		execute << strSQL;
		execute.execute();
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

MySesion CDatabaseIO::GetSession()
{
	return MySesion(m_SessionPoolPtr);
}

// void CDatabaseIO::Dequeue()
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	while (m_nowSession >= m_minSession)//必须循环，必须在判断之后再wait()
// 	{
// 		m_cond.wait(m_oMutex);//会原子性的unlock m_oMutex 并进入等待
// 		//wait执行完毕之后会自动重现加锁
// 	}
// 
// 	assert(m_nowSession < m_minSession);
// 
// 	++m_nowSession;
// }

// void CDatabaseIO::Enqueue()
// {
// 	{
// 		boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 		--m_nowSession;
// 	}
// 
// 	m_cond.notify_one();//提示一个开启
// }

// void CDatabaseIO::ReleaseSession(Poco::Data::Session & session)
// {
// 	session.commit();
// 	session.close();
// 	Enqueue();
// }

