#ifndef __WatcherMgr_h__
#define __WatcherMgr_h__
#include <map>
#include "WatcherBase.h"
#include "SharedPtr.h"
#include "OSMutex.h"
#include "OSHeaders.h"
#include "Singleton.h"

class WatcherMgr
{
public:
    WatcherMgr(int num = 50000);
    ~WatcherMgr();

    bool                   RegWatcher(SharedPtr<WatcherBase> pWatcher);
    //bool                 RegWatcher(WatcherBase* pHandler);
    SharedPtr<WatcherBase> GetWatcher(UInt32 connID);
    bool                   RemoveWatcher(SharedPtr<WatcherBase> pWatcher);
    bool                   RemoveWatcher(UInt32 connID);
    static UInt32          GetNextConnId();
private:
    std::map<UInt32,SharedPtr<WatcherBase> > m_ConnID2WatcherMap;
    UInt32*                                  m_FdToConnID;
    static UInt32                            m_curConnID;
    OSMutex                                  m_connIDMutex;
};

typedef Singleton<WatcherMgr> gWatcherMgrInst;
#endif //__HandlerMgr_h__