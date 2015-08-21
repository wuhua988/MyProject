#include "QuitUserCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::QuitUserRequest,QuitUserCommand)

QuitUserCommand::QuitUserCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::ums::QuitUserRequest;
}

QuitUserCommand::QuitUserCommand(Framework* pFramework)
:CliCmd(pFramework)
{
    m_uCmdID = cmd::ums::QuitUserRequest;
}

QuitUserCommand::~QuitUserCommand()
{
    LOG_DEBUG("~QuitUserCommand");
}

void QuitUserCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
        LOG_DEBUG("Receive QuitUserCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool QuitUserCommand::SendRequest()
{
    umsProtocol::QuitUserRequest quitReq;
    quitReq.set_user_id(m_user_id);
    quitReq.set_terminal_type(m_terminal_type);
    quitReq.set_token(m_token);

    string data;
    if(quitReq.SerializeToString(&data))
    {
        SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::QuitUserRequest,2,200,data);
    }else{
        LOG_DEBUG("Serialize QuitUserRequest Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send QuitUserRequest:"
              "ConnID    :%u,"          
              "SeqID     :%u,"               
              "user_id   :%u," 
              "term_type :%u,"          
              "token     :%s,",        
              m_uiUseConnID,                 
              m_uiSeq,                        
              m_user_id,  
              m_terminal_type,			 
              m_token.c_str());
    StartTimer();
    return true;
}

void QuitUserCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    umsProtocol::QuitUserResponse quitRes;
    if(!quitRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
    {
        LOG_DEBUG("Deserialize QuitUserResponse Failed: \
                  ConnID:%u,                             \
                  SeqID :%u,                             \
                  Cost  :%ld",
                  pMsgBlock->ConnID(),
                  pMsgBlock->Header()->Sequence(),
                  m_responseTime - m_requestTime);
    }

    umsProtocol::Result res = quitRes.result();
    UInt32 code = res.code();                       // ´íÎó´úÂë 0³É¹¦£¬1Ê§°Ü
    string msg = res.msg();	                        // ´íÎóÃèÊö
    UInt64 user_id = quitRes.user_id();             // ÓÃ»§ID

    LOG_DEBUG("Receive QuitUserResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"  
              "Cost      :%ld,"
              "user_id   :%u,"   
              "code      :%u,"  
              "msg       :%s,",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime,
              user_id,  
              code,
              msg.c_str());
    delete pMsgBlock;
}

void QuitUserCommand::SetParam(UInt64        user_id
                               ,UInt32        terminal_type
                               ,const string& token)
{
    m_terminal_type = terminal_type;
    m_user_id       = user_id    ;
    m_token         = token      ;
}
