#ifndef  __BusinessTimerThread_h__
#define  __BusinessTimerThread_h__

#include "../EventBaseThread.h"

class BusinessTimerThread : public EventBaseThread
{
public:
    BusinessTimerThread(Framework* pFw);
    virtual ~BusinessTimerThread();
public:
    void RegTimer(Timer* pTimer);
    void CancelTimeout(Timer* pTimer);
};

#endif 
