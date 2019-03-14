
/********************************************************************
    创建时间：    2016/10/06  12:58:07 
    文件名：      MsSqlBase.h
    作者：        谢小军
    
    功能：       1.mssql 公共工具类
                2.主要用与mssql数据库的操作工具函数集合
                
    说明：       1. 
                2. 
*********************************************************************/
#ifndef MSSQLBASE_H_
#define MSSQLBASE_H_

//mssql head
#include "../PointBase.h"
#include "MsSqlConnection.h"
#include "../asio/DefineBase.h"
//--------------end

class CMSsqlBase
{
public:
// 	void GetString(sql::SQLString & str, char *dest, unsigned int len);
// 	void GetString(sql::SQLString & str, std::string & dest, unsigned int len);
	void destroy_db_res(_RecordsetPtr & db_res);
	bool isClosed(_ConnectionPtr & con);

	void OnCatch(std::exception & e,std::string path = "error.txt");
public:
	_RecordsetPtr OnQuery(_ConnectionPtr & con,const std::string & sql);

};

class CMsResultSet : public CMSsqlBase, public CPointBase<CMsResultSet >
{
public:
	CMsResultSet(_RecordsetPtr & ps);
	~CMsResultSet();
public:
	bool IsNull();
	bool isClosed();
	bool next();
	/*
		获取int值
	*/
	int getInt(const std::string & columnLabel);
	MY_UINT32 getUint(const std::string & columnLabel);
	std::string getString(const std::string & columnLabel);
private:
	void OnDestroy();

public:
	_RecordsetPtr px_back = nullptr;
	bool isNext = false;
};

class ConnectionMS{
public:
	_ConnectionPtr con = nullptr;
	unsigned int m_creatTime = 0;
};

#endif // !MYSQLBASE_H_
