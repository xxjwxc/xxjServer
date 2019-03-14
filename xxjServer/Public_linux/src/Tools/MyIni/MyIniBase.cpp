#include "MyIniBase.h"
#include <fstream>
#include <iostream>
#include <sstream> 


std::tuple<bool, std::string> CIniBase::GetJsonStr(const std::string & strPath)
{
	//如果是个很大的多行存储的文本型文件可以这么读：  
	//char buf[1024];                //临时保存读取出来的文件内容  
								   //std::string str;
	std::ifstream infile;
	infile.open(strPath.c_str());
	if (infile.is_open())          //文件打开成功,说明曾经写入过东西  
	{
		std::stringstream buffer;
// 		std::string str((std::istreambuf_iterator<char>(infile)),
// 			(std::istreambuf_iterator<char>()));
		buffer << infile.rdbuf();
		infile.close();

		return std::make_tuple(true, std::string(buffer.str()));
	}
	else
	{
		std::cout << "error: find " << strPath << " fail." << std::endl;
		std::cin.get();
		return std::make_tuple(false, "");
	}
}

bool CIniBase::SaveText(const std::string & str, const std::string & strPath)
{
	//如果是个很大的多行存储的文本型文件可以这么读：  
	std::ofstream outfile;
	outfile.open(strPath.c_str(), std::ios::out/*|std::ios::app*/);
	if (outfile.is_open())          //文件打开成功,说明曾经写入过东西  
	{
		outfile << str;
		outfile.close();
		return true;
	}
	else
	{
		std::cout << "error:find " << strPath << " fail." << std::endl;
		std::cin.get();
		return false;
	}
}

bool CIniBase::ParseDBinfo(CMyJson::JsonRoot & json, DatabaseInfo & dbinfo)
{
	if (!(json.isNull()))
	{
		dbinfo.name = CMyJson::GetStrValue(json, "name");
		dbinfo.host = CMyJson::GetStrValue(json, "host");
		dbinfo.user = CMyJson::GetStrValue(json, "user");
		dbinfo.password = CMyJson::GetStrValue(json, "password");

		return true;
	}
	return false;
}

bool CIniBase::ReadText(const std::string & path, CallBack call)
{
	std::tuple<bool, std::string> tup = GetJsonStr(path);
	if (std::get<0>(tup))
	{
		return call(std::get<1>(tup));
	}
	else
	{
		return false;
	}
}
