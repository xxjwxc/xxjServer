/********************************************************************
创建时间：    2016/07/18  15:09:11
文件名：      MyInit.h
作者：        谢小军

功能：        1.//配置项目
2.

说明：        1.
2.
*********************************************************************/
#ifndef _MYINIT_H_
#define _MYINIT_H_

#include <Tools/MyIni/MyIniBase.h>
#include <Tools/MySingleton.h>
#include <Tools/DefineBase.h>

#define  __int_ 0
#define  __string_ 1
#define  ___datetime_ 2
typedef struct tagSqlPutInfo
{
	std::string name;
	int type;
}SqlPutInfo;

typedef struct tagSqlInfo
{
	std::string key; //sql关键字
	std::vector<std::string> sql; //sql 执行语言，占位符使用?占位
	std::vector<SqlPutInfo> verInput;//参数
	std::vector<SqlPutInfo> verOutput;//结果集0:int ,1:string,2:datetime
	bool m_isRecordSet;
	tagSqlInfo()
	{
		Clear();
	}
	void Clear()
	{
		key.clear(); //sql关键字
		sql.clear(); //sql 执行语言，占位符使用?占位
		verInput.clear();//参数
		verOutput.clear();//结果集0:int ,1:string,2:datetime
		m_isRecordSet = false;
	}
}SqlInfo;




class CMyIni :public CIniBase, public MySingleton<CMyIni>
{
public:
	CMyIni(void);
	~CMyIni(void);
public:
	bool ReadText();
	bool ParseJson(const std::string & str);
	bool ParseSql(const std::string & str);
	bool ParseDBinfo(CMyJson::JsonRoot & json, DatabaseInfo & dbinfo);

	DatabaseInfo m_dbInfo;//数据库信息
	MY_UINT32 m_timeOut = 0;

	std::string m_serverIP;
	int m_serverPort;
	std::string m_title;
	std::string m_exestart;
	bool m_isInit = false;
	int m_max_min[2];
	std::map<std::string, SqlInfo> m_sqlMap;

	int db_timeOut = 0;

public://数据库配置函数

};

#endif