#ifndef __ConnectPoolCluster_h__
#define __ConnectPoolCluster_h__
#include <vector>
#include "ConnectPool.h"

class Framework;
class ConnectPoolCluster
{
public:	    
	ConnectPoolCluster(Framework* framework = NULL);
	virtual ~ConnectPoolCluster();

	void                                   AddPool(const std::string& addr, UInt32 max);//添加服务器的客户端连接池,param addr 服务器地址(ip:port)    
	void                                   SetRetryInterval(int n);//设置连接池失败后重试的时间时间隔(秒),应用于Clster中所有Pool    
	void                                   RemovePool(const std::string& addr);//从连接池集群中删除某个地址的连接池    
	virtual SharedPtr<ConnectPool>         Peek();//该函数采用轮循方式从连接池集合中获取一个连接池
    //采用哈希定位方式从集合中获取一个连接池,内部缺省采用 CRC32 的哈希算法,子类可以重载此虚函数，采用自己的集群获取方式
	//param key 键值字符串，如果该值为 NULL，则内部自动切换到轮循方式
	virtual SharedPtr<ConnectPool>         Peek(const char* key);    
	std::vector< SharedPtr<ConnectPool> >& GetPools();//获得所有的服务器的连接池    
	size_t                                 Size() const;//获得连接池集合中连接池对象的个数                                           
	void                                   RemoveConnection();
	Framework*                             GetFramework();
protected:
    //子类必须实现此函数用来创建连接池对象
	virtual SharedPtr<ConnectPool>         CreatePool(const std::string& addr,int max) = 0;

private:
	std::vector<SharedPtr<ConnectPool> > m_pools;
	size_t     m_nextIndex;			// 下一个要访问的的下标值
	UInt32     m_retry_inter;
	OSMutex    m_lock;  
	Framework* m_pFramework;
};

#endif //__ConnectPoolCluster_h__