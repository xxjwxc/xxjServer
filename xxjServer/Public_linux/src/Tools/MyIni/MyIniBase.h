/********************************************************************
    创建时间：    2016/07/18  15:31:41 
    文件名：      MyIniBase.h
    作者：        谢小军
    
    功能：        1.
                 2.
                
    说明：        1. 
                 2. 
*********************************************************************/
#ifndef __INIBASE_H_
#define __INIBASE_H_

#include <tuple>
#include <string>
#include <functional>
#include "../Json/MyJson.h"
#include "../DefineBase.h"

class CIniBase
{
protected:
	typedef std::function<bool(const std::string & str)> CallBack;
	std::tuple<bool, std::string> GetJsonStr(const std::string & str);
	bool SaveText(const std::string & str, const std::string & strPath);
	bool ParseDBinfo(CMyJson::JsonRoot & json, DatabaseInfo & dbinfo);
public:
	bool ReadText(const std::string & path, CallBack call);
};

#endif