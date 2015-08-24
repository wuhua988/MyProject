#ifndef _PBReflect_h_
#define _PBReflect_h_
#include <string>
#include <map>
#include "google/protobuf/message.h"
#include "SharedPtr.h"
#include "OSHeaders.h"

using std::string;
using google::protobuf::Message;

class PBReflect
{
public:
    enum ErrorCode {
        kNoError = 0,
        kInvalidLength = 1,
        kInvalidData = 2,
        kUnknownMessageType = 3,
        kParseError = 4,
	};
	
public:   
    static void               RegisterPbMsg(const UInt32& cmdID,const std::string& name);	
    static SharedPtr<Message> ParsePBMsg(const char *data, int len, const UInt32& cmdID, ErrorCode *error);
    static SharedPtr<Message> CreateMsg(const std::string &msgTypeName);
    static const std::string &ErrorToStr(ErrorCode error);
private:
    static std::map<UInt32,std::string> m_Cmd2NameMap;
    static bool GetNameByCmdID(int cmdID,std::string& TypeName);
};

#endif  //_PBReflect_h_