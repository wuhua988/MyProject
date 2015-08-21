#include "RedisPoolCluster.h"
#include "RedisConnectPool.h"

RedisPoolCluster::RedisPoolCluster(int conn_timeout /* = 30 */,
	int rw_timeout /* = 30 */)
: ConnectPoolCluster(NULL)
,m_conn_timeout(conn_timeout)
, m_rw_timeout(rw_timeout)
{
}

RedisPoolCluster::~RedisPoolCluster()
{
}

SharedPtr<ConnectPool> RedisPoolCluster::CreatePool(const std::string& addr,int max)
{
	SharedPtr<RedisConnectPool> pRedisClientPool = new RedisConnectPool(addr,max);
	//SharedPtr<ConnectPool> connPool = pRedisClientPool.cast<ConnectPool>();
	return pRedisClientPool;
}
