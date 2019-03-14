/********************************************************************
	创建时间：	2014/08/07  11:35:23 
	文件名：    	MySingleton.h
	作者：		谢小军
	
	功能：		1.
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef MYSINGLETION_H_
#define MYSINGLETION_H_
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>

template <class T>
class MySingleton : boost::noncopyable
{
public:
	//获取类的唯一实例
	static inline boost::shared_ptr<T> instance()
	{
		boost::call_once(&MySingleton::Init, _once);

		return _instance;
	}

	static void Init()
	{
		//if (CMyIni::isLog)
		//	std::cout<<"Init()\n";
		_isNew = true;
		_instance = boost::shared_ptr<T>(new T);
		_isNew = false;
		_instance->OnInit();
	}

	//全局初始化唯一实例
	virtual void OnInit() {}

	//释放类的唯一实例
	static void destroy()
	{
		if (!_instance)
			return;

		_instance.reset();
	}
protected:
	MySingleton()
	{
		if (!_isNew)
			assert(false&&"not allowed init from base config");//不允许构造
	};
	~MySingleton(){ destroy(); };

	static boost::shared_ptr<T> _instance;
	static boost::once_flag _once;
	static bool _isNew;
	
};

template <class T>
bool MySingleton<T>::_isNew = false;

template<typename T>
boost::once_flag MySingleton<T>::_once = BOOST_ONCE_INIT;
template<typename T>
boost::shared_ptr<T> MySingleton<T>::_instance;

/*
//定义 静态变量
#define DECLARE_SINGLETON_MEMBER(_Ty)\
	template <> boost::shared_ptr<_Ty> MySingleton<_Ty>::_instance;
*/


#endif