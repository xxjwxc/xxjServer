#include "IAsioTools.h"
//#include <memory.h>
//#include <stdlib.h>


IAsioTools::IAsioTools()
{
	MyTools::memset(_Data, 0, 2 * MAX_RESERVE_SIZE);
	_len = 0;
}

// std::string CMyAsioTools::Serializing(const std::string & str )
// {
// 	char _str[MAX_HEAD_LEN +1];
// 	memset(_str,0,MAX_HEAD_LEN +1);
// 	sprintf_s(_str,MAX_HEAD_STR,str.length());
// 	return std::string(_str) + str;
// }

IAsioTools::~IAsioTools()
{
}

MY_UINT32 IAsioTools::SerializingBinary(const MsgInfo^ msg,array<Byte>^&  _dir)
{
					//  总长度 +  msg.m_head.unMsgID + msg.m_body_len + msg.m_body
	MY_UINT32 len = sizeof(MY_UINT32) + sizeof(MY_UINT32) + sizeof(MY_UINT32) + msg->m_body_len;
	MY_UINT32 offset = 0;
	_dir = gcnew array<Byte>(len);
	char * pChar = MyTools::ArrayToChar(_dir);
	Array::Clear(_dir, 0, len);
	MyTools::memset(pChar, 0, len);
	writeUInt(pChar, len - sizeof(MY_UINT32), offset);
	writeUInt(pChar + offset, msg->m_head.unMsgID, offset);
	writeUInt(pChar + offset, msg->m_body_len, offset);
	writeArray(pChar + offset, MyTools::ArrayToChar(msg->m_body), msg->m_body_len, offset);
	return len;
}

string^ IAsioTools::Serializing(string^ str)
{
	char _str[MAX_HEAD_LEN + 1];
	MyTools::memset(_str, 0, MAX_HEAD_LEN + 1);
//	sprintf_s(_str, MAX_HEAD_STR, str.length());
#ifdef _WIN32
	sprintf_s(_str, MAX_HEAD_LEN + 1, MAX_HEAD_STR, (str->Length));
#else
	sprintf(_str, MAX_HEAD_STR, str.length());
#endif // _WIN32
	std::string^ stt = gcnew std::string(_str);
	stt = stt->Insert(stt->Length,str);
	return  stt;
}

array<MsgInfo^>^ IAsioTools::UnSerializingBinary(const char * data, int size)
{
	List<MsgInfo^>^ ver = gcnew List<MsgInfo^>();
	MyTools::memcpy(_Data + _len, data, size);
	_len += size;
	int f = 0, temp = 0;
	MY_UINT32 _sLen = 0;

	if (_len > 2*MAX_RESERVE_SIZE)
	{
		throw gcnew  System::SystemException("buffer not enough");
		return nullptr;
	}
	while (true)//开始拆分查找
	{
		if (_len - f < MAX_HEAD_LEN)//不够长度了
		{
			MyTools::memcpy(_Data, _Data + f, _len - f);
			//memset(_Data+_len-f,0,2*MAX_RESERVE_SIZE-_len+f);
			_len = _len - f;
			break;
		}
		else
		{
			Rmemcpy(&_sLen, _Data + f, MAX_HEAD_LEN);
			if (_len - (f + MAX_HEAD_LEN) < _sLen)//不满足要求了
			{
				MyTools::memcpy(_Data, _Data + f, _len - f);
				//memset(_Data+_len-f,0,2*MAX_RESERVE_SIZE-_len+f);
				_len = _len - f;
				break;
			}
			else//可以找到一个
			{
				if (_sLen > 0)
				{
					array<Byte>^ _buffer = gcnew array<Byte>(_sLen);
					
					for (size_t i=0;i<_sLen;i++)
					{
						_buffer[i] = _Data[f + MAX_HEAD_LEN + i];
					}
// 
// 					Array::Clear(_buffer, 0, _sLen);
// 					char * pChar = MyTools::ArrayToChar(_buffer);
// 					MyTools::memcpy(pChar, _Data + f + MAX_HEAD_LEN, _sLen);
					MsgInfo^ info = gcnew MsgInfo();
					if (UnSerializingMsg(_buffer, _sLen, info))
					{
						ver->Add(info);
					}
					f += MAX_HEAD_LEN + _sLen;
				}

			}
		}
	}

	return ver->ToArray();
}

bool IAsioTools::UnSerializingMsg(array<Byte>^ _src, MY_UINT32 size, MsgInfo^ & info)
{
	if (size <= sizeof(MY_UINT32))
	{
		assert(false && "UnSerializingMsg error.");
		return false;
	}
	MY_UINT32 offset = 0;
	info->Clear();
	char * pChar = MyTools::ArrayToChar(_src);
	ReadUInt((info->m_head.unMsgID), pChar, offset);//type
	ReadUInt((info->m_body_len), pChar + offset, offset);//type
	//info->m_body_len = size - sizeof(MY_UINT32);
	info->m_body =gcnew array<Byte>(info->m_body_len);
	//ReadArray(MyTools::ArrayToChar(info->m_body), pChar + offset,info->m_body_len,offset);
	ReadArray(info->m_body, pChar + offset, info->m_body_len, offset);

	assert(offset <= size);
	return true;

}

array<std::string^>^ IAsioTools::UnSerializingString(const char * data, int size)
{
	List<std::string^>^ ver = gcnew List<std::string^>();
	if (!data)return ver->ToArray();

	//assert(_len1 + size < 2*MAX_RESERVE_SIZE);
	MyTools::memcpy(_Data + _len, data, size);
	_len += size;
	int f = 0, temp = 0;
	char _str[MAX_HEAD_LEN + 1];

	while (true)//开始拆分查找
	{
		if (_len - f < MAX_HEAD_LEN)//不够长度了
		{
			MyTools::memcpy(_Data, _Data + f, _len - f);
			MyTools::memset(_Data + _len - f, 0, 2 * MAX_RESERVE_SIZE - _len + f);
			_len = _len - f;
			break;
		}
		else
		{
			MyTools::memset(_str, 0, MAX_HEAD_LEN + 1);
			MyTools::memcpy(_str, _Data + f, MAX_HEAD_LEN);
			int len = MyTools::atoi(_str);
			if (_len - (f + MAX_HEAD_LEN) < len)//不满足要求了
			{
				MyTools::memcpy(_Data, _Data + f, _len - f);
				MyTools::memset(_Data + _len - f, 0, 2 * MAX_RESERVE_SIZE - _len + f);
				_len = _len - f;
				break;
			}
			else//可以找到一个
			{
				std::string^ str = gcnew std::string(_Data + f + MAX_HEAD_LEN);
				ver->Add(str);
				f += MAX_HEAD_LEN + len;
			}
		}
	}

	return ver->ToArray();
}


void IAsioTools::Clear()
{
	_len = 0;
}

void IAsioTools::writeUInt(void * _dir, MY_UINT32 v, MY_UINT32 & offset)
{
	Rmemcpy(_dir, &v, sizeof(MY_UINT32));
	offset += sizeof(MY_UINT32);
}

void IAsioTools::writeArray(void * _dir, char * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	Rmemcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::writeInt(void * _dir, int v, MY_UINT32 & offset)
{
	Rmemcpy(_dir, &v, sizeof(int));
	offset += sizeof(int);
}

void IAsioTools::writeString(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	MyTools::memcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::ReadUIntP(void * _dir, const void * _src, MY_UINT32 & offset)
{
	Rmemcpy(_dir, _src, sizeof(MY_UINT32));
	offset += sizeof(MY_UINT32);
}

void IAsioTools::ReadUInt(MY_UINT32 % _dir, const void * _src, MY_UINT32 & offset)
{
	MY_UINT32 dir = 0;
	ReadUIntP(&dir, _src, offset);
	_dir = dir;
}

void IAsioTools::ReadArray(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	Rmemcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::ReadArray(array<Byte>^ _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	const char *  src = (const char *)_src;
	for (unsigned i = 0; i < len; i++)
	{
		_dir[i] = src[len - i - 1];
	}
	offset += len;
}

void IAsioTools::ReadString(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	MyTools::memcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::Rmemcpy(void * _Dst, const void * _Src, size_t _Size)
{
	char * dst = (char *)_Dst;
	const char *  src = (const char *)_Src;
	for (unsigned i = 0; i < _Size; i++)
	{
		dst[i] = src[_Size - i - 1];
	}
}
