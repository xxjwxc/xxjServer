/********************************************************************
    创建时间:     2017/04/21  10:21:43 
    文件名:       MySqlite3.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.sqlite3数据基础类
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef _SQLITE3_BASE_H_
#define _SQLITE3_BASE_H_
#include <map>
#include <string>
#include <boost/thread.hpp>
#include "src/sqlite3.h"
#include "../PointBase.h"
#include "../MySingleton.h"


class Sqlite3db:public CPointBase<sqlite3>
{
public:
	explicit Sqlite3db(sqlite3 * db);
	~Sqlite3db();
	sqlite3 * Get();
public:
	void OnDestory();

};

class  Sqlite3Mgr:public MySingleton<Sqlite3Mgr>
{
public:
	bool ConnectionSqlite(const std::string & db);
	std::shared_ptr<Sqlite3db> GetConnection(const std::string & db); //获取一个//获得数据库连接
	bool ReleaseConnection(const std::string & db);
protected:
	bool IsHaveBuild(const std::string & db);
	std::tuple<bool,std::shared_ptr<Sqlite3db>> BuildOneDB(const std::string &db);
private:
	boost::mutex m_oMutex;
	std::map<std::string, std::shared_ptr<Sqlite3db>> m_db_mgr;

};

#endif