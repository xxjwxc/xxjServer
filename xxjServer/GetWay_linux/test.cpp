#include "test.h"
#include "src/data/MyIni.h"

void DealTest::OnInit()
{
	MySqlConnPool::instance()->ConnectionMySql(CMyIni::instance()->m_dbInfo.host, CMyIni::instance()->m_dbInfo.port, CMyIni::instance()->m_dbInfo.name, CMyIni::instance()->m_dbInfo.user, CMyIni::instance()->m_dbInfo.password, CMyIni::instance()->m_max_min[1], CMyIni::instance()->m_max_min[0], 60);
}

void DealTest::OnDeal()
{
	try
	{
		CMyConnectionEx conn(MySqlConnPool::instance()->GetConnection());
		if (!conn.IsNull())
		{
			for (int i = 0; i < 10; i++)
			{
				sql::SQLString sql = "CALL test();";
				sql = conn.EscapeString(sql);
				CMyPreparedStatement prep_stmt = conn->prepareStatement(sql);
				if (!prep_stmt.IsNull())
				{
					CMyResultSet db_res = prep_stmt->executeQuery();
					if (!db_res.IsNull())
					{
						while (db_res->next())
						{
							//std::cout << db_res->getInt("id") << ":" << db_res->getInt("timestamp");
						}
						//std::cout << std::endl;
					}
				}
			}
		}
	}
	catch (sql::SQLException & e)
	{
		std::cout<<"error:" << e.what();
	}

	return;
}

void DealTest::OnTimeOut() {
	OnDeal();
}