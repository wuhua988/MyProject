#ifndef __RedisPoolCluster_h__
#define __RedisPoolCluster_h_
#include "../ConnectPoolCluster.h"

class RedisPoolCluster : public ConnectPoolCluster
{
public:
	RedisPoolCluster(int conn_timeout = 30, int rw_timeout = 30);
	virtual ~RedisPoolCluster();
protected:
    virtual SharedPtr<ConnectPool> CreatePool(const std::string& addr,int max);
private:
	int   m_conn_timeout;
	int   m_rw_timeout;
};

#endif //__RedisPoolCluster_h__
