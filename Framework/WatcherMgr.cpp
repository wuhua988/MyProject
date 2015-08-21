#include "WatcherMgr.h"
#include "ClientConnectionMgr.h"

UInt32 WatcherMgr::m_curConnID = 0;
WatcherMgr::WatcherMgr(int num)
{
    LOG_NOTICE("Watcher Number：%d",num);
    m_FdToConnID = new UInt32[num];
}
WatcherMgr::~WatcherMgr()
{
	OSMutexLocker theLocker(&m_connIDMutex);
    if(m_FdToConnID != NULL)
        delete []m_FdToConnID;//no  use 
	
	m_ConnID2WatcherMap.clear();
}
bool WatcherMgr::RegWatcher(SharedPtr<WatcherBase> pWatcher)
{    
    if (pWatcher.isNull()){
        LOG_ERROR("WatcherMgr Register Handler But The Arg Handler Is NULL!");
        return false;
    }

    UInt32 connID = WatcherMgr::GetNextConnId();
    pWatcher->SetConnID(connID);
    
    if(pWatcher->Open() != 0){
        LOG_ERROR("WatcherMgr Register Handler Error When Open It!");
        return false;
    }
    
	if(pWatcher->GetServerID() == 0)
	{
		OSMutexLocker theLocker(&m_connIDMutex);
		if (m_ConnID2WatcherMap.find(connID) != m_ConnID2WatcherMap.end())
        {
            LOG_DEBUG("The connection:%d,already exist!",connID);
            return false;
        }

        m_ConnID2WatcherMap[connID] = pWatcher;
        LOG_DEBUG("WatcherMgr::RegWatcher connID:%u fd:%d", connID, pWatcher->GetHandle());
	}else{
		return gCliConnMgr::instance()->AddCliConn(pWatcher.cast<WatcherCliConn>());
	}

    return true;
}

//bool WatcherMgr::RegWatcher(WatcherBase* pWatcher)
//{
//    SharedPtr<WatcherBase> shareObj = pWatcher; 
//    return RegWatcher(shareObj);
//}

SharedPtr<WatcherBase> WatcherMgr::GetWatcher(UInt32 connID)
{
    OSMutexLocker theLocker(&m_connIDMutex);
    SharedPtr<WatcherBase> watcher;
    std::map<UInt32, SharedPtr<WatcherBase> >::iterator it = m_ConnID2WatcherMap.find(connID);
    if (it == m_ConnID2WatcherMap.end())
    {
        LOG_TRACE("Can't Find watcher In WatcherMgr By ConnID:%d",connID);
        return watcher;        
    }

    watcher = it->second;
    LOG_DEBUG("Find watcher In WatcherMgr By ConnID:%d",connID);
    return watcher;
}

bool WatcherMgr::RemoveWatcher(SharedPtr<WatcherBase> pWatcher)
{
    if(pWatcher.isNull())
        return false;
    return RemoveWatcher(pWatcher->GetConnID());
}

bool WatcherMgr::RemoveWatcher(UInt32 connID)
{
    OSMutexLocker theLocker(&m_connIDMutex);
    std::map<UInt32, SharedPtr<WatcherBase> >::iterator it = m_ConnID2WatcherMap.find(connID);
    if (it == m_ConnID2WatcherMap.end())
	{
		LOG_DEBUG("Can't Find Watcher By connID:%u,when Remove Watcher!", connID);
		return false;
	}
        
    m_ConnID2WatcherMap.erase(it);
    LOG_DEBUG("WatcherMgr::RemoveWatcher connID:%u", connID);
    return true;
}
    
UInt32 WatcherMgr::GetNextConnId()
{
    static OSMutex connIDMutex;
    connIDMutex.Lock();
    ++m_curConnID;
    connIDMutex.Unlock();
    return m_curConnID;
	/*
	    CwxMutexGuard<CwxMutexLock> lock(&m_connMapMutex);
    CWX_UINT32 uiConnId = m_uiCurConnId + 1;
    while(1)
    {
        if (m_connMap.find(uiConnId) == m_connMap.end()) break;
        uiConnId++;
        if (uiConnId > CWX_APP_MAX_CONN_ID) uiConnId = CWX_APP_MIN_CONN_ID;
    }
    m_uiCurConnId = uiConnId;
    return uiConnId;
	*/
}