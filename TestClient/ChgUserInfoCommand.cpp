#include "ChgUserInfoCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::presence::ChangeOnlineState,ChgUserInfoCommand)

ChgUserInfoCommand::ChgUserInfoCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::presence::ChangeOnlineState;
}

ChgUserInfoCommand::ChgUserInfoCommand(Framework* pFramework)
    :CliCmd(pFramework)
{
    m_uCmdID = cmd::presence::ChangeOnlineState;
}

ChgUserInfoCommand::~ChgUserInfoCommand()
{
    LOG_DEBUG("~ChgUserInfoCommand");
}

void ChgUserInfoCommand::OnTimer()
{
    if(CliCmd::OnTimer(PresenceServerID))
        LOG_DEBUG("Receive ChgUserInfoCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",PresenceServerID,m_uiUseConnID,m_uiSeq);
}

bool ChgUserInfoCommand::SendRequest()
{
    tt_presence::ChgUserInfoRequset chgUserInfoReq;
    chgUserInfoReq.set_user_id(    m_user_id);
    chgUserInfoReq.set_acc_ident(  m_acc_ident);
    chgUserInfoReq.set_trans_ident(m_trans_ident);
    chgUserInfoReq.set_is_camera(  m_is_camera);
    chgUserInfoReq.set_ttype(      m_ttype);
    chgUserInfoReq.set_user_state( m_user_state);
    chgUserInfoReq.set_msg_type(   m_msg_type);

    string data;
    if(chgUserInfoReq.SerializeToString(&data))
    {
        SendMessage(PresenceServerID,m_uiUseConnID,m_uiSeq,cmd::presence::ChangeOnlineState,2,200,data);
    }else{
        LOG_DEBUG("Serialize ChgUserInfoRequset Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send ChgUserInfoRequset:"
              "ConnID     :%u,"          
              "SeqID      :%u,"          
              "user_id    :%u,"          
              "acc_ident  :%s,"          
              "trans_ident:%s,"          
              "is_camera  :%d,"          
              "ttype      :%d,"          
              "user_state :%d,"          
              "msg_type   :%d,",        
              m_uiUseConnID,                 
              m_uiSeq,                  
              m_user_id,    
              m_acc_ident.c_str(),    
              m_trans_ident.c_str(),   
              m_is_camera,    
              m_ttype,    
              m_user_state,
              m_msg_type);
    StartTimer();
    return true;
}

void ChgUserInfoCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    LOG_DEBUG("Receive ChgUserInfoResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"
              "Cost      :%ld",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime);
    delete pMsgBlock;
}

void ChgUserInfoCommand::SetParam(UInt64 user_id,                    
                                  const string& acc_ident,                    
                                  const string& trans_ident,                 
                                  bool   is_camera,                 
                                  UInt32 ttype,
                                  tt_presence::UserState user_state,
                                  tt_presence::UserInfoMessageType msg_type,
                                  const string& msg_content)
{
    m_user_id     = user_id    ;
    m_acc_ident   = acc_ident  ;
    m_trans_ident = trans_ident;
    m_is_camera   = is_camera  ;
    m_ttype       = ttype      ;
    m_user_state  = user_state ;
    m_msg_type    = msg_type   ;
    m_msg_content = msg_content;
}
