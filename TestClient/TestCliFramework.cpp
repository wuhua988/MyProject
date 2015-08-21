#include "TestCliFramework.h"
#include "PBReflect.h"
#include "Business/ProtoCmdBase.h"
#include "MsgHeader.h"
#include "ConnUserData.h"
#include "TestCaseCtx.h"
#include "OS.h"

void TestCliFramework::OnMessage(MsgBlock* pMsgBlock)
{
	assert(pMsgBlock != NULL);
	//PBReflect::ErrorCode error;
	//SharedPtr<Message> msgPtr = PBReflect::ParsePBMsg(pMsgBlock->get_ptr()
	//                                                 ,pMsgBlock->length()
	//												   ,pMsgBlock->Header()->Command()
	//												   ,&error);
	if(pMsgBlock->Header()->Mode() == MsgHeader::TMCP_Mod_Request ||
	   pMsgBlock->Header()->Mode() == MsgHeader::TMCP_Mod_Notify)
	{
	    SharedPtr<ProtoCmdBase> cmdPtr = ProtoCmdBase::CreateObject(pMsgBlock->Header()->Command());
	}
	else if(pMsgBlock->Header()->Mode() == MsgHeader::TMCP_Mod_Response)
	{		
		ConnInfo* pCIF = GetConnInfo(pMsgBlock->ServerID(),pMsgBlock->ConnID());
		if(pCIF)
		{
			ConnUserData* pUData = (ConnUserData*)pCIF->GetUserData();
			if(!pUData)
			{
				LOG_DEBUG("ConnUserData Is NULL When OnMessage,ServerID:%u,ConnID:%u"
				          ,pMsgBlock->ServerID()
						  ,pMsgBlock->ConnID());
				return;
			}
			SharedPtr<ProtoCmdBase> cmdPtr = pUData->GetCmd_R(pMsgBlock->Header()->Sequence());
			if(!cmdPtr.isNull())
			{
			    cmdPtr->OnResponse(pMsgBlock);
			    TestCaseCtx::m_ResponseNum++;
				SInt64 eTime = OS::Milliseconds();
		        gTestCaseCtxPtr::instance()->SetEndTime(eTime);
			}
			else
			{
				LOG_DEBUG("Can't Find ProtoCmd When OnResponse:ServerID:%u,ConnID:%u,Seq:%u"
			             ,pMsgBlock->ServerID()
						 ,pMsgBlock->ConnID()
						 ,pMsgBlock->Header()->Sequence());
			}
		}
	}
	else
	{
		LOG_DEBUG("Error Mode:%d",pMsgBlock->Header()->Mode());
	}
}

int TestCliFramework::OnConnCreated(ConnInfo* pConnInfo)
{
	assert(pConnInfo != NULL);
	LOG_DEBUG("TestCliFramework Receive Connection Created Event,ConnID:%u,RemoteIP:%s,RemotePort:%d"
	         ,pConnInfo->GetConnID(),pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
    if(pConnInfo->GetUserData() == NULL)
	{
		pConnInfo->SetUserData(new ConnUserData());
	}
	return 0;
}

int TestCliFramework::OnFailConnect(ConnInfo* pConnInfo)
{
	assert(pConnInfo != NULL);
	LOG_DEBUG("TestCliFramework Receive Connection Failed Event,ConnID:%u,RemoteIP:%s,RemotePort:%d"
	         ,pConnInfo->GetConnID(),pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
	return 0;
}

int TestCliFramework::OnConnClosed(ConnInfo* pConnInfo)
{
	assert(pConnInfo != NULL);
	LOG_DEBUG("TestCliFramework Receive Connection Closed Event,ConnID:%u,RemoteIP:%s,RemotePort:%d"
	         ,pConnInfo->GetConnID(),pConnInfo->GetRemoteAddr().c_str(),pConnInfo->GetRemotePort());
    if(pConnInfo->GetUserData() != NULL)
	{
		delete (ConnUserData*)pConnInfo->GetUserData();
	}
	return 0;
}
