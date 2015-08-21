#ifndef __WatcherListener_h__
#define __WatcherListener_h__
#include "WatcherBase.h"
#include "TCPListenerSocket.h"

class WatcherListener : public WatcherBase
{
public:
    WatcherListener(const char* host, UInt16 port,EventLoop* evLoop,Framework* pFw);
    virtual ~WatcherListener();

    virtual int    Open (void * arg= 0);
    virtual int    HandleEvent(int revents, SOCKET handle = Socket::kInvalidFileDesc);
    virtual int    Close(SOCKET handle = Socket::kInvalidFileDesc);
    virtual SOCKET GetHandle();
private:
	TCPListenerSocket m_socket;
};
#endif //__WatcherListener_h__