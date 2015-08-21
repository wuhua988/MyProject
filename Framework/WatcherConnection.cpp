#include "WatcherConnection.h"

WatcherConnection::WatcherConnection(EventLoop* evLoop,Framework* pFw)
: WatcherBase(evLoop,pFw)
{
    m_connInfo.SetHandler(this);
    m_pTimer = new EventTimer(this,5,5);
}

WatcherConnection::~WatcherConnection()
{
    if(m_pTimer){
        if(m_pTimer->IsTiming())
            StopTimer();
        delete m_pTimer;
    }
}

std::string WatcherConnection::GetRemoteAddr()
{
    LOG_ERROR("WatcherConnection::GetRemoteAddr");
    return "";
}

UInt16 WatcherConnection::GetRemotePort()
{
    LOG_ERROR("WatcherConnection::GetRemotePort");
    return 0;
}

ConnInfo* WatcherConnection::GetConnInfo()
{
    return &m_connInfo;
}

void WatcherConnection::StartTimer()
{
    //LOG_DEBUG("StartTimer:0x%x",this);
    m_loop->RegTimer(m_pTimer);
}

void WatcherConnection::StopTimer()
{
   // LOG_DEBUG("StopTimer:0x%x",this);
    m_loop->CancelTimeout(m_pTimer);
}
