#ifndef __EventWorkThread_h__
#define __EventWorkThread_h__

#include "EventBaseThread.h"
class EventWorkThread : public EventBaseThread
{
public:
    EventWorkThread(Framework* pFw);
    virtual ~EventWorkThread();
private:
   
};
#endif
