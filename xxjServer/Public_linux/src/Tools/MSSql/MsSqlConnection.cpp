#include "MsSqlConnection.h"
#include "..\MyTools.h"
#include <boost\date_time\posix_time\time_formatters.hpp>

//#pragma comment(lib, "libmysql.lib")

_ConnectionPtr CMsSqlConnection::GetConnection(const DatabaseInfo & dbInfo)
{
	try
	{
		_ConnectionPtr pConn("ADODB.Connection");

		if (!pConn)
			return nullptr;

		//获取连接字符串
		std::string connectionStr = OnGetConnectionStr(dbInfo);

		//步骤1--打开连接
		//pConn->Open("Provider=SQLOLEDB;Data Source=LITTLEX;Initial Catalog=mysqltest;", "nihao", "111111", -1);
		if (!SUCCEEDED(pConn->Open(connectionStr.c_str(), dbInfo.user.c_str(), dbInfo.password.c_str(), adCmdUnspecified)))
		{
			return nullptr;
		}

		return pConn;
	}
	catch (_com_error & e)
	{
		printf(e.ErrorMessage());
		//MessageBox(nullptr,e.what(),"数据库错误消息",MB_OK);
		std::vector<std::string> ver;
		ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + e.ErrorMessage());
		MyTools::PrintToFile("error.txt", ver, false);
	}
	catch (std::exception &e)
	{
		printf(e.what());
		//MessageBox(nullptr,e.what(),"数据库错误消息",MB_OK);
		std::vector<std::string> ver;
		ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + e.what());
		MyTools::PrintToFile("error.txt", ver, false);
		//::MessageBox(nullptr, e.what(), "错误消息", MB_OK | MB_ICONERROR);
#ifdef _MFC_VER
        ::MessageBox(nullptr, "连接数据库失败", "错误消息", MB_OK | MB_ICONERROR);
#endif // _DEBUG
	}

	return nullptr;

}

void CMsSqlConnection::ReleaseConnection(_ConnectionPtr & conn)
{
	DestroyConnection(conn);
}

CMsSqlConnection::CMsSqlConnection()
{
	::CoInitialize(NULL);
}

CMsSqlConnection::~CMsSqlConnection()
{
	 ::CoUninitialize();
}

void CMsSqlConnection::DestroyConnection(_ConnectionPtr & conn)
{
	if (conn == nullptr)
	{
		return;
	}
	try
	{
		if (conn->State != adStateClosed)
		{
			conn->Close();
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what()<<std::endl;
		printf("fuck1!!\n");
	}

	conn = nullptr;
}

std::string CMsSqlConnection::OnGetConnectionStr(const DatabaseInfo & dbinfo)
{
	std::string conStr = "";
	//在此添加自己的连接设置
// 	if (dbinfo.user.empty() && dbinfo.password.empty())
// 	{
// 		conStr = "Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog="
// 			+ dbinfo.name + ";Data Source=" + dbinfo.host;
// 		if (dbinfo.port.length() > 0)
// 		{
// 			conStr += "," + dbinfo.port;
// 		}
// 	}
// 	else
// 	{
// 		conStr = "Provider=SQLOLEDB;Data Source=" + server + ";Initial Catalog=" + dbinfo.name + ";";
// 	}

	std::string server = dbinfo.host;
	if (dbinfo.port.length() > 0)
		server += "," + dbinfo.port;
	conStr = "Provider=SQLOLEDB;Data Source=" + server + ";Initial Catalog=" + dbinfo.name + ";";
	return conStr;
}

