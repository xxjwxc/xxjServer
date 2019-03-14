#include "ODBCDatabase.h"

bool CODBCDatabaseIO::ConnectionODBC(const std::string& Server, const std::string& Database,
	const std::string& UID, const std::string &PWD, int minSession /*= 1*/, int maxSession /*= 2*/, std::size_t timeout /*= 60U*/)
{
	std::string str = "Driver={SQL Server};Server="+Server+";Database="+Database+";UID="+UID+"; PWD="+PWD+";";
	return CDatabaseIO::Connection("ODBC", str, minSession, maxSession, timeout);
}

bool CODBCDatabaseIO::ConnectionMySql(const std::string& host, const std::string& port, const std::string & Database, const std::string& user, const std::string &PWD, int minSession /*= 1*/, int maxSession /*= 2*/, std::size_t timeout /*= 60U*/)
{
	//"Driver=MySQL ODBC 5.3 Unicode Driver;SERVER=182.254.133.58;UID=ifs;PWD=iceworkadmin1210;DATABASE=cxh;PORT=3306"
	//"Driver=MySQL ODBC 5.3 Unicode Driver;SERVER=182.254.133.58;UID=ifs;PWD=iceworkadmin1210;DATABASE=cxh;PORT=3306"
	std::string str = "Driver=MySQL ODBC 5.3 Unicode Driver;SERVER=" + host + ";DATABASE=" + Database + ";UID=" + user + ";PWD=" + PWD + ";Port=" + port + ";";
	return CDatabaseIO::Connection("ODBC", str, minSession, maxSession, timeout);
// 	std::string str = "host=" + host + ";port=" + port + ";user="+user+";password="+PWD+";db="+Database+";";//compress=true;auto-reconnect=true
// 	return CDatabaseIO::Connection("MySql", str, minSession, maxSession, timeout);

}


