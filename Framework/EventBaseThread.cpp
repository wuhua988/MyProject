#include "EventBaseThread.h"
#include "Framework.h"

EventBaseThread::EventBaseThread(Framework* pFw)
{
    assert(pFw != NULL);
    m_pFrameWork = pFw;
}

EventBaseThread::~ EventBaseThread()
{
    if(m_pipeWatcher != NULL)
    {
        gWatcherMgrInst::instance()->RemoveWatcher(m_pipeWatcher->GetConnID());
        delete m_pipeWatcher;
    }
}

void EventBaseThread::Init()
{
	m_pipeWatcher = new WatcherNoticePipe(&m_loop,m_pFrameWork);

    if(!gWatcherMgrInst::instance()->RegWatcher(m_pipeWatcher))
    {
        LOG_ERROR("Register PipeLine Handler Error!");
    }
}

EventLoop* EventBaseThread::GetEvLoop()
{
    return &m_loop;
}

void EventBaseThread::Entry()
{
    m_loop.Run();
}

bool EventBaseThread::AddNotice(Notice* pNotice)
{
    return m_pipeWatcher->AddNotice(pNotice);
}

