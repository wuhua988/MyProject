#ifndef __WatcherNoticePipe_h__
#define __WatcherNoticePipe_h__

#include "WatcherBase.h"
#include "OSMutex.h"
typedef struct Notice
{
    UInt32 type;
	UInt32 serverID;
    void* args;//or use union type
    Notice* next;
}Notice;

class WatcherNoticePipe : public WatcherBase
{
public:
    typedef void (*NoticeCB)(Notice* notice);
    enum{
		NOTICE_EVENT = 0,
		NOTICE_TCP_CONN = 1,
		NOTICE_TCP_CLIENT = 2,
		NOTICE_LISTEN = 3,
        NOTICE_CLOSE_CONN = 4,
		NOTICE_NUM = 5,
    };
	
	WatcherNoticePipe(EventLoop* evLoop,Framework* pFw = NULL);
	virtual ~WatcherNoticePipe();

	virtual int                   Open(void * arg= 0);
    virtual int                   HandleEvent(int event, SOCKET handle = Socket::kInvalidFileDesc);
    virtual int                   Close(SOCKET handle = Socket::kInvalidFileDesc);
	                              
	bool                          AddNotice(Notice* pNotice);
	static void                   AddNoticeToPipe(EventBaseThread* pThread,UInt32 type,UInt32 svrid,void* arg);
private:                          
    void                          InitPipe();
	void                          ProcessNotices();
	void                          RegCallBack();
	static void                   EventCB(Notice* notice);
	static void                   TcpConnCB(Notice* notice);
	static void                   TcpConnClientCB(Notice* notice);
	static void                   ListenCB(Notice* notice);
    static void                   CloseConnCB(Notice* notice);
	static SharedPtr<WatcherBase> GetWatcherByNoticeConnID(Notice* notice);
	
    int      m_readFD;
    int      m_writeFD;
	OSMutex  m_Mutex;             
    Notice*  m_noticeTail;
	Notice*  m_noticeHead;
	NoticeCB m_noticeCB[NOTICE_NUM];


};
#endif //__WatcherNoticePipe_h__
