#include "SqliteBase.h"

Sqlite3db::Sqlite3db(sqlite3 * db)
{
	px = db;
}

Sqlite3db::~Sqlite3db()
{
	OnDestory();
}

sqlite3 * Sqlite3db::Get()
{
	return px;
}

void Sqlite3db::OnDestory()
{
	if (px)
	{
		sqlite3_close(px);
		px = nullptr;
	}
}

bool Sqlite3Mgr::ConnectionSqlite(const std::string & db)
{
	if (IsHaveBuild(db))
	{
		return true;
	}
	else
	{
		auto tup = BuildOneDB(db);
		if (std::get<0>(tup))
		{
			boost::mutex::scoped_lock oLock(m_oMutex);//»¥³âËø
			m_db_mgr[db] = std::get<1>(tup);
			return true;
		}else {
			return false;
		}
	}
}

std::shared_ptr<Sqlite3db> Sqlite3Mgr::GetConnection(const std::string & db)
{
	if (ConnectionSqlite(db))
	{
		boost::mutex::scoped_lock oLock(m_oMutex);//»¥³âËø
		return (m_db_mgr.find(db)->second);
	}
	else {
		return std::shared_ptr<Sqlite3db>(new Sqlite3db(nullptr));
	}
}

bool Sqlite3Mgr::ReleaseConnection(const std::string & db)
{
	if (IsHaveBuild(db))
	{
		boost::mutex::scoped_lock oLock(m_oMutex);//»¥³âËø
		m_db_mgr.erase(db);
	}

	return true;
}

bool Sqlite3Mgr::IsHaveBuild(const std::string & db)
{
	boost::mutex::scoped_lock oLock(m_oMutex);//»¥³âËø
	return m_db_mgr.find(db) != m_db_mgr.end();
}

std::tuple<bool, std::shared_ptr<Sqlite3db>> Sqlite3Mgr::BuildOneDB(const std::string &db)
{
	sqlite3 * _db = nullptr;
	int rc = sqlite3_open(db.c_str(), &_db);
	auto ptr = std::shared_ptr<Sqlite3db>(new Sqlite3db(_db));
	return std::make_tuple((rc == SQLITE_OK),ptr);
}
