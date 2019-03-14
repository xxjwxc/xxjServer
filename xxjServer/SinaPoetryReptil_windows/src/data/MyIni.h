#ifndef MY_INI_H_
#define MY_INI_H_
#include <Tools/MyIni/MyIniBase.h>
#include <Tools/MySingleton.h>

class CMyIni:public CIniBase,public MySingleton<CMyIni>
{
public:
	CMyIni();
protected:
	bool OnDeal(const std::string & strjson);
public:
	DatabaseInfo m_dbInfo;
	std::string m_url;
	MY_UINT32 m_timeOut;
	int m_max_min[2];
private:
	bool m_isInit = false;
};

#endif // !MY_INI_H_
