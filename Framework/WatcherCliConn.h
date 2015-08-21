#ifndef __WatcherCliConn_h__
#define __WatcherCliConn_h__
#include "WatcherTcpConn.h"

class WatcherCliConn : public WatcherTcpConn
{
public:
    WatcherCliConn(EventLoop* evLoop,Framework* pFw);
    virtual ~WatcherCliConn();

    virtual int Open (void * arg= 0);
    virtual int HandleEvent(int revents, SOCKET handle = Socket::kInvalidFileDesc);
	int         HandleReConnect();
	virtual int HandleTimeOut();
	virtual int HandleError(OS_Error err);
};

typedef SharedPtr<WatcherCliConn> WatcherCliConnPtr;
#endif //__WatcherCliConn_h__