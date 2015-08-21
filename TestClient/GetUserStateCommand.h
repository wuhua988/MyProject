#ifndef __GetUserStateCommand_h__
#define __GetUserStateCommand_h__
#include <vector>
#include "CliCmd.h"
#include "protocol/presence.pb.h"
using std::string;

class GetUserStateCommand : public CliCmd
{
	DECLARE_CLASS()
public:
    GetUserStateCommand();
    GetUserStateCommand(Framework* pFramework);
    virtual ~GetUserStateCommand();
    
    virtual void OnTimer();
    virtual bool SendRequest();
    virtual void OnResponse(MsgBlock* pMsgBlock);
    void SetParam(const std::vector<UInt64>& user_list);
private:
    std::vector<UInt64> m_user_list;
};

#endif //__GetUserStateCommand_h__