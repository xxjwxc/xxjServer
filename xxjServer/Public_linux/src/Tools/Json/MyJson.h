#ifndef MYJSON_H_
#define MYJSON_H_
#include "./include/json.h"
#include <stdint.h>
#include <string.h>


//Json 相关操作
namespace CMyJson
{
	//创建相关
	typedef JsonXXJ::Value JsonRoot;//整个Json对象
	//初始化JSON头
	inline JsonRoot CreatJson(unsigned int id)
	{
		JsonXXJ::Value json;
		json["ID"] = JsonXXJ::Value(id);
		return json;
	}
	//写入相关
	//增加一个json数据
	template<typename T>
	inline void AddValue(JsonRoot & json, const std::string strKey, const T & v)
	{
		json[strKey] = JsonXXJ::Value(v);
	}

	//增加一个json数据 偏特化
	template<>
	inline void AddValue(JsonRoot & json, const std::string strKey, const JsonRoot & jsonChild)
	{
		json[strKey] = jsonChild;
	}

	//增加一个json数据 偏特化
	template<>
	inline void AddValue(JsonRoot & json, const std::string strKey, const unsigned long & v)
	{
		char str[20];
		memset(str,0,20);
#ifdef _WIN32
		sprintf_s(str, 20, "%lu", v);
#else
		sprintf(str, "%lu", v);
#endif // _WIN32
		
		json[strKey] = JsonXXJ::Value(str);
	}

	//增加一个json数据 偏特化
	template<>
	inline void AddValue(JsonRoot & json, const std::string strKey, const int64_t & v)
	{
		char str[70];
		memset(str,0,70);
		//_i64toa(v,str,69);
#ifdef _WIN32
		sprintf_s(str, 70, "%I64d", v);
#else
		sprintf(str, "%I64d", v);
#endif // _WIN32
		
		json[strKey] = JsonXXJ::Value(str);
	}

	//初始化一个数组
	inline void InitArray(JsonRoot & json, const std::string strKey)
	{
		json[strKey].resize(0);
	}

	//为数组设置一个元素
	template<typename T>
	inline void AddArrayValue(JsonRoot & json, const std::string strKey, const T & v)
	{
		json[strKey].append(v);
	}

	template<>
	inline void AddArrayValue(JsonRoot & json, const std::string strKey, const unsigned long & v)
	{
		char str[20];
		memset(str,0,20);
#ifdef _WIN32
		sprintf_s(str, 20, "%lu", v);
#else
		sprintf(str, "%lu", v);
#endif // _WIN32
		json[strKey].append(str);
	}

	template<>
	inline void AddArrayValue(JsonRoot & json, const std::string strKey, const int64_t & v)
	{
		char str[70];
		memset(str,0,70);
#ifdef _WIN32
		sprintf_s(str, 70, "%I64d", v);
#else
		sprintf(str, "%I64d", v);
#endif // _WIN32
		json[strKey].append(str);
	}


	//从一个字符串中初始化json
	inline bool InitJson(const std::string str, JsonRoot & json)
	{
		//启用严格模式，让非法的json解析时直接返回false，不自动容错
		JsonXXJ::Reader reader(JsonXXJ::Features::strictMode());
		json.clear();
		if (reader.parse(str,json))
			return true;
		else
			return false;
	}
	//读取相关
	//读取一个json属性
	template<typename T>
	inline T GetValue(const JsonRoot & json, const std::string strKey)
	{
		
		if (!json[strKey].isNull())
			return json[strKey];
		else
			return T();
	}

	template<>
	inline double GetValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asDouble();
		else
			return 0.0f;
	}

	template<>
	inline std::string GetValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asString();
		else
			return "";
	}

	template<>
	inline int GetValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asInt();
		else
			return 0;
	}

	template<>
	inline bool GetValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asBool();
		else
			return false;
	}

	template<>
	inline unsigned int GetValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asUInt();
		else
			return 0;
	}

	inline void GetStrValue(const JsonRoot & json, const std::string strKey, std::string & v)
	{
		if (!json[strKey].isNull())
			v = json[strKey].asString();
		else
			v = "";
	}

	inline std::string GetStrValue(const JsonRoot & json, const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asString();
		else
			return "";
	}

	inline std::string GetJsonStr(const JsonRoot & json)
	{
		// 转换为字符串（带格式）   
		//return json.toStyledString();  
		// 输出无格式json字符串   
		JsonXXJ::FastWriter writer;  
		return writer.write(json);  
	}

	inline std::string GetJsonFormatStr(const JsonRoot & json)
	{
		// 转换为字符串（带格式）   
		return json.toStyledString();  
		// 输出无格式json字符串   
// 		Json::FastWriter writer;
// 		return writer.write(json);
	}
};

#endif