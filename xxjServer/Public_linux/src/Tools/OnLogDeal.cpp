#include "OnLogDeal.h"
#include "MyTools.h"
#ifdef _WIN32
#include <direct.h>
#else
#include <time.h>
#include <sys/stat.h> 
#endif // _WIN32

boost::mutex OnLogDeal::m_mutex;
std::map<std::string, std::vector<std::string>> OnLogDeal::m_log_map;

void OnLogDeal::PrintToFile(const std::string & fname, const std::vector<std::string> & prints, bool isClear /*= true*/)
{
		boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)										 //boost::unique_lock<boost::shared_mutex> writeLock(m_rw_mutex);//写锁
		m_log_map[fname].insert(m_log_map[fname].end(),prints.begin(),prints.end());
}

void OnLogDeal::OnDealCallBack()
{
	if (!m_log_map.empty())
	{
		std::map<std::string, std::vector<std::string>> temp;
		{
			boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)										 //boost::unique_lock<boost::shared_mutex> writeLock(m_rw_mutex);//写锁
			temp = m_log_map;
			m_log_map.clear();
		}

		for (auto kvp:temp)
		{
			MyTools::PrintToFile(kvp.first, kvp.second, false);
		}
	}
}

void OnLogDeal::PrintError(const std::string & strerror)
{
#ifdef _WIN32
	_mkdir("loginfo");
#else
	mkdir("loginfo", S_IRWXU);
#endif // _WIN32
	std::string path = "loginfo/" + MyTools::GetCurrentDay() + ".txt";
	std::vector<std::string> ver;
	ver.push_back(strerror + "\t[" + MyTools::GetCurrentDate() + "]");
	PrintToFile(path, ver, false);
}
