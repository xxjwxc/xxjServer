/********************************************************************
    创建时间:     2017/05/26  10:52:25 
    文件名:       MyStreamBase.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.
                2.
                
    说明:        1. 测试例子


	MY_UINT32 test = 10;
	bool b = true;
	std::vector<MY_UINT32> ver;
	ver.push_back(123);
	std::string str = MyTools::GetUtf8(std::string("我们"));
	std::map<std::string, std::string> map;
	map["aaa"] = "bbb";
	//序列化
	CMyStreamBase ostream;
	ostream<< map << str << b;
	// 	ostream.Push(str);
	// 	ostream.Push(b);
	ostream.Push(test).Push(ver);
	//ostream.Push(&map);

	//const auto & ver1 = ostream.OnGetBuffer();
	boost::scoped_array<unsigned char> ptr;
	int len = ostream.OnGetBuffer(ptr);

	//反序列化
	MY_UINT32 test1 = 0;
	bool b1 = false;
	std::vector<MY_UINT32> ver1;
	std::string str1 = "";
	std::map<std::string, std::string> map1;
	ostream.OnSetBuffer(ptr, len);
	ostream>>map1>>str1>>b1;
	ostream.Pop(test1).Pop(ver1);
	str1 = MyTools::GetGB2312(str1);
	bool bb = ostream.IsEnd();

*********************************************************************/
#include "MyStreamBase.h"


template <typename T>
CMyStreamBase & CMyStreamBase::Push(const T & val)
{
	Memcpy((const T *)&val, sizeof(T));
	return *this;
}

template <typename T>
CMyStreamBase & CMyStreamBase::Pop(T & val)
{
	Memcpy((T *)&val, sizeof(T));
	return *this;
}

/////////////////////////////////////////////////////////
///delete/////////////////////////////////////////////////
// 
// template <typename T>
// CMyStreamBase & CMyStreamBase::Push(const T * val)
// {
// 	assert(false && "not support point");
// 	return *this;
// }
// 
// template <typename T>
// CMyStreamBase & CMyStreamBase::Pop(const T * val)
// {
// 	assert(false && "not support point");
// 	return *this;
// }
// 
// template<typename T>
// CMyStreamBase& CMyStreamBase::operator<<(const T * roData)
// {
// 	assert(false && "not support point");
// 	return *this;
// }
// 
// 
// template<typename T>
// CMyStreamBase& CMyStreamBase::operator<<(T * roData)
// {
// 	assert(false && "not support point");
// 	return *this;
// }
// 
// template<typename T>
// CMyStreamBase& CMyStreamBase::operator>>(T * roData)
// {
// 	assert(false && "not support point");
// 	return *this;
// }
//
// template <typename T>
// CMyStreamBase & CMyStreamBase::Push(T * val)
// {
// 	return Push((const T *)val);
// }
// 
// 
// template <typename T>
// CMyStreamBase & CMyStreamBase::Pop(T * val)
// {
// 	return Pop((const T *)val);
// }
// 
// template <typename T>
// CMyStreamBase & CMyStreamBase::Pop(const T & val)
// {
// 	return Pop((const T *)&val);
// }
////////////////////////////////////////////
//序列化

template <typename T>
CMyStreamBase & CMyStreamBase::Push(const std::vector<T> &val)
{
	size_t len = val.size();
	Push(len);
	for (const auto & kvp : val)
	{
		Push(kvp);
	}

	return *this;
}


template <typename T, typename P>
CMyStreamBase & CMyStreamBase::Push(const std::map<T, P> &val)
{
	size_t len = val.size();
	Push(len);
	for (const auto & kvp : val)
	{
		Push(kvp.first);
		Push(kvp.second);
	}
	return *this;
}

template <typename T>
CMyStreamBase & CMyStreamBase::Push(const std::set<T> &val)
{
	MY_UINT32 len = val.size();
	Push(len);
	for (const auto & kvp : val)
	{
		Push(kvp);
	}
	return *this;
}

template<typename T>
CMyStreamBase& CMyStreamBase::operator<<(const T& roData)
{
	return Push(roData);
}

///////////////////////////////////////////

////////////////////////////////////////////
//反序列化
template <typename T>
CMyStreamBase & CMyStreamBase::Pop(std::vector<T> &val)
{
	val.clear();
	MY_UINT32 len = 0;
	Pop(len);
	T temp;
	for (size_t i=0;i<len;i++)
	{
		Pop(temp);
		val.push_back(temp);
	}

	return *this;
}


template <typename T, typename P>
CMyStreamBase & CMyStreamBase::Pop(std::map<T, P> &val)
{
	val.clear();
	MY_UINT32 len = 0;
	Pop(len);
	T key;
	P value;
	for (size_t i = 0; i < len; i++)
	{
		Pop(key);
		Pop(value);

		val[key] = value;
	}
	
	return *this;
}


template <typename T>
CMyStreamBase & CMyStreamBase::Pop(std::set<T> &val)
{
	val.clear();
	MY_UINT32 len = 0;
	Pop(len);
	T temp;
	for (int i = 0; i < len; i++)
	{
		Pop(temp);
		val.insert(temp);
	}

	return *this;
}


template<typename T>
CMyStreamBase& CMyStreamBase::operator>>(T & roData)
{
	return Pop(roData);
}

////////////////////////////////////////////