#include "ServerConnection.h"

//DECLARE_SINGLETON_MEMBER(CServerConnectionData);

void CServerConnectionData::AddOneConnection( const Session_ptr & _ptr )
{
	_ptr->OnInit();//先假设登录
	{
		boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
		//boost::unique_lock<boost::shared_mutex> writeLock(m_rw_mutex);//写锁
		m_verConnection->push_back(_ptr);
	}

	std::cout<<"connect number:"<<m_verConnection->size()<<std::endl;
}

bool CServerConnectionData::DeleteOneConnection(const Session_ptr & _ptr )
{
	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
	//boost::unique_lock<boost::shared_mutex> writeLock(m_mutex);//写锁
	std::vector<Session_ptr>::iterator it = m_verConnection->begin();
	while(it != m_verConnection->end())
	{
		if (*it == _ptr)
		{
			(*it)->Socket().close();//关闭连接
			(*it)->m_isDelete = true;
			it = m_verConnection->erase(it);
			return true;
		}
		++it;
	}

	return false;
}


bool CServerConnectionData::DeleteOneNoSoket(Session_ptr & _ptr)
{
	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
	std::vector<Session_ptr>::iterator it = m_verConnection->begin();
	while(it != m_verConnection->end())
	{
		if (*it == _ptr)
		{
			it = m_verConnection->erase(it);
			return true;
		}
		++it;
	}

	return false;
}


// void CServerConnectionData::SendAllMessage( const std::string str )
// {
// 	CMyAsioTools tools;
// 	std::string strMsg = tools.Serializing(str);
// 
// 	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
// 	//boost::shared_lock<boost::shared_mutex> readLock(m_rw_mutex);
// 	std::vector<Session_ptr>::iterator it = m_verConnection->begin();
// 	while(it != m_verConnection->end())
// 	{
// 		(*it)->OnSend(strMsg);
// 		
// 		it++;
// 	}
// 
// 	std::cout<<"send count:"<<m_verConnection->size()<<std::endl;
// }

void CServerConnectionData::OnTimerCallBack()
{
	++m_timeDida;
	if(m_timeDida >= MAX_TIME_OUT_DIDA)
	{
		m_timeDida = 0;
		boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
		int n = 0;
		std::vector<Session_ptr>::iterator it = m_verConnection->begin();
		while(it != m_verConnection->end())
		{
			if ((*it)->OnTimeOut())
			{
				(*it)->Socket().close();//关闭连接
				(*it)->m_isDelete = true;
				it = m_verConnection->erase(it);
			}
			else
				it++;
		}
	}

}

CServerConnectionData::~CServerConnectionData()
{
	boost::mutex::scoped_lock writeLock(m_mutex);//写锁 非递归锁(不可重入锁)
}
