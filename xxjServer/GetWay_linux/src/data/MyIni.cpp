#include "MyIni.h"
#include <fstream> 
#include <iostream>
#include <Tools/MyTools.h>
#include <boost/lexical_cast.hpp>

CMyIni::CMyIni(void)
{
	m_serverPort = 0;
	m_isInit = ReadText();
}

CMyIni::~CMyIni(void)
{
}

bool CMyIni::ReadText()
{
	std::tuple<bool, std::string> tup = GetJsonStr("config.json");
	if (std::get<0>(tup))
	{
		ParseJson(std::get<1>(tup));
		/*
		std::tuple<bool, std::string> tup1 = GetJsonStr("sql.json");
		if (std::get<0>(tup))
		{
			return ParseSql(std::get<1>(tup1));
		}
		else
		{
			return false;
		}
		*/
	}
	else
	{
		return false;
	}
}

bool CMyIni::ParseJson( const std::string & str )
{
	//m_verTOaddress.clear();
	//std::vector<std::string>().swap(m_verTOaddress);
	CMyJson::JsonRoot json;
	CMyJson::InitJson(str, json);
	m_serverIP = CMyJson::GetStrValue(json, "serverIP");
	m_serverPort = CMyJson::GetValue<int>(json, "serverPort");
	m_title = CMyJson::GetStrValue(json, "title");
	m_exestart = CMyJson::GetStrValue(json, "exestart");
	db_timeOut = CMyJson::GetValue<int>(json, "db_timeOut");
	m_timeOut = CMyJson::GetValue<MY_UINT32>(json, "timeOut");
	if (db_timeOut < 0)
		db_timeOut = 0;
	for (int i = 0; i < 2;i++)
	{
		m_max_min[i] = json["max_min"][i].asInt();
	}
	

	//IGameBase::isLog = CMyJson::GetValue<bool>(json, "isLog");
	ParseDBinfo(json["dbInfo"], m_dbInfo);


	return true;
}

bool CMyIni::ParseDBinfo(CMyJson::JsonRoot & json, DatabaseInfo & dbinfo)
{
	dbinfo.name = CMyJson::GetStrValue(json, "name");
	dbinfo.host = CMyJson::GetStrValue(json, "host");
	dbinfo.user = CMyJson::GetStrValue(json, "user");
	dbinfo.password = CMyJson::GetStrValue(json, "password");
	dbinfo.port = CMyJson::GetStrValue(json, "port");

	return true;
}

bool CMyIni::ParseSql(const std::string & str)
{
	//m_verTOaddress.clear();
	//std::vector<std::string>().swap(m_verTOaddress);
	m_sqlMap.clear();
	CMyJson::JsonRoot json;
	CMyJson::InitJson(str, json);
	
	if (!json["data"].isNull())
	{
		SqlInfo info;
		std::string str;
		SqlPutInfo temp;
		for (MY_UINT32 i = 0; i < json["data"].size(); i++)
		{
			info.Clear();
			info.key = CMyJson::GetValue<std::string>(json["data"][i], "key");
			str = CMyJson::GetValue<std::string>(json["data"][i], "sql");
			info.sql = MyTools::SplitString(str, "?");
			for (MY_UINT32 j = 0; j < json["data"][i]["input"].size();j++)
			{
				str = json["data"][i]["input"][j].asString();
				auto ver = MyTools::SplitString(str, ":");
				temp.name = ver.at(0);
				temp.type = boost::lexical_cast<int>(ver.at(1));
				info.verInput.push_back(temp);
			}
			assert(info.sql.size() == (info.verInput.size()+1));
			for (MY_UINT32 j = 0; j < json["data"][i]["output"].size(); j++)
			{
				str = json["data"][i]["output"][j].asString();
				auto ver = MyTools::SplitString(str, ":");
				temp.name = ver.at(0);
				temp.type = boost::lexical_cast<int>(ver.at(1));
				info.verOutput.push_back(temp);
			}
			info.m_isRecordSet = CMyJson::GetValue<bool>(json["data"][i], "isRecordSet");
			m_sqlMap[info.key] = info;
		}
	}
	return true;
}
