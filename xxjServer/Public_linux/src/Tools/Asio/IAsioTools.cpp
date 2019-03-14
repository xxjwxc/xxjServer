#include "IAsioTools.h"
#include "../MyTools.h"
#include <stdexcept>

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

MY_UINT32 IAsioTools::SerializingBinary(const MsgInfo & msg, boost::scoped_array<char> & _dir)
{
					//  总长度 +  msg.m_head.unMsgID + msg.m_body_len + msg.m_body
	MY_UINT32 len =sizeof(MY_UINT32) + sizeof(MY_UINT32) + sizeof(MY_UINT32) + msg.m_body_len;
	MY_UINT32 offset = 0;
	_dir.reset(new char[len]);
	return SerializingBinary(msg, _dir.get());

// 	memset(_dir.get(), 0, len);
// 	writeUInt(_dir.get(), len - sizeof(MY_UINT32), offset);
// 	writeUInt(_dir.get()+offset, msg.m_head.unMsgID, offset);
// 	writeUInt(_dir.get() + offset, msg.m_body_len,offset );
// 	writeArray(_dir.get() + offset, msg.m_body.get(), msg.m_body_len, offset);
// 	return len;

// 	switch (msg.m_head.unMsgID)
// 	{
		// 	case MSG_SEND_RESULT_ID://返回消息
		// 		{
		// 			len = sizeof(MY_UINT32) + sizeof(MsgBaseHead);
		// 			_dir.reset(new char[len]);
		// 			memset(_dir.get(),0,len);
		// 			writeUInt(_dir.get()+offset,len - sizeof(MY_UINT32),offset);
		// 			//memcpy(_dir.get(),&len,sizeof(MY_UINT32));
		// 			writeUInt(_dir.get()+offset,msg.m_head.unMsgID,offset);
		// 			//memcpy(_dir.get()+sizeof(MY_UINT32),&msg.m_head.unMsgID,sizeof(MY_UINT32));
		// 			writeInt(_dir.get()+offset,msg.m_head.nResult,offset);
		// 			//memcpy(_dir.get()+2*sizeof(MY_UINT32),&msg.m_head.nResult,sizeof(int));
		// 		}break;
		// 	case MSG_SEND_ALL_MSG_ID://收消息 (广播)
		// 	case MSG_SEND_TO_MSG_ID://收消息 (私人消息)
		// 		{
		// 			len = sizeof(MY_UINT32)//len
		// 				+ sizeof(MY_UINT32) //ID
		// 				+ sizeof(MY_UINT32) //uID
		// 				+ sizeof(MY_UINT32) //level
		// 				+ sizeof(MY_UINT32) //viplevel
		// 				+ sizeof(MY_UINT32) + msg.m_user.m_name.size()
		// 				+ sizeof(MY_UINT32) + msg.text.size();
		// 
		// 			_dir.reset(new char[len]);
		// 			memset(_dir.get(),0,len);
		// 
		// 			writeUInt(_dir.get()+offset,len - sizeof(MY_UINT32),offset);
		// 
		// 			writeUInt(_dir.get()+offset,msg.m_head.unMsgID,offset);
		// 			writeUInt(_dir.get()+offset,msg.m_user.m_id,offset);
		// 			writeUInt(_dir.get()+offset,msg.m_user.m_level,offset);
		// 			writeUInt(_dir.get()+offset,msg.m_user.m_vipLevel,offset);
		// 			writeUInt(_dir.get()+offset,msg.m_user.m_name.size(),offset);
		// 			writeString(_dir.get()+offset,&msg.m_user.m_name[0],msg.m_user.m_name.size(),offset);
		// 
		// 			writeUInt(_dir.get()+offset,msg.text.size(),offset);
		// 			writeString(_dir.get()+offset,&msg.text[0],msg.text.size(),offset);
		// 		}break;
	//}
//	return len;
}

MY_UINT32 IAsioTools::SerializingBinary(const MsgInfo & msg, std::shared_ptr<char> & _dir)
{
	MY_UINT32 len = sizeof(MY_UINT32) + sizeof(MY_UINT32) + sizeof(MY_UINT32) + msg.m_body_len;
	_dir = std::shared_ptr<char>(new char[len], std::default_delete<char[]>());
	return SerializingBinary(msg,_dir.get());
}

MY_UINT32 IAsioTools::SerializingBinary(const MsgInfo & msg, char * _dir)
{
	MY_UINT32 len = sizeof(MY_UINT32) + sizeof(MY_UINT32) + sizeof(MY_UINT32) + msg.m_body_len;
	MY_UINT32 offset = 0;
	memset(_dir, 0, len);
	writeUInt(_dir, len - sizeof(MY_UINT32), offset);
	writeUInt(_dir + offset, msg.m_head.unMsgID, offset);
	writeUInt(_dir + offset, msg.m_body_len, offset);
	writeArray(_dir + offset, msg.m_body.get(), msg.m_body_len, offset);
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
	std::vector<MsgInfo> ver;
	memcpy(_Data + _len, data, size);
	_len += size;
	int f = 0, temp = 0;
	MY_UINT32 _sLen = 0;

	if (_len > MAX_RESERVE_SIZE)
	{
		assert(false && "buffer enough ");
	}

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
					boost::scoped_array<char> _buffer(new char[_sLen]);
					memcpy(_buffer.get(), _Data + f + MAX_HEAD_LEN, _sLen);
					MsgInfo info;
					if (UnSerializingMsg(_buffer, _sLen, info))
					{
						ver.push_back(std::move(info));
					}
					f += MAX_HEAD_LEN + _sLen;
				}

			}
		}
	}

	return ver;
}

bool IAsioTools::UnSerializingMsg(boost::scoped_array<char> & _src, MY_UINT32 size, MsgInfo & info)
{
	if (size <= sizeof(MY_UINT32))
	{
		throw std::runtime_error("UnSerializingMsg error.");
		//assert(false && "UnSerializingMsg error.");
		return false;
	}
	MY_UINT32 offset = 0;
	info.Clear();
	ReadUInt(&info.m_head.unMsgID, _src.get(), offset);//type
	ReadUInt(&info.m_body_len, _src.get()+offset, offset);
	info.m_body.reset(new unsigned char[info.m_body_len]);
	ReadArray(info.m_body.get(),_src.get() + offset,info.m_body_len,offset);

	if (offset > size)
	{
		throw std::runtime_error("UnSerializingMsg error，format error");
		return false;
	}
	assert(offset <= size);
	return true;

// 	switch (info.m_head.unMsgID)
// 	{
		// 		case MSG_LOGIN_ID:
		// 			{
		// // 				memcpy(&info.m_user.m_id,_src.get()+offset,sizeof(MY_UINT32));
		// // 				offset+=sizeof(MY_UINT32);
		// 				ReadUInt(&info.m_user.m_id,_src.get()+offset,offset);
		// 				ReadUInt(&info.m_user.m_level,_src.get()+offset,offset);
		// 				ReadUInt(&info.m_user.m_vipLevel,_src.get()+offset,offset);
		// 
		// 				MY_UINT32 len = 0;
		// 				ReadUInt(&len,_src.get()+offset,offset);
		// 
		// 				info.m_user.m_name.resize(len,0);
		// 				ReadString(&info.m_user.m_name[0],_src.get()+offset,len,offset);
		// 
		// 				bReturn = true;
		// 			}break;
		// 		case MSG_CHANGE_NAME_ID:
		// 			{
		// 				MY_UINT32 len = 0;
		// 				ReadUInt(&len,_src.get()+offset,offset);
		// 
		// 				info.m_user.m_name.resize(len,0);
		// 				ReadString(&info.m_user.m_name[0],_src.get()+offset,len,offset);
		// 
		// 				bReturn = true;
		// 			}break;
		// 		case MSG_CHANGE_LEVEL_ID:
		// 			{
		// 				ReadUInt(&info.m_user.m_level,_src.get()+offset,offset);
		// 				ReadUInt(&info.m_user.m_vipLevel,_src.get()+offset,offset);
		// 
		// 				bReturn = true;
		// 			}break;
		// 		case MSG_SEND_FROM_MSG_ID:
		// 			{
		// 				ReadUInt(&info.toID,_src.get()+offset,offset);
		// 
		// 				MY_UINT32 len = 0;
		// 				ReadUInt(&len,_src.get()+offset,offset);
		// 
		// 				info.text.resize(len,0);
		// 				ReadString(&info.text[0],_src.get()+offset,len,offset);
		// 
		// 				bReturn = true;
		// 			}break;
	//}

// 	assert(offset <= size);
// 	return bReturn;
}

std::vector<std::string> IAsioTools::UnSerializingString(const char * data, int size)
{
	std::vector<std::string> ver;
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

void IAsioTools::writeArray(void * _dir, const unsigned char * _src, MY_UINT32 len, MY_UINT32 & offset)
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
