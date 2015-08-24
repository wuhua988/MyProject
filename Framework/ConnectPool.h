#ifndef __ConnectPool_h__
#define __ConnectPool_h__
#include <string>
#include <list>
#include "WatcherConnection.h"
#include "OSMutex.h"
#include "SharedPtr.h"
#include "OSHeaders.h"

class Framework;
class ConnectPool
{
public:
    ConnectPool(const std::string& strAddr, UInt32 max,Framework* pFramework = NULL);
    
    virtual ~ConnectPool();
    
    
    void                                 SetRetryInterval(UInt32 retry_inter);//设置连接池异常的重试时间间隔    
    void                                 SetIdleTTL(UInt32 ttl);//设置连接池中空闲连接的空闲生存周期,毫秒    
    virtual SharedPtr<WatcherConnection> Peek();//从连接池中尝试性获取一个连接,当服务器不可用,距上次服务端连接异常时间间隔    
    virtual void                         Put(SharedPtr<WatcherConnection> conn);//释放一个连接至连接池中,当该连接池对应的服务器不可用或调用者希望关闭该连接时    
    int                                  CheckIdle(UInt32 ttl);//检查连接池中空闲的连接,将过期的连接释放掉(时间间隔超过ttl的连接),毫秒                
    void                                 SetAlive(bool ok);//设置连接池的存活状态
    bool                                 Aliving();                 
    std::string                          GetAddr() const;
    UInt32                               GetCount() const;
    virtual                              void Init();
protected:
    virtual SharedPtr<WatcherConnection> CreateConnection() = 0;
    SharedPtr<WatcherConnection>         Peek(bool erase);
protected:
    bool        m_bAlive;
    UInt32      m_retryInterval;
    SInt64      m_lastDead;	   // 该连接池对象上次不可用时的时间截(毫秒)
    
    std::string m_strAddr;	   // 连接池对应的服务器地址，IP:PORT
    UInt32      m_iMax;            // 最大连接数
    UInt32      m_iCount;          // 当前的连接数
    UInt32      m_idleTTL;      // 空闲连接的生命周期(毫秒)
    SInt64      m_lastCheck;	   // 上次检查空闲连接的时间截(毫秒)
    UInt32      m_checkInterval;   // 检查空闲连接的时间间隔
    Framework*  m_pFramework;
    
    OSMutex     m_lock;
    std::list<SharedPtr<WatcherConnection> > m_connsList;	// 连接池集合
};

#endif //__ConnectPool_h__