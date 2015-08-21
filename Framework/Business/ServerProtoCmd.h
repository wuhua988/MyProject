#ifndef __ServerProtoCmd_h__
#define __ServerProtoCmd_h__

#include <string>
#include <map>
#include "ProtoCmdBase.h"
#include "ClientProtoCmd.h"
#include "SharedPtr.h"

class ServerProtoCmd : public ProtoCmdBase
{
public:
    explicit ServerProtoCmd(Framework* pFramework = NULL);
    virtual ~ServerProtoCmd();
	virtual void OnTimer(int state);
	virtual void OnRequest(MsgBlock* pMsgBlock) = 0;
protected:
    virtual void SendClientRequest();
    virtual void SendResponse();
	void         SendErrResponse();
private:
    MsgBlock* m_pOriMsgBlock; //Ô´ÇëÇó°ü
    std::map<std::string,SharedPtr<ClientProtoCmd> > m_clientMap;
};


#endif //__ServerProtoCmd_h__