/********************************************************************
    创建时间:     2017/05/25  15:06:23 
    文件名:       MyStreamBase.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.数据流序列化，反序列化公共类
                2.流操作
                
    说明:        1. 支持map,vector,string，基础类型，
                2. 不支持指针序列化反序列化
*********************************************************************/
#ifndef __MYSTREAM_BASE_H_
#define __MYSTREAM_BASE_H_
#include <string>
#include <boost/smart_ptr/scoped_array.hpp>
#include <vector>
#include <map>
#include <set>

#include "../DefineBase.h"
class CMyStreamBase
{
//序列化
public:
	void OnSetBuffer(boost::scoped_array<unsigned char> & src, MY_UINT32 len);
	void OnSetBuffer(boost::scoped_array<signed char> & src, MY_UINT32 len);
	bool IsEnd();
//反序列化
public:
	MY_UINT32 Size();
	void Clear();
	
	MY_UINT32 OnGetBuffer(boost::scoped_array<unsigned char> & ptr);
	MY_UINT32 OnGetBuffer(boost::scoped_array<signed char> & ptr);
	std::vector<unsigned char> OnGetBuffer();
//序列化
public:
	template <typename T>
	CMyStreamBase & Push(const T & val);

	CMyStreamBase & Push(const std::string & val);
	template <typename T>
	CMyStreamBase & Push(const std::vector<T> &val);
	template <typename T, typename P>
	CMyStreamBase & Push(const std::map<T, P> &val);
	template <typename T>
	CMyStreamBase & Push(const std::set<T> &val);

	template<typename T>
	CMyStreamBase& operator<<(const T & roData);

//反序列化
public:
	template <typename T>
	CMyStreamBase & Pop(T & val);

	CMyStreamBase & Pop(std::string & val);

	template <typename T>
	CMyStreamBase & Pop(std::vector<T> &val);
	template <typename T, typename P>
	CMyStreamBase & Pop(std::map<T, P> &val);
	template <typename T>
	CMyStreamBase & Pop(std::set<T> &val);
	template<typename T>
	CMyStreamBase& operator>>(T & roData);

private://禁止项
	template <typename T>
	CMyStreamBase & Push(const T * val) = delete;
	template <typename T>
	CMyStreamBase & Push(T * val) = delete;
	template<typename T>
	CMyStreamBase& operator<<(const T * roData) = delete;
	template<typename T>
	CMyStreamBase& operator<<(T * roData) = delete;


	template <typename T>
	CMyStreamBase & Pop(const T * val) = delete;//error
	template <typename T>
	CMyStreamBase & Pop(T * val) = delete;//error
	template <typename T>
	CMyStreamBase & Pop(const T & val) = delete;//error
	template<typename T>
	CMyStreamBase& operator>>(T * roData)=delete;
private:
	void Memcpy(void * _Des, size_t _Size);//反序列化
	void Memcpy(const void * _Src, size_t _Size);//序列化
	std::vector<unsigned char> m_ver;
	MY_UINT32 m_indexPop = 0;
};
#endif // !__MYSTREAM_BASE_H_
