#ifndef MYPROTOBUF_HPP_
#define MYPROTOBUF_HPP_
#include "MyProtobuf.h"

template <typename T>
std::shared_ptr<T> CMyProtobuf::OnCreatMessage(const std::string& typeName)
{
	std::shared_ptr<T> P = nullptr;
	//google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			P.reset(dynamic_cast<T *>(prototype->New()));
		}
	}
	return P;
}

template<typename T>
void ProtobufDispatcher::OnMessage(std::shared_ptr<T> & message) const
{
	if (message)
	{
		const auto it = callbacks_.find(message->GetDescriptor());
		if (it != callbacks_.end())
		{
			auto s = boost::any_cast<const std::function<void(std::shared_ptr<google::protobuf::Message>&)>>(it->second);
			s(message);
		}
		else
		{
			MY_ASSERT(false, "do not define callback!!");
			//defaultCallback_(message);
		}
	}
}

template<typename T>
void ProtobufDispatcher::OnRegisterMessageCallback(const std::function<void(std::shared_ptr<google::protobuf::Message>&)>& callback)
{
	callbacks_[T::descriptor()] = callback;
}

#endif