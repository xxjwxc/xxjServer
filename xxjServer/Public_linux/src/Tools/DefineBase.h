/********************************************************************
    创建时间：    2015/08/06  10:22:21 
    文件名：      DefineBase.h
    作者：        xxj
    
    功能:		1.用于基础数据定义
				2.
                
    说明:		1. 
                2. 
*********************************************************************/
#ifndef _DEFINEBASE_H_
#define _DEFINEBASE_H_
#include <map>
#include <cassert>
#include <string.h>

#define MY_ASSERT(condition,msg) ((condition)?((void)0):(std::cerr<<(msg),std::abort()))

typedef unsigned int MY_UINT32;
typedef unsigned long long MY_ULONGLONG;
typedef long long MY_LONGLONG;
#define ZeroVar(p) memset((&p),0,sizeof( p ))

typedef struct tagDatabaseInfo
{
	std::string name;
	std::string host;
	std::string user;
	std::string password;
	std::string port;
}DatabaseInfo;

#ifdef _WIN32
#else
#define strncpy_s(a,b,c,d) strncpy(a, c, d)//windows支持
#define MyCout(a) std::cout << MyTools::GetUtf8(a) << std::endl;
#endif

#endif

