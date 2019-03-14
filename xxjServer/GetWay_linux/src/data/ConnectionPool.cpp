#include "ConnectionPool.h"
#include "MyIni.h"
#include <thread>
#include <Tools/Rand/MyRand.h>
#include <Tools/MyTools.h>

//#define  __SQLLITE  //SQLite数据库需开起此宏
//#define  __MYSQL	//MySql需要开启此红
//#define  __ODBC		//ODBC需要开启此宏

//注意，需要引入两个头
#include <Tools/MySql/SQLDefine.h>
#include <boost/locale/encoding.hpp>
#include <boost/lexical_cast.hpp>
#include <Tools/MyTimes.h>


#ifdef __ODBC
#include <Tools/MSSql/MsSqlPool.h>
#elif defined __MYSQL
#include <Tools/MySql/MySqlPool.h>
//文件编码
#define  CODE_ENCODED "GB2312"//程序编码方式
#define  DATABASE_ENCODED "GB2312"//数据库编码方式 sqlserver要改成GB2312， sqlite 是UTF-8
#define  DATABASE_ENCODED1 "UTF-8"//SQLserver数据库编码方式
#else
#endif // __MYSQL



ConnectionPool::ConnectionPool()
{
	Init();
}

ConnectionPool::~ConnectionPool()
{

}

void ConnectionPool::Init()
{
#ifdef __ODBC
	MsSqlConnPool::instance()->ConnectionMySql(CMyIni::instance()->m_dbInfo.host, CMyIni::instance()->m_dbInfo.port, CMyIni::instance()->m_dbInfo.name, CMyIni::instance()->m_dbInfo.user, CMyIni::instance()->m_dbInfo.password, CMyIni::instance()->m_max_min[1], CMyIni::instance()->m_max_min[0], 60, CMyIni::instance()->db_timeOut);
#elif defined __MYSQL
	MySqlConnPool::instance()->ConnectionMySql(CMyIni::instance()->m_dbInfo.host, CMyIni::instance()->m_dbInfo.port, CMyIni::instance()->m_dbInfo.name, CMyIni::instance()->m_dbInfo.user, CMyIni::instance()->m_dbInfo.password, CMyIni::instance()->m_max_min[1], CMyIni::instance()->m_max_min[0], 60);
#else
#endif // __MYSQL
}

#ifdef __ODBC
std::string ConnectionPool::OnDo(const SqlInfo & info, const CMyJson::JsonRoot & json)
{
	std::string strReturn = "";
	try
	{
		CMyJson::JsonRoot jsonRoot;
		jsonRoot["result"] = 0;
		CMsConnectionEx conn = MsSqlConnPool::instance()->GetConnection();
		if (!conn.IsNull())
		{
			std::string sql = BuildSql(info.sql, info.verInput, json["input"]);//先build sql
			//sql = conn.EscapeString(sql);
			//std::string sql0 = MyTools::GetUtf8(sql);
			CMsResultSet db_res = conn->executeQuery(sql);
			if (!db_res.IsNull())
			{
				while (db_res->next())
				{
					CMyJson::JsonRoot json;
					for (MY_UINT32 i = 0; i < info.verOutput.size(); i++)
					{
						if (info.verOutput.at(i).type == __int_)
							json[info.verOutput.at(i).name] = db_res->getInt(info.verOutput.at(i).name);
						else if (info.verOutput.at(i).type == __string_)
							json[info.verOutput.at(i).name] = /*MyTools::GetGB2312*/ std::string(db_res->getString(info.verOutput.at(i).name));
						else if (info.verOutput.at(i).type == ___datetime_)
							json[info.verOutput.at(i).name] = /*MyTools::GetGB2312*/ std::string(db_res->getString(info.verOutput.at(i).name));
					}
					CMyJson::AddArrayValue(jsonRoot, "ansData", json);
				}
			}
		}

		strReturn = CMyJson::GetJsonStr(jsonRoot);
	}
	catch (std::exception & exc)
	{
		std::string __errorInfo = exc.what();
		MyTools::PrintError(__errorInfo);
		std::cerr << exc.what() << std::endl;
		strReturn = "{\"result\":-11}";//参数错误
	}

	return strReturn;
}
#elif defined __MYSQL
std::string ConnectionPool::OnDo(const SqlInfo & info, const CMyJson::JsonRoot & json)
{
	std::string strReturn = "";
	try
	{
		CMyJson::JsonRoot jsonRoot;
		jsonRoot["result"] = 0;
		CMyConnectionEx conn(MySqlConnPool::instance()->GetConnection());
		if (!conn.IsNull())
		{
			sql::SQLString sql = BuildSql(info.sql, info.verInput, json["input"]);//先build sql
			sql = conn.EscapeString(sql);
			/*
				CMyPreparedStatement prep_stmt = conn->prepareStatement(sql);
				if (!prep_stmt.IsNull())
				{
					CMyResultSet db_res = prep_stmt->executeQuery();
				}
			*/
			//std::string sql0 = MyTools::GetUtf8(sql);
			CMyStatement stmt = conn->createStatement();
			if (!stmt.IsNull())
			{
				CMyResultSet db_res = stmt->executeQuery(sql);
				if (info.m_isRecordSet)
				{
					if (!db_res.IsNull())
					{
						while (db_res->next())
						{
							CMyJson::JsonRoot json;
							for (int i = 0; i < info.verOutput.size(); i++)
							{
								if (info.verOutput.at(i).type == __int_)
									json[info.verOutput.at(i).name] = db_res->getInt(info.verOutput.at(i).name);
								else if (info.verOutput.at(i).type == __string_)
									json[info.verOutput.at(i).name] = MyTools::GetGB2312(std::string(db_res->getString(info.verOutput.at(i).name)));
								else if (info.verOutput.at(i).type == ___datetime_)
									json[info.verOutput.at(i).name] = MyTools::GetGB2312( std::string(db_res->getString(info.verOutput.at(i).name)));
							}
							CMyJson::AddArrayValue(jsonRoot, "ansData", json);
						}
					}
				}
			}
		}

		strReturn = CMyJson::GetJsonStr(jsonRoot);
	}
	catch (std::exception & exc)
	{
		std::string __errorInfo = exc.what();
		MyTools::PrintError(__errorInfo);
		std::cerr << exc.what() << std::endl;
		strReturn = "{\"result\":-11}";//参数错误
	}

	return strReturn;
}
#else
#endif

std::string ConnectionPool::BuildSql(const std::vector<std::string> & sql, const std::vector<SqlPutInfo> & verInput, const CMyJson::JsonRoot & json)
{
	std::string str= sql.at(0);
	for (MY_UINT32 i = 0; i < verInput.size();i++)
	{
		str.append(GetSql(verInput.at(i),json));
		str.append(sql.at(i + 1));
	}

	return str;
}

std::string ConnectionPool::GetSql(const SqlPutInfo & info, const CMyJson::JsonRoot & json)
{
	if (info.type == __int_)
	{
		return boost::lexical_cast<std::string>(json[info.name].asInt());
	}
	else if (info.type == __string_)
	{
#ifdef __ODBC
		return std::string("'") + json[info.name].asString() + std::string("'");
#else
		return std::string("'") + boost::locale::conv::between(json[info.name].asString(), DATABASE_ENCODED1, CODE_ENCODED) + std::string("'");
#endif // __ODBC

	}
	else if (info.type == ___datetime_)
	{
		return std::string("'") + json[info.name].asString() + std::string("'");
	}

	assert(false);
	return "";
}