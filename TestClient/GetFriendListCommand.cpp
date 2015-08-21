#include "GetFriendListCommand.h"
#include "Command_Define.h"
#include "ClientConnectionMgr.h"
#include "CommonDef.h"
#include "ConnUserData.h"

IMPLEMENT_CLASS(cmd::ums::GetFriendListRequest,GetFriendListCommand)

GetFriendListCommand::GetFriendListCommand()
:CliCmd(NULL)
{
	m_uCmdID = cmd::ums::GetFriendListRequest;
}

GetFriendListCommand::GetFriendListCommand(Framework* pFramework)
:CliCmd(pFramework)
{
	m_uCmdID = cmd::ums::GetFriendListRequest;
}

GetFriendListCommand::~GetFriendListCommand()
{
	LOG_DEBUG("~GetFriendListCommand");
}

void GetFriendListCommand::OnTimer()
{
    if(CliCmd::OnTimer(UMSServerID))
	    LOG_DEBUG("Receive GetFriendListCommand Timer Event,ServerID:%u,ConnID:%u,SeqID:%u",UMSServerID,m_uiUseConnID,m_uiSeq);
}

bool GetFriendListCommand::SendRequest()
{
	umsProtocol::GetFriendListRequest FListReq;
	FListReq.set_user_id(m_user_id);
	FListReq.set_version_id(0);
	
	string data;
	if(FListReq.SerializeToString(&data))
	{
		SendMessage(UMSServerID,m_uiUseConnID,m_uiSeq,cmd::ums::GetFriendListRequest,2,200,data);
	}else{
		LOG_DEBUG("Serialize GetFriendListRequest Meaasge Failed");
		return false;
	}
	
	LOG_DEBUG("Send GetFriendListRequest:"
	         "ConnID    :%u,"          
			 "SeqID     :%u,"                   
			 "ver_id    :%u,"          
			 "user_id   :%u,",        
			 m_uiUseConnID,                 
			 m_uiSeq,                       
			 m_user_id,              
			 m_version_id);
	StartTimer();
	return true;
}

void GetFriendListCommand::OnResponse(MsgBlock* pMsgBlock)
{
	assert(pMsgBlock != NULL);
	StopTimer();
	CliCmd::OnResponse();
	m_responseTime = OS::Milliseconds();
	umsProtocol::GetFriendListResponse fListRes;
    if(!fListRes.ParseFromArray(pMsgBlock->get_ptr(),pMsgBlock->length()))
	{
        LOG_DEBUG("Deserialize GetFriendListResponse Failed: \
		         ConnID:%u,                             \
			     SeqID :%u                              \
				 Cost  :%ld",
				 pMsgBlock->ConnID(),
			     pMsgBlock->Header()->Sequence(),
				 m_responseTime - m_requestTime);
    }
	
	umsProtocol::Result res = fListRes.result();
    UInt32 code = res.code();                       // ´íÎó´úÂë 0³É¹¦£¬1Ê§°Ü
	string msg = res.msg();	                        // ´íÎóÃèÊö
	
	std::vector<UInt64> friend_id_list;
	for (int i = 0; i < fListRes.friend_id_size(); i++)
	{
		friend_id_list.push_back(fListRes.friend_id(i));
	}
	
	LOG_DEBUG("Receive GetFriendListResponse:"
	         "ConnID      :%u,"  
			 "SeqID       :%u,"  
			 "Cost        :%ld,"
			 "user_id     :%u,"  
			 "version_id  :%u,"  
			 "friend_size :%u,"  
			 "code        :%u,"  
			 "msg         :%s,",
			 pMsgBlock->ConnID(),
			 pMsgBlock->Header()->Sequence(),
			 m_responseTime - m_requestTime,
			 fListRes.user_id(),  
			 fListRes.version_id(), 
			 fListRes.friend_id_size(),
			 code,
			 msg.c_str());
	delete pMsgBlock;
}

void GetFriendListCommand::SetParam(UInt64 version_id,UInt64 user_id)
{
	m_version_id = version_id;
	m_user_id    = user_id    ;
}
