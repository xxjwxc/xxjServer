/********************************************************************
    创建时间:     2018/10/27  16:59:40 
    文件名:       MyLeveldb.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.leveldb数据库管理
                2.
                
    说明:        1. 
                2. 
*********************************************************************/

#pragma once
#include "../MySingleton.h"
#include <cassert>
#include <leveldb/db.h>
class CMyLeveldb:public MySingleton<CMyLeveldb>
{
public:
	virtual void OnInit() {}

	//初始化数据库
	bool OnInit(const std::string & dbname);

	//添加一个数据(自动更新)
	bool OnAdd(const std::string & key, const std::string & v);

	//获取一个数据
	bool OnGet(const std::string & key, std::string & v);

	//删除一个数据
	bool OnDelete(const std::string & key);


	~CMyLeveldb();
private:
	bool m_isInit = false;
	leveldb::DB * m_db = nullptr;
};