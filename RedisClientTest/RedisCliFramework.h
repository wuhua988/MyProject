#ifndef _RedisCliFramework_h_
#define _RedisCliFramework_h_
#include "Framework.h"
class RedisCliFramework : public Framework
{
public:
    RedisCliFramework(){}
	~RedisCliFramework(){}
    virtual void OnMessage(MsgBlock* pMsgBlock);
    virtual int OnConnCreated(ConnInfo* pConnInfo);   
    virtual int OnFailConnect(ConnInfo* pConnInfo);
    virtual int OnConnClosed(ConnInfo* pConnInfo);
};

#endif