#include "WatcherListener.h"
#include "Framework.h"
#include "EventWorkThread.h"
#include "WatcherTcpConn.h"
#include "EventThreadPool.h"
#include "SocketUtils.h"

WatcherListener::WatcherListener(const char* host, UInt16 port,EventLoop* evLoop,Framework* pFw) : WatcherBase(evLoop,pFw)
{  
    assert(pFw != NULL);
    UInt32 theIPAddr = SocketUtils::ConvertStringToAddr(host);
    if(OS_NoErr != m_socket.Initialize(theIPAddr,port))
    {
        LOG_ERROR("Initialize Listener failed!,IP:%s,Port:%d",host,port);
    }
}

WatcherListener::~WatcherListener()
{
   
}

int WatcherListener::Open (void * arg)
{ 
	ev_io_set(&m_watcher, m_socket.GetSocketFD(), EV_READ);
    StartWatcher();
	char* addrStr = m_socket.GetLocalAddrStr()->GetAsCString();
	if(addrStr)
	{
	    LOG_NOTICE("Lister IP:%s,Port:%u, Successed!",addrStr,m_socket.GetLocalPort());
	    delete addrStr;
	}
    return 0;
}

int WatcherListener::HandleEvent(int revents, SOCKET handle)
{
	if(revents & EV_READ)
	{
        int fd;
        struct sockaddr_in addr;
        int ret = m_socket.Accept(fd,addr);
        if(ret == -1)
        {
            LOG_NOTICE("Accept New Connection,FD:%d",fd);
            EventWorkThread* pThread = EventThreadPool::GetRandomWorkThread();
	        WatcherTcpConn* pWatcher = new WatcherTcpConn(pThread->GetEvLoop(),m_pFrameWork);
            
            pWatcher->GetSocket()->Set(fd, &addr);
            pWatcher->GetSocket()->InitNonBlocking();
            pWatcher->GetConnInfo()->SetState(ConnInfo::STATUS_CONNECTED);
			pWatcher->GetConnInfo()->SetServerID(0);
            
	        pWatcher->SetThread(pThread);
            Notice* pNotice = new Notice;
            pNotice->args = pWatcher;
			pNotice->serverID = 0;
	        pNotice->type = WatcherNoticePipe::NOTICE_TCP_CONN;
	        pThread->AddNotice(pNotice);
        } else {
            LOG_ERROR("WatcherListener::HandleEvent,Accept Error:%d",ret);
            return 1;
        }
	}
    return 0;
}

int WatcherListener::Close(SOCKET handle)
{
    m_socket.Close();
    m_bIsClose = true;
    m_uiConnID = 0;
    return 0;
}

SOCKET WatcherListener::GetHandle()
{
    return m_socket.GetSocketFD();
}