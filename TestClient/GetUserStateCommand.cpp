#include "GetUserStateCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::presence::GetOnlineState,GetUserStateCommand)

GetUserStateCommand::GetUserStateCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::presence::GetOnlineState;
}

GetUserStateCommand::GetUserStateCommand(Framework* pFramework)
:CliCmd(pFramework)
{
    m_uCmdID = cmd::presence::GetOnlineState;
}

GetUserStateCommand::~GetUserStateCommand()
{
    LOG_DEBUG("~GetUserStateCommand");
}

void GetUserStateCommand::OnTimer()
{
    if(CliCmd::OnTimer(PresenceServerID))
        LOG_DEBUG("Receive GetUserStateCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",PresenceServerID,m_uiUseConnID,m_uiSeq);
}

bool GetUserStateCommand::SendRequest()
{
    tt_presence::GetUserStateRequset uStateReq;
    std::vector<UInt64>::const_iterator it;
    for (it = m_user_list.begin(); it != m_user_list.end(); ++it)
    {
        uStateReq.add_user_list(*it);
    }
    string data;
    if(uStateReq.SerializeToString(&data))
    {
        SendMessage(PresenceServerID,m_uiUseConnID,m_uiSeq,cmd::presence::GetOnlineState,2,200,data);
    }else{
        LOG_DEBUG("Serialize GetUserStateRequset Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send GetUserStateRequset:"
              "ConnID    :%u,"          
              "SeqID     :%u,"          
              "id_size   :%u,",        
              m_uiUseConnID,                 
              m_uiSeq,                     
              m_user_list.size());
    StartTimer();
    return true;
}

void GetUserStateCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    tt_presence::GetUserStateResponse uStateRes;
    if(!uStateRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
    {
        LOG_DEBUG("Deserialize GetUserStateResponse Failed: \
                  ConnID:%u,                             \
                  SeqID :%u,                             \
                  Cost  :%ld",
                  pMsgBlock->ConnID(),
                  pMsgBlock->Header()->Sequence(),
                  m_responseTime - m_requestTime);
    }

    for (int i = 0; i < uStateRes.user_list_size(); i++)
    {
        //uStateRes->user_list(i).user_id());
        //uStateRes->user_list(i).user_state());
        //uStateRes->user_list(i).ttype());
        //std::string str = uStateRes->user_list(i).trans_ident();
    }

    LOG_DEBUG("Receive GetUserStateResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"  
              "Cost      :%ld,"
              "id_size   :%u,",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime,
              uStateRes.user_list_size());
    delete pMsgBlock;
}

void GetUserStateCommand::SetParam(const std::vector<UInt64>& user_list)
{
    m_user_list = user_list;
}
