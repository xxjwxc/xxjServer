#include "IAsioTools.h"

IAsioTools::IAsioTools()
{
	memset(_Data, 0, 2 * MAX_RESERVE_SIZE);
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

MY_UINT32 IAsioTools::SerializingBinary(const MsgInfo msg, array<Byte>^ _dir)
{
	//  总长度 +  msg.m_head.unMsgID + msg.m_body_len + msg.m_body
	MY_UINT32 len =sizeof(MY_UINT32) + sizeof(MY_UINT32) + sizeof(MY_UINT32) + msg.m_body_len;
	MY_UINT32 offset = 0;
	_dir = gcnew array<Byte>(len);
	char * pChar = ArrayToByte(_dir);
	Array::Clear(_dir, 0, len);
	memset(pChar, 0, len);
	writeUInt(pChar, len - sizeof(MY_UINT32), offset);
	writeUInt(pChar +offset, msg.m_head.unMsgID, offset);
	writeUInt(pChar + offset, msg.m_body_len,offset );
	writeArray(pChar + offset, ArrayToByte(msg.m_body), msg.m_body_len, offset);
	return len;
}

std::string IAsioTools::Serializing(const std::string & str)
{
	char _str[MAX_HEAD_LEN + 1];
	memset(_str, 0, MAX_HEAD_LEN + 1);
//	sprintf_s(_str, MAX_HEAD_STR, str.length());
#ifdef _WIN32
	sprintf_s(_str, MAX_HEAD_LEN + 1, MAX_HEAD_STR, str.length());
#else
	sprintf(_str, MAX_HEAD_STR, str.length());
#endif // _WIN32
	return std::string(_str) + str;
}

std::vector<MsgInfo> IAsioTools::UnSerializingBinary(const char * data, int size)
{
	vector<MsgInfo> ver;
	memcpy(_Data + _len, data, size);
	_len += size;
	int f = 0, temp = 0;
	MY_UINT32 _sLen = 0;

	while (true)//开始拆分查找
	{
		if (_len - f < MAX_HEAD_LEN)//不够长度了
		{
			memcpy(_Data, _Data + f, _len - f);
			//memset(_Data+_len-f,0,2*MAX_RESERVE_SIZE-_len+f);
			_len = _len - f;
			break;
		}
		else
		{
			Rmemcpy(&_sLen, _Data + f, MAX_HEAD_LEN);
			if (_len - (f + MAX_HEAD_LEN) < _sLen)//不满足要求了
			{
				memcpy(_Data, _Data + f, _len - f);
				//memset(_Data+_len-f,0,2*MAX_RESERVE_SIZE-_len+f);
				_len = _len - f;
				break;
			}
			else//可以找到一个
			{
				if (_sLen > 0)
				{
					array<Byte>^ _buffer = gcnew array<Byte>(_sLen);
					Array::Clear(_buffer, 0, _sLen);
					char * pChar = ArrayToByte(_buffer);
					memcpy(pChar, _Data + f + MAX_HEAD_LEN, _sLen);
					MsgInfo info;
					if (UnSerializingMsg(_buffer, _sLen, %info))
					{
						ver.push_back(info);
					}
					f += MAX_HEAD_LEN + _sLen;
				}

			}
		}
	}

	return ver;
}

bool IAsioTools::UnSerializingMsg(array<Byte>^ _src, MY_UINT32 size, MsgInfo^ info)
{
	char * pChar = ArrayToByte(_src);
	if (size <= sizeof(MY_UINT32))
	{
		assert(false && "UnSerializingMsg error.");
		return false;
	}
	MY_UINT32 offset = 0;
	info->Clear();
	ReadUInt((info->m_head.unMsgID), pChar, offset);//type
	info->m_body_len = size - sizeof(MY_UINT32);
	info->m_body = gcnew array<Byte>(info->m_body_len);
	ReadArray(ArrayToByte(info->m_body), pChar + offset,info->m_body_len,offset);

	assert(offset <= size);
	return true;
}

char * IAsioTools::ArrayToByte(array<System::Byte>^ byteArray)
{
	pin_ptr<System::Byte> p = &byteArray[0];
	unsigned char* pby = p;
	char* pch = reinterpret_cast<char*>(pby);
	return pch;
}

vector<std::string> IAsioTools::UnSerializingString(const char * data, int size)
{
	vector<std::string> ver;
	if (!data)return ver;

	//assert(_len1 + size < 2*MAX_RESERVE_SIZE);
	memcpy(_Data + _len, data, size);
	_len += size;
	int f = 0, temp = 0;
	char _str[MAX_HEAD_LEN + 1];

	while (true)//开始拆分查找
	{
		if (_len - f < MAX_HEAD_LEN)//不够长度了
		{
			memcpy(_Data, _Data + f, _len - f);
			memset(_Data + _len - f, 0, 2 * MAX_RESERVE_SIZE - _len + f);
			_len = _len - f;
			break;
		}
		else
		{
			memset(_str, 0, MAX_HEAD_LEN + 1);
			memcpy(_str, _Data + f, MAX_HEAD_LEN);
			int len = atoi(_str);
			if (_len - (f + MAX_HEAD_LEN) < len)//不满足要求了
			{
				memcpy(_Data, _Data + f, _len - f);
				memset(_Data + _len - f, 0, 2 * MAX_RESERVE_SIZE - _len + f);
				_len = _len - f;
				break;
			}
			else//可以找到一个
			{
				ver.push_back(MyTools::GetGB2312(std::string(_Data + f + MAX_HEAD_LEN, len)));
				f += MAX_HEAD_LEN + len;
			}
		}
	}

	return ver;
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
	memcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::ReadUInt(void * _dir, const void * _src, MY_UINT32 & offset)
{
	Rmemcpy(_dir, _src, sizeof(MY_UINT32));
	offset += sizeof(MY_UINT32);
}

void IAsioTools::ReadUInt(MY_UINT32^ _dir, const void * _src, MY_UINT32 & offset)
{
	ReadUInt(&_dir,_src,offset);
}

void IAsioTools::ReadArray(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	Rmemcpy(_dir, _src, len);
	offset += len;
}

void IAsioTools::ReadString(void * _dir, const void * _src, MY_UINT32 len, MY_UINT32 & offset)
{
	memcpy(_dir, _src, len);
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
