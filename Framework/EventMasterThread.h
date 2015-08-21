#ifndef _EventMasterThread_h_
#define _EventMasterThread_h_
#include "EventBaseThread.h"

class EventMasterThread : public EventBaseThread
{
public:
    EventMasterThread(Framework* pFw);
    virtual ~EventMasterThread();
};

#endif
