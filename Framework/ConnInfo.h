#ifndef __ConnInfo_h__
#define __ConnInfo_h__
#include <string>
#include "OSHeaders.h"

class WatcherConnection;
class ConnInfo
{
public:
    ///连接状态定义
    enum ConnStatus
    {
        STATUS_NONE = 0,
        STATUS_CONNECTING,
        STATUS_CONNECTED,
        STATUS_DISCONNECT
    };
public:
    ConnInfo();
    ~ConnInfo();
public:
    UInt32             GetConnID() const; 
    UInt32             GetServerID() const; 
    void               SetConnID(UInt32 uiConnId);
    void               SetServerID(UInt32 uiConnId);
                       
    UInt32             GetListenId() const;
    void               SetListenId(UInt32 uiListenId);
    UInt16             GetState() const;
    void               SetState(UInt16 unState);
                       
    time_t             GetCreateTime() const;
    void               SetCreateTime(time_t ttTime);
                       
    bool               IsActive() const;
    void               SetActive(bool bActive);
                       
    void*              GetUserData() const;
    void               SetUserData(void* userData);
	                   
    std::string        GetRemoteAddr();
    void               SetRemoteAddr(std::string strIP);
                       
    UInt16             GetRemotePort();
    void               SetRemotePort(UInt16 port);
                       
    void               SetInterval(UInt32 iInterval);
    void               SetExpire(UInt32 iExpire);
	                   
    void               SetConnIdent(const std::string& strIdent);
    std::string&       GetConnIdent();
    WatcherConnection* GetHandler();
    void               SetHandler(WatcherConnection*);
private:
    UInt32             m_uiListenId; 
    UInt32             m_uiConnId;
    UInt16             m_ConnState; 
    time_t             m_CreateTime;
    bool               m_bActiveConn;        
    void*              m_pUserData;
                       
    std::string        m_active_conn_ip;
    std::string        m_strIdent;
    UInt16             m_conn_port;
    WatcherConnection* m_pHandler; 
};

inline void ConnInfo::SetConnID(UInt32 uiConnId)
{
    m_uiConnId = uiConnId;
}

inline UInt32 ConnInfo::GetListenId() const 
{
    return m_uiListenId;
}

inline void ConnInfo::SetListenId(UInt32 uiListenId)
{
    m_uiListenId = uiListenId;
}


inline UInt16 ConnInfo::GetState() const
{
    return m_ConnState;
}
inline void ConnInfo::SetState(UInt16 unState)
{
    m_ConnState = unState;
}

inline time_t ConnInfo::GetCreateTime() const 
{
    return m_CreateTime;
}

inline void ConnInfo::SetCreateTime(time_t ttTime)
{
    m_CreateTime = ttTime;
}


inline bool ConnInfo::IsActive() const 
{
    return m_bActiveConn;
}

inline void ConnInfo::SetActive(bool bActive)
{
    m_bActiveConn = bActive;
}

inline void*  ConnInfo::GetUserData() const
{
    return  m_pUserData;
}

inline void ConnInfo::SetUserData(void* userData)
{
    m_pUserData = userData;
}

inline WatcherConnection* ConnInfo::GetHandler()
{
    return m_pHandler;
}

inline void ConnInfo::SetHandler(WatcherConnection*  pHandler)
{
    m_pHandler = pHandler;
}

inline void ConnInfo::SetRemoteAddr(std::string strIP)
{
	m_active_conn_ip = strIP;
}

inline void ConnInfo::SetRemotePort(UInt16 port)
{
	m_conn_port = port;
}

inline void ConnInfo::SetConnIdent(const std::string& strIdent)
{
	m_strIdent = strIdent;
}

inline std::string& ConnInfo::GetConnIdent()
{
	return m_strIdent;
}

#endif//__ConnInfo_h__