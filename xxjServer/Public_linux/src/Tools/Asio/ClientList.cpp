#include "ClientList.h"

CClientList::CClientList():m_mapClient(new std::map<MY_UINT32,Session_ptr>)
{
	m_isStop = true;
}

CClientList::~CClientList()
{

}

void CClientList::AddOneClient( const Session_ptr & _ptr )
{
	//必须全程持锁
	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
	if(!m_mapClient.unique())//其它有人在读
	{
		m_mapClient.reset(new std::map<MY_UINT32,Session_ptr>(*m_mapClient));
	}

	//----删除老的
	std::map<MY_UINT32,Session_ptr>::iterator it = m_mapClient->find(_ptr->m_user.m_id);
	if (it != m_mapClient->end())
	{
		(*it).second->Socket().close();//关闭连接
		(*it).second->m_isDelete = true;
	}
	//-----------------------------------------end
	assert(m_mapClient.unique());
	(*m_mapClient)[_ptr->m_user.m_id] = _ptr;

	std::cout<<"m_mapClient:"<<m_mapClient->size()<<std::endl;
}

bool CClientList::DeleteOnClient( Session_ptr & _ptr )
{
	//必须全程持锁
	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
	if(!m_mapClient.unique())//其它有人在读
	{
		m_mapClient.reset(new std::map<MY_UINT32,Session_ptr>(*m_mapClient));
	}
	assert(m_mapClient.unique());

	std::map<MY_UINT32,Session_ptr>::iterator it = m_mapClient->find(_ptr->m_user.m_id);
	if (it != m_mapClient->end())
	{
		(*it).second->Socket().close();//关闭连接
		(*it).second->m_isDelete = true;
		m_mapClient->erase(it);
		return true;
	}
	else
		return false;
}

void CClientList::SendMsgToBroadcast( const MsgInfo & info )
{
	m_broadcastQue.Enqueue(info);
	//组织数据
// 	CMyJson::JsonRoot json = m_myJsonOp.CreatJson(MSG_SEND_ALL_MSG_ID);
// 	m_myJsonOp.AddValue(json,"uID",info.m_user.m_id);
// 	m_myJsonOp.AddValue(json,"name",info.m_user.m_name);
// 	m_myJsonOp.AddValue(json,"text",info.text);
// 	std::string strMsg = m_myJsonOp.GetJsonStr(json);
// 	CMyAsioTools tools;
// 	std::string  strSend= tools.Serializing(strMsg);
// 
// 	boost::shared_ptr<std::map<MY_UINT32,Session_ptr> > temp;
// 	{
// 		boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
// 		temp = m_mapClient;
// 		assert(!m_mapClient.unique());
// 	}
// 
// 	//遍历发送
// 	std::map<MY_UINT32,Session_ptr>::iterator c_it = temp->begin();
// 	while(c_it != temp->end())
// 	{
// 		c_it->second->OnSend(strSend);//发送消息
// 		c_it++;
// 	}
// 
// 	
// 	std::cout<<"send total:"<<temp->size()<<std::endl;
}

void CClientList::SendMsgToOne( const MsgInfo & info )
{
	m_oneQue.Enqueue(info);
	//组织数据
// 	CMyJson::JsonRoot json = m_myJsonOp.CreatJson(MSG_SEND_TO_MSG_ID);
// 	m_myJsonOp.AddValue(json,"uID",info.m_user.m_id);
// 	m_myJsonOp.AddValue(json,"name",info.m_user.m_name);
// 	m_myJsonOp.AddValue(json,"text",info.text);
// 	std::string strMsg = m_myJsonOp.GetJsonStr(json);
// 	CMyAsioTools tools;
// 	std::string  strSend= tools.Serializing(strMsg);
// 	//
// 
// 	boost::shared_ptr<std::map<MY_UINT32,Session_ptr> > temp;
// 	{
// 		boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
// 		temp = m_mapClient;
// 		assert(!m_mapClient.unique());
// 	}
// 
// 	//遍历发送
// 	std::map<MY_UINT32,Session_ptr>::iterator c_it = temp->find(info.toID);
// 	if (c_it != temp->end())
// 	{
// 		c_it->second->OnSend(strSend);
// 	}

}

void CClientList::OnStartBroadcast()
{
	std::vector<MsgInfo> ver;
	CMyAsioTools tools;
	boost::scoped_array<char> _buffer;
	MY_UINT32 size = 0;
	std::string strSend;
	while(!m_isStop)
	{
		strSend.clear();
		ver.clear();
		m_broadcastQue.Dequeue(ver);//取出一个数据

		boost::shared_ptr<std::map<MY_UINT32,Session_ptr> > temp;
		{
			boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
			temp = m_mapClient;
			assert(!m_mapClient.unique());
		}


		for (size_t i=0;i<ver.size();i++)
		{
			boost::scoped_array<char> _temp;
			MY_UINT32 len = tools.Serializing(ver.at(i),_temp);

			//遍历发送
			std::map<MY_UINT32,Session_ptr>::iterator c_it = temp->begin();
			while(c_it != temp->end())
			{
				c_it->second->OnSend(_temp,len);//发送消息
				c_it++;
			}
		}

		//std::cout<<"send total:"<<temp->size()<<std::endl;
	}
}

void CClientList::OnStartOne()
{
	CMyAsioTools tools;
	while(!m_isStop)
	{
		MsgInfo info = m_oneQue.Dequeue();

		boost::scoped_array<char> _temp;
		MY_UINT32 len = tools.Serializing(info,_temp);
		//

		boost::shared_ptr<std::map<MY_UINT32,Session_ptr> > temp;
		{
			boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
			temp = m_mapClient;
			assert(!m_mapClient.unique());
		}

		//发给接收者
		std::map<MY_UINT32,Session_ptr>::iterator c_it = temp->find(info.toID);
		if (c_it != temp->end())
		{
			c_it->second->OnSend(_temp,len);
		}

		//发给发送者
		c_it = temp->find(info.m_user.m_id);
		if (c_it != temp->end())
		{
			c_it->second->OnSend(_temp,len);
		}


	}
}

void CClientList::OnStart()
{
	m_isStop = false;

	m_broadcastThrd = boost::thread(boost::bind(&CClientList::OnStartBroadcast,this));//用于与客户端连接线程
	m_broadcastThrd.yield();//自动运行

	m_oneThrd = boost::thread(boost::bind(&CClientList::OnStartOne,this));//用于与客户端连接线程
	m_oneThrd.yield();//自动运行
}

void CClientList::OnStop()
{
	m_isStop = true;

	m_broadcastThrd.interrupt();//中断
	m_oneThrd.interrupt();//中断
	m_broadcastThrd.join();
	m_oneThrd.join();
}

void CClientList::OnClear()
{
	//遍历发送
	std::map<MY_UINT32,Session_ptr>::iterator c_it = m_mapClient->begin();
	while(c_it != m_mapClient->end())
	{
		(*c_it).second->CloseSocket();//关闭连接
		(*c_it).second->m_isDelete = true;
		c_it++;
	}

	m_mapClient->clear();
}



