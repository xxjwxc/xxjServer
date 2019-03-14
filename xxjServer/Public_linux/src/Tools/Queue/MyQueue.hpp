//#include "stdafx.h"
#include "MyQueue.h"

template<typename TYPE>
size_t CMyQueue<TYPE>::size()
{
	//boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
	//	size_t size=theQueue_.size();
	return theQueue_.size();
}

template<typename TYPE>
bool CMyQueue<TYPE>::empty()
{
	//boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
	return theQueue_.empty();
}

template<typename TYPE>
TYPE CMyQueue<TYPE>::Dequeue(bool isWait/* = true*/)
{
	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
	if (isWait)
	{
		while (theQueue_.empty())//必须循环，必须在判断之后再wait()
		{
			m_cond.wait(m_oMutex);//会原子性的unlock m_oMutex 并进入等待
								  //wait执行完毕之后会自动重现加锁
		}

		assert(!theQueue_.empty());
	}

	TYPE tmp = theQueue_.front();
	theQueue_.pop();
	return tmp;
}

template<typename TYPE>
void CMyQueue<TYPE>::Dequeue(std::vector<TYPE> & ver, bool isWait /*= true*/)
{
	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
	if (isWait)
	{
		while (theQueue_.empty())//必须循环，必须在判断之后再wait()
		{
			m_cond.wait(m_oMutex);//会原子性的unlock m_oMutex 并进入等待
								  //wait执行完毕之后会自动重现加锁
		}

		assert(!theQueue_.empty());
	}

	ver.clear();
	int n = 0;
	while (!theQueue_.empty() && n < MAX_SEND_COUNT)
	{
		ver.push_back(theQueue_.front());
		theQueue_.pop();
		++n;
	}
}

template<typename TYPE>
void CMyQueue<TYPE>::Enqueue(const TYPE & e)
{
	{
		boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
		theQueue_.push(e);
	}

	m_cond.notify_one();//提示一个开启
}

template<typename TYPE>
void CMyQueue<TYPE>::Clear()
{
	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
	while (!theQueue_.empty())
	{
		theQueue_.pop();
	}
}

// 
// template<typename TYPE>
// TYPE CMyQueue<TYPE>::pop()
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	if (!theQueue_.empty())
// 	{
// 		TYPE tmp = theQueue_.front();
// 		theQueue_.pop();
// 		return tmp;
// 	}
// 	else
// 		return TYPE();//空的返回默认
// }
// 
// template<typename TYPE>
// void CMyQueue<TYPE>::push( const TYPE &val )
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	theQueue_.push(val);
// }
// 
// template<typename TYPE>
// size_t CMyQueue<TYPE>::size()
// {
// 	//boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	//	size_t size=theQueue_.size();
// 	return theQueue_.size();
// }
// 
// template<typename TYPE>
// bool CMyQueue<TYPE>::empty()
// {
// 	//boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	return theQueue_.empty();
// }
// 
// template<typename TYPE>
// TYPE CMyQueue<TYPE>::Dequeue()
// {
// 	boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 	while(theQueue_.empty())//必须循环，必须在判断之后再wait()
// 	{
// 		m_cond.wait(m_oMutex);//会原子性的unlock m_oMutex 并进入等待
// 		//wait执行完毕之后会自动重现加锁
// 	}
// 
// 	assert(!theQueue_.empty());
// 
// 	TYPE tmp = theQueue_.front();
// 	theQueue_.pop();
// 	return tmp;
// }
// 
// template<typename TYPE>
// bool CMyQueue<TYPE>::Enqueue(const TYPE & e )
// {
// 	{
// 		boost::mutex::scoped_lock oLock(m_oMutex);//互斥锁
// 		theQueue_.push(e);
// 	}
// 
// 	m_cond.notify_one();//提示一个开启
// }

