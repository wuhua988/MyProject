#ifndef __GetFriendInfoCommand_h__
#define __GetFriendInfoCommand_h__
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class GetFriendInfoCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    GetFriendInfoCommand();
    GetFriendInfoCommand(Framework* pFramework);
	~GetFriendInfoCommand();
	
	virtual void OnTimer();
	virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
	void SetParam(UInt64 user_id,UInt64 friend_id);
private:
	UInt64 m_friend_id;
	UInt64 m_user_id;
};

#endif //__GetFriendInfoCommand_h__