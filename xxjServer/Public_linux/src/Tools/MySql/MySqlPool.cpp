#include "MySqlPool.h"
#include "../MyTimes.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>  
#endif // _WIN32

#include <boost/date_time.hpp>
#include "../OnLogDeal.h"
#include "../MyTools.h"


bool MySqlConnPool::ConnectionMySql(const std::string& _host, const std::string& _port,const std::string & _Database, const std::string& _user,const std::string &_PWD, int _minSession /*= 1*/, int _maxSession /*= 2*/, std::size_t _timeout /*= 60U*/)
{
	if (!m_isInit)
	{
		m_isInit = true;
		if (_port.length() > 0)
		{

			db_info.host = _host + std::string(":") + _port;
		}
		else
		{
			db_info.host = _host;
		}
		db_info.name = _Database;
		db_info.password = _PWD;
		db_info.user = _user;
		minSession = _minSession;
		maxSession = _maxSession;
		timeout = _timeout;
		
		for (int i = 0; i < maxSession;i++)
		{
			m_queue.Enqueue(Connection());
		}
		return true;
	}

	return false;
}

bool MySqlConnPool::ConnectionMySql(DatabaseInfo info, int minSession /*= 1*/, int maxSession /*= 2*/, std::size_t timeout /*= 60U*/)
{
	return ConnectionMySql(info.host,info.port,info.name,info.user,info.password,minSession,maxSession,timeout);
}

void MySqlConnPool::OnReleaseOne(sql::Connection * con, unsigned int creatTime)
{
	assert(m_isInit&&"not init");
	Connection info;
	info.con = con;
	info.m_creatTime = creatTime;
	if (info.con)
	{
		info.con->commit();
		if (info.con->isClosed())
			OnDestroy(info.con);
	}

	m_queue.Enqueue(info);

	//std::cout << "OnReleaseOne : size " << m_queue.Size() << std::endl;
}

Connection MySqlConnPool::GetConnection()
{
	assert(m_isInit&&"not init");
	std::cout << "getconnection : size " << m_queue.Size() << std::endl;
	Connection info = m_queue.Dequeue();

	if (info.isClosed())
	{
		OnDestroy(info.con);

		while (!info.con )
		{
			info.con = CMySqlConnection::instance()->GetConnection(db_info);
			info.m_creatTime = CMyTimes::get_utc();

			if (!info.con)
			{
				std::cout << "----------------------------fuck--------------!\n";
				std::vector<std::string> ver;
				ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + "fuck-connect error:" 
					+ db_info.host + db_info.name + db_info.user + db_info.password);
				OnLogDeal::PrintToFile("error/error.txt", ver, false);
#ifdef _WIN32
				Sleep(1000);
#else
				sleep(1);
#endif // _WIN32
				if (!m_isLoop)
				{
					MyTools::PrintError(ver.at(0));
					break;
				}
			}
		}
		
		if (info.con && info.con->isClosed())
		{
			OnDestroy(info.con);
		}
	}
	return info;
}

MySqlConnPool::~MySqlConnPool()
{
	while (m_queue.Empty()){
		Connection info = m_queue.Dequeue();
		if (info.con != nullptr)
		{
			info.m_creatTime = 0;
			OnDestroy(info.con);
		}
	}
}

void MySqlConnPool::OnDestroy(sql::Connection *& px)
{
	assert(m_isInit&&"not init");
	if (px)
	{
		//px->commit();
		CMySqlConnection::instance()->ReleaseConnection(px);
		px = nullptr;
	}
}

