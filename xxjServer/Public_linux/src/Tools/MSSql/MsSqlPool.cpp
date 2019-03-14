#include "MsSqlPool.h"
#include "..\MyTimes.h"
#include <windows.h>

bool MsSqlConnPool::ConnectionMySql(const std::string& _host, const std::string& _port, const std::string & _Database, const std::string& _user, const std::string &_PWD, int _minSession /*= 1*/, int _maxSession /*= 2*/, std::size_t _timeout /*= 60U*/, std::size_t db_timeout)
{
	if (!m_isInit)
	{
		m_isInit = true;
		if (_port.length() > 0)
			db_info.host = _host + std::string(",") + _port;
		else
			db_info.host = _host;

		db_info.name = _Database;
		db_info.password = _PWD;
		db_info.user = _user;
		minSession = _minSession;
		maxSession = _maxSession;
		timeout = _timeout;
		db_timeOut = db_timeout;

		for (int i = 0; i < maxSession;i++)
		{
			m_queue.Enqueue(ConnectionMS());
		}
		return true;
	}

	return false;
}

void MsSqlConnPool::OnReleaseOne(_ConnectionPtr con, unsigned int creatTime)
{
	assert(m_isInit&&"not init");
	ConnectionMS info;
	info.con = con;
	info.m_creatTime = creatTime;
	MY_UINT32 timeLost = CMyTimes::get_utc() - creatTime;
	if (con && timeLost > db_timeOut)
	{
		OnDestroy(info.con);
	}

	m_queue.Enqueue(info);

	//std::cout << "OnReleaseOne : size " << m_queue.Size() << std::endl;
}

ConnectionMS MsSqlConnPool::GetConnection()
{
	assert(m_isInit&&"not init");
	std::cout << "getconnection : size " << m_queue.Size() << std::endl;
	ConnectionMS info = m_queue.Dequeue();
	if (info.con == nullptr || info.con->State == adStateClosed)
	{
		if (info.con != nullptr)//isClosed()
		{
			OnDestroy(info.con);
		}

		info.m_creatTime = CMyTimes::get_utc();
		while (!info.con )
		{
			info.con = CMsSqlConnection::instance()->GetConnection(db_info);
			if (!info.con)
			{
				std::cout << "----------------------------fuck--------------!\n";
				Sleep(1000);
			}
		}
		
		if (info.con->State == adStateClosed)
		{
			OnDestroy(info.con);
		}
	}
	return info;
}

MsSqlConnPool::~MsSqlConnPool()
{
	while (m_queue.Empty()){
		ConnectionMS info = m_queue.Dequeue();
		if (info.con != nullptr)
		{
			info.m_creatTime = 0;
			OnDestroy(info.con);
		}
	}
}

void MsSqlConnPool::OnDestroy(_ConnectionPtr & px)
{
	assert(m_isInit&&"not init");
	if (px)
	{
		//px->commit();
		CMsSqlConnection::instance()->ReleaseConnection(px);
		px = nullptr;
	}
}

CMsResultSet CMsConnectionEx::executeQuery(const std::string & sql)
{
	return OnQuery(px_back, sql);
}
