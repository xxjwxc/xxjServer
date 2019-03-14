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
//#include <cliext/vector>

#include "../ClientTypeDefine.h"

public ref class CMyStreamBase
{
public:
	CMyStreamBase();
//序列化
public:
	void OnSetBuffer(PTR(array<Byte>) src, MY_UINT32 len);
	bool IsEnd();

	MY_UINT32 OnGetBuffer(REF(PTR(array<Byte>)) ptr);
	PTR(array<Byte>) OnGetBuffer();

//反序列化
public:
	MY_UINT32 Size();
	void Clear();
	//序列化
public:
	PTR(CMyStreamBase) Push(const void * _Src, size_t _Size);

	PTR(CMyStreamBase) PushString(PTR(string) val);

	generic<typename T>
	PTR(CMyStreamBase) Push(T val);

	generic<typename T>
	PTR(CMyStreamBase) Push(PTR(array<T>) val);//对应vector

	generic<typename T>
	PTR(CMyStreamBase) Push(PTR(List<T>) val);//对应Set

	generic<typename T,typename P>
	PTR(CMyStreamBase) Push(Dictionary<T,P>^ val);//对应map
//反序列化
public:
	PTR(CMyStreamBase) Pop(void * _Src, size_t _Size);

	generic<typename T>
	PTR(CMyStreamBase) Pop(REF(T) val);

	generic<typename T>
	PTR(CMyStreamBase) PopString(REF(T) val);

	generic <typename T>
	PTR(CMyStreamBase) Pop(REF(PTR(array<T>)) val);//对应vector

	generic<typename T>
	PTR(CMyStreamBase) Pop(REF(PTR(List<T>)) val);//对应Set

	generic<typename T, typename P>
	PTR(CMyStreamBase) Pop(Dictionary<T, P>^ % val);//对应map

private:
	void Memcpy(void * _Des, size_t _Size);//反序列化
	void Memcpy(const void * _Src, size_t _Size);//序列化
	PTR(List<Byte>) m_ver;
	MY_UINT32 m_indexPop = 0;
};



#endif // !__MYSTREAM_BASE_H_
