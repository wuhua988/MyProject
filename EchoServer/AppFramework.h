#ifndef _AppFramework_h_
#define _AppFramework_h_
#include "Framework.h"
class AppFramework : public Framework
{
public:
    AppFramework(){}
	~AppFramework(){}
    virtual void OnMessage(MsgBlock* pMsgBlock);
    virtual int OnConnCreated(ConnInfo* pConnInfo);   
    virtual int OnFailConnect(ConnInfo* pConnInfo);
    virtual int OnConnClosed(ConnInfo* pConnInfo);
};

#endif