/********************************************************************
	创建时间：	2015/05/17  22:37:13
	文件名：		SQLDefine.h
	作者：		xxj
	
	功能：		1. 公共配置
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef __SQLDEFINE_H_
#define __SQLDEFINE_H_
#include <string>

class DBDataBuf : public std::streambuf
{
public:
	DBDataBuf(char * d, size_t s) {
		setg(d, d, d + s);
	}
};

// typedef struct tagDatabaseInfo
// {
// 	std::string name;
// 	std::string host;
// 	std::string user;
// 	std::string password;
// 	std::string port;
// }DatabaseInfo;

// #define  __SQLLITE  //SQLite数据库需开起此宏
// #define  __MYSQL	//MySql需要开启此红
// #define  __ODBC		//ODBC需要开启此宏


#endif