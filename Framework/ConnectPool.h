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
    
    
    void                                 SetRetryInterval(UInt32 retry_inter);//�������ӳ��쳣������ʱ����    
    void                                 SetIdleTTL(UInt32 ttl);//�������ӳ��п������ӵĿ�����������,����    
    virtual SharedPtr<WatcherConnection> Peek();//�����ӳ��г����Ի�ȡһ������,��������������,���ϴη���������쳣ʱ����    
    virtual void                         Put(SharedPtr<WatcherConnection> conn);//�ͷ�һ�����������ӳ���,�������ӳض�Ӧ�ķ����������û������ϣ���رո�����ʱ    
    int                                  CheckIdle(UInt32 ttl);//������ӳ��п��е�����,�����ڵ������ͷŵ�(ʱ��������ttl������),����                
    void                                 SetAlive(bool ok);//�������ӳصĴ��״̬
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
    SInt64      m_lastDead;	   // �����ӳض����ϴβ�����ʱ��ʱ���(����)
    
    std::string m_strAddr;	   // ���ӳض�Ӧ�ķ�������ַ��IP:PORT
    UInt32      m_iMax;            // ���������
    UInt32      m_iCount;          // ��ǰ��������
    UInt32      m_idleTTL;      // �������ӵ���������(����)
    SInt64      m_lastCheck;	   // �ϴμ��������ӵ�ʱ���(����)
    UInt32      m_checkInterval;   // ���������ӵ�ʱ����
    Framework*  m_pFramework;
    
    OSMutex     m_lock;
    std::list<SharedPtr<WatcherConnection> > m_connsList;	// ���ӳؼ���
};

#endif //__ConnectPool_h__