#include "OnDataDeal.h"
#include "MyIni.h"
#include <Tools/MyTools.h>
#include <boost/format.hpp>
#include <Tools/MySql/MySqlPool.h>

void OnDataDeal::OnInit()
{
	MySqlConnPool::instance()->ConnectionMySql(CMyIni::instance()->m_dbInfo.host, CMyIni::instance()->m_dbInfo.port, CMyIni::instance()->m_dbInfo.name, CMyIni::instance()->m_dbInfo.user, CMyIni::instance()->m_dbInfo.password, CMyIni::instance()->m_max_min[1], CMyIni::instance()->m_max_min[0], 60);
}

bool OnDataDeal::OnSaveData(const std::vector<DataInfo> & ver)
{
	try
	{
		CMyConnectionEx conn(MySqlConnPool::instance()->GetConnection());
		if (!conn.IsNull())
		{
			MY_UINT32 timestamp = 0;
			std::string errMsg;
			for (const auto & kvp : ver)
			{
				if (timestamp < kvp.timestamp)
				{
					timestamp = kvp.timestamp;
				}

				sql::SQLString sql = (boost::format("INSERT INTO show_tbl(username,timestamp,content,media_box,heard,flag) VALUES('%s',%d,'%s','%s','%s',0);")
					% MyTools::GetUtf8(kvp.username)
					% kvp.timestamp
					%MyTools::GetUtf8(kvp.content)
					% MyTools::GetUtf8(kvp.media_box)
					% MyTools::GetUtf8(kvp.heard)
					).str();
				CMyPreparedStatement prep_stmt = conn->prepareStatement(sql);
				if (!prep_stmt.IsNull())
				{
					prep_stmt->executeUpdate();
				}
			}

			if (m_max_timestamp < timestamp)
			{
				m_max_timestamp = timestamp;
			}

		}
	}
	catch (sql::SQLException & e)
	{
		std::cout << "error:" << e.what();
	}
	return true;
}

// bool OnDataDeal::OnCreat()
// {
// 	if (m_db.OnInit(CMyIni::instance()->m_dbInfo.host))
// 	{
// 		return true;
// 	}
// 	return false;
// }
// 
// bool OnDataDeal::OnDestory()
// {
// 	return m_db.OnDestory();
// }

bool OnDataDeal::OnLoadTimestamp()
{
	try
	{
		CMyConnectionEx conn(MySqlConnPool::instance()->GetConnection());
		if (!conn.IsNull())
		{
			sql::SQLString sql = "select max(timestamp) as max from show_tbl;";
			sql = conn.EscapeString(sql);
			CMyStatement prep_stmt = conn->createStatement();
			if (!prep_stmt.IsNull())
			{
				CMyResultSet db_res = prep_stmt->executeQuery(sql);
				if (!db_res.IsNull())
				{
					while (db_res->next())
					{
						m_max_timestamp = db_res->getInt("max");
						//std::cout << db_res->getInt("id") << ":" << db_res->getInt("timestamp");
					}
					//std::cout << std::endl;
				}
			}

		}
	}
	catch (sql::SQLException & e)
	{
		std::cout << "error:" << e.what();
	}

	return true;
}

