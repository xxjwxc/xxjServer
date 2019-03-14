
/********************************************************************
    创建时间:     2017/04/21  10:21:43 
    文件名:       MySqlite3.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.sqlite3数据库操作
                2.数据库应用类
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef MYSQLITE3_H_
#define MYSQLITE3_H_
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "SqliteBase.h"
#include "src/sqlite3.h"
#include "../PointBase.h"

class CMySqlite3
{
public:
	~CMySqlite3();
	bool OnInit(const std::string & dbNmame);
public:
	bool OnQuery(const std::string & sql, char** & pResult,int & nRow,int & nCol, char * &errMsg);
	bool OnQuery(const std::string & sql, std::vector<std::map<std::string,std::string>> & m_result,std::string & errMsg);
	int  OnExec(const std::string & sql, std::string & errMsg);
	bool OnDestory();
private:
	std::string m_dbName;
	bool m_isInit = false;
	std::shared_ptr<Sqlite3db> m_ptrDb;

};
#endif