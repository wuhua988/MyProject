#include "WatcherBase.h"
#include "EventBaseThread.h"
#include "Framework.h"
#include "ClientConnectionMgr.h"

WatcherBase::WatcherBase()
{

} 

WatcherBase::WatcherBase(EventLoop* evLoop,Framework* pFw) 
: m_loop(evLoop)
, m_pThread(NULL)
, m_uiConnID(0)
, m_uiServerID(0)
, m_bIsClose(false)
, m_pFrameWork(pFw)
{
    //if(evLoop == NULL)
    //{
    //	LOG_ERROR("ev_loop Is NULL!");
    //	exit(EXIT_FAILURE);
    //}

    ev_init(&m_watcher, WatcherBase::Callback);
    m_watcher.data = this;
    m_pThread = NULL;
}


WatcherBase::~WatcherBase()
{
    StopWatcher();
    assert(!ev_is_active(&m_watcher));
    assert(!ev_is_pending(&m_watcher));
    Close(0);
    LOG_DEBUG("Oh My God!!! I'm dead!!! My ConnID:%u",m_uiConnID);
} 

void WatcherBase::StartWatcher() {
    if (!ev_is_active(&m_watcher)){
        ev_io_start(m_loop->GetEvLoop(), &m_watcher);
        // LOG_DEBUG("IO Watcher Start Working:0x%x",&m_watcher);
    }
}

void WatcherBase::StopWatcher() {
    if (ev_is_active(&m_watcher)) {
        ev_io_stop(m_loop->GetEvLoop(), &m_watcher);
        // LOG_DEBUG("IO Watcher Stop Working:0x%x",&m_watcher);
    }
}

int WatcherBase::Close(SOCKET handle)
{
    return 0;
}

void WatcherBase::SetEvent(int events)
{
    bool active = IsActive();
    if (active)
        StopWatcher();
    ev_io_set(&m_watcher, m_watcher.fd, events);
    if (active)
        StartWatcher();
}

void WatcherBase::AddEvent(int events)
{
    if (m_watcher.events & events)
        return;
    m_watcher.events |= events;
    SetEvent(m_watcher.events);
}

void WatcherBase::RemoveEvent(int events)
{
    if(!(m_watcher.events & events))
        return;
    m_watcher.events &= ~events;
    SetEvent(m_watcher.events);    
}

bool WatcherBase::IsActive() const
{
    return ev_is_active(&m_watcher) != 0;
}

bool WatcherBase::IsPending() const
{ 
    return ev_is_pending(&m_watcher) != 0;
}

void WatcherBase::SetHandle(SOCKET handle)
{
    //m_handle = handle;
}

SOCKET WatcherBase::GetHandle()
{
    return -1;
}

void WatcherBase::SetConnID(UInt32 connID)
{
    m_uiConnID = connID;
}

UInt32 WatcherBase::GetConnID()
{
    return m_uiConnID;
}

void WatcherBase::SetServerID(UInt32 serverID)
{
    m_uiServerID = serverID;
}

UInt32 WatcherBase::GetServerID()
{
    return m_uiServerID;
}

void WatcherBase::SetThread(EventBaseThread* pThread)
{
    assert(pThread != NULL);
    m_pThread = pThread;
}

EventBaseThread* WatcherBase::GetThread()
{
    return m_pThread;
}

Framework* WatcherBase::GetFramework()
{
    return m_pFrameWork;
}

void WatcherBase::Callback(EV_P_ ev_io *w, int revents)
{
    /*UInt32 connID = (UInt32)w->data;
    SharedPtr<WatcherBase> pSharedHandler = gHandlerMgrInst.GetHandler(connID);
    if(pSharedHandler.isNull()){
    LOG_NOTICE("Can't Find Handler By ConnID:%u",connID);
    }

    int ret = pSharedHandler->HandleEvent(revents,w->fd);*/

    WatcherBase* pWatcher = (WatcherBase*)w->data;
    int ret = pWatcher->HandleEvent(revents,w->fd);
    if(ret){
        if(pWatcher->GetServerID() == 0)
            gWatcherMgrInst::instance()->RemoveWatcher(pWatcher->GetConnID());
        else
            gCliConnMgr::instance()->Remove(pWatcher->GetConnID());
    }
}
