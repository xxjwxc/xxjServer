//#include "stdafx.h"
#include "MySqlConnection.h"
#include <boost/date_time/posix_time/time_formatters.hpp>
#include "../OnLogDeal.h"

void CMySqlConnection::OnInit()
{
	m_pDriver = get_driver_instance();
}

//#pragma comment(lib, "libmysql.lib")

sql::Connection * CMySqlConnection::GetConnection(const DatabaseInfo & dbInfo)
{
	//sql::Driver *driver = nullptr;
	sql::Connection * con = nullptr;
	try
	{
// 		static sql::Driver *m_pDriver =  get_driver_instance();
// 
		if (m_pDriver == NULL)
		{
			return NULL;
		}

		//m_pDriver->threadInit();

		//DatabaseInfo dbInfo = CMyIni::instance()->m_dbInfo;//数据库连接信息

		/* create a database connection using the Driver */

		{
			boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
			con = m_pDriver->connect(dbInfo.host, dbInfo.user, dbInfo.password);
		}


		if (con == NULL)
		{
			return NULL;
		}

		/* turn off the autocommit */
		con->setAutoCommit(0);
		con->setSchema(dbInfo.name);
		//m_pDriver->threadEnd();

		/*
			sql::Statement *stmt = NULL;
			sql::Savepoint *savept = NULL;
			savept = conn->setSavepoint("SAVE_POINT");
			stmt = conn->createStatement();

			// 回滚操作
			conn->rollback(savept);
			conn->releaseSavepoint(savept);

		*/
	}
	catch (sql::SQLException &e)
	{
		printf(e.what());
		//MessageBox(nullptr,e.what(),"数据库错误消息",MB_OK);
		std::vector<std::string> ver;
		ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + e.what());
		OnLogDeal::PrintToFile("error/error.txt", ver, false);
		//::MessageBox(nullptr, e.what(), "错误消息", MB_OK | MB_ICONERROR);
#ifdef _MFC_VER
        ::MessageBox(nullptr, "连接数据库失败", "错误消息", MB_OK | MB_ICONERROR);
#endif // _DEBUG
		std::cout << "error:" << e.what() << std::endl;//请输入参数
		if (con != NULL)
		{
			try
			{
				con->close();
			}
			catch (sql::SQLException &e)
			{
				printf("fuck!!\n");
			}

			delete con;
		}
		con = NULL;
	}

	return con;

}

void CMySqlConnection::ReleaseConnection(sql::Connection *& conn)
{
	DestroyConnection(conn);
}

void CMySqlConnection::DestroyConnection(sql::Connection *& conn)
{
	if (conn == NULL)
	{
		return;
	}
	try
	{
		if (!conn->isClosed())
		{
			conn->close();
		}
		
	}
	catch (sql::SQLException &e)
	{
		std::cout << e.what()<<std::endl;
		printf("fuck1!!\n");
	}

	delete conn;
	conn = nullptr;
}

