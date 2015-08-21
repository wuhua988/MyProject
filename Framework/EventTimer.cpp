#include "EventTimer.h"
#include "WatcherBase.h"
#include "log.h"
#include "WatcherMgr.h"

EventTimer::EventTimer(WatcherBase* pHandler,const Float64 dExpire,const Float64 dInterval)
:Timer(dExpire,dInterval)
{
    assert(pHandler);
    m_pHandler = pHandler;
	//if (m_timerElem.IsMemberOfAnyHeap())
        //sIdleThread->CancelTimeout(this);
}

void EventTimer::Callback(const UInt32 revents)
{
    if(m_pHandler->HandleEvent(revents))
    {
        LOG_NOTICE("The ConnID:%d,Is Close Because Of TimeOut!",m_pHandler->GetConnID());
        gWatcherMgrInst::instance()->RemoveWatcher(m_pHandler->GetConnID());
        return;
    }
}