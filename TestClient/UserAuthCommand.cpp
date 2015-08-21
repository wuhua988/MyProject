#include "UserAuthCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::AuthUserRequest,UserAuthCommand)

UserAuthCommand::UserAuthCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::ums::AuthUserRequest;
}

UserAuthCommand::UserAuthCommand(Framework* pFramework)
    :CliCmd(pFramework)
{
    m_uCmdID = cmd::ums::AuthUserRequest;
}

UserAuthCommand::~UserAuthCommand()
{
    LOG_DEBUG("~UserAuthCommand");
}

void UserAuthCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
        LOG_DEBUG("Receive UserAuthCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool UserAuthCommand::SendRequest()
{
    umsProtocol::AuthUserRequest pauthreq;
    pauthreq.set_login_name(m_login_name);
    pauthreq.set_password(m_password);
    pauthreq.set_terminal_type(m_terminal);
    pauthreq.set_mac_addr(m_mac_addr);
    pauthreq.set_login_type(m_login_type);
    pauthreq.set_user_ip(m_user_ip);
    if (m_login_type == 5)
    {
        pauthreq.set_user_id(m_user_id);
        pauthreq.set_token(m_token);
    }    

    string data;
    if(pauthreq.SerializeToString(&data))
    {
        SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::AuthUserRequest,2,200,data);
    }else{
        LOG_DEBUG("Serialize AuthUserRequest Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send AuthUserRequest:"
              "ConnID    :%u,"          
              "SeqID     :%u,"          
              "login_name:%s,"          
              "password  :%s,"          
              "terminal  :%u,"          
              "mac_addr  :%s,"          
              "login_type:%u,"          
              "user_ip   :%s,"          
              "user_id   :%u,"          
              "token     :%s,",        
              m_uiUseConnID,                 
              m_uiSeq,                  
              m_login_name.c_str(),   
              m_password.c_str(),     
              m_terminal,             
              m_mac_addr.c_str(),     
              m_login_type,           
              m_user_ip.c_str(),      
              m_user_id,              
              m_token.c_str());
    StartTimer();
    return true;
}

void UserAuthCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    umsProtocol::AuthUserResponse pauthres;
    if(!pauthres.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
    {
        LOG_DEBUG("Deserialize AuthUserResponse Failed: \
                  ConnID:%u,                            \
                  SeqID :%u,                            \
                  Cost  :%ld",
                  pMsgBlock->ConnID(),
                  pMsgBlock->Header()->Sequence(),
                  m_responseTime - m_requestTime);
    }

    umsProtocol::Result res = pauthres.result();
    UInt32 code = res.code();                       // 错误代码 0成功，1失败
    string msg = res.msg();	                        // 错误描述
    UInt64 user_id = pauthres.user_id();            // 用户ID
    UInt64 tt_number = pauthres.tt_number();        // TT号
    string nick_name = pauthres.nick_name();        // 用户昵称
    string avatar_id = pauthres.avatar_id();        // 用户头像ID
    string avatar = pauthres.avatar();	            // 用户自定义头像
    string token = pauthres.token();	            // 登录令牌

    LOG_DEBUG("Receive AuthUserResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"  
              "Cost      :%ld,"
              "StartTime :%ld,"
              "user_id   :%u,"  
              "tt_number :%u,"  
              "nick_name :%s,"  
              "avatar_id :%s,"  
              "avatar    :%s,"  
              "token     :%s,"  
              "code      :%u,"  
              "msg       :%s,",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime,
              m_responseTime,
              user_id,  
              tt_number,
              nick_name.c_str(),
              avatar_id.c_str(),
              avatar.c_str(),   
              token.c_str(),
              code,
              msg.c_str());
    delete pMsgBlock;
}

void UserAuthCommand::SetParam(const string& login_name
                               ,const string& password
                               ,UInt32 terminal
                               ,const string& mac_addr
                               ,UInt32 login_type
                               ,const string& user_ip
                               ,UInt64 user_id
                               ,const string& token)
{
    m_login_name = login_name ;
    m_password   = password   ;
    m_terminal   = terminal   ;
    m_mac_addr   = mac_addr   ;
    m_login_type = login_type ;
    m_user_ip    = user_ip    ;
    m_user_id    = user_id    ;
    m_token      = token      ;
}