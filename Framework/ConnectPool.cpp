#include "ConnectPool.h"
#include "OS.h"

ConnectPool::ConnectPool(const std::string& strAddr,UInt32 max,Framework* pFramework)
:m_bAlive(true),m_retryInterval(1),m_strAddr(strAddr),m_iMax(max),m_iCount(0),m_idleTTL(5)
,m_checkInterval(2),m_pFramework(pFramework)
{
	m_lastDead = OS::Milliseconds();  
	m_lastCheck = OS::Milliseconds(); 
	if(m_iMax < 1)
		m_iMax = 10;
}
	
void ConnectPool::Init()
{
	OSMutexLocker locker(&m_lock);
	for(UInt32 i = 0; i < m_iMax; i++)
	{
		SharedPtr<WatcherConnection> connSharedObj = CreateConnection();
		m_connsList.push_front(connSharedObj);
		m_iCount++;
	}
}

ConnectPool::~ConnectPool()
{
	OSMutexLocker locker(&m_lock);
	m_connsList.clear();	
}

void ConnectPool::SetRetryInterval(UInt32 retry_inter)
{
	m_retryInterval = retry_inter;
}

void ConnectPool::SetIdleTTL(UInt32 ttl)
{
	m_idleTTL = ttl;
}

SharedPtr<WatcherConnection> ConnectPool::Peek()
{
	return Peek(true);
}

SharedPtr<WatcherConnection> ConnectPool::Peek(bool erase)
{
	OSMutexLocker locker(&m_lock);
	if (m_bAlive == false)
	{
		SInt64 now = OS::Milliseconds();
		if (m_retryInterval <= 0 || m_lastDead + m_retryInterval < now)
		{
		    return SharedPtr<WatcherConnection>();
		}
		m_bAlive = true;
		LOG_DEBUG("Reset ConnectPool State: %s", GetAddr().c_str());
	}

	SharedPtr<WatcherConnection> connSharedObj;

	std::list<SharedPtr<WatcherConnection> >::iterator it = m_connsList.begin();
	if (it != m_connsList.end())
	{
		connSharedObj = *it;
		if(erase)
			m_connsList.erase(it);
	}

	return connSharedObj;
}

void ConnectPool::Put(SharedPtr<WatcherConnection> conn)
{
    // 将归还的连接放在链表首部，这样在调用释放过期连接
    // 时比较方便，有利于尽快将不忙的连接关闭
	OSMutexLocker locker(&m_lock);
    m_connsList.push_front(conn);
}

int ConnectPool::CheckIdle(UInt32 ttl)
{
	return 0;
}

void ConnectPool::SetAlive(bool ok)
{
	m_bAlive = ok;
	if(!ok){
		m_lastDead = OS::Milliseconds();
	}
}

bool ConnectPool::Aliving()
{
	return m_bAlive;
}

std::string ConnectPool::GetAddr() const
{
	return m_strAddr;
}

UInt32 ConnectPool::GetCount() const
{
	return m_iCount;
}

