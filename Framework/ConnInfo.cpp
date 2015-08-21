#include "ConnInfo.h"
#include "WatcherConnection.h"
ConnInfo::ConnInfo()
{
    m_uiListenId = 0;
	m_uiConnId = 0;
    m_ConnState = STATUS_NONE;  
    m_pUserData = NULL;
    m_bActiveConn = false;
    m_pHandler = NULL;
}

ConnInfo::~ConnInfo()
{
}

std::string ConnInfo::GetRemoteAddr()
{
    if(m_bActiveConn)
        return m_active_conn_ip;
    return m_pHandler->GetRemoteAddr();
}


UInt16 ConnInfo::GetRemotePort()
{
    if(m_bActiveConn)
        return m_conn_port;
	return m_pHandler->GetRemotePort();
}

void ConnInfo::SetInterval(UInt32 iInterval)
{
    m_pHandler->m_pTimer->SetInterval(iInterval);
}

void ConnInfo::SetExpire(UInt32 iExpire)
{
    m_pHandler->m_pTimer->SetExpire(iExpire);
}

UInt32 ConnInfo::GetConnID() const 
{
    return m_pHandler->GetConnID();
}

UInt32 ConnInfo::GetServerID() const
{
	return m_pHandler->GetServerID();
}

void ConnInfo::SetServerID(UInt32 uiServerId)
{
	m_pHandler->SetServerID(uiServerId);
    //m_ServerID = uiServerId;
}