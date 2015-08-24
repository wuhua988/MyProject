#ifndef __RedisConnectPool_h__
#define __RedisConnectPool_h__
#include "../ConnectPool.h"

class RedisConnectPool : public ConnectPool
{
public:
    RedisConnectPool(const std::string& strAddr, UInt32 max);
    virtual ~RedisConnectPool();

    //�����������ӳ�ʱʱ�估���� IO ��д��ʱʱ��(��)
    void    SetTimeout(int conn_timeout, int rw_timeout);
protected:
    virtual SharedPtr<WatcherConnection> CreateConnection();

private:
    int m_conn_timeout;
    int m_rw_timeout;
};

#endif //__RedisConnectPool_h__
