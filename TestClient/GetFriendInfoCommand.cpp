#include "GetFriendInfoCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::GetFriendInfoRequest,GetFriendInfoCommand)

GetFriendInfoCommand::GetFriendInfoCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::ums::GetFriendInfoRequest;
}

GetFriendInfoCommand::GetFriendInfoCommand(Framework* pFramework)
:CliCmd(pFramework)
{
    m_uCmdID = cmd::ums::GetFriendInfoRequest;
}

GetFriendInfoCommand::~GetFriendInfoCommand()
{
    LOG_DEBUG("~GetFriendInfoCommand");
}

void GetFriendInfoCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
        LOG_DEBUG("Receive GetFriendInfoCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool GetFriendInfoCommand::SendRequest()
{
    umsProtocol::GetFriendInfoRequest fFInfoReq;
    fFInfoReq.set_user_id(m_user_id);
    fFInfoReq.set_friend_id(m_friend_id);

    string data;
    if(fFInfoReq.SerializeToString(&data))
    {
        SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::GetFriendInfoRequest,2,200,data);
    }else{
        LOG_DEBUG("Serialize GetFriendInfoRequest Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send GetFriendInfoRequest:"
              "ConnID    :%u,"          
              "SeqID     :%u,"                   
              "friend_id :%lu,"          
              "user_id   :%lu,",        
              m_uiUseConnID,                 
              m_uiSeq,                      
              m_user_id,              
              m_friend_id);
    StartTimer();
    return true;
}

void GetFriendInfoCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    umsProtocol::GetFriendInfoResponse fInfoRes;
    if(!fInfoRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
    {
        LOG_DEBUG("Deserialize GetFriendInfoResponse Failed: \
                  ConnID:%u,                             \
                  SeqID :%u,                             \
                  Cost  :%ld",
                  pMsgBlock->ConnID(),
                  pMsgBlock->Header()->Sequence(),
                  m_responseTime - m_requestTime);
    }

    umsProtocol::Result res = fInfoRes.result();
    UInt32 code = res.code();                       // ´íÎó´úÂë 0³É¹¦£¬1Ê§°Ü
    string msg = res.msg();	                        // ´íÎóÃèÊö

    LOG_DEBUG("Receive GetFriendInfoResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"  
              "cost      :%ld"
              "user_id   :%u,"
              "tt_number :%u,"  
              "user_name :%s,"  
              "nick_name :%s,"  
              "sex       :%u,"  
              "mobile    :%s,"  
              "birthday  :%s,"  
              "email     :%s," 
              "user_grade:%u," 
              "age       :%u," 
              "constella :%u,"
              "sign      :%s,"
              "like      :%s," 
              "city      :%s," 
              "personal  :%s," 
              "real_name :%s," 
              "friend_id :%u," 
              "code      :%u,"  
              "msg       :%s,",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime,
              fInfoRes.user_id(),
              fInfoRes.tt_number(),
              fInfoRes.user_name().c_str(),
              fInfoRes.nick_name().c_str(),
              fInfoRes.sex(),
              fInfoRes.mobile().c_str(),
              fInfoRes.birthday().c_str(),
              fInfoRes.email().c_str(),
              fInfoRes.user_grade(),
              fInfoRes.age(),
              fInfoRes.constellation(),
              fInfoRes.sign().c_str(),
              fInfoRes.like().c_str(),
              fInfoRes.city().c_str(),
              fInfoRes.personal_desc().c_str(),
              fInfoRes.real_name().c_str(),
              fInfoRes.friend_id(),
              code,
              msg.c_str());
    delete pMsgBlock;
}

void GetFriendInfoCommand::SetParam(UInt64 user_id,UInt64 friend_id)
{
    m_user_id   = user_id;
    m_friend_id = friend_id;
}
