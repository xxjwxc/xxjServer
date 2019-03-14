/********************************************************************
    创建时间：    2016/11/20  13:57:08 
    文件名：      CMyHttpTools.h
    作者：        谢小军
    
    功能：        1.httpTools
                 2.http工具
                
    说明：        1. 单类调用
                 2. 
*********************************************************************/
#ifndef MYHTTPTOOLS_H_
#define MYHTTPTOOLS_H_
#include <string>
#include <tuple>

class CMyHttpTools
{
public:
	CMyHttpTools();
	~CMyHttpTools();
public:
	/**
	* 请求一个url并返回值(同步，阻塞方式)
	* 参数:
	* url:请求的地址
	* strFields:请求的参数,多个参数以&分割:如:A=a&B=b&C=c
	* isPost:是否是post请求，false为get请求
	*返回值:
	*tuple<0> :请求是否正确完成
	*tuple<1> :当请求正确完整之后返回的内容
	*/
	std::tuple<bool, std::string> OnOpenHttp(const std::string & url, const std::string & strFields = "",bool isPost = true,size_t timeOutSecond = 60L );
	/**
	* 请求一个安全https内容并返回值(同步，阻塞方式)
	* 参数:
	* url:请求的地址
	* strFields:请求的参数,多个参数以&分割:如:A=a&B=b&C=c
	* isPost:是否是post请求，false为get请求
	*返回值:
	*tuple<0> :请求是否正确完成
	*tuple<1> :当请求正确完整之后返回的内容
	*/
	std::tuple<bool, std::string> OnOpenHttps(const std::string & url, const std::string & strFields = "", bool isPost=true, size_t timeOutSecond = 60L);

	/** 
	* 以rest json 方式发送数据
	* 参数:
	* url:请求的地址
	* strFields:请求的参数,json字符串
	* isPost:是否是post请求，false为get请求
	*返回值:
	*tuple<0> :请求是否正确完成
	*tuple<1> :当请求正确完整之后返回的内容
	*/
	std::tuple<bool, std::string> OnOpenHttpByjson(const std::string & url, const std::string & strJson = "{}", bool isPost = true, size_t timeOutSecond = 60L);


};


#endif
