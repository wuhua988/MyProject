#ifndef __RedisConnectPool_h__
#define __RedisConnectPool_h__
#include "../ConnectPool.h"

class RedisConnectPool : public ConnectPool
{
public:
    RedisConnectPool(const std::string& strAddr, UInt32 max);
    virtual ~RedisConnectPool();

    //设置网络连接超时时间及网络 IO 读写超时时间(秒)
    void    SetTimeout(int conn_timeout, int rw_timeout);
protected:
    virtual SharedPtr<WatcherConnection> CreateConnection();

private:
    int m_conn_timeout;
    int m_rw_timeout;
};

#endif //__RedisConnectPool_h__
