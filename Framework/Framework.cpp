#include "Framework.h"
#include "WatcherNoticePipe.h"
#include "WatcherListener.h"
#include "WatcherTcpConn.h"
#include "EventThreadPool.h"
#include "EventMasterThread.h"
#include "EventWorkThread.h"
#include "SharedPtr.h"
#include "OS.h"
#include "SocketUtils.h"
#include "ClientConnectionMgr.h"

Framework::Framework()
:m_iEventThreadNum(3)
,m_iBusinThreadNum(3)
{

}

Framework::~Framework()
{

}

int Framework::Init(int eventThreadNum, int businThreadNum)
{
    if(eventThreadNum >= 0)
        m_iEventThreadNum = eventThreadNum;
    if(businThreadNum >= 0)
        m_iBusinThreadNum = businThreadNum;
    gCliConnMgr::instance()->SetFramework(this);

    OS::Initialize();
    OSThread::Initialize();
    SocketUtils::Initialize(false);
    return 0;
}

int Framework::Run()
{
    EventThreadPool::AddThreads(m_iEventThreadNum,EventThreadPool::Type_Work,this);
    EventThreadPool::AddThreads(m_iBusinThreadNum,EventThreadPool::Type_BusinessTimer,this);
    return 0;
}

int Framework::NoticeTcpListen(char const* host,UInt16 port)
{
    EventMasterThread* pThread = EventThreadPool::GetMasterThread();
    if(!pThread)
    {
        LOG_ERROR("There is no master thread to used!");
        return -1;
    }

    WatcherListener* pWatcher = new WatcherListener(host,port,pThread->GetEvLoop(),this);
    pWatcher->SetThread(pThread);

    WatcherNoticePipe::AddNoticeToPipe(pThread,WatcherNoticePipe::NOTICE_LISTEN,0,pWatcher);
    return 0;
}

int Framework::NoticeTcpConnect(UInt32 uiServerID
                               ,char const* szAddr
                               ,UInt16 iPort
                               ,UInt32 uiConnNum
                               ,Float64 fExpire
                               ,Float64 fInterval
                               ,void* userData)
{
    gCliConnMgr::instance()->Connect(uiServerID,szAddr,iPort,uiConnNum,fExpire,fInterval);
    return 0;
}

int Framework::NoticeCloseConnection(UInt32 uiServerID,UInt32 connID)
{
    SharedPtr<WatcherBase> pShareObj = gWatcherMgrInst::instance()->GetWatcher(connID);
    if(pShareObj.isNull()){
        return 1;
    }
    PointerSizedUInt pConnID = connID;
    //intptr_t或uintptr_t中,int i2 = *((int*)&aatta);
    WatcherNoticePipe::AddNoticeToPipe(pShareObj->GetThread(),WatcherNoticePipe::NOTICE_CLOSE_CONN,uiServerID,(void*)pConnID);
    return 0;
}

void Framework::SendMsg(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    SharedPtr<WatcherBase> pShareObj;
    if(pMsgBlock->ServerID() == 0)
        pShareObj = gWatcherMgrInst::instance()->GetWatcher(pMsgBlock->ConnID());
    else
        pShareObj = gCliConnMgr::instance()->GetCliConn(pMsgBlock->ServerID(),pMsgBlock->ConnID());
    SharedPtr<WatcherConnection> HandlerConn = pShareObj.cast<WatcherConnection>();
    if(HandlerConn.isNull()){
        LOG_TRACE("Cant't Find Connection Handler by ConnID:%d When Send Message",pMsgBlock->ConnID());
        return;
    }

    if(!HandlerConn->SendData(pMsgBlock)){
        WatcherNoticePipe::AddNoticeToPipe(pShareObj->GetThread(),WatcherNoticePipe::NOTICE_EVENT,HandlerConn->GetServerID(),(void*)pMsgBlock->ConnID());
    }
}

void Framework::OnMessage(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    LOG_DEBUG("Receive Msg ConnID:%d",pMsgBlock->ConnID());
    StrPtrLen str(pMsgBlock->wr_ptr(),pMsgBlock->length());    
}


void Framework::OnSignal(int signum)
{

}

int Framework::OnConnCreated(ConnInfo* pConnInfo)
{
    assert(pConnInfo != NULL);
    LOG_DEBUG("ConnCreated Remote IP:%s,Remote Port:%d",pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
    return 0;
}

int Framework::OnFailConnect(ConnInfo* pConnInfo)
{
    assert(pConnInfo != NULL);
    LOG_DEBUG("FailConnect Remote IP:%s,Remote Port:%d,And Conitnue to Connect it!",pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
    return 0;
}
int Framework::OnConnClosed(ConnInfo* pConnInfo)
{
    assert(pConnInfo != NULL);
    LOG_DEBUG("ConnClosed Remote IP:%s,Remote Port:%d",pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
    return 0;
}

ConnInfo* Framework::GetConnInfo(UInt32 uiServerID,UInt32 uiConnID)
{
    ConnInfo* cif = NULL;
    SharedPtr<WatcherConnection> connPtr;
    if(uiServerID == 0)
    {        
        SharedPtr<WatcherBase> pWBase = gWatcherMgrInst::instance()->GetWatcher(uiConnID);
        if(!pWBase.isNull())
            connPtr = pWBase.cast<WatcherConnection>();
    }
    else
    {
        connPtr = gCliConnMgr::instance()->GetCliConn(uiServerID,uiConnID);

    }
    if(!connPtr.isNull())
    {
        cif = connPtr->GetConnInfo();		
    }else{
        LOG_DEBUG("Can't Find ConnInfo,ServerID:%u,ConnID:%u",uiServerID,uiConnID);
    }
    return cif;
}
