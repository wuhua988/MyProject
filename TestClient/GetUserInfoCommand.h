#ifndef __GetUserInfoCommand_h__
#define __GetUserInfoCommand_h__
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class GetUserInfoCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    GetUserInfoCommand();
    GetUserInfoCommand(Framework* pFramework);
	~GetUserInfoCommand();
	
	virtual void OnTimer();
	virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
	void SetParam(UInt64 user_id);
private:
	UInt64 m_user_id;
};

#endif //__GetUserInfoCommand_h__