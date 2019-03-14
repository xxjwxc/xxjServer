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

generic <typename T> where T : gcnew()
ref class MySingleton
{
public:
	static T Instance()
	{
		return instance;
	}
protected:
	MySingleton()
	{
		assert(false&&"not allowed init from base config");//不允许构造
	};
	~MySingleton(){ };
private:
	static T instance = gcnew T;
	
};

/*
//定义 静态变量
#define DECLARE_SINGLETON_MEMBER(_Ty)\
	template <> boost::shared_ptr<_Ty> MySingleton<_Ty>::_instance;
*/


#endif