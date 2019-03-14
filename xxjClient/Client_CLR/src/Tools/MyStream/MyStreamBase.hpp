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
#ifndef __MYSTREAM_BASE_HPP_
#define __MYSTREAM_BASE_HPP_
#include "MyStreamBase.h"

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Push(T val)
{
	if (val->GetType() == System::String::typeid)//字符串
	{
		return PushString((string ^)val);
	}
	else {
		return Push((const void *)&val, sizeof(T));
	}

}

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Push(PTR(array<T>) val)
{
	MY_UINT32 len = val->Length;
	Push(len);
	for (size_t i=0;i<len;i++)
	{
		Push(val[i]);
	}

	return this;
}

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Push(PTR(List<T>) val)
{
	MY_UINT32 len = val->Count;
	Push(len);
	for (size_t i = 0; i < len; i++)
	{
		Push(val[i]);
	}

	return this;
}



generic<typename T, typename P>
PTR(CMyStreamBase) CMyStreamBase::Push(Dictionary<T, P>^ val)
{
	MY_UINT32 len = val->Count;
	
	Push(len);
	for each (KeyValuePair<T, P> kvp in val)
	{
		Push(kvp.Key);
		Push(kvp.Value);
	}

// 	List<T> keys = val->Keys;
// 	for (size_t i =0;i<len;i++)
// 	{
// 		Push(keys[i]);
// 		Push(val[keys[i]]);
// 	}

	return this;
}

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::PopString(REF(T) val)
{
	assert(val->GetType() == System::String::typeid);
	MY_UINT32 len = 0;
	Pop(len);
	array<Byte>^ bts = gcnew array<Byte>(len);
	for (int i = 0; i != bts->Length; i++)
	{
		Pop(bts[i]);
	}

	val = safe_cast<T>(System::Text::Encoding::UTF8->GetString(bts, 0, bts->Length));

	return this;
}

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Pop(REF(T) val)
{
	if (val->GetType() == System::String::typeid)//字符串
	{
		return PopString(val);
// 		MY_UINT32 len = 0;
// 		Pop(len);
// 		array<Byte>^ bts = gcnew array<Byte>(len);
// 		for (int i = 0; i != bts->Length; i++)
// 		{
// 			Pop(bts[i]);
// 		}
// 
// 		val = safe_cast<T>(System::Text::Encoding::UTF8->GetString(bts, 0, bts->Length));

		return this;
	}
	else {
		T temp;
		Memcpy((void *)&temp, sizeof(T));
		val = temp;
	}

	return this;
}

generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Pop(REF(PTR(array<T>)) val)
{
	MY_UINT32 len = 0;
	Pop(len);
	val = gcnew array<T>(len);
	T temp;
	for (int i = 0; i < len; i++)
	{
		Pop(temp);
		val[i] = temp;
	}

	return this;
}


generic<typename T>
PTR(CMyStreamBase) CMyStreamBase::Pop(REF(PTR(List<T>)) val)
{
	val->Clear();

	MY_UINT32 len = 0;
	Pop(len);
	T temp;
	for (int i = 0; i < len; i++)
	{
		Pop(temp);
		val->Add(temp);
	}

	return this;
}

generic<typename T,typename P>
PTR(CMyStreamBase) CMyStreamBase::Pop(Dictionary<T, P>^ % val)
{
	val->Clear();
	MY_UINT32 len = 0;
	Pop(len);
	T key;
	P value;
	if (T::typeid == System::String::typeid)
		key = safe_cast<T>(gcnew string(""));
	else 
		key = safe_cast<T>(System::Activator::CreateInstance(T::typeid));

	if (P::typeid == System::String::typeid)
		value = safe_cast<P>(gcnew string(""));
	else
		value = safe_cast<P>(System::Activator::CreateInstance(P::typeid));

	for (int i = 0; i < len; i++)
	{
		Pop(key);
		Pop(value);

		val[key] = value;
	}

	return this;
}


#endif // !__MYSTREAM_BASE_H_
