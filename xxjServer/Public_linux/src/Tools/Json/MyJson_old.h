#ifndef MYJSON_WIN_H_
#define MYJSON_WIN_H_
#include "./include/json.h"


//Json 相关操作
class CMyJson
{
public:
	typedef Json::Value JsonRoot;//整个Json对象
	//初始化JSON头
	JsonRoot CreatJson(unsigned int id)
	{
		Json::Value json;
		json["ID"] = Json::Value(id);
		return json;
	}
public://写入相关
	//增加一个json数据
	template<typename T>
	void AddValue(JsonRoot & json,const std::string strKey,const T & v) const
	{
		json[strKey] = Json::Value(v);
	}

	//增加一个json数据 偏特化
	template<>
	void AddValue(JsonRoot & json,const std::string strKey,const JsonRoot & jsonChild) const
	{
		json[strKey] = jsonChild;
	}

	//增加一个json数据 偏特化
	template<>
	void AddValue(JsonRoot & json,const std::string strKey,const unsigned long & v) const
	{
		char str[20];
		memset(str,0,20);
		sprintf_s(str,20,"%lu",v);
		json[strKey] = Json::Value(str);
	}

	//增加一个json数据 偏特化
	template<>
	void AddValue(JsonRoot & json,const std::string strKey,const __int64 & v) const
	{
		char str[70];
		memset(str,0,70);
		//_i64toa(v,str,69);
		sprintf_s(str,70,"%I64d",v);
		json[strKey] = Json::Value(str);
	}

	//为数组设置一个元素
	template<typename T>
	void AddArrayValue(JsonRoot & json,const std::string strKey,const T & v) const
	{
		json[strKey].append(v);
	}

	template<>
	void AddArrayValue(JsonRoot & json,const std::string strKey,const unsigned long & v) const
	{
		char str[20];
		memset(str,0,20);
		sprintf_s(str,20,"%lu",v);
		json[strKey].append(str);
	}

	template<>
	void AddArrayValue(JsonRoot & json,const std::string strKey,const __int64 & v) const
	{
		char str[70];
		memset(str,0,70);
		sprintf_s(str,70,"%I64d",v);
		json[strKey].append(str);
	}


	//从一个字符串中初始化json
	bool InitJson(const std::string str,JsonRoot & json) const
	{
		Json::Reader reader;
		json.clear();
		if (reader.parse(str,json))
			return true;
		else
			return false;
	}
public://读取相关
	//读取一个json属性
	template<typename T>
	T GetValue(const JsonRoot & json,const std::string strKey) const
	{
		if (!json[strKey].isNull())
			return json[strKey];
		else
			return NULL;
	}

	template<>
	std::string GetValue(const JsonRoot & json,const std::string strKey)const
	{
		if (!json[strKey].isNull())
			return json[strKey].asString();
		else
			return "";
	}

	template<>
	int GetValue(const JsonRoot & json,const std::string strKey)const
	{
		if (!json[strKey].isNull())
			return json[strKey].asInt();
		else
			return 0;
	}

	template<>
	bool GetValue(const JsonRoot & json,const std::string strKey)const
	{
		if (!json[strKey].isNull())
			return json[strKey].asBool();
		else
			return false;
	}

	template<>
	unsigned int GetValue(const JsonRoot & json,const std::string strKey)const
	{
		if (!json[strKey].isNull())
			return json[strKey].asUInt();
		else
			return 0;
	}

	void GetStrValue(const JsonRoot & json,const std::string strKey,std::string & v)
	{
		if (!json[strKey].isNull())
			v = json[strKey].asString();
		else
			v = "";
	}

	std::string GetStrValue(const JsonRoot & json,const std::string strKey)
	{
		if (!json[strKey].isNull())
			return json[strKey].asString();
		else
			return "";
	}

	std::string GetJsonStr(const JsonRoot & json) const
	{
		// 转换为字符串（带格式）   
		//return json.toStyledString();  
		// 输出无格式json字符串   
		Json::FastWriter writer;  
		return writer.write(json);  
	}

	std::string GetJsonFormatStr(const JsonRoot & json) const
	{
		// 转换为字符串（带格式）   
		return json.toStyledString();  
		// 输出无格式json字符串   
// 		Json::FastWriter writer;
// 		return writer.write(json);
	}
};

#endif

// 遍历相关 (数据)
// {
//     "uploadid": "UP000000",
// 		"code": "0",
// 		"msg": "",
// 		"files":
//     [
// 	{
// 		"code": "0",
//             "msg": "",
//             "filename": "1D_16-35_1.jpg",
//             "filesize": "196690",
//             "width": "1024",
//             "height": "682",
//             "images":
// 		[
// 		{
// 			"url": "fmn061/20111118",
// 				"type": "large",
// 				"width": "720",
// 				"height": "479"
// 		},
// 		{
// 				"url": "fmn061/20111118",
//                     "type": "main",
//                     "width": "200",
//                     "height": "133"
// 			}
//             ]
// 	}
//     ]
// }
// 遍历相关（解析）
// int ParseJsonFromFile(const char* filename)
// {
// 	// 解析json用Json::Reader
// 	Json::Reader reader;
// 	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...
// 	Json::Value root;       
// 	
// 	std::ifstream is;
// 	is.open (filename, std::ios::binary );  
// 	if (reader.parse(is, root))
// 	{
// 		std::string code;
// 		if (!root["files"].isNull())  // 访问节点，Access an object value by name, create a null member if it does not exist.
// 			code = root["uploadid"].asString();
// 		
// 		// 访问节点，Return the member named key if it exist, defaultValue otherwise.
// 		code = root.get("uploadid", "null").asString();
// 		
// 		// 得到"files"的数组个数
// 		int file_size = root["files"].size();
// 		
// 		// 遍历数组
// 		for(int i = 0; i < file_size; ++i)
// 		{
// 			Json::Value val_image = root["files"][i]["images"];
// 			int image_size = val_image.size();
// 			for(int j = 0; j < image_size; ++j)
// 			{
// 				std::string type = val_image[j]["type"].asString();
// 				std::string url = val_image[j]["url"].asString();
// 			}
// 		}
// 	}
// 	is.close();
// 	return 0;
// }