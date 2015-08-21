#ifndef __EventTimer_h__
#define __EventTimer_h__
#include "Timer.h"

class WatcherBase;
class EventTimer : public Timer
{
public:
    EventTimer(WatcherBase* pHandler,const Float64 dExpire,const Float64 dInterval = 0);
    virtual void Callback(const UInt32 revents);
private:
    EventTimer(const EventTimer& other);
    EventTimer& operator=(const EventTimer & other);
    WatcherBase* m_pHandler;
};

#endif //__EventTimer_h__