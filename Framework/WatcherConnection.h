#ifndef __WatcherConnection_h__
#define __WatcherConnection_h__
#include <string>
#include "ConnInfo.h"
#include "WatcherBase.h"
#include "EventTimer.h"

class MsgBlock;
class WatcherConnection : public WatcherBase
{
public:
    WatcherConnection(EventLoop* evLoop,Framework* pFw);
    virtual ~WatcherConnection();
	
    ConnInfo*           GetConnInfo();
    virtual std::string GetRemoteAddr();
    virtual UInt16      GetRemotePort();
	
    virtual bool        SendData(MsgBlock* pMsgBlock) = 0; //call by other thread
protected:
    void                StartTimer();
    void                StopTimer(); 
    ConnInfo    m_connInfo;
    EventTimer* m_pTimer;  
    friend class ConnInfo;
};

#endif //WatcherConnection_h__