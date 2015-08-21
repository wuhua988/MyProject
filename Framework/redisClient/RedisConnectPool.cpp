#include "RedisConnection.h"
#include "RedisConnectPool.h"

RedisConnectPool::RedisConnectPool(const std::string& strAddr, UInt32 max)
: ConnectPool(strAddr, max, NULL)
, m_conn_timeout(30)
, m_rw_timeout(60)
{
}

RedisConnectPool::~RedisConnectPool()
{
}

void RedisConnectPool::SetTimeout(int conn_timeout,int rw_timeout)
{
	m_conn_timeout = conn_timeout;
	m_rw_timeout = rw_timeout;
}

SharedPtr<WatcherConnection> RedisConnectPool::CreateConnection()
{
	RedisConnection* conn = new RedisConnection(m_strAddr, m_conn_timeout,m_rw_timeout);
	return conn;
}

