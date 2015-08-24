#include "WatcherTcpConn.h"
#include "MsgBlock.h"
#include "SocketUtils.h"
#include "Framework.h"

WatcherTcpConn::WatcherTcpConn(EventLoop* evLoop,Framework* pFw)
: WatcherConnection(evLoop,pFw)
{
    assert(pFw != NULL);
    m_pSendMsgCur = NULL;
    m_recvMsgData = NULL;
    m_iRecvHeadLen = 0;
    m_iRecvDataLen = 0;
    m_iSendLen = 0;
    m_connInfo.SetHandler(this);
    m_connInfo.SetActive(false);
    m_bNoticeToFrm = false;
}

WatcherTcpConn::~WatcherTcpConn()
{
    if(m_recvMsgData)
        delete m_recvMsgData;
    while(1)
    {
        OSQueueElem* pElem = GetMsg();
        if(pElem){
            MsgBlock* pMsgBlock = (MsgBlock*)pElem->GetEnclosingObject();
            assert(pMsgBlock != NULL);
            delete pMsgBlock;
        }else{
            break;
        }
    }
    GetFramework()->OnConnClosed(&m_connInfo);
}

int WatcherTcpConn::Open(void * arg)
{
    m_pTimer->SetTimer(30,0);
    //StartTimer();
    assert(m_socket.GetSocketFD() != Socket::kInvalidFileDesc);

    if(m_socket.GetSocketFD() == Socket::kInvalidFileDesc){
        LOG_ERROR("WatcherTcpConn::Open,Socket FD is Invalid!");
        return 1;
    }

    ev_io_set(&m_watcher,m_socket.GetSocketFD(), EventLoop::EventMask_Read | EventLoop::EventMask_Write);
    StartWatcher();
    return 0;
}

int WatcherTcpConn::HandleEvent(int revents, SOCKET handle)
{   
    MACRO_DEBUG(revents & EventLoop::EventMask_Error,"Error Event Occurrence ConnID:%d",m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Read,"Read Event Occurrence ConnID:%d",m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Write,"WRITE Event Occurrence ConnID:%d",m_uiConnID);
    MACRO_DEBUG(revents & EventLoop::EventMask_Timer,"TimeOut Event Occurrence ConnID:%d",m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTING,"ConnID:%d State:STATUS_CONNECTING",m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_DISCONNECT,"ConnID:%d State:STATUS_DISCONNECT",m_uiConnID);
    MACRO_DEBUG(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTED,"ConnID:%d State:STATUS_CONNECTED",m_uiConnID);
    if(!m_bNoticeToFrm){
        GetFramework()->OnConnCreated(&m_connInfo);
        m_bNoticeToFrm = true;
    }	

    return HandleNormal(revents,handle);
}

int WatcherTcpConn::HandleNormal(int revents, SOCKET handle)
{
    if(m_connInfo.GetState() == ConnInfo::STATUS_CONNECTED)
    {
        if(revents & EventLoop::EventMask_Error){
            if(HandleError(OSThread::GetErrno()) != 0) return 1;
        }

        if(revents & EventLoop::EventMask_Read){
            if(HandleRead() != 0) return 1;			
        }

        if(revents & EventLoop::EventMask_Write){
            if(HandleWrite() != 0) return 1;
        }

        if(revents & EventLoop::EventMask_Timer){
            if(HandleTimeOut() != 0) return 1;
        }
    }else{
        LOG_ERROR("Connect Status:%d",m_connInfo.GetState());
        assert(false);
        return 1;
    }
    return 0;
}

int WatcherTcpConn::Close(SOCKET handle)
{
    m_socket.Close();
    StopWatcher();
    //m_uiConnID = 0;
    m_bIsClose = true;
    m_connInfo.SetState(ConnInfo::STATUS_DISCONNECT);
    return 0;
}

TCPSocket* WatcherTcpConn::GetSocket()
{
    return &m_socket;
}

SOCKET WatcherTcpConn::GetHandle()
{
    return m_socket.GetSocketFD();
}

int WatcherTcpConn::HandleRead()
{
    UInt32  recv_size = 0;
    UInt32 need_size = 0;

    while(1)
    {
        need_size = MsgHeader::TMCP_HEAD_LENGTH - m_iRecvHeadLen;
        if (need_size > 0 ){
            OS_Error err = m_socket.Read(m_szHeadBuf + m_iRecvHeadLen,need_size,&recv_size);
            if(err != OS_NoErr && err != EAGAIN){
                return HandleError(err);
            }else if(err == EAGAIN){
                return 0;
            }			

            m_iRecvHeadLen += recv_size;
            if (recv_size < need_size){
                return 0;
            }

            if(!m_header.FromNet(m_szHeadBuf)){
                LOG_ERROR("Msg header is error connID:%u",m_uiConnID);
                return 1;
            }
            m_recvMsgData = new MsgBlock(m_header.Length(),m_uiServerID,m_uiConnID,m_header);
        }
        //recv body
        need_size = m_header.Length() - m_iRecvDataLen;
        if (need_size > 0){
            OS_Error err = m_socket.Read(m_recvMsgData->wr_ptr(), need_size,&recv_size);
            if(err != OS_NoErr && err != EAGAIN){
                //delete m_recvMsgData
                if(m_recvMsgData)
                    delete m_recvMsgData;
                return HandleError(err);
            }else if(err == EAGAIN){
                return 0;
            }
            //move write pointer
            m_recvMsgData->wr_ptr(recv_size);
            m_iRecvDataLen += recv_size;
            if (recv_size < need_size){
                return 0;
            }
        }

        GetFramework()->OnMessage(m_recvMsgData);

        m_recvMsgData = NULL;
        m_iRecvHeadLen = 0;
        m_iRecvDataLen = 0;
    }
    return 0;
}

int WatcherTcpConn::HandleWrite()
{
    while(1)
    {
        if(m_pSendMsgCur == NULL){
            OSQueueElem* pElem = GetMsg();
            if(pElem){
                m_pSendMsgCur = (MsgBlock*)pElem->GetEnclosingObject();
                LOG_DEBUG("Get An Message From Queue,Remained:%u,ConnID:%u",m_MsgQueue.GetLength(),m_pSendMsgCur->ConnID());
            } else {
                RemoveEvent(EventLoop::EventMask_Write);
                LOG_DEBUG("There Are No Msg To Send,And Remove The Write Event,ConnID:%d",m_uiConnID);
                break;
            }
        }
        UInt32 sendSize = 0;
        UInt32 len = m_pSendMsgCur->length() - m_pSendMsgCur->rd_pos();
        OS_Error err = m_socket.Send(m_pSendMsgCur->rd_ptr(), len, &sendSize);
        if (err == OS_NoErr){
            if(sendSize < len){
                m_pSendMsgCur->rd_ptr(sendSize);
                AddEvent(EventLoop::EventMask_Write);
                break;
            } else {
                delete m_pSendMsgCur;
                m_pSendMsgCur = NULL;
            }
        }else if(err == EAGAIN){
            AddEvent(EventLoop::EventMask_Write);
            LOG_DEBUG("---------Receive EAGAIN,ConnID:%u",m_pSendMsgCur->ConnID());
            break;
        } else {
            return HandleError(err);
        }
    }
    return 0;
}

bool WatcherTcpConn::SendData(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    if(pMsgBlock == NULL)
        return true;

    OSMutexLocker locker(&m_mutex);
    bool sendSuccess = false;
    if(m_MsgQueue.GetLength() == 0)
    {
        UInt32 sendSize = 0;
        UInt32 len = pMsgBlock->length() - pMsgBlock->rd_pos();
        OS_Error err = m_socket.Send(pMsgBlock->rd_ptr(), len, &sendSize);
        if (err == OS_NoErr){
            if(sendSize < len){
                pMsgBlock->rd_ptr(sendSize);
            } else {
                delete pMsgBlock;
                sendSuccess = true;
            }
        } else {
            LOG_DEBUG("Error Happend when Send Packet,ErrNo:%d,ErrString:%s,ConnID:%u",err,strerror(err),m_uiConnID);
            delete pMsgBlock;
        }
    }

    if(sendSuccess == false){
        LOG_DEBUG("There are %u Message Remained!",m_MsgQueue.GetLength());
        m_MsgQueue.EnQueue(&pMsgBlock->m_QueueElem);
    }
    LOG_DEBUG("Send Packet To Client By ConnID:%u,Once:%s",m_uiConnID,sendSuccess ? "Yes" : "NO");
    return sendSuccess;
}

int WatcherTcpConn::HandleError(OS_Error err)
{
    //if(err == ENOTCONN){
    LOG_WARN("HandleError,Error Number:%d,Error String:%s,ConnID:%u",err,strerror(err),m_uiConnID);
    LOG_DEBUG("Close The Connection!connID:%u",m_uiConnID);
    return 1;
}

int WatcherTcpConn::HandleTimeOut()
{
    LOG_WARN("IP:%s,Port:%d,is Time Out!,ServerID:%u,ConnID:%d",m_connInfo.GetRemoteAddr().c_str(),m_connInfo.GetRemotePort(),GetServerID(),m_uiConnID);
    return 1;
}

OSQueueElem* WatcherTcpConn::GetMsg()
{
    OSMutexLocker locker(&m_mutex);	
    return m_MsgQueue.DeQueue();
}

void WatcherTcpConn::PutMsg(MsgBlock* msg)
{
    assert(msg != NULL);
    OSMutexLocker locker(&m_mutex);
    m_MsgQueue.EnQueue(&msg->m_QueueElem);
}

std::string WatcherTcpConn::GetRemoteAddr()
{
    StrPtrLen* theRemoteAddrStr = m_socket.GetRemoteAddrStr();
    return std::string(theRemoteAddrStr->Ptr, theRemoteAddrStr->Len);
}

UInt16 WatcherTcpConn::GetRemotePort()
{
    return m_socket.GetRemotePort();    
}
