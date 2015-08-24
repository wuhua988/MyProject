#ifndef __RedisConnection_h__
#define __RedisConnection_h__
#include <string>
#include "../WatcherConnection.h"
#include "TCPSocket.h"

using std::string;
class RedisConnection : public WatcherConnection
{
public:		
    RedisConnection(const string& addr, int conn_timeout = 60,int rw_timeout = 30, bool retry = true);
    virtual ~RedisConnection();

    virtual int Open (void * arg= 0);
    virtual int Close(SOCKET handle = Socket::kInvalidFileDesc);
    virtual SOCKET GetHandle(); 
    virtual int HandleEvent(int, SOCKET);
    virtual bool SendData(MsgBlock* pMsgBlock);
    TCPSocket* GetSocket();
    bool ReadLine(string& strResult, ssize_t max_size = 2048);
    bool Read_N(string& strResult, ssize_t n);
    bool SendCommand(const string& strCmd);
private:
    TCPSocket m_socket;
    std::string m_strAddr;
    int   m_conn_timeout;
    int   m_rw_timeout;
    bool  m_bRetry;
    
    friend class RedisClient;
};
#endif //__RedisConnection_h__