#include "Timer.h"
#include "log.h"

Timer::Timer()
{
    m_timerElem.SetEnclosingObject(this);
    SetTimer(1,0);
}

Timer::Timer(const Float64 dExpire,const Float64 dInterval)
{
    m_timerElem.SetEnclosingObject(this);
    SetTimer(dExpire,dInterval);
	LOG_DEBUG("TimerExp:%lf",dExpire);
}
//void Timer::WatcherCallback(struct ev_loop   * pLoop,
//                            struct ev_timer  * pWatcher,
//                            int                iREV)
//{
//	Timer *pThis = (Timer *)pWatcher->data;
//	pThis->Callback(iREV);
//}

void Timer::SetTimer(const Float64 dExpire,const Float64 dInterval)
{
    m_Interval = dInterval;  
    m_Expire = dExpire;
    m_timerElem.SetValue(dExpire);    
}

void Timer::SetExpire(const Float64 dExpire)
{
    m_Expire = dExpire;
    m_timerElem.SetValue(dExpire);
}

Float64 Timer::GetExpire()
{
    return m_Expire;
}

void Timer::SetInterval(const Float64 dInterval)
{
    m_Interval = dInterval;
}

Float64 Timer::GetInterval()
{
    return m_Interval;
}

bool Timer::Repeat()
{
    if(m_Interval > 0.)
    {
        return true;
    }
    return false;
}

bool Timer::IsTiming()
{
    return m_timerElem.IsMemberOfAnyHeap();
}

void Timer::ReSet()
{
    m_timerElem.SetValue(m_Expire);
}

Timer::~Timer()
{ 
}

