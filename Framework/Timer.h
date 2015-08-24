#ifndef __Timer_h__
#define __Timer_h__
#include <assert.h>
#include "OSHeaders.h"
#include "OSHeap.h"

class Timer
{
public:
    //repeat and after are milliseconds
    Timer();
    virtual ~Timer();
    Timer(const Float64 dExpire,const Float64 dInterval);
	
    void         SetTimer(const Float64 dExpire,const Float64 dInterval);
    void         SetExpire(const Float64 dExpire);
    Float64      GetExpire();
    void         SetInterval(const Float64 dInterval);
    Float64      GetInterval();
	virtual void Callback(const UInt32 revents) = 0;
    bool         Repeat();
    bool         IsTiming();//是否在定时中
    void         ReSet();
private:
    // Does not exist
    Timer(const Timer& other);
    Timer& operator = (const Timer & other);

protected:
    OSHeapElem m_timerElem;
    Float64    m_Expire;
    Float64    m_Interval;
    //static void WatcherCallback(struct ev_loop   * pLoop,
    //                            struct ev_timer  * pWatcher,
    //                            int                iREV);
    
    friend class TimerMgr;
};

#endif