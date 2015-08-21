#ifndef __WatcherBase_h__
#define __WatcherBase_h__

#include <assert.h>
#include "OSHeaders.h"
#include "Socket.h"
#include "log.h"
#include "SharedPtr.h"
#include "EventLoop.h"

class EventBaseThread;
class Framework;
class WatcherBase
{
public:	
	virtual ~WatcherBase();	
	WatcherBase();
	WatcherBase(EventLoop* p_loop,Framework* pFw = NULL);
	
    void             SetThread(EventBaseThread* pThread);   
    EventBaseThread* GetThread();   
    virtual int      Open (void * arg = 0) = 0;
    virtual int      HandleEvent(int event, SOCKET handle = Socket::kInvalidFileDesc) = 0;
    virtual int      Close(SOCKET handle = Socket::kInvalidFileDesc);                     
    virtual void     SetHandle(SOCKET handle);
    virtual SOCKET   GetHandle();
    void             SetConnID(UInt32 connID);
    UInt32           GetConnID();
	void             SetServerID(UInt32 serverID);
    UInt32           GetServerID();
    Framework*       GetFramework();
    static void      Callback(EV_P_ ev_io *watcher, int revents);    
    void             AddEvent(int events);//供外界调用	
protected:
    void             StartWatcher();
    void             StopWatcher();
	void             SetEvent(int events);
    void             RemoveEvent(int events);
	bool             IsActive() const;
	bool             IsPending() const;

    ev_io            m_watcher;
	EventLoop*       m_loop;
    EventBaseThread* m_pThread;
    //SOCKET m_handle;
    UInt32           m_uiConnID;
	UInt32           m_uiServerID;
    bool             m_bIsClose;
	Framework*       m_pFrameWork;
};
#endif //__WatcherBase_h__
