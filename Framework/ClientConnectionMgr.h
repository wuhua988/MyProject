#ifndef __ClientConnectionMgr_h__
#define __ClientConnectionMgr_h__
#include <vector>
#include <string>
#include <map>
#include "SharedPtr.h"
#include "OSMutex.h"
#include "OSHeaders.h"
#include "WatcherCliConn.h"
#include "Singleton.h"

class MsgBlock;
class Framework;

class ClientHost
{
public:
    typedef std::vector<UInt32 >::size_type clientSize;
    ClientHost(const char* strHost, UInt16 uiPort, UInt32 uiServerID);
    ~ClientHost();	
    
    bool       AddCliConn(UInt32 uiConnID); 
    UInt32     GetServerID(); 
    bool       GetCliConnID(UInt32& uiConnID);                                    
    clientSize GetConnCount();                                                                    
    bool       Close();                                                                             
    bool       Close(UInt32 connID);                                                               
    bool       Remove(UInt32 uiConnID);
    void       SetTimerInfo(Float64 fExpire,Float64 fInterval);
private:
    typedef std::vector<UInt32 > vecClientConn;
    ClientHost();
    ClientHost(const ClientHost& other);
    ClientHost& operator=(const ClientHost& other);

    vecClientConn m_clientConns;
    std::string   m_strServerIdent;      //ip + port
    std::string   m_strHostIP;           //remote ip
    UInt16        m_uiPort;              //remote port
    UInt32        m_uiServerID; 
    Float64       m_fExpire;
    Float64       m_fInterval;
    OSMutex       m_ConnsMutex;
};
typedef SharedPtr<ClientHost> ClientHostPtr;

class HostService
{
public: 
    typedef std::map<std::string,ClientHostPtr >::size_type hostSize;
    HostService(UInt32 uiServerID);
    ~HostService();
    bool     AddCliConn(const char* strHost,UInt16 uiPort,Float64 fExpire,Float64 fInterval);
    bool     AddCliConnToHost(WatcherCliConnPtr pClientConn);
    hostSize GetHostCount();
    UInt32   GetServerID();
    bool     GetCliConnID(UInt32& uiConnID);
    bool     Remove(WatcherCliConnPtr pClientConn);	
    bool     Close();
    bool     Close(UInt32 connID);
private:
    typedef std::map<std::string,ClientHostPtr > mapStr2Host;
    HostService();
    HostService(const HostService& other);
    HostService& operator=(const HostService& other);    
	
    mapStr2Host m_clientHost;	
    UInt32      m_uiServerID; 
	OSMutex     m_HostMutex;
};
typedef SharedPtr<HostService> HostServicePtr;

class ClientConnMgr
{
public:
    ClientConnMgr();
    ClientConnMgr(Framework* pFw);
    ~ClientConnMgr();
    void                     SetFramework(Framework* pFw);
    bool                     Connect(UInt32  uiServerID
                                    ,const   char* strHost
                                    ,UInt16  uiPort
                                    ,UInt32  uiConnNum
                                    ,Float64 fExpire
                                    ,Float64 fInterval); 
    void                     Close();                                         //关闭所有连接 
    bool                     Close(UInt32 uiServerID);                        //关闭ServerID的所有连接
    bool                     Close(UInt32 uiServerID,UInt32 uiConnID);        //关闭ServerID中的一条连接
    bool                     SendData(MsgBlock* pMsgBlock);                   //发送数据
    WatcherCliConnPtr        GetCliConn(UInt32 uiServerID);
    bool                     GetCliConnID(UInt32 uiServerID,UInt32& uiConnID);
    WatcherCliConnPtr        GetCliConn(UInt32 uiServerID,UInt32 uiConnID);
    static WatcherCliConnPtr GetCliConnPtr(UInt32 uiServerID,UInt32 uiConnID);
    bool                     AddCliConn(WatcherCliConnPtr pConn);             //由libev事件线程调用
    bool                     Remove(UInt32 uiConnID);
private:              
    HostServicePtr           GetHostServicePtr(UInt32 uiServerID); 
    WatcherCliConnPtr        GetCliConnByID(UInt32 uiConnID);
    
    ClientConnMgr(const ClientConnMgr& other);
    ClientConnMgr& operator=(const ClientConnMgr& other);
private:
    typedef std::map<UInt32,HostServicePtr >    mapSvrID2Service;
    typedef std::map<UInt32,WatcherCliConnPtr > mapConnID2Conn;
    mapSvrID2Service                            m_SvrID2Service;
    mapConnID2Conn                              m_ConnID2Conn;
    Framework*                                  m_pFramework;
    OSMutex                                     m_SvrID2Server_Mutex;
    OSMutex                                     m_ConnID2Conn_Mutex;
};

typedef Singleton<ClientConnMgr> gCliConnMgr;

#endif //__ClientConnectionMgr_h__