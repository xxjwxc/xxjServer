#include "MyProtobuf.h"
#include <google\protobuf\message.h>


std::shared_ptr<google::protobuf::Message> CMyProtobuf::OnCreatMessage(const std::string& typeName)
{
	std::shared_ptr<google::protobuf::Message> ptr = nullptr;
	//google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			ptr.reset(prototype->New());
		}
	}
	return ptr;
}

