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

	void                                   AddPool(const std::string& addr, UInt32 max);//��ӷ������Ŀͻ������ӳ�,param addr ��������ַ(ip:port)    
	void                                   SetRetryInterval(int n);//�������ӳ�ʧ�ܺ����Ե�ʱ��ʱ���(��),Ӧ����Clster������Pool    
	void                                   RemovePool(const std::string& addr);//�����ӳؼ�Ⱥ��ɾ��ĳ����ַ�����ӳ�    
	virtual SharedPtr<ConnectPool>         Peek();//�ú���������ѭ��ʽ�����ӳؼ����л�ȡһ�����ӳ�
    //���ù�ϣ��λ��ʽ�Ӽ����л�ȡһ�����ӳ�,�ڲ�ȱʡ���� CRC32 �Ĺ�ϣ�㷨,����������ش��麯���������Լ��ļ�Ⱥ��ȡ��ʽ
	//param key ��ֵ�ַ����������ֵΪ NULL�����ڲ��Զ��л�����ѭ��ʽ
	virtual SharedPtr<ConnectPool>         Peek(const char* key);    
	std::vector< SharedPtr<ConnectPool> >& GetPools();//������еķ����������ӳ�    
	size_t                                 Size() const;//������ӳؼ��������ӳض���ĸ���                                           
	void                                   RemoveConnection();
	Framework*                             GetFramework();
protected:
    //�������ʵ�ִ˺��������������ӳض���
	virtual SharedPtr<ConnectPool>         CreatePool(const std::string& addr,int max) = 0;

private:
	std::vector<SharedPtr<ConnectPool> > m_pools;
	size_t     m_nextIndex;			// ��һ��Ҫ���ʵĵ��±�ֵ
	UInt32     m_retry_inter;
	OSMutex    m_lock;  
	Framework* m_pFramework;
};

#endif //__ConnectPoolCluster_h__