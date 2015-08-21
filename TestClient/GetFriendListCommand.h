#ifndef __GetFriendListCommand_h__
#define __GetFriendListCommand_h__
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class GetFriendListCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    GetFriendListCommand();
    GetFriendListCommand(Framework* pFramework);
    ~GetFriendListCommand();
    
    virtual void OnTimer();
    virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
    void SetParam(UInt64 version_id,UInt64 user_id);
private:
    UInt64 m_version_id;
    UInt64 m_user_id;
};

#endif //__GetFriendListCommand_h__