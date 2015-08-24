#ifndef _ClientFramework_h_
#define _ClientFramework_h_
#include "Framework.h"
#include "Singleton.h"
class ClientFramework : public Framework
{
public:
    ClientFramework(){}
    ~ClientFramework(){}
    virtual void OnMessage(MsgBlock* pMsgBlock);
    virtual int OnConnCreated(ConnInfo* pConnInfo);   
    virtual int OnFailConnect(ConnInfo* pConnInfo);
    virtual int OnConnClosed(ConnInfo* pConnInfo);
};

typedef Singleton<ClientFramework> gEchoCliFw;
#endif