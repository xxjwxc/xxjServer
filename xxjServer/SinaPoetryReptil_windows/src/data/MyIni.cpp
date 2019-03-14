#include "MyIni.h"
#include <functional>

CMyIni::CMyIni()
{
	m_isInit = ReadText("config.json",std::bind(&CMyIni::OnDeal,this,std::placeholders::_1));
}

bool CMyIni::OnDeal(const std::string & str)
{
	CMyJson::JsonRoot json;
	if (CMyJson::InitJson(str, json))
	{
		m_url = CMyJson::GetValue<std::string>(json,"URL");
		ParseDBinfo(json["dbInfo"],m_dbInfo);
		m_timeOut = CMyJson::GetValue<MY_UINT32>(json,"timeOut");
		for (int i = 0; i < 2; i++)
		{
			m_max_min[i] = json["max_min"][i].asInt();
		}

		return true;
	}

	assert(false && "json init error.");
	return false;
}


