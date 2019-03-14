/********************************************************************
    创建时间：    2015/08/06  14:52:15 
    文件名：      SignalSlotTrivial.h
    作者：        xxj
    
    功能:		1.信号槽
				2.用于事件回调
                
    说明:		1. 
                2. 
*********************************************************************/
#ifndef SIGNALSLOTTRIVIAL_H_
#define SIGNALSLOTTRIVIAL_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <memory>
#include <vector>
#include <boost/function.hpp>

// template<typename Signature>
// class SignalTrivial;

class SignalTrivial: boost::noncopyable
{
 public:
  typedef boost::function<void ()> Functor;

  //添加一个
  void connect(Functor& func)
  {
    functors_.push_back(func);
  }

  void call()
  {
	//注意：非线程安全的 需要由各自的回调控制安全性
    std::vector<Functor>::iterator it = functors_.begin();
    for (; it != functors_.end(); ++it)
    {
      (*it)();
    }
  }

  void Clear()
  {
	  functors_.clear();
  }

 private:
	std::vector<Functor> functors_;
};

#endif 
