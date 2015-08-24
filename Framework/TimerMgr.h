#ifndef __TimerMgr__h__
#define __TimerMgr__h__
#include "OSHeap.h"
#include "OSMutex.h"
#include "EventLoop.h"

class Timer;
class TimerMgr
{
public:   
    TimerMgr(EventLoop* pEventLoop,bool bExclusion = true);
    ~TimerMgr();
    static void CallBack(struct ev_loop *loop,ev_timer *w,int revents);
    void        RegTimer(Timer* pTimer);
    void        CancelTimeout(Timer* pTimer);
    void        Init();
    void        Process(struct ev_loop *loop,int revents);
private:
    EventLoop* m_pEventLoop;
    OSHeap     m_TimerHeap;
    OSMutex    m_heapMutex;
    ev_timer   m_timer;
    bool       m_bExclusion;//排他
private:
    TimerMgr(const TimerMgr& other);
    TimerMgr& operator=(const TimerMgr& other);
    Timer* GetTimeOutTimer(ev_tstamp now,int revents);
    friend class EventLoop;
};

#endif //__TimerMgr__h__