/********************************************************************
    创建时间:     2017/08/22  14:46:29 
    文件名:       MyCereal.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.本地序列化相关操作
                2.使用cereal作为基础套件
                
    说明:        1. 支持，xml，二进制，json格式
                2. 
*********************************************************************/
#ifndef MYCEREAL_H_
#define MYCEREAL_H_
#include "include/cereal/types/unordered_map.hpp"
#include "include/cereal/types/memory.hpp"
#include "include/cereal/archives/binary.hpp"
#include "include/cereal/archives/xml.hpp"
#include "include/cereal/archives/json.hpp"
#include "include/cereal/types/string.hpp"
#include <fstream>
#include <Tools/MyTools.h>


//侵入式函数每次需要调用
#define META(...) template <class Archive>\
				void serialize(Archive & ar)\
				{\
				ar(__VA_ARGS__);\
				}\

namespace CMyCereal 
{
	//加载一个本地序列化文件到内存 
	//二进制方法
	template<typename T>
	inline bool OnLoadBinary(std::string dir,T & out) {
		if (MyTools::FileIsExist(dir)) {
			try
			{
				std::ifstream is(dir, std::ios::binary);
				cereal::BinaryInputArchive archive(is);
				archive(out);
				return true;
			}
			catch (std::exception & e)
			{
				MyTools::PrintError(e.what());
			}
		}
		return false;
	}

	//保存内存到本地文件
	//二进制方法
	template<typename T>
	inline bool OnSaveBinary(std::string dir,const T & out) {
		try
		{
			std::ofstream os(dir, std::ios::binary);
			cereal::BinaryOutputArchive archive(os);
			archive(out);
			return true;
		}
		catch (std::exception & e)
		{
			MyTools::PrintError(e.what());
		}
		return false;
	}
}


#endif // !__
