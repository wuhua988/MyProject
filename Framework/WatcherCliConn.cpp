#include "WatcherCliConn.h"
#include "MsgBlock.h"
#include "SocketUtils.h"
#include "Framework.h"

WatcherCliConn::WatcherCliConn(EventLoop* evLoop,Framework* pFw)
: WatcherTcpConn(evLoop,pFw)
{
    assert(pFw != NULL);
    m_connInfo.SetActive(true);
}

WatcherCliConn::~WatcherCliConn()
{
}

int WatcherCliConn::Open(void * arg)
{
    if(m_socket.GetSocketFD() == Socket::kInvalidFileDesc && OS_NoErr != m_socket.Open()){
        LOG_ERROR("Create Socket Failed!");
        return 1;
    }

    UInt32 theIPAddr = SocketUtils::ConvertStringToAddr(m_connInfo.GetRemoteAddr().c_str());
    OS_Error err = m_socket.Connect(theIPAddr, m_connInfo.GetRemotePort());
    if(err != OS_NoErr){
        m_connInfo.SetState(ConnInfo::STATUS_CONNECTING);
        StartTimer();
    } else {
        m_connInfo.SetState(ConnInfo::STATUS_CONNECTED);
    }

    if(m_socket.GetSocketFD() == Socket::kInvalidFileDesc){
        LOG_ERROR("WatcherCliConn::Open,Socket FD is Invalid,RemoteIP:%s,RemotePort:%d,ServerID:%u,ConnID:%u"
            ,m_connInfo.GetRemoteAddr().c_str(),m_connInfo.GetRemotePort(),GetServerID(),GetConnID());
        return 1;
    }

    ev_io_set(&m_watcher,m_socket.GetSocketFD(), EventLoop::EventMask_Write);
    StartWatcher();
    return 0;
}

int WatcherCliConn::HandleEvent(int revents, SOCKET handle)
{   
    MACRO_DEBUG(revents & EventLoop::EventMask_Error,"Error Event Occurrence ServerID:%u,ConnID:%d",GetServerID(),m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Read,"Read Event Occurrence ServerID:%u,ConnID:%d",GetServerID(),m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Write,"WRITE Event Occurrence ServerID:%u,ConnID:%d",GetServerID(),m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Timer,"TimeOut Event Occurrence ServerID:%u,ConnID:%d",GetServerID(),m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTING,"ServerID:%u,ConnID:%d State:STATUS_CONNECTING",GetServerID(),m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_DISCONNECT,"ServerID:%u,ConnID:%d State:STATUS_DISCONNECT",GetServerID(),m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTED,"ServerID:%u,ConnID:%d State:STATUS_CONNECTED",GetServerID(),m_uiConnID);

    if(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTING)
    {        
        int socket_result = m_socket.GetSocketError();
        MACRO_WARN(socket_result,"Socket Error ConnID:%d, Error Num:%d,Str:%s",m_uiConnID,socket_result,strerror(socket_result));
        if(socket_result == 0)
        {
            m_connInfo.SetState(ConnInfo::STATUS_CONNECTED);
            StopTimer();
            AddEvent(EventLoop::EventMask_Read);
            GetFramework()->OnConnCreated(&m_connInfo);
        }else if(socket_result != EINPROGRESS && 
            socket_result != EISCONN     &&
            socket_result != EINTR       && 
            socket_result != EALREADY)
        {
            GetFramework()->OnFailConnect(&m_connInfo);
            m_connInfo.SetState(ConnInfo::STATUS_DISCONNECT);  
            RemoveEvent(EventLoop::EventMask_Write);		
        }
    }
    else if(m_connInfo.GetState() == ConnInfo::STATUS_DISCONNECT)
    {
        return HandleReConnect();
    }
    else if(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTED)
    {
        WatcherTcpConn::HandleNormal(revents,handle);
    }else{
        LOG_WARN("-----UnDesired Place!-----");
        assert(false);
    }
    return 0;
}

int WatcherCliConn::HandleReConnect()
{
    MACRO_DEBUG(m_connInfo.IsActive(),"HandleReConnect ServerID:%u,ConnID:%d",GetServerID(),m_uiConnID);
    Close();
    return Open();
}

int WatcherCliConn::HandleError(OS_Error err)
{
    LOG_WARN("Client Connect,Error Number:%d,Error String:%s,ServerID:%u,ConnID:%u",err,strerror(err),GetServerID(),m_uiConnID);
    return HandleReConnect();
}

int WatcherCliConn::HandleTimeOut()
{
    LOG_WARN("HandleTimeOut,ConnID:%u",m_uiConnID);
    if(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTING ){    	
        LOG_WARN("Connect To IP:%s,Port:%d,Faild!,ServerID:%u,ConnID:%d",m_connInfo.GetRemoteAddr().c_str(),m_connInfo.GetRemotePort(),m_uiConnID);
        return HandleReConnect();
    }else{
        LOG_WARN("IP:%s,Port:%d,is Time Out!,ConnID:%d",m_connInfo.GetRemoteAddr().c_str(),m_connInfo.GetRemotePort(),m_uiConnID);
        return 1;
    } 
    return 0;
}

