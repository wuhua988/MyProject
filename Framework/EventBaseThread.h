#ifndef  __EventBaseThread_h__
#define  __EventBaseThread_h__

#include "OSThread.h"
#include "WatcherNoticePipe.h"

class Framework;
class EventBaseThread : public OSThread
{
public:
    explicit EventBaseThread(Framework* pFw);
    virtual ~EventBaseThread();
    virtual void Entry();

    bool         AddNotice(Notice* pNotice);
    EventLoop*   GetEvLoop();
    void         Init();
private:
    EventLoop          m_loop;
    WatcherNoticePipe* m_pipeWatcher;
    Framework*         m_pFrameWork;
};

#endif 
