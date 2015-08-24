#include <assert.h>
#include "log.h"
#include "TimerMgr.h"
#include "Timer.h"

TimerMgr::TimerMgr(EventLoop* pEventLoop,bool bExclusion)
:m_pEventLoop(pEventLoop)
,m_bExclusion(bExclusion)
{
    assert(pEventLoop != NULL); 
    ev_init(&m_timer, CallBack);
    m_timer.data = this;
    //CallBack(pEventLoop->GetEvLoop(), &m_timer, 0);
}

TimerMgr::~TimerMgr()
{
    ev_timer_stop(m_pEventLoop->GetEvLoop(), &m_timer);
}

void TimerMgr::Init()
{
    ev_timer_set(&m_timer, 2.0, 2.);
    ev_timer_start(m_pEventLoop->GetEvLoop(), &m_timer);
}

void TimerMgr::CancelTimeout(Timer* pTimer)
{
    Assert(pTimer != NULL);
    if(m_bExclusion)
        m_heapMutex.Lock();
    m_TimerHeap.Remove(&pTimer->m_timerElem);
    if(m_bExclusion)
        m_heapMutex.Unlock();
    pTimer->ReSet();    
}

void TimerMgr::RegTimer(Timer* pTimer)
{
    assert(pTimer != NULL);
    if (pTimer->m_timerElem.IsMemberOfAnyHeap())
    {
        LOG_NOTICE("Register Repeat Timer!");
        return;
    }

    if(m_bExclusion)
        m_heapMutex.Lock();
    pTimer->m_timerElem.SetValue(ev_time() + pTimer->GetExpire());
    //LOG_DEBUG("pTimer->Expire:%f,ElemValue:%ld",pTimer->GetExpire(),pTimer->m_timerElem.GetValue());
    m_TimerHeap.Insert(&pTimer->m_timerElem);
    if(m_bExclusion)
        m_heapMutex.Unlock();
}

Timer* TimerMgr::GetTimeOutTimer(ev_tstamp now,int revents)
{
    Timer* pTimer = NULL;
    if(m_bExclusion)
        m_heapMutex.Lock();
    if((m_TimerHeap.CurrentHeapSize() > 0) && (m_TimerHeap.PeekMin()->GetValue() <= now))
    {
        //Timer* tmp = (Timer*)m_TimerHeap.PeekMin()->GetEnclosingObject();
        //LOG_DEBUG("TimerHeap->Value:%ld,TimerHeap->Expire:%f"
        //         ,m_TimerHeap.PeekMin()->GetValue()
        //		   ,tmp->GetExpire());
        pTimer = (Timer*)m_TimerHeap.ExtractMin()->GetEnclosingObject();	
        pTimer->Callback(revents);		
    }
    if(m_bExclusion)
        m_heapMutex.Unlock();
    return pTimer;
}

void TimerMgr::Process(struct ev_loop *loop,int revents)
{
    ev_tstamp after = 5;
    ev_tstamp now = ev_now(loop);	
    Timer* pTimer = NULL;
    LOG_DEBUG("TimerMgrItemSize->Size:%u",m_TimerHeap.CurrentHeapSize());
    while (1)
    {		
        pTimer = GetTimeOutTimer(now,revents);
        if(!pTimer)
            break;
        /* if the timer is repeated, put it to the heap again */
        if(pTimer->Repeat()){
            RegTimer(pTimer);
        }
        //pTimer->Callback(revents);
    }

    if(m_bExclusion)
        m_heapMutex.Lock();
    if(m_TimerHeap.CurrentHeapSize() > 0)
        after = m_TimerHeap.PeekMin()->GetValue() - now;
    if(m_bExclusion)
        m_heapMutex.Unlock();

    LOG_DEBUG("Timer After:%f",after);
    ev_timer_set(&m_timer, after, 0.);
    ev_timer_start(loop, &m_timer);
}

void TimerMgr::CallBack(struct ev_loop *loop, ev_timer *w, int revents)
{    
    TimerMgr* pTimerMgr = (TimerMgr*)w->data;
    assert(pTimerMgr != NULL);
    pTimerMgr->Process(loop,revents);
}
