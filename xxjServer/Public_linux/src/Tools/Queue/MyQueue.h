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
#include <queue>//std queue
#include <vector>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#define  MAX_SEND_COUNT 10000

template<typename TYPE>
class CMyQueue
{
private:
	std::queue<TYPE> theQueue_;
	boost::mutex m_oMutex;
	boost::condition_variable_any m_cond;//条件变量
public:
	CMyQueue(){};
	~CMyQueue(){};
public:
	void Dequeue(std::vector<TYPE> & ver,bool isWait = true);//isWait:为空时是否等待
	TYPE Dequeue(bool isWait = true);//条件变量得到一个消息
	void Enqueue(const TYPE & e);//插入一个元素
	inline bool Empty()  { return empty(); }
	inline size_t Size() { return size(); };//队列长度
	void Clear();//清除元素
protected:
// 	TYPE pop();//出一个队列
// 	void push( const TYPE &val );//压入一个队列
	size_t size();//队列长度
	bool empty();//判空
};

#endif