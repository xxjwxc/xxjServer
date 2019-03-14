/********************************************************************
    创建时间:     2018/10/27  16:25:19 
    文件名:       base64.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.base64编解码
                2.
                
    说明:        1. 
                2. 
*********************************************************************/

#pragma once
#include<string>

class CMyBase64
{
public:
	//编码
	static std::string Encode(const std::string& input );
	//解码
	static std::string Decode(const std::string& input);
};


