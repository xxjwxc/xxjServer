#include "MyLeveldb.h"

bool CMyLeveldb::OnInit(const std::string & dbname)
{
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, dbname, &m_db);

	m_isInit = status.ok();
	return m_isInit;
}

bool CMyLeveldb::OnAdd(const std::string & key, const std::string & v)
{
	if (m_isInit)
	{
		std::string value;
		leveldb::Status s = m_db->Put(leveldb::WriteOptions(), key, v);
		return s.ok();
	}
	return false;

}

bool CMyLeveldb::OnGet(const std::string & key, std::string & v)
{
	if (m_isInit)
	{
		leveldb::Status s = m_db->Get(leveldb::ReadOptions(), key, &v);
		return s.ok();
	}
	return false;
}

bool CMyLeveldb::OnDelete(const std::string & key)
{
	if (m_isInit)
	{
		leveldb::Status s = m_db->Delete(leveldb::WriteOptions(), key);
		return s.ok();
	}
	return false;
}

CMyLeveldb::~CMyLeveldb()
{
	if (m_db)
		delete m_db;/*É¾³ýÊý¾Ý¿â*/
}

