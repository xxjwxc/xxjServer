#include "MyClient.h"
#include <boost/locale/encoding.hpp>
#include "MyAsioTools.h"
#include <Tools/MyTools.h>
#include "../data/ConnectionPool.h"
#include "../data/flatbuffers/test_generated.h"
#include <Tools/MyTimes.h>


//bool IGameBase::isLog = false;

CMyClient::CMyClient(boost::asio::io_service & ioservice)
	:ClientSession(ioservice)
{
	m_myAsioTools = boost::shared_ptr<IAsioTools>(new CMyAsioTools);
	m_loginTime = 0;
}

CMyClient::~CMyClient()
{
}

void CMyClient::OnRead(std::vector<MsgInfo> & ver)
{
	std::vector<MsgInfo>::iterator it = ver.begin();
	while(it != ver.end())
	{
		if (!DealMsg(*it))
			break;

		it++;
	}
// 	for (std::string mpv:ver)
// 	{
// 		if (IGameBase::isLog)
// 			std::cout << "deal message :"<<mpv << std::endl;
// 	}
}

void CMyClient::OnRead(std::vector<std::string> & ver)
{
// 	for (std::string mpv : ver)
// 	{
// 		if (IGameBase::isLog)
// 			std::cout << "deal message :" << mpv << std::endl;
// 		boost::thread thrd(boost::bind(&CMyClient::OnTest,this,mpv));
// 	}
	if (ver.size() > 0)
	{
		if (isLog)
			std::cout << "deal message :" << ver.at(0) << std::endl;
		OnDealMessage(ver.at(0));
		//boost::thread thrd(boost::bind(&CMyClient::OnTest, this, ver.at(0)));
	}
	
}

bool CMyClient::OnSend(const boost::scoped_array<char> & str,MY_UINT32 len )
{
	WriteSomeSomething(str,len);
	return true;
}

void CMyClient::OnConnect()
{
	if (isLog)
		std::cout << "get a new client:"<<Socket().remote_endpoint().address()<<":"<<Socket().remote_endpoint().port() << std::endl;
}

void CMyClient::DisConnect()
{
	if (isLog)
		std::cout << "close a client"<< std::endl;

	OnDestroye();
}

void CMyClient::OnInit()
{
	m_isInit = true;
	m_loginTime = CMyTimes::get_utc();
}

bool CMyClient::OnTimeOut()
{
	if (!m_isInit)
		return false;
	else
	{
		MY_UINT32 now = CMyTimes::get_utc();
		if (now - CMyIni::instance()->m_timeOut > m_loginTime)
			return true;

		return false;//还没有超时
	}
	return false;
}

void CMyClient::UnSerializing(const char * data, int size)
{
	//字符串
// 	std::vector<std::string> ver = m_myAsioTools->UnSerializingString(data, size);//解包
// 	this->OnRead(ver);
	//二进制
	std::vector<MsgInfo> ver = m_myAsioTools->UnSerializingBinary(data, size);//解包
	//ReadSomeSomething();//继续读取
	this->OnRead(ver);
}

void CMyClient::WriteResult(const std::string & str1)
{
	CMyAsioTools tools;
	std::string str = tools.Serializing(str1);
	if (str.length() > 0)
	{
// 		boost::scoped_array<char> _buffer;
// 		_buffer.reset(new char[str.length() + 1]);
// 		memcpy(_buffer.get(), str.c_str(), str.length() + 1);
// 		WriteSomeSomething(_buffer, str.length() + 1);
		WriteSomeSomething(str);
	}

	// 	MsgInfo info;
	// 	info.m_head.unMsgID = MSG_SEND_RESULT_ID;
	// 	info.m_head.nResult = nResult;
	// 	boost::scoped_array<char> _buffer;
	// 	CMyAsioTools tools;
	// 	MY_UINT32 len = tools.Serializing(info,_buffer);
	// 	if(len > 0)
	// 	{
	// 		WriteSomeSomething(_buffer,len);
	// 	}
}

bool CMyClient::DealMsg( MsgInfo & info )
{

	boost::scoped_array<char> m_buffer;
	CMyAsioTools tools;
	int64_t size = tools.SerializingBinary(info, m_buffer);
	WriteSomeSomething(m_buffer, size);
	////////// Deserialize //////////
	//flatbuffers
	try
	{
		flatbuffers::Verifier tex_verify((uint8_t*)info.m_body.get(), info.m_body_len);
		bool verify_flag = TestApp::VerifyTestObjBuffer(tex_verify);
		if (verify_flag)
		{
			auto obj = TestApp::GetTestObj((uint8_t*)info.m_body.get());

			std::cout << obj->id() << std::endl;
			if(obj->name())
				std::cout << obj->name()->c_str() << std::endl;
			std::cout << obj->flag() << std::endl;
			if (obj->list())
				for (size_t i = 0; i < obj->list()->size(); i++)
				{
					std::cout << obj->list()->Get(i) << std::endl;
				}
			if (obj->kv())
			{
				std::cout << obj->kv()->key() << std::endl;
				std::cout << obj->kv()->value() << std::endl;
			}
		}

	}
	catch (const std::exception& e)
	{
		std::cout << e.what()<<std::endl;
	}

	//-----------------------------end
	return true;
}

void CMyClient::OnWriteDown()
{
	 //写完后立即关闭
	 //DisConnect();
	 //CloseSocket();
}

void CMyClient::OnDestroye()
{
	if (!m_isDelete)//已经删除过
	{
		CServerConnectionData::instance()->DeleteOneConnection(shared_from_this());
	}
}

//逻辑入口
void CMyClient::OnDealMessage(std::string str)
{
	CMyJson::JsonRoot json;
	std::string strResult = "{\"result\":-1}";//参数错误
	try
	{
		if (CMyJson::InitJson(str, json))
		{
			std::string strKey = CMyJson::GetValue<std::string>(json, "reqKey");
// 			if (boost::iequals(strKey, "login"))//登录
// 			{
// 				//特殊逻辑
// 			}
// 			else if (boost::iequals(strKey, "register"))//注册
// 			{
// 				//特殊逻辑......
// 			}

			const auto & map = CMyIni::instance()->m_sqlMap;
			const auto & it = map.find(strKey);
			if (it != map.end())
			{
				strResult = ConnectionPool::instance()->OnDo(it->second, json);
			}
		}
		else
		{
			WriteResult(MyTools::GetUtf8("{\"result\":-6}"));
			DisConnect();//关闭
			return;
		}
	}
	catch (std::exception& exc)
	{
		std::string __errorInfo = exc.what();
		MyTools::PrintError(__errorInfo);
		std::cerr << exc.what() << std::endl;
	}

	WriteResult(MyTools::GetUtf8(strResult));
}


