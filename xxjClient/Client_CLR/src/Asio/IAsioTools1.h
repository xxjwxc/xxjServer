#ifndef __IASIOTOOLS_H_
#define __IASIOTOOLS_H_
#include <vector>
//#include <boost/smart_ptr/scoped_array.hpp>
#include "../Tools/ClientTypeDefine.h"
#include "../data/GameDefine.h"

#define MAX_RESERVE_SIZE 1024*8  //最大接收尺寸
#define MAX_HEAD_LEN sizeof(MY_UINT32)  
#define MAX_HEAD_STR "%04d" 

class IAsioTools
{
public:
	IAsioTools();
	~IAsioTools();
public://二进制
	virtual MY_UINT32 SerializingBinary(const MsgInfo msg, array<Byte>^ _dir);//序列化
	virtual std::vector<MsgInfo> UnSerializingBinary(const char * data, int size);//反序列化 二进制
public://字符串
	std::string Serializing(const std::string & str);//序列化
	std::vector<std::string> UnSerializingString(const char * data, int size);//反序列化 字符串

protected:
	char _Data[2 * MAX_RESERVE_SIZE];
	int _len;

protected:
	void writeUInt(void * _dir, MY_UINT32 v, MY_UINT32 & offset);
	void writeArray(void * _dir, char * _src,MY_UINT32 len, MY_UINT32 & offset);
	void writeInt(void * _dir, int v, MY_UINT32 & offset);
	void writeString(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset);

	void ReadUInt(void * _dir, const void * _src, MY_UINT32 & offset);
	void ReadUInt(MY_UINT32^ _dir, const void * _src, MY_UINT32 & offset);
	void ReadArray(void * _dir, const void * _src,MY_UINT32 len, MY_UINT32 & offset);
	void ReadString(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset);
	void Rmemcpy(void * _Dst, const void * _Src, size_t _Size);
private:
	virtual bool UnSerializingMsg(array<Byte>^ _src, MY_UINT32 size, MsgInfo^ info);
	char * ArrayToByte(array<System::Byte>^ byteArray);

};



#endif