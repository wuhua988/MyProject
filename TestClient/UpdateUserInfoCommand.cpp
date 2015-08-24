#include "UpdateUserInfoCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::UpdateUserInfoRequest,UpdateUserInfoCommand)

UpdateUserInfoCommand::UpdateUserInfoCommand()
:CliCmd(NULL)
{
    m_uCmdID = cmd::ums::UpdateUserInfoRequest;
}

UpdateUserInfoCommand::UpdateUserInfoCommand(Framework* pFramework)
:CliCmd(pFramework)
{
    m_uCmdID = cmd::ums::UpdateUserInfoRequest;
}

UpdateUserInfoCommand::~UpdateUserInfoCommand()
{
    LOG_DEBUG("~UpdateUserInfoCommand");
}

void UpdateUserInfoCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
        LOG_DEBUG("Receive UpdateUserInfoCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool UpdateUserInfoCommand::SendRequest()
{
    umsProtocol::UpdateUserInfoRequest uUIReq;
    uUIReq.set_user_id(      m_user_id);		
    uUIReq.set_nick_name(    m_nick_name);		
    uUIReq.set_sex(          m_sex);			
    uUIReq.set_mobile(       m_mobile);		
    uUIReq.set_birthday(     m_birthday);		
    uUIReq.set_email(        m_email);			
    uUIReq.set_age(          m_age);			
    uUIReq.set_constellation(m_constellation);	
    uUIReq.set_sign(         m_sign);			
    uUIReq.set_like(         m_like);	        
    uUIReq.set_city(         m_city);			
    uUIReq.set_personal_desc(m_personal_desc);	
    uUIReq.set_real_name(    m_real_name);	

    string data;
    if(uUIReq.SerializeToString(&data))
    {
        SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::UpdateUserInfoRequest,2,200,data);
    }else{
        LOG_DEBUG("Serialize UpdateUserInfoRequest Meaasge Failed");
        return false;
    }

    LOG_DEBUG("Send UpdateUserInfoRequest:"
              "ConnID    :%u,"          
              "SeqID     :%u,"          
              "user_id:%u,"          
              "nick_name  :%s,"          
              "sex  :%u,"          
              "mobile  :%s,"          
              "birthday:%s,"          
              "email   :%s,"          
              "age   :%u,"  
              "constellation   :%u,"
              "sign   :%s,"
              "like   :%s,"
              "city   :%s,"
              "personal_desc   :%s,"	
              "real_name   :%s,",        
              m_uiUseConnID,                 
              m_uiSeq,			 
              m_user_id,
              m_nick_name.c_str(),	
              m_sex,			
              m_mobile.c_str(),		
              m_birthday.c_str(),		
              m_email.c_str(),		
              m_age,			
              m_constellation,
              m_sign.c_str(),			
              m_like.c_str(),	        
              m_city.c_str(),			
              m_personal_desc.c_str(),
              m_real_name.c_str());
    StartTimer();
    return true;
}

void UpdateUserInfoCommand::OnResponse(MsgBlock* pMsgBlock)
{
    assert(pMsgBlock != NULL);
    StopTimer();
    CliCmd::OnResponse();
    m_responseTime = OS::Milliseconds();
    umsProtocol::UpdateUserInfoResponse uUIRes;
    if(!uUIRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
    {
        LOG_DEBUG("Deserialize UpdateUserInfoResponse Failed: \
                  ConnID:%u,                                   \
                  SeqID :%u,                                   \
                  Cost  :%ld",
                  pMsgBlock->ConnID(),
                  pMsgBlock->Header()->Sequence(),
                  m_responseTime - m_requestTime);
    }

    umsProtocol::Result res = uUIRes.result();
    UInt32 code = res.code();                       // ´íÎó´úÂë 0³É¹¦£¬1Ê§°Ü
    string msg = res.msg();	                        // ´íÎóÃèÊö

    LOG_DEBUG("Receive UpdateUserInfoResponse:"
              "ConnID    :%u,"  
              "SeqID     :%u,"  
              "Cost      :%ld,"
              "code      :%u,"  
              "msg       :%s,",
              pMsgBlock->ConnID(),
              pMsgBlock->Header()->Sequence(),
              m_responseTime - m_requestTime,
              code,
              msg.c_str());
    delete pMsgBlock;
}

void UpdateUserInfoCommand::SetParam(UInt64        user_id,	
                                     const string& nick_name,	
                                     UInt32        sex,		
                                     const string& mobile,	
                                     const string& birthday,	
                                     const string& email,		
                                     UInt32        age,		
                                     UInt32        constellation,
                                     const string& sign,		
                                     const string& like,	       
                                     const string& city,		
                                     const string& personal_desc,
                                     const string& real_name)
{
    m_user_id       = user_id;
    m_nick_name		= nick_name;	 
    m_sex			= sex;		
    m_mobile		= mobile;	
    m_birthday		= birthday;	
    m_email			= email;		
    m_age			= age;		
    m_constellation	= constellation;
    m_sign			= sign;		
    m_like	        = like;	      
    m_city			= city;		
    m_personal_desc	= personal_desc;
    m_real_name     = real_name;
}	