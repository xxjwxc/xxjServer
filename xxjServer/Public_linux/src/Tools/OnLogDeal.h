/********************************************************************
    创建时间：    2016/08/03  17:50:19 
    文件名：      OnLogDeal.h
    作者：        谢小军
    
    功能：        1.日志入口
                 2.
                
    说明：        1. 
                 2. 
*********************************************************************/

#ifndef _ONLOGDEAL_H_
#define _ONLOGDEAL_H_
#include <string>
#include <vector>
#include <map>
#include <boost/thread/mutex.hpp>

class OnLogDeal
{
public:
	static void PrintToFile(const std::string & fname,const std::vector<std::string> & prints, bool isClear = true);
public:
	static void OnDealCallBack();//开始处理日志
private:
	static boost::mutex m_mutex;
	static std::map<std::string, std::vector<std::string>> m_log_map;
	static void PrintError(const std::string & strerror);

};


#endif // 

