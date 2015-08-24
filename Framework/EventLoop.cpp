#include <assert.h>
#include "EventLoop.h"
#include "TimerMgr.h"
#include "log.h"
#include "WatcherMgr.h"

EventLoop::EventLoop()
{    
    m_loop = (struct ev_loop*)ev_loop_new(EVFLAG_AUTO);
    assert(m_loop != NULL);
    m_pTimerMgr = new TimerMgr(this);
    assert(m_pTimerMgr != NULL);
    m_pTimerMgr->Init();
	m_pCheck = new WatcherCheck(this);
	m_pCheck->Open();
}

EventLoop::~EventLoop()
{
    if(m_pTimerMgr)
        delete m_pTimerMgr;

	delete m_pCheck;
    ev_loop_destroy(m_loop);
}

void EventLoop::Run()
{
    ev_run(m_loop, 0);
}

void EventLoop::RunOnce()
{
    ev_run(m_loop, EVRUN_ONCE);
}

void EventLoop::Stop()
{
    ev_break(m_loop, EVBREAK_ALL);
}

void EventLoop::RegTimer(Timer* pTimer)
{
    m_pTimerMgr->RegTimer(pTimer);
}

void EventLoop::SetTimerMgrExclusion(bool bExclusion)
{
    m_pTimerMgr->m_bExclusion = bExclusion;
}

void EventLoop::CancelTimeout(Timer* pTimer)
{
    m_pTimerMgr->CancelTimeout(pTimer);
}

void EventLoop::AddCheckPending(CheckPending_t* pCPending)
{
    m_pCheck->AddCheckPending(pCPending);
}

struct ev_loop* EventLoop::GetEvLoop()
{
    return m_loop;
}