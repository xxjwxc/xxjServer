/********************************************************************
    创建时间:     2016/12/19  15:10:15 
    文件名:       MyProtobuf.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef MYPROTOBUF_H_
#define MYPROTOBUF_H_
#include <boost/any.hpp>
#include "../../MySingleton.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <map>
#include <iostream>
#include "../../DefineBase.h"
//#include "../../../test/query.pb.h"


class CMyProtobuf:public MySingleton<CMyProtobuf>
{
public:
	//处理pb的反射，通过协议字串动态生成一个协议
	template <typename T> 
	std::shared_ptr<T> OnCreatMessage(const std::string& typeName);

	//注意释放
	//使用以下方式转换  
	//std::shared_ptr<DeriveClass> ptr = std::dynamic_pointer_cast<DeriveClass>(ptrBase);  
	std::shared_ptr<google::protobuf::Message> OnCreatMessage(const std::string& typeName);

};

//消息分发器
class ProtobufDispatcher
{
public:
	typedef std::function<void()> Functor;

	//ProtobufDispatcher(std::function<void(std::shared_ptr<google::protobuf::Message> message)> discardProtobufMessage)
	//	: defaultCallback_(discardProtobufMessage)
	ProtobufDispatcher(){}

	template<typename T>
	void OnMessage(std::shared_ptr<T> & message) const;


	template<typename T>
	void OnRegisterMessageCallback(const std::function<void(std::shared_ptr<google::protobuf::Message>&)>& callback);

	std::map<const google::protobuf::Descriptor*, boost::any> callbacks_;
	//std::function<void(std::shared_ptr<google::protobuf::Message>&)> defaultCallback_;
};



#endif