#ifndef __WatcherCheck_h__
#define __WatcherCheck_h__
#include "ev.h"
#include "OSHeaders.h"

typedef struct CheckPending_t
{
    UInt32 type;
	UInt32 serverID;
    void* args;
    CheckPending_t* next;
}CheckPending_t;

class EventLoop;
class WatcherCheck
{
public:
    typedef void (*CheckPendingCB)(CheckPending_t* pCPending);
	enum PendingType {
        PENDING_CLOSE_CONN  = 0,
        PENDING_NUM         = 1,
    };
	
    WatcherCheck(EventLoop* evLoop);
    virtual ~WatcherCheck();
    
    virtual int Open(void * arg= 0);
    virtual int HandleEvent(int event);
    virtual int Close();	
    void        AddCheckPending(CheckPending_t* pCPending);
private:
    void        ProcessCheckPending();
    static void Callback(struct ev_loop* loop, struct ev_check* instance, int revents);
    void        NoticeFrameCloseConn(CheckPending_t* pCPending);
    
    ev_check        m_check;
    EventLoop*      m_loop;
    CheckPending_t* m_CheckPendingHead;
};
#endif //__WatcherCheck_h__
