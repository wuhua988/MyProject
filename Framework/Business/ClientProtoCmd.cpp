#include "ClientProtoCmd.h"
#include "ServerProtoCmd.h"
#include "BusinessTimerThread.h"
#include "../EventThreadPool.h"

ClientProtoCmd::ClientProtoCmd(ServerProtoCmd* pServerCmd,Framework* pFramework)
: ProtoCmdBase(pFramework)
, m_uiUseConnID(0)
, m_uiSeq(0)
, m_timer(this,30,0)
, m_pServerProtocolCmd(pServerCmd)
, m_pTimerThread(NULL)
{

}

ClientProtoCmd::~ClientProtoCmd()
{
    if(m_timer.IsTiming())
        StopTimer();
}

void ClientProtoCmd::OnTimer()
{
    LOG_NOTICE("OnTimer:Nothing To Do!");
}

bool ClientProtoCmd::ScheduleCache(SharedPtr<ProtoCmdBase> cmdPtr,UInt32 uiServerID)
{
    LOG_NOTICE("ScheduleCache:Nothing To Do,ServerID:%u",uiServerID);
    return false;
}

void ClientProtoCmd::StartTimer()
{
    m_pTimerThread = EventThreadPool::GetRandomTimerThread();
    if(m_pTimerThread == NULL)
    {
        LOG_ERROR("Get BusinessTimerThread From ThreadPool Failed!");
        return;
    }
    m_pTimerThread->RegTimer(&m_timer);
}

void ClientProtoCmd::StopTimer()
{
    if(m_pTimerThread == NULL)
    {
        LOG_ERROR("BusinessTimerThread Is NULL Can't Stop Timer!");
        return;
    }
    m_pTimerThread->CancelTimeout(&m_timer);
}

