#ifndef __WatcherTcpConn_h__
#define __WatcherTcpConn_h__
#include "WatcherConnection.h"
#include "TCPSocket.h"
#include "OSQueue.h"
#include "MsgHeader.h"

class WatcherTcpConn : public WatcherConnection
{
public:
    WatcherTcpConn(EventLoop* evLoop,Framework* pFw);
    virtual ~WatcherTcpConn();

    virtual int         Open (void * arg= 0);
    virtual int         HandleEvent(int revents, SOCKET handle = Socket::kInvalidFileDesc);
    virtual int         Close(SOCKET handle = Socket::kInvalidFileDesc);
    virtual             SOCKET GetHandle(); 
    int                 HandleRead();
	int                 HandleWrite();
	virtual int         HandleTimeOut();
	virtual int         HandleError(OS_Error err);
	bool                SendData(MsgBlock* pMsgBlock); 
    TCPSocket*          GetSocket();	
    virtual std::string GetRemoteAddr();
    virtual UInt16      GetRemotePort();
private:
    OSQueueElem*        GetMsg();
	void                PutMsg(MsgBlock* msg);
protected:
    int                 HandleNormal(int revents, SOCKET handle);
	TCPSocket m_socket;
private:	
	MsgHeader m_header;
	char      m_szHeadBuf[MsgHeader::TMCP_HEAD_LENGTH];
	MsgBlock* m_recvMsgData;
    UInt32    m_iRecvHeadLen; 
    UInt32    m_iRecvDataLen;
	OSMutex   m_mutex;
    OSQueue   m_MsgQueue;
	
	MsgBlock* m_pSendMsgCur;
	size_t    m_iSendLen;
	bool      m_bNoticeToFrm;
};
#endif //__WatcherTcpConn_h__