#include "WatcherNoticePipe.h"
#include "WatcherListener.h"
#include "WatcherTcpConn.h"
#include "EventBaseThread.h"
#include "WatcherMgr.h"
#include "ClientConnectionMgr.h"


WatcherNoticePipe::WatcherNoticePipe(EventLoop* evLoop,Framework* pFw) 
: WatcherBase(evLoop,pFw)
{
    RegCallBack();
	InitPipe();
	m_noticeTail = NULL;
	m_noticeHead = NULL;
}

WatcherNoticePipe::~WatcherNoticePipe()
{
    Socket::Close(m_readFD);
    Socket::Close(m_writeFD);
	OSMutexLocker theLocker(&m_Mutex);
	while(m_noticeHead != NULL)
	{
		Notice* next = m_noticeHead->next;
		delete m_noticeHead;
		m_noticeHead = next;
	}
}

int WatcherNoticePipe::Open (void * arg)
{
    ev_io_set(&m_watcher, m_readFD, EV_READ);
    StartWatcher();
    return 0;
}

int WatcherNoticePipe::HandleEvent(int event, SOCKET handle)
{
    char data[1];
    if (1 != ::read(m_readFD, data, 1))
    {//pipe is closed 
        LOG_ERROR("Failure to read the notice pipe, errno=%d", errno);
    }
    ProcessNotices();
    return 0;
}

int WatcherNoticePipe::Close(SOCKET handle)
{
    return 0;
}

void WatcherNoticePipe::InitPipe()
{
    int pipefd[2];
	if (pipe(pipefd) == -1) {
        LOG_ERROR("init pipe error!");
        exit(EXIT_FAILURE);
    }

    Socket::InitNonBlocking(pipefd[0]);
    Socket::InitNonBlocking(pipefd[1]);
	m_readFD = pipefd[0];
	m_writeFD = pipefd[1];
}

void WatcherNoticePipe::ProcessNotices()
{
    OSMutexLocker theLocker(&m_Mutex);
	while(m_noticeHead != NULL)
	{
        if( m_noticeCB[m_noticeHead->type] != NULL)
            m_noticeCB[m_noticeHead->type](m_noticeHead);
		Notice* next = m_noticeHead->next;
		delete m_noticeHead;
		m_noticeHead = next;
	}
}

void WatcherNoticePipe::RegCallBack()
{
    m_noticeCB[NOTICE_EVENT] = &WatcherNoticePipe::EventCB;
    m_noticeCB[NOTICE_TCP_CONN] = &WatcherNoticePipe::TcpConnCB;
	m_noticeCB[NOTICE_TCP_CLIENT] = &WatcherNoticePipe::TcpConnClientCB;
    m_noticeCB[NOTICE_LISTEN]= &WatcherNoticePipe::ListenCB;
	m_noticeCB[NOTICE_CLOSE_CONN] = &WatcherNoticePipe::CloseConnCB;
}

void WatcherNoticePipe::EventCB(Notice* notice)
{
    SharedPtr<WatcherBase> pShareObj = GetWatcherByNoticeConnID(notice);
    if(pShareObj.isNull())
        return;
    pShareObj->AddEvent(EventLoop::EventMask_Write);
    LOG_DEBUG("WatcherNoticePipe:EventCB,Set The ConnID:%d Write Event",pShareObj->GetConnID());
}

void WatcherNoticePipe::TcpConnCB(Notice* notice)
{
    SharedPtr<WatcherTcpConn> pWatcher = (WatcherTcpConn*)notice->args;
    LOG_DEBUG("WatcherNoticePipe:TcpConnCB");
    if(pWatcher.isNull())
    {
        LOG_ERROR("WatcherNoticePipe:TcpConnCB,notice->args is NULL");
        return;
    }
    if(!gWatcherMgrInst::instance()->RegWatcher(pWatcher))
    {
        LOG_ERROR("Add Tcp Conn To WatcherMgr Failed!");
		return;
    }
	
	//有引起死锁的可能
	//if(!pWatcher->GetConnInfo()->IsActive())
	//    pWatcher->GetFramework()->OnConnCreated(pWatcher->GetConnInfo());    
}

void WatcherNoticePipe::TcpConnClientCB(Notice* notice)
{
    SharedPtr<WatcherCliConn> pWatcher = (WatcherCliConn*)notice->args;
    LOG_DEBUG("WatcherNoticePipe:TcpConnClientCB");
    if(pWatcher.isNull())
    {
        LOG_ERROR("WatcherNoticePipe:TcpConnCB,notice->args is NULL");
        return;
    }
    if(!gWatcherMgrInst::instance()->RegWatcher(pWatcher))
    {
        LOG_ERROR("Add Tcp Client Conn To ConnMgr Failed!");
		return;
    }   
}

void WatcherNoticePipe::ListenCB(Notice* notice)
{
    WatcherListener* pWatcher = (WatcherListener*)notice->args;
    LOG_DEBUG("WatcherNoticePipe:ListenCB");
    if(pWatcher == NULL)
    {
        LOG_ERROR("WatcherNoticePipe:ListenCB,notice->args is NULL");
        return;
    }

    if(!gWatcherMgrInst::instance()->RegWatcher(pWatcher))
    {
        LOG_ERROR("Register PipeLine Handler Error!");
        delete pWatcher;
    }     
}

void WatcherNoticePipe::CloseConnCB(Notice* notice)
{    
    /*PointerSizedUInt pConnID = (PointerSizedUInt)notice->args;
    LOG_DEBUG("WatcherNoticePipe:CloseConnCB,ConnID:%d",pConnID);
    gWatcherMgrInst::instance()->RemoveWatcher(pConnID);*/
	assert(notice != NULL);
	SharedPtr<WatcherBase> watchObj = GetWatcherByNoticeConnID(notice);

	if(watchObj.isNull())
		return;
	
	CheckPending_t* pCP = new CheckPending_t;
	assert(pCP != NULL);
	pCP->type = WatcherCheck::PENDING_CLOSE_CONN;
	pCP->serverID = notice->serverID;
	pCP->args = notice->args;
	
	watchObj->GetThread()->GetEvLoop()->AddCheckPending(pCP);
}

bool WatcherNoticePipe::AddNotice(Notice* pNotice)
{
    OSMutexLocker theLocker(&m_Mutex);
	if(!pNotice){
	    LOG_ERROR("The Notice Is NULL!");
	    return false;
	}

	if (1 != ::write(m_writeFD, "n", 1)){
        LOG_ERROR("Failure to write msg to notice pipe, errno=%d", errno);  
        delete pNotice	;
        return false;        
    } else {
		pNotice->next = m_noticeHead;
	    m_noticeHead = pNotice;
	}
	return true;
}

SharedPtr<WatcherBase> WatcherNoticePipe::GetWatcherByNoticeConnID(Notice* notice)
{
	assert(notice != NULL);
	SharedPtr<WatcherBase> watchObj;
	PointerSizedUInt pConnID = (PointerSizedUInt)notice->args;
    if(notice->serverID == 0)
		watchObj= gWatcherMgrInst::instance()->GetWatcher(pConnID);
	else
		watchObj = gCliConnMgr::instance()->GetCliConn(notice->serverID,pConnID);
	if(watchObj.isNull())
	{
		LOG_ERROR("Can't Find Wacher ServerID:%u,ConnID:%u",notice->serverID,pConnID);
	}
	return watchObj;
}

void WatcherNoticePipe::AddNoticeToPipe(EventBaseThread* pThread,UInt32 type,UInt32 svrid,void* arg)
{
    assert(pThread != NULL);
    assert(arg != NULL);
    Notice* pNotice = new Notice();
	pNotice->args = arg;
	pNotice->type = type;
	pNotice->serverID = svrid;
	pThread->AddNotice(pNotice);
}