#ifndef _TestCliFramework_h_
#define _TestCliFramework_h_
#include "Framework.h"
class TestCliFramework : public Framework
{
public:
    TestCliFramework(){}
    ~TestCliFramework(){}
    virtual void OnMessage(MsgBlock* pMsgBlock);
    virtual int OnConnCreated(ConnInfo* pConnInfo);   
    virtual int OnFailConnect(ConnInfo* pConnInfo);
    virtual int OnConnClosed(ConnInfo* pConnInfo);
};

typedef Singleton<TestCliFramework> gTestCliFw;
#endif