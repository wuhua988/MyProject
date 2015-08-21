#include "GetUserInfoCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::GetUserInfoRequest,GetUserInfoCommand)

GetUserInfoCommand::GetUserInfoCommand()
:CliCmd(NULL)
{
	m_uCmdID = cmd::ums::GetUserInfoRequest;
}

GetUserInfoCommand::GetUserInfoCommand(Framework* pFramework)
:CliCmd(pFramework)
{
	m_uCmdID = cmd::ums::GetUserInfoRequest;
}

GetUserInfoCommand::~GetUserInfoCommand()
{
	LOG_DEBUG("~GetUserInfoCommand");
}

void GetUserInfoCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
	    LOG_DEBUG("Receive GetUserInfoCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool GetUserInfoCommand::SendRequest()
{
	umsProtocol::GetUserInfoRequest UserInfoReq;
	UserInfoReq.set_user_id(m_user_id);  
	
	string data;
	if(UserInfoReq.SerializeToString(&data))
	{
		SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::GetUserInfoRequest,2,200,data);
	}else{
		LOG_DEBUG("Serialize GetUserInfoRequest Meaasge Failed");
		return false;
	}
	
	LOG_DEBUG("Send GetUserInfoRequest:"
	         "ConnID    :%u,"          
			 "SeqID     :%u,"              
			 "user_id   :%u,",        
			 m_uiUseConnID,                 
			 m_uiSeq,   
			 m_user_id);
	StartTimer();
	return true;
}

void GetUserInfoCommand::OnResponse(MsgBlock* pMsgBlock)
{
	assert(pMsgBlock != NULL);
	StopTimer();
	CliCmd::OnResponse();
	m_responseTime = OS::Milliseconds();
	umsProtocol::GetUserInfoResponse pUserInfoRes;
    if(!pUserInfoRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
	{
        LOG_DEBUG("Deserialize GetUserInfoResponse Failed: \
		         ConnID:%u,                             \
			     SeqID :%u,                             \
				 Cost  :%ld",
				 pMsgBlock->ConnID(),
			     pMsgBlock->Header()->Sequence(),
				 m_responseTime - m_requestTime);
    }
	
	umsProtocol::Result res = pUserInfoRes.result();
    UInt32 code = res.code();                       // ´íÎó´úÂë 0³É¹¦£¬1Ê§°Ü
	string msg = res.msg();	                        // ´íÎóÃèÊö
	
	LOG_DEBUG("Receive GetUserInfoResponse:"
	         "ConnID    :%u,"  
			 "SeqID     :%u,"  
			 "Cost      :%ld,"
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
             pUserInfoRes.user_id(),
	         pUserInfoRes.tt_number(),
	         pUserInfoRes.user_name().c_str(),
	         pUserInfoRes.nick_name().c_str(),
	         pUserInfoRes.sex(),
	         pUserInfoRes.mobile().c_str(),
	         pUserInfoRes.birthday().c_str(),
	         pUserInfoRes.email().c_str(),
	         pUserInfoRes.user_grade(),
	         pUserInfoRes.age(),
	         pUserInfoRes.constellation(),
	         pUserInfoRes.sign().c_str(),
	         pUserInfoRes.like().c_str(),
	         pUserInfoRes.city().c_str(),
	         pUserInfoRes.personal_desc().c_str(),
	         pUserInfoRes.real_name().c_str(),
	         pUserInfoRes.friend_id(),
			 code,
			 msg.c_str());
	delete pMsgBlock;
}

void GetUserInfoCommand::SetParam(UInt64 user_id)
{
	m_user_id = user_id;
}
