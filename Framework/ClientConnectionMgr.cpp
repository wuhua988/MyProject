#include <sstream>
#include "ClientConnectionMgr.h"
#include "MsgBlock.h"
#include "EventThreadPool.h"
#include "WatcherNoticePipe.h"
#include "WatcherNoticePipe.h"
#include "EventWorkThread.h"
#include "Framework.h"

ClientHost::ClientHost(const char* strHost, UInt16 uiPort, UInt32 uiServerID)
: m_strHostIP(strHost)
, m_uiPort(uiPort)
, m_uiServerID(uiServerID)
, m_fExpire(2)
, m_fInterval(2)
{
    std::ostringstream oss;
    oss << strHost << ':' << uiPort;
    m_strServerIdent = oss.str();
}

ClientHost::~ClientHost()
{

}

bool ClientHost::AddCliConn(UInt32 uiConnID)
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    m_clientConns.push_back(uiConnID);
    return true;
}

UInt32  ClientHost::GetServerID()
{
    return m_uiServerID;
}

bool ClientHost::Close()
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    vecClientConn::iterator iter = m_clientConns.begin();
    for(; iter != m_clientConns.end(); iter++)
    {
        WatcherCliConnPtr ccPtr = ClientConnMgr::GetCliConnPtr(m_uiServerID,*iter);
        if(ccPtr.isNull())
        {
            continue;
        }
        WatcherNoticePipe::AddNoticeToPipe(ccPtr->GetThread()
                                          ,WatcherNoticePipe::NOTICE_CLOSE_CONN
                                          ,m_uiServerID
                                          ,(void*)(PointerSizedUInt)(*iter));	
    }
    return true;
}

bool ClientHost::Close(UInt32 connID)
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    WatcherCliConnPtr ccPtr = ClientConnMgr::GetCliConnPtr(m_uiServerID,connID);
    if(ccPtr.isNull()){
        return false;
    }
    PointerSizedUInt pConnID = connID;
    WatcherNoticePipe::AddNoticeToPipe(ccPtr->GetThread()
                                      ,WatcherNoticePipe::NOTICE_CLOSE_CONN
                                      ,m_uiServerID
                                      ,(void*)pConnID);	
    return true;
}

bool ClientHost::Remove(UInt32 uiConnID)
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    vecClientConn::iterator iter = m_clientConns.begin();
    for(; iter != m_clientConns.end(); iter++)
    {
        if((*iter) == uiConnID)
        {
            m_clientConns.erase(iter);
            return true;
        }
    }
    return false;
}

void ClientHost::SetTimerInfo(Float64 fExpire,Float64 fInterval)
{
    if(fExpire <= 0)
        m_fExpire = 2;
    else
        m_fExpire = fExpire;

    if(fInterval <= 0)
        m_fInterval = 2;
    else
        m_fInterval = fInterval;
}

bool ClientHost::GetCliConnID(UInt32& uiConnID)
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    if(m_clientConns.size() == 0)
        return false;
    static int last_thread = - 1;
    int index = (last_thread + 1) % m_clientConns.size();
    uiConnID = m_clientConns[index];
    last_thread = index;
    return true;
}


ClientHost::clientSize ClientHost::GetConnCount()
{
    OSMutexLocker theLocker(&m_ConnsMutex);	
    return m_clientConns.size();
}

//----------HostService Class Begin------------------------------------------------------------------------------//

HostService::HostService(UInt32 uiServerID)
:m_uiServerID(uiServerID)
{

}

HostService::~HostService()
{

}

bool HostService::AddCliConn(const char* strHost
                            ,UInt16 uiPort
                            ,Float64 fExpire
                            ,Float64 fInterval)
{
    std::string strIdent(strHost);
    std::ostringstream oss;
    oss << strHost << ':' << uiPort;
    OSMutexLocker theLocker(&m_HostMutex);
    mapStr2Host::iterator iter = m_clientHost.find(oss.str());
    if(iter == m_clientHost.end())
    {
        ClientHostPtr ch = new ClientHost(strHost,uiPort,m_uiServerID);
        ch->SetTimerInfo(fExpire,fInterval);
        m_clientHost[oss.str()] = ch;
        LOG_DEBUG("Add Host:%s",oss.str().c_str());
    }	
    return true;
}

bool HostService::AddCliConnToHost(WatcherCliConnPtr pClientConn)
{
    if(pClientConn.isNull())
    {
        return false;
    }
    OSMutexLocker theLocker(&m_HostMutex);
    mapStr2Host::iterator iter = m_clientHost.find(pClientConn->GetConnInfo()->GetConnIdent());
    if(iter == m_clientHost.end())
    {
        LOG_ERROR("Can't Find Host In Service,ServerID:%u,ConnID:%u,Ident:%s"
                 ,pClientConn->GetServerID()
                 ,pClientConn->GetConnID()
                 ,pClientConn->GetConnInfo()->GetConnIdent().c_str());
        return false;
    }	

    iter->second->AddCliConn(pClientConn->GetConnID());
    return true;
}

HostService::hostSize HostService::GetHostCount()
{
    OSMutexLocker theLocker(&m_HostMutex);
    return m_clientHost.size();
}

bool HostService::GetCliConnID(UInt32& uiConnID)
{
    OSMutexLocker theLocker(&m_HostMutex);
    mapStr2Host::iterator iter = m_clientHost.begin();
    if(iter == m_clientHost.end())
        return false;
    static int last_host = - 1;
    int index = (last_host + 1) % m_clientHost.size();
    advance(iter,index);
    bool bOK = iter->second->GetCliConnID(uiConnID);
    last_host = index;
    return bOK;
}

UInt32 HostService::GetServerID()
{
    return m_uiServerID;
}

bool HostService::Remove(WatcherCliConnPtr pClientConn)
{
    OSMutexLocker theLocker(&m_HostMutex);
    std::string strIdent = pClientConn->GetConnInfo()->GetConnIdent();
    mapStr2Host::iterator iter = m_clientHost.find(strIdent);
    if(iter == m_clientHost.end())
    {
        LOG_ERROR("Can't Find Client Host By Ident:%s",strIdent.c_str());
    }else{
        iter->second->Remove(pClientConn->GetConnInfo()->GetConnID());
        if(iter->second->GetConnCount() == 0)
            m_clientHost.erase(iter);
    }
    return true;
}

bool HostService::Close()
{
    OSMutexLocker theLocker(&m_HostMutex);
    mapStr2Host::iterator iter = m_clientHost.begin();
    for(;iter != m_clientHost.end();iter++)
    {
        iter->second->Close();
    }
    return true;
}

bool HostService::Close(UInt32 connID)
{
    WatcherCliConnPtr ccPtr = ClientConnMgr::GetCliConnPtr(m_uiServerID,connID);
    if(ccPtr.isNull()){
        return false;
    }
    OSMutexLocker theLocker(&m_HostMutex);
    mapStr2Host::iterator iter = m_clientHost.find(ccPtr->GetConnInfo()->GetConnIdent());
    if(iter == m_clientHost.end())
    {
        LOG_ERROR("Can't Find Host Form ClientHostMap,Ident:%s",ccPtr->GetConnInfo()->GetConnIdent().c_str());
        return false;
    }

    iter->second->Close(connID);
    return true;
}

//----------HostService Class End-------------------------------------------------------------------------------//


//----------ClientConnMgr Class Begin------------------------------------------------------------------------------//

ClientConnMgr::ClientConnMgr()
:m_pFramework(NULL)
{
}

ClientConnMgr::ClientConnMgr(Framework* pFw)
:m_pFramework(pFw)
{
    assert(pFw);
}

ClientConnMgr::~ClientConnMgr()
{

}

void ClientConnMgr::SetFramework(Framework* pFw)
{
    assert(pFw);
    m_pFramework = pFw;
}

bool ClientConnMgr::Connect(UInt32  uiServerID
                           ,const   char* strHost
                           ,UInt16  uiPort
                           ,UInt32  uiConnNum
                           ,Float64 fExpire
                           ,Float64 fInterval)
{
    bool isOK = true;
    {
        OSMutexLocker theLocker(&m_SvrID2Server_Mutex);
        mapSvrID2Service::iterator iter = m_SvrID2Service.find(uiServerID); 
        HostServicePtr hsPtr;       
        if(iter == m_SvrID2Service.end())
        {
            hsPtr = new HostService(uiServerID);
            m_SvrID2Service[uiServerID] = hsPtr;
        }else{
            hsPtr = iter->second;
        }

        if(hsPtr.isNull())
        {
            LOG_ERROR("HostService Is NULL,ServerID:%u,Host:%s,Port:%d",uiServerID,strHost,uiPort);
            return false;
        }
        isOK = hsPtr->AddCliConn(strHost,uiPort,fExpire,fInterval);	
    }
    std::ostringstream oss;
    for(UInt32 i = 0; i < uiConnNum; i++)
    {
        EventWorkThread* pThread = EventThreadPool::GetRandomWorkThread();
        if(!pThread)
        {
            LOG_ERROR("Get Null Thread From EventWorkThreadPool When Client Connect,Host:%s,Port:%d",strHost,uiPort);
            return false;
        }
        WatcherCliConn* pWatcher = new WatcherCliConn(pThread->GetEvLoop(),m_pFramework);
        pWatcher->SetThread(pThread);
        pWatcher->GetConnInfo()->SetRemoteAddr(std::string(strHost));
        pWatcher->GetConnInfo()->SetRemotePort(uiPort);
        pWatcher->GetConnInfo()->SetInterval(fInterval);
        pWatcher->GetConnInfo()->SetExpire(fExpire);
        pWatcher->GetConnInfo()->SetServerID(uiServerID);

        oss.str("");
        oss << strHost << ':' << uiPort;
        pWatcher->GetConnInfo()->SetConnIdent(oss.str());

        WatcherNoticePipe::AddNoticeToPipe(pThread,WatcherNoticePipe::NOTICE_TCP_CLIENT,uiServerID,pWatcher);
    }	

    return isOK;
}

void ClientConnMgr::Close()
{
    OSMutexLocker theLocker(&m_SvrID2Server_Mutex);	
    mapSvrID2Service::iterator iter = m_SvrID2Service.begin();
    if(iter != m_SvrID2Service.end())
    {
        iter->second->Close();
    }
}

bool ClientConnMgr::Close(UInt32 uiServerID)
{
    OSMutexLocker theLocker(&m_SvrID2Server_Mutex);
    mapSvrID2Service::iterator iter = m_SvrID2Service.find(uiServerID);
    if(iter != m_SvrID2Service.end())
    {
        return iter->second->Close();
    }
    return false;
}

bool ClientConnMgr::Close(UInt32 uiServerID,UInt32 uiConnID)
{
    OSMutexLocker theLocker(&m_SvrID2Server_Mutex);
    mapSvrID2Service::iterator iter = m_SvrID2Service.find(uiServerID);
    if(iter != m_SvrID2Service.end())
    {
        return iter->second->Close(uiConnID);
    }
    return false;	
}

bool ClientConnMgr::SendData(MsgBlock* pMsgBlock)
{
    return true;
}

HostServicePtr ClientConnMgr::GetHostServicePtr(UInt32 uiServerID)
{
    OSMutexLocker theLocker(&m_SvrID2Server_Mutex);
    mapSvrID2Service::iterator iter = m_SvrID2Service.find(uiServerID);
    if(iter == m_SvrID2Service.end())
        return HostServicePtr();
    return iter->second; 
}

bool ClientConnMgr::GetCliConnID(UInt32 uiServerID,UInt32& uiConnID)
{
    HostServicePtr ccs = GetHostServicePtr(uiServerID);
    if(ccs.isNull())
        return false;
    if(!ccs->GetCliConnID(uiConnID))
        return false;
    return true;
}

WatcherCliConnPtr ClientConnMgr::GetCliConn(UInt32 uiServerID)     
{
    HostServicePtr ccs = GetHostServicePtr(uiServerID);
    if(ccs.isNull())
        return WatcherCliConnPtr();

    UInt32 uiConnID;
    if(!ccs->GetCliConnID(uiConnID))
        return WatcherCliConnPtr();

    return GetCliConn(uiServerID,uiConnID);
}   

WatcherCliConnPtr ClientConnMgr::GetCliConn(UInt32 uiServerID,UInt32 uiConnID)
{
    OSMutexLocker theLocker(&m_ConnID2Conn_Mutex);
    mapConnID2Conn::iterator iter = m_ConnID2Conn.find(uiConnID);
    if(iter != m_ConnID2Conn.end())
    {
        return iter->second;
    }
    return SharedPtr<WatcherCliConn>();
}

bool ClientConnMgr::Remove(UInt32 uiConnID)
{
    WatcherCliConnPtr ccp;
    {
        OSMutexLocker theLocker(&m_ConnID2Conn_Mutex);	
        mapConnID2Conn::iterator iter = m_ConnID2Conn.find(uiConnID);
        if(iter == m_ConnID2Conn.end())
        {
            LOG_ERROR("Can't Find Connection From ClientConnMgr,ConnID:%d",uiConnID);
            return false;
        }
        ccp = iter->second;
        m_ConnID2Conn.erase(iter);
    }
    {
        OSMutexLocker theLocker(&m_SvrID2Server_Mutex);	
        mapSvrID2Service::iterator iterService = m_SvrID2Service.find(ccp->GetServerID());
        if(iterService != m_SvrID2Service.end())
        {
            if(!iterService->second->Remove(ccp))
                return false;
            if(iterService->second->GetHostCount() == 0)
                m_SvrID2Service.erase(iterService);
        }
    }
    return true;
}

bool ClientConnMgr::AddCliConn(WatcherCliConnPtr pConn)
{
    if(pConn.isNull())
    {
        LOG_ERROR("Pass To ClientConnMgr NULL Connection");
        return false;
    }

    {
        OSMutexLocker theLocker(&m_ConnID2Conn_Mutex);	
        mapConnID2Conn::iterator iter = m_ConnID2Conn.find(pConn->GetConnID());
        if(iter != m_ConnID2Conn.end())
        {
            LOG_ERROR("This Connection Already Exists In ClientConnMgr,ServerID:%u,ConnID:%d",pConn->GetServerID(),pConn->GetConnID());
            return false;
        }

        m_ConnID2Conn[pConn->GetConnID()] = pConn;
    }

    {
        OSMutexLocker theLocker(&m_SvrID2Server_Mutex);	
        mapSvrID2Service::iterator iterService = m_SvrID2Service.find(pConn->GetServerID());
        if(iterService != m_SvrID2Service.end())
        {
            if(!(iterService->second->AddCliConnToHost(pConn)))
                return false;
        }else{
            LOG_ERROR("Can't Find ClientHost From ClientConnMgr,ServerID:%u,ConnID:%d",pConn->GetServerID(),pConn->GetConnID());
            return false;
        }
    }

    return true;
}

WatcherCliConnPtr ClientConnMgr::GetCliConnPtr(UInt32 uiServerID,UInt32 uiConnID)
{
    WatcherCliConnPtr ccPtr = gCliConnMgr::instance()->GetCliConn(uiServerID,uiConnID);
    if(ccPtr.isNull())
    {
        LOG_ERROR("Can't Find Clent Connection From ClientConnMgr,ServerID:%u,ConnID:%u",uiServerID,uiConnID);
    }
    return ccPtr;
}
//----------ClientConnMgr Class End------------------------------------------------------------------------------//

