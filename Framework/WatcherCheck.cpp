#include "WatcherCheck.h"
#include <assert.h>
#include "EventLoop.h"
#include "log.h"
#include "OSHeaders.h"
#include "WatcherMgr.h"
#include "ClientConnectionMgr.h"

WatcherCheck::WatcherCheck(EventLoop* evLoop) 
: m_loop(evLoop)
{
	assert(evLoop != NULL);
	m_CheckPendingHead = NULL;
	ev_check_init(&m_check,WatcherCheck::Callback);	
	m_check.data = this;
}

WatcherCheck::~WatcherCheck()
{
	ev_check_stop(m_loop->GetEvLoop(),&m_check);
	while(m_CheckPendingHead != NULL)
	{
		CheckPending_t* next = m_CheckPendingHead->next;
		delete m_CheckPendingHead;
		m_CheckPendingHead = next;
	}
}

int WatcherCheck::Open (void * arg)
{
    ev_check_start(m_loop->GetEvLoop(),&m_check);
    return 0;
}

int WatcherCheck::HandleEvent(int event)
{
    ProcessCheckPending();
    return 0;
}

int WatcherCheck::Close()
{
	ev_check_stop(m_loop->GetEvLoop(),&m_check);
    return 0;
}

void WatcherCheck::ProcessCheckPending()
{
	while(m_CheckPendingHead != NULL)
	{
		switch(m_CheckPendingHead->type)
		{
		case PENDING_CLOSE_CONN:
		    NoticeFrameCloseConn(m_CheckPendingHead);
		break;
		default:
		break;
		}
		CheckPending_t* next = m_CheckPendingHead->next;
		delete m_CheckPendingHead;
		m_CheckPendingHead = next;
	}
}

void WatcherCheck::AddCheckPending(CheckPending_t* pCPending)
{
	assert(pCPending != NULL);
	if(!pCPending)
		return;
	pCPending->next = m_CheckPendingHead;
	m_CheckPendingHead = pCPending;
}

void WatcherCheck::Callback(struct ev_loop* loop, struct ev_check* instance, int revents)
{
	LOG_ERROR("Check Event!");
	WatcherCheck* pCheckWatcher = (WatcherCheck*)instance->data;
    pCheckWatcher->HandleEvent(revents);
}

void WatcherCheck::NoticeFrameCloseConn(CheckPending_t* pCPending)
{
	assert(pCPending != NULL);
	PointerSizedUInt pConnID = (PointerSizedUInt)pCPending->args;
    LOG_DEBUG("CloseConnCB,ServerID:%u,ConnID:%d",pCPending->serverID,pConnID);
	if(pCPending->serverID == 0)
        gWatcherMgrInst::instance()->RemoveWatcher(pConnID);
	else
		gCliConnMgr::instance()->Remove(pConnID);
}
