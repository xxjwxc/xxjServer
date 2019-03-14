//#include "stdafx.h"
#include "MySqlBase.h"
#include <boost/date_time/posix_time/time_formatters.hpp>
#include "../OnLogDeal.h"

void CMysqlBase::GetString(const sql::SQLString & str, char *dest, unsigned int len)
{
	const char *t = str.c_str();
	if (t == NULL)
	{
		dest[0] = '\0';
	}
	else
	{
		assert(len >= str.length());
#ifdef _WIN32
		strncpy_s(dest, len, t, str.length());
#else
		strncpy(dest, t, str.length());
#endif
		
		dest[str.length() + 1] = '\0';
	}
}

void CMysqlBase::GetString(const sql::SQLString & str, std::string & dest, unsigned int len)
{
	dest = str;
	return;
	char * pTemp = new char[len + 1];
	GetString(str, pTemp, len);
	dest = std::string(pTemp);
	delete[]pTemp;
}

void CMysqlBase::destroy_db_res(sql::PreparedStatement *& prep_stmt, sql::ResultSet *& db_res)
{
	destroy_db_res(db_res);
	destroy_db_res(prep_stmt);
}

void CMysqlBase::destroy_db_res(sql::PreparedStatement *& prep_stmt)
{
	if (prep_stmt != NULL)
	{
		try
		{
			prep_stmt->close();
		}
		catch (sql::SQLException& e)
		{
			printf("fuck!");
		}

		delete prep_stmt;
		prep_stmt = NULL;
	}
}

void CMysqlBase::destroy_db_res(sql::ResultSet *& db_res)
{
	if (db_res != NULL)
	{
		try
		{
			db_res->close();
		}
		catch (sql::SQLException& e)
		{
			printf("fuck!");
		}

		delete db_res;
		db_res = NULL;
	}
}

void CMysqlBase::OnCatch(std::exception & e, std::string path)
{
	std::vector<std::string> ver;
	ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + e.what());
	OnLogDeal::PrintToFile(path, ver, false);
#ifdef _MFC_VER
	::MessageBox(nullptr, e.what(), "错误消息", MB_OK | MB_ICONERROR);
#endif
	std::cout << "error:" << e.what() << std::endl;//请输入参数
}

bool Connection::isClosed()
{
	if (!con)
		return true;
	else
	{
		if (m_creatTime > 0 && (CMyTimes::get_utc() - m_creatTime) > 2 * 3600)
		{
			return true;
		}
		else
		{
			return  OnDealisClosed();
		}
	}
}

bool _isclosed = false;
boost::mutex _oMutex;
MY_UINT32 _time_isClosed = 0;
bool Connection::OnDealisClosed()
{
	if (_isclosed)//已经检测到有连接失败了
	{
		MY_UINT32 now = CMyTimes::get_utc();
		if (now - _time_isClosed > 60) //一分钟超时再检测
		{
			boost::mutex::scoped_lock oLock(_oMutex);//互斥锁
			if (now - _time_isClosed > 60)//再次检测
			{
				_time_isClosed = CMyTimes::get_utc();
				_isclosed = (!con->isValid());
				return _isclosed;
			}else {
				return true;
			}
			
		}
		else {
			return true;
		}

	}else {//没有检测到有连接失败
		boost::mutex::scoped_lock oLock(_oMutex);//互斥锁
		if (_isclosed)
		{
			return true;
		}else {
			_isclosed = (!con->isValid());
			return _isclosed;
		}
	}
}
