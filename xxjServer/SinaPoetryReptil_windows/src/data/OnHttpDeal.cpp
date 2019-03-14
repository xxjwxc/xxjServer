#include "OnHttpDeal.h"
#include <iostream>
#include "MyIni.h"
#include <Tools/MyTools.h>
#include <boost/format.hpp>
#include <Tools/MyTimes.h>
#include "OnDataDeal.h"


void MyHttpDeal::OnTimeOut()
{
	std::cout << CMyTimes::GetTimeStr(CMyTimes::get_utc()) << ":start" << std::endl;
	std::tuple<bool, std::string> tup = m_myHttpTools.OnOpenHttps("https://weibo.com/shiciguyun","",false);
	//std::tuple<bool, std::string> tup = m_myHttpTools.OnOpenHttps(CMyIni::instance()->m_url, "", false);
	if (std::get<0>(tup))
	{
		OnAnalyzeUrl(MyTools::GetGB2312(std::get<1>(tup)));
	}
}

void MyHttpDeal::OnAnalyzeUrl(std::string & html)
{
	MY_UINT32 & timestamp_max = OnDataDeal::instance()->m_max_timestamp;
	//std::cout << html<<std::endl;
	CMyJson::JsonRoot root;
	if (CMyJson::InitJson(html, root))
	{
		DataInfo info;
		MY_UINT32 timestamp = 0;
		CMyJson::JsonRoot json_statuses = CMyJson::GetValue<CMyJson::JsonRoot>(root, "statuses");
		if (!json_statuses.isNull())
		{
			std::vector<DataInfo> ver_add;
			for (int i = 0; i < json_statuses.size();i++)
			{
				info.Clear();
				//¾ßÌåÀàÈÝ
				CMyJson::JsonRoot json_content = json_statuses[i];
				if (!json_content.isNull() && !json_content["original_pic"].isNull())
				{
					info.timestamp = AnalyzeDateTime(CMyJson::GetValue<std::string>(json_content,"created_at"));
					if (info.timestamp > timestamp_max)
					{
						info.content = CMyJson::GetValue<std::string>(json_content, "text");
						info.username = CMyJson::GetValue<std::string>
							(CMyJson::GetValue<CMyJson::JsonRoot>(json_content, "user"), "name");
						info.media_box = OnGetPicUrls(CMyJson::GetValue<CMyJson::JsonRoot>(json_content, "pic_urls"),
							CMyJson::GetValue<std::string>(json_content, "original_pic"));
						info.heard = CMyJson::GetValue<std::string>
							(CMyJson::GetValue<CMyJson::JsonRoot>(json_content, "user"), "avatar_large");
						ver_add.push_back(info);
						std::cout << CMyTimes::GetTimeStr(CMyTimes::get_utc()) << ":" << info.content<<std::endl;
					}
				}
			}
			if (ver_add.size() > 0){
				std::reverse(ver_add.begin(), ver_add.end());
				OnDataDeal::instance()->OnSaveData(ver_add);
			}
		}
	}

}

MY_UINT32 MyHttpDeal::AnalyzeDateTime(const std::string & str)
{
	if (str.empty() || str.length() != 30)
		return 0;

	MY_UINT32 day, hour, min, sec, year;
	char week[8] = { 0 }, month[8] = {0};
	sscanf(str.c_str(), "%3s %3s %2d %2d:%2d:%2d +0800 %4d"
		,week
		,month
		,&day
		,&hour
		,&min
		,&sec
		,&year
	);

	static std::map<std::string, int> s_map = {
		std::map<std::string, int>::value_type("Jan",1),
		std::map<std::string, int>::value_type("Feb",2),
		std::map<std::string, int>::value_type("Mar",3),
		std::map<std::string, int>::value_type("Apr",4),
		std::map<std::string, int>::value_type("May",5),
		std::map<std::string, int>::value_type("Jun",6),
		std::map<std::string, int>::value_type("Jul",7),
		std::map<std::string, int>::value_type("Aug",8),
		std::map<std::string, int>::value_type("Sep",9),
		std::map<std::string, int>::value_type("Oct",10),
		std::map<std::string, int>::value_type("Nov",11),
		std::map<std::string, int>::value_type("Dec",12),
	};
	try
	{
		struct tm tm1;
		tm1.tm_year = year;
		tm1.tm_mon = s_map[month];
		tm1.tm_mday = day;
		tm1.tm_hour = hour;
		tm1.tm_min = min;
		tm1.tm_sec = sec;
		tm1.tm_year -= 1900;
		tm1.tm_mon--;
		tm1.tm_isdst = -1;
		return mktime(&tm1);
	}
	catch (...)
	{
		printf("error Get_Time_from_str");
	}

	return 0;
}

std::string MyHttpDeal::OnGetPicUrls(CMyJson::JsonRoot & json_root, std::string original_pic)
{
	if (original_pic.empty() || original_pic.length() < 0 || json_root.isNull())
		return "{}";

	unsigned int loc = original_pic.rfind("/", original_pic.length());
	if (loc != std::string::npos)
	{
		CMyJson::JsonRoot json;
		std::string str_head(original_pic.begin(), original_pic.begin() + loc);
		for (int i = 0; i < json_root.size(); i++)
		{
			std::string str = CMyJson::GetValue<std::string>(json_root[i], "thumbnail_pic");

			unsigned int loc = str.rfind("/", str.length());
			if (loc != std::string::npos)
			{
				CMyJson::AddArrayValue(json, "images", str_head + std::string(str.begin() + loc, str.end()));
			}
		}

		return CMyJson::GetJsonStr(json);
	}

	return "{}";
}
