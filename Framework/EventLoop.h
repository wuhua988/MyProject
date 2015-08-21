#ifndef __EventLoop_h__
#define __EventLoop_h__
#include "ev.h"
#include "WatcherCheck.h"

class TimerMgr;
class Timer;
class EventLoop
{
public:
    enum EventMask {
        EventMask_Undefined = EV_UNDEF,
        EventMask_None      = EV_NONE,
        EventMask_Read      = EV_READ,
        EventMask_Write     = EV_WRITE,
        EventMask_Timer     = EV_TIMER,
        EventMask_Periodic  = EV_PERIODIC,
        EventMask_Signal    = EV_SIGNAL,
        EventMask_Child     = EV_CHILD,
        EventMask_Stat      = EV_STAT,
        EventMask_Idle      = EV_IDLE,
        EventMask_Check     = EV_CHECK,
        EventMask_Prepare   = EV_PREPARE,
        EventMask_Fork      = EV_FORK,
        EventMask_Async     = EV_ASYNC,
        EventMask_Embed     = EV_EMBED,
        EventMask_Error     = EV_ERROR
    };
	
    EventLoop();
    ~EventLoop();
    void            RegTimer(Timer* pTimer);
	void            AddCheckPending(CheckPending_t* pCPending);
    void            CancelTimeout(Timer* pTimer);
	void            SetTimerMgrExclusion(bool bExclusion);
    struct ev_loop* GetEvLoop();
    
    void            Run();
    void            Stop();
    void            RunOnce();
    const char*     GetEventMaskStr(int mask);    
private:
    TimerMgr*       m_pTimerMgr;
    struct ev_loop* m_loop;
	WatcherCheck*   m_pCheck;
};

inline const char* EventLoop::GetEventMaskStr(int mask)
{
    switch(mask)
    {
        case EventMask_Undefined:
            return "EventMask_Undefined";
        case EventMask_Read:
           return "EventMask_Read";
        case EventMask_Write:
            return "EventMask_Write";
        case EventMask_Timer:
            return "EventMask_Timer";
    }
    return NULL;
}

#endif //__EventLoop_h__