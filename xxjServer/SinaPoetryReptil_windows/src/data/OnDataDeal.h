
/********************************************************************
    创建时间:     2017/04/25  14:45:33 
    文件名:       DataDeal.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.数据读取库相关
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef DATA_DEAL_H_
#define DATA_DEAL_H_
#include <vector>
#include <Tools/MySingleton.h>
#include <Tools/DefineBase.h>
//#include <Tools/Sqlite3/MySqlite3.h>

typedef struct tagDataInfo {
	MY_UINT32 timestamp;
	std::string content;
	std::string username;
	std::string media_box;
	std::string heard;
	void Clear()
	{
		content.clear();
		timestamp = 0;
		username.clear();
		media_box.clear();
		heard.clear();
	}
}DataInfo;

class OnDataDeal:public MySingleton<OnDataDeal>
{
public:
	virtual void OnInit() override;
public:
	bool OnSaveData(const std::vector<DataInfo> & ver);
	bool OnLoadTimestamp();
public:
	bool OnCreat();
	bool OnDestory();
	MY_UINT32 m_max_timestamp = 0;
private:
	//CMySqlite3 m_db;
};


#endif // !MY_INI_H_
