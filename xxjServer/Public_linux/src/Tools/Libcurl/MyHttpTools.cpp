#include "MyHttpTools.h"
#include "curl/curl.h"
#include <iostream>
#include <algorithm>


size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	char * p = (char *)buffer;
	std::string *fptr = (std::string*)userp;
	fptr->append(p, nmemb);
	return nmemb;
}


CMyHttpTools::CMyHttpTools()
{
}

CMyHttpTools::~CMyHttpTools()
{
}

std::tuple<bool, std::string> CMyHttpTools::OnOpenHttp(const std::string & url, const std::string & strFields, bool isPost, size_t timeOutSecond)
{
	CURL *curl;
	CURLcode res;
	std::string strBack;
	std::string postFields = strFields;
	//struct curl_slist *http_header = NULL;
	std::string str_url = url;
	if (!isPost && postFields.length()>0)
	{
		str_url.append("?").append(postFields);
	}

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());//URL地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());//POST参数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//对返回的数据进行操作的函数地址
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBack);//设置WRITEFUNCTION的第四个参数值
	curl_easy_setopt(curl, CURLOPT_POST, isPost?1:0);//设置为非0表示本次操作为POST
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//设置为非0在执行时打印请求信息
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);//设置为非0将响应头信息同响应体一起传给WRITEFUNCTION
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);//设置为非0，响应头信息Location
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/token/curlpostLogin.cookie");//设置对应的COOKIEFILE路径，该路径文件并不一定需要在物理磁盘上实际存在
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeOutSecond);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	if (res != CURLE_OK)
	{
#ifdef _DEBUG
		switch (res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			std::cerr << "不支持的协议,由URL的头部指定\n";
			//fprintf(stderr, "不支持的协议,由URL的头部指定\n");
		case CURLE_COULDNT_CONNECT:
			std::cerr << "不能连接到remote主机或者代理\n";
		case CURLE_HTTP_RETURNED_ERROR:
			std::cerr << "http返回错误\n";
		case CURLE_READ_ERROR:
			std::cerr << "读本地文件错误\n";
		default:
			std::cerr << "返回值:" << res << std::endl;
		}
#endif
		return std::tuple<bool,std::string>(false, "");
	}

	return std::tuple<bool, std::string>(true, strBack);
}

std::tuple<bool, std::string> CMyHttpTools::OnOpenHttps(const std::string & url, const std::string & strFields, bool isPost, size_t timeOutSecond)
{
	CURL *curl;
	CURLcode res;
	std::string strBack;
	std::string postFields = strFields;
	//struct curl_slist *http_header = NULL;
	std::string str_url = url;
	if (!isPost&& postFields.length()>0)
	{
		str_url.append("?").append(postFields);
	}

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // 跳过证书检查  
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);  // 从证书中检查SSL加密算法是否存在  
	curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());//URL地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());//POST参数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//对返回的数据进行操作的函数地址
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBack);//设置WRITEFUNCTION的第四个参数值
	curl_easy_setopt(curl, CURLOPT_POST, isPost ? 1 : 0);//设置为非0表示本次操作为POST
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//设置为非0在执行时打印请求信息
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);//设置为非0将响应头信息同响应体一起传给WRITEFUNCTION
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);//设置为非0，响应头信息Location
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/token/curlpostLogin.cookie");//设置对应的COOKIEFILE路径，该路径文件并不一定需要在物理磁盘上实际存在
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeOutSecond);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	if (res != CURLE_OK)
	{
#ifdef _DEBUG
		switch (res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			std::cerr << "不支持的协议,由URL的头部指定\n";
			//fprintf(stderr, "不支持的协议,由URL的头部指定\n");
		case CURLE_COULDNT_CONNECT:
			std::cerr << "不能连接到remote主机或者代理\n";
		case CURLE_HTTP_RETURNED_ERROR:
			std::cerr << "http返回错误\n";
		case CURLE_READ_ERROR:
			std::cerr << "读本地文件错误\n";
		default:
			std::cerr << "返回值:" << res << std::endl;
		}
#endif
		return std::tuple<bool, std::string>(false, "");
	}

	return std::tuple<bool, std::string>(true, strBack);
}

std::tuple<bool, std::string> CMyHttpTools::OnOpenHttpByjson(const std::string & url, const std::string & strJson, bool isPost, size_t timeOutSecond)
{
	CURL *curl;
	CURLcode res;
	std::string strBack;
	std::string postFields = strJson;
	//struct curl_slist *http_header = NULL;
	std::string str_url = url;
	if (!isPost && postFields.length() > 0)
	{
		str_url.append("?").append(postFields);
	}

	//curl = curl_slist_append(nullptr,"Content-Type:application/json;charset=UTF-8");
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, str_url.c_str());//URL地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());//POST参数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//对返回的数据进行操作的函数地址
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strBack);//设置WRITEFUNCTION的第四个参数值
	curl_easy_setopt(curl, CURLOPT_POST, isPost ? 1 : 0);//设置为非0表示本次操作为POST
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);//设置为非0在执行时打印请求信息
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);//设置为非0将响应头信息同响应体一起传给WRITEFUNCTION
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);//设置为非0，响应头信息Location
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/token/curlpostLogin.cookie");//设置对应的COOKIEFILE路径，该路径文件并不一定需要在物理磁盘上实际存在
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeOutSecond);
	curl_slist *plist = curl_slist_append(NULL,"Content-Type:application/json;charset=UTF-8");// 设置http发送的内容类型为JSON 
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
	res = curl_easy_perform(curl);
	curl_slist_free_all(plist);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	
	if (res != CURLE_OK)
	{
#ifdef _DEBUG
		switch (res)
		{
		case CURLE_UNSUPPORTED_PROTOCOL:
			std::cerr << "不支持的协议,由URL的头部指定\n";
			//fprintf(stderr, "不支持的协议,由URL的头部指定\n");
		case CURLE_COULDNT_CONNECT:
			std::cerr << "不能连接到remote主机或者代理\n";
		case CURLE_HTTP_RETURNED_ERROR:
			std::cerr << "http返回错误\n";
		case CURLE_READ_ERROR:
			std::cerr << "读本地文件错误\n";
		default:
			std::cerr << "返回值:" << res << std::endl;
		}
#endif
		return std::tuple<bool, std::string>(false, "");
	}

	return std::tuple<bool, std::string>(true, strBack);
}
