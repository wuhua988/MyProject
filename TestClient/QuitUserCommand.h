#ifndef __QuitUserCommand_h__
#define __QuitUserCommand_h__
#include <string>
#include "CliCmd.h"
#include "protocol/user_management_service.pb.h"
using std::string;

class QuitUserCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    QuitUserCommand();
    QuitUserCommand(Framework* pFramework);
	~QuitUserCommand();
	
	virtual void OnTimer();
	virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
	void SetParam(UInt64        user_id
				 ,UInt32        terminal_type
	             ,const string& token);
private:	
	UInt64 m_user_id;
	UInt32 m_terminal_type; // 终端类型
	string m_token ;
};

#endif //__UserAuthCommand_h__