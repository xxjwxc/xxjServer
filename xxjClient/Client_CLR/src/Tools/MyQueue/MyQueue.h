/********************************************************************
	创建时间：	2014/08/15  16:26:54 
	文件名：		MyQueue.h
	作者：		谢小军
	
	功能：		1. 线程安全队列
				2.
				
	说明：	    1. 多线程状态下的安全队列
				2. 
*********************************************************************/
#ifndef MYQUEUE_H_
#define MYQUEUE_H_
#include "../ClientTypeDefine.h"

#define  MAX_SEND_COUNT 10000

generic<typename TYPE>
ref class CMyQueue
{
private:
	System::Threading::Mutex^ m_mutex;

	Queue<TYPE>^ m_queue;
public:
	CMyQueue()
	{
		m_mutex = gcnew System::Threading::Mutex();
		m_queue = gcnew Queue<TYPE>();
	};
	~CMyQueue() {};
public:
	TYPE Dequeue(bool isWait) {//条件变量得到一个消息
		if (isWait)
		{
			while (empty())
			{
				System::Threading::Thread::Sleep(10);
			}
		}

		m_mutex->WaitOne();
		if (empty())
		{
			m_mutex->ReleaseMutex();
			return Dequeue(isWait);
		}
		else {
		
			TYPE temp = m_queue->Dequeue();
			m_mutex->ReleaseMutex();
			return temp;
		}

	};
	void Enqueue(TYPE  e) {//插入一个元素
		m_mutex->WaitOne();
		m_queue->Enqueue(e);
		m_mutex->ReleaseMutex();
	};
	inline bool Empty()  { return empty(); }
	inline size_t Size() { return size(); };//队列长度
protected:
// 	TYPE pop();//出一个队列
// 	void push( const TYPE &val );//压入一个队列
	size_t size() {//队列长度
		return m_queue->Count;
	};
	bool empty() {//判空
		return Size() == 0;
	};
};



#endif