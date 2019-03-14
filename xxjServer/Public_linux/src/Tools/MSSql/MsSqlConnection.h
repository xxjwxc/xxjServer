/********************************************************************
    创建时间：    2015/09/23  9:34:18 
    文件名：        CMsSqlConnection.h
    作者：        谢小军
    
    功能：       1.用于MySql数据库连接使用
                2.
                
    说明：       1. 
                2. 
*********************************************************************/
#ifndef __MSSQLCONNECTION_H_
#define __MSSQLCONNECTION_H_

#include <winsock2.h>
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF"), rename("BOF", "adoBOF")
#include "..\MySingleton.h"
#include "..\MySql\SQLDefine.h"
#include "MsSqlBase.h"


class CMsSqlConnection :public MySingleton<CMsSqlConnection>
{
public:
	_ConnectionPtr GetConnection(const DatabaseInfo & dbInfo);
	void ReleaseConnection(_ConnectionPtr & conn);
	CMsSqlConnection();
	~CMsSqlConnection();
private:
	void DestroyConnection(_ConnectionPtr & conn);
	std::string OnGetConnectionStr(const DatabaseInfo & dbinfo);//获取连接字符串
	

};


#endif // !CMySqlConnection_H_