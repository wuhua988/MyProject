#include "ConnectPoolCluster.h"
#include "log.h"

ConnectPoolCluster::ConnectPoolCluster(Framework* framework)
: m_retry_inter(1),m_pFramework(framework)
{}
ConnectPoolCluster::~ConnectPoolCluster()
{
	OSMutexLocker locker(&m_lock);
	m_pools.clear();
}

void ConnectPoolCluster::AddPool(const std::string& addr, UInt32 max)
{
    OSMutexLocker locker(&m_lock);

	std::vector<SharedPtr<ConnectPool> >::iterator it = m_pools.begin();
	for(; it != m_pools.end(); ++it)
	{
		if(addr == (*it)->GetAddr())
		{
			return ;
		}
	}

	SharedPtr<ConnectPool> pool = CreatePool(addr, max);
	pool->SetRetryInterval(m_retry_inter);
	pool->Init();
	m_pools.push_back(pool);
	LOG_DEBUG("Add one service, addr: %s, count: %u", addr.c_str(), max);
}

void ConnectPoolCluster::SetRetryInterval(int n)
{
	
}

void ConnectPoolCluster::RemovePool(const std::string& addr)
{
	
}

SharedPtr<ConnectPool> ConnectPoolCluster::Peek()
{
	SharedPtr<ConnectPool> pool;
	size_t pool_size, n;

	OSMutexLocker locker(&m_lock);
	static int pool_idx = - 1;
	pool_size = m_pools.size();
	if (pool_size == 0)
	{
		LOG_DEBUG("Pools's Size Is 0!");
		return SharedPtr<ConnectPool>();
	}

	for(size_t i = 0; i < pool_size; i++)
	{
		n = (pool_idx + 1) % pool_size;
		pool = m_pools[n];
		pool_idx = n;
		if (pool->Aliving())
		{
			return pool;
		}
	}

	LOG_DEBUG("All Pool(size=%d) Is Dead!", (int) pool_size);	
	return SharedPtr<ConnectPool>();
}

SharedPtr<ConnectPool> ConnectPoolCluster::Peek(const char* key)
{
	return SharedPtr<ConnectPool>();
}

std::vector< SharedPtr<ConnectPool> >& ConnectPoolCluster::GetPools()
{
	return m_pools;
}

size_t ConnectPoolCluster::Size() const
{
	return m_pools.size();
}

Framework* ConnectPoolCluster::GetFramework()
{
	return m_pFramework;
}

