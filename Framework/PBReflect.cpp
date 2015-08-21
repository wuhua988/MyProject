#include "PBReflect.h"
#include "log.h"

std::map<UInt32,std::string> PBReflect::m_Cmd2NameMap;
SharedPtr<Message> PBReflect::ParsePBMsg(const char *data, int len, const UInt32& cmdID, ErrorCode *error)
{
	if(data == NULL || len <= 0)
	{
		*error = kNoError;
		return SharedPtr<Message>(NULL);
	}
	*error = kNoError;
	
	std::map<UInt32,std::string>::iterator iter = m_Cmd2NameMap.find(cmdID);
	if(iter == m_Cmd2NameMap.end())
	{
		LOG_ERROR("Can't Find TypeName By Command ID:%u",cmdID);
		return NULL;
	}
	
	SharedPtr<Message> message = CreateMsg(iter->second);
	if (!message) {
		*error = kUnknownMessageType;
		return NULL;
	}
	if (!message->ParseFromArray((const char *)data , len )) {
		*error = kParseError;
		return NULL;
	}

	return SharedPtr<Message>(message);
}

bool PBReflect::GetNameByCmdID(int cmdID,std::string& TypeName)
{
	std::map<UInt32,std::string>::iterator iter = m_Cmd2NameMap.find(cmdID);
	if(iter == m_Cmd2NameMap.end())
	{
		LOG_ERROR("Can't Find TypeName By Command ID:%u",cmdID);
		return false;
	}
	
    TypeName = iter->second;
	return true;
}

void PBReflect::RegisterPbMsg(const UInt32& cmdID,const std::string& name)
{
	std::map<UInt32,std::string>::iterator iter = m_Cmd2NameMap.find(cmdID);
	if(iter == m_Cmd2NameMap.end())
	{
		m_Cmd2NameMap[cmdID] = name;
	}else{
		LOG_ERROR("The Command:%u,Name:%s,Already Existed",cmdID,name.c_str());
	}
}

SharedPtr<Message> PBReflect::CreateMsg(const std::string &msgTypeName)
{
	Message *message = NULL;
	const google::protobuf::Descriptor *descriptor = 
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(msgTypeName);
	if (descriptor) {
		const Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return SharedPtr<Message>(message);
}

namespace {
	const std::string kNoErrorStr = "NoError";
	const std::string kInvalidLengthStr = "InvalidLength";
	const std::string kInvalidDataStr = "InvalidNameLen";
	const std::string kUnknownMessageTypeStr = "UnknownMessageType";
	const std::string kParseErrorStr = "ParseError";
	const std::string kUnexpectedStr = "UnexpectedStr";
}

const std::string &PBReflect::ErrorToStr(ErrorCode error)
{
	switch(error) {
	case kNoError:
		return kNoErrorStr;

	case kInvalidLength:
		return kInvalidLengthStr;

	case kInvalidData:
		return kInvalidDataStr;

	case kUnknownMessageType:
		return kUnknownMessageTypeStr;

	case kParseError:
		return kParseErrorStr;
	default:
	    return kUnexpectedStr;
	}
}
