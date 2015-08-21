#include "CliCmd.h"
#include "ClientConnectionMgr.h"
#include "ConnUserData.h"
#include "CommonDef.h"
#include "TestCaseCtx.h"

CliCmd::CliCmd()
:ClientProtoCmd(NULL,NULL)
{
    //m_timer.SetTimer(30,0);
}

CliCmd::CliCmd(Framework* pFramework)
:ClientProtoCmd(NULL,pFramework)
{
    //m_timer.SetTimer(30,0);
    TestCaseCtx::m_createCmdObjs++;
}

CliCmd::~CliCmd()
{
    TestCaseCtx::m_deleteCmdObjs++;
}

bool CliCmd::ScheduleCache(SharedPtr<ProtoCmdBase> cmdPtr,UInt32 uiServerID)
{
    UInt32 connID = 0;; 
    if(!GetConnID(uiServerID,connID))
        return false;
    m_uiUseConnID = connID;
    WatcherCliConnPtr CCP = gCliConnMgr::instance()->GetCliConn(uiServerID,connID);
    if(!CCP.isNull())
    {
        ConnUserData* pUData = (ConnUserData*)CCP->GetConnInfo()->GetUserData();
        if(!pUData)
        {
            LOG_DEBUG("ConnUserData Is NULL When Set Cache,ServerID:%u,ConnID:%u",uiServerID,connID);
            return false;
        }
        m_uiSeq = GetSeqID();
        return pUData->AddCmd(m_uiSeq,cmdPtr);
    }else{
        LOG_DEBUG("There Are No Connection To Send Command,ServerID:%u",uiServerID);
    }
    return false;
}

bool CliCmd::GetConnID(UInt32 uiServerID,UInt32& uiConnID)
{
    WatcherCliConnPtr cliPtr = gCliConnMgr::instance()->GetCliConn(uiServerID);
    if(cliPtr.isNull())
    {
        LOG_DEBUG("Can't Find WatcherCliConn By ServerID:%u",uiServerID);
        return false;
    }
    uiConnID = cliPtr->GetConnID();
    return true;
}

bool CliCmd::OnTimer(UInt32 uiServerID)
{
    SharedPtr<WatcherConnection> connPtr = gCliConnMgr::instance()->GetCliConn(uiServerID,m_uiUseConnID);
    m_responseTime = OS::Milliseconds();
    LOG_DEBUG("OnTimer,ServerID:%u,ConnID:%u,SeqID:%u,RequsetTime:%ld,ResponseTime:%ld,Cost:%ld,OnTimer->Expire:%f"
             ,uiServerID
             ,m_uiUseConnID
             ,m_uiSeq
             ,m_requestTime
             ,m_responseTime
             ,m_responseTime - m_requestTime
             ,m_timer.GetExpire());
    if(!connPtr.isNull())
    {
        ConnInfo* pCIF = (ConnInfo*)connPtr->GetConnInfo();
        ConnUserData* pUData = (ConnUserData*)pCIF->GetUserData();
        if(!pCIF || !pCIF->GetUserData())
        {
            LOG_DEBUG("ConnUserData Is NULL When OnTimer,ServerID:%u,ConnID:%u,SeqID:%u",uiServerID,m_uiUseConnID,m_uiSeq);
            return false;
        }
        SharedPtr<ProtoCmdBase> cmdPtr = pUData->GetCmd_R(m_uiSeq);
        if(!cmdPtr.isNull())
        {
            TestCaseCtx::m_timeoutNum++;	
            return true;
        }
        else
            LOG_DEBUG("Command Was Already Removed When OnTimer,ServerID:%u,ConnID:%u,SeqID:%u",uiServerID,m_uiUseConnID,m_uiSeq);			
    }
    return false;
}

void CliCmd::OnResponse()
{
    m_responseTime = OS::Milliseconds();
    SInt64 interval = m_responseTime - m_requestTime;
    gTestCaseCtxPtr::instance()->SetTotle(interval);
    gTestCaseCtxPtr::instance()->SetMinTime(interval);
    gTestCaseCtxPtr::instance()->SetMaxTime(interval);
}
